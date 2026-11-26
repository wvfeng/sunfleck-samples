#include "showcase_model.hpp"
#include "showcase_bindings.hpp"

#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#endif

static bool verify(sunfleck::App& app, sunfleck::FleckPage<ShowcaseModel>& page, ShowcaseModel& model)
{
    page.poll(0);
    app.activateButton();
    if (page.takeError() || model.count() != 1 || app.findFleckNode("count").text() != "Count: 1") {
        return false;
    }
    model.sharedText().set("Updated by C++");
    if (app.findFleckNode("bound_editor").text() != "Updated by C++" ||
        app.findFleckNode("bound_peer").text() != "Updated by C++" ||
        app.findFleckNode("snapshot").text() != "Shared text") {
        return false;
    }
    const auto theme = app.findFleckNode("theme");
    theme.setValue(UI_BUILTIN_MATERIAL_3);
    app.findFleckNode("dark").setValue(1);
    model.applyTheme(theme);
    page.poll(0);

    uint32_t width = 0, height = 0;
    const auto* pixels = ui_app_pixels(app.nativeHandle(), &width, &height);
    // Sample the padding, away from the antialiased corner rim.
    const size_t background = width >= 16 && height >= 8 ? (static_cast<size_t>(height - 8) * width + width - 16) * 4 : 0;
    if (!pixels || width != 1000 || height != 760 ||
        pixels[background] != 0x17 || pixels[background + 1] != 0x12 || pixels[background + 2] != 0x1b) {
        return false;
    }

    const auto pages = app.findFleckNode("pages");
    for (uint32_t index = 0; index < 5; ++index) {
        pages.setValue(index);
        page.poll(0);
    }
    app.activateButton();
    page.poll(0);
    return !page.takeError() && app.findFleckNode("popup").value() == 1;
}

static int run(int argc, char** argv)
{
    const bool headless = argc == 2 && std::string_view(argv[1]) == "--test";
    if (argc != 1 && !headless) {
        return 1;
    }
    std::setlocale(LC_CTYPE, "");
    try {
        const ui_app_options options{sizeof(options), UI_API_VERSION,
            static_cast<uint32_t>(headless ? UI_SOFTWARE : UI_AUTO), headless ? unsigned{UI_HEADLESS} : unsigned{UI_SHOW_RENDERER | UI_DEBUG_PIPE}};
        sunfleck::App app(options);
        ShowcaseModel model(app);
        auto page = sunfleck::generated::load_showcase::load(app, model);
        model.initializeResources();
        app.openWindow();
        if (headless) {
            return verify(app, *page, model) ? 0 : 1;
        }
        page->run();
        return 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return run(__argc, __argv);
}
#else
int main(int argc, char** argv)
{
    return run(argc, argv);
}
#endif
