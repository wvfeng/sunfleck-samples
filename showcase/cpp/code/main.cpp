#include "showcase.h"

#include <clocale>
#include <cstdlib>
#include <memory>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#endif

static int run(int argc, char** argv)
{
    const bool headless = argc == 2 && std::string_view(argv[1]) == "--test";
    if (argc != 1 && !headless) {
        return 1;
    }
    std::setlocale(LC_CTYPE, "");
    const ui_app_options options{sizeof(options), UI_API_VERSION,
        static_cast<uint32_t>(headless ? UI_SOFTWARE : UI_AUTO), headless ? unsigned{UI_HEADLESS} : unsigned{UI_SHOW_RENDERER | UI_DEBUG_PIPE}};
    ui_app* handle = nullptr;
    if (ui_app_create(&options, &handle) != UI_OK) {
        return 1;
    }
    const std::unique_ptr<ui_app, decltype(&ui_app_destroy)> app(handle, ui_app_destroy);
    showcase_nodes nodes{};
    return build_showcase(app.get(), &nodes) == UI_OK
        ? showcase_run(app.get(), &nodes, headless) : 1;
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

