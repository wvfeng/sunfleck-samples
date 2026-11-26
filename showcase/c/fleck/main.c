#include "showcase.h"

#include <locale.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

static int run(int argc, char** argv)
{
    const int headless = argc == 2 && strcmp(argv[1], "--test") == 0;
    if (argc != 1 && !headless) {
        return 1;
    }
    setlocale(LC_CTYPE, "");
    const ui_app_options options = {sizeof(options), UI_API_VERSION,
        headless ? UI_SOFTWARE : UI_AUTO, headless ? UI_HEADLESS : UI_SHOW_RENDERER | UI_DEBUG_PIPE};
    ui_app* app = NULL;
    if (ui_app_create(&options, &app) != UI_OK) {
        return 1;
    }
    showcase_nodes nodes = {0};
    const int result = load_showcase(app, NULL) == UI_OK && showcase_fleck_handles(app, &nodes) == UI_OK
        ? showcase_run(app, &nodes, headless) : 1;
    ui_app_destroy(app);
    return result;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR arguments, int show)
{
    (void)instance;
    (void)previous;
    (void)arguments;
    (void)show;
    return run(__argc, __argv);
}
#else
int main(int argc, char** argv)
{
    return run(argc, argv);
}
#endif
