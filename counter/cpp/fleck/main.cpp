#include "main_model.hpp"
#include "counter_bindings.hpp"
#include "../../source_map.h"
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

static bool finish(sunfleck::FleckPage<MainModel>& page)
{
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (page.busy() && std::chrono::steady_clock::now() < deadline) {
        if (page.poll(16) == UI_EXITED) {
            return false;
        }
    }
    return !page.busy();
}

static bool verify(sunfleck::App& app, sunfleck::FleckPage<MainModel>& page, const MainModel& model)
{
    auto label = app.findFleckNode("count");
    auto button = app.findFleckNode("increment");
    page.poll(0);
    app.activateButton();
    if (!page.busy() || button.text() != "Increment") {
        return false;
    }
    app.activateButton();
    if (model.count() != 0 || label.text() != "Count: 0" || button.text() != "Increment" ||
        !finish(page) || model.count() != 1 || label.text() != "Count: 1" ||
        button.text() != "Increment") {
        return false;
    }

    app.activateButton();
    button.setText("Custom label");
    return page.busy() && finish(page) && model.count() == 2 &&
        label.text() == "Count: 2" && button.text() == "Custom label";
}


static int run(bool headless)
{
    try {
        const ui_app_options options{sizeof(options), UI_API_VERSION,
            static_cast<uint32_t>(headless ? UI_SOFTWARE : UI_AUTO), headless ? unsigned{UI_HEADLESS} : unsigned{UI_SHOW_RENDERER | UI_DEBUG_PIPE}};
        sunfleck::App app(options);

        MainModel model(app);
        auto page = sunfleck::generated::load_counter::load(app, model);
        sunfleck::detail::check(attach_counter_source_map(app.nativeHandle()));
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
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR arguments, int show)
{
    (void)instance;
    (void)previous;
    (void)show;
    return run(std::strcmp(arguments, "--test") == 0);
}
#else
int main(int argc, char** argv)
{
    return run(argc == 2 && std::strcmp(argv[1], "--test") == 0);
}
#endif
