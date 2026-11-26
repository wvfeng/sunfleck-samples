#include "showcase.h"
#include <string.h>

static int compare_pages(ui_app* code, ui_app* fleck, const showcase_nodes* code_nodes,
                         const showcase_nodes* fleck_nodes)
{
    ui_node* code_pages = code_nodes->handles[SHOWCASE_PAGES];
    ui_node* fleck_pages = fleck_nodes->handles[SHOWCASE_PAGES];
    for (uint32_t page = 0; page < 6; ++page) {
        if (ui_node_set_value(code_pages, page) != UI_OK ||
            ui_node_set_value(fleck_pages, page) != UI_OK ||
            ui_app_poll(code, 0) != UI_OK || ui_app_poll(fleck, 0) != UI_OK) {
            return 1;
        }
        uint32_t width = 0, height = 0, other_width = 0, other_height = 0;
        const uint8_t* pixels = ui_app_pixels(code, &width, &height);
        const uint8_t* other = ui_app_pixels(fleck, &other_width, &other_height);
        if (!pixels || !other || width != other_width || height != other_height ||
            memcmp(pixels, other, (size_t)width * height * 4) != 0) {
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    const ui_app_options options = {sizeof(options), UI_API_VERSION, UI_SOFTWARE, UI_HEADLESS};
    ui_app *code = NULL, *fleck = NULL;
    showcase_state code_demo = {0}, fleck_demo = {0};
    showcase_nodes code_nodes = {0}, fleck_nodes = {0};
    int result = 1;
    if (ui_app_create(&options, &code) == UI_OK && ui_app_create(&options, &fleck) == UI_OK &&
        build_showcase(code, &code_nodes) == UI_OK && load_showcase(fleck, NULL) == UI_OK &&
        showcase_fleck_handles(fleck, &fleck_nodes) == UI_OK &&
        showcase_connect(&code_demo, code, &code_nodes) == UI_OK &&
        showcase_connect(&fleck_demo, fleck, &fleck_nodes) == UI_OK &&
        ui_app_open_window(code) == UI_OK && ui_app_open_window(fleck) == UI_OK) {
        result = 0;
        for (unsigned id = 0; id < UI_BUILTIN_THEME_COUNT && !result; ++id) {
            for (unsigned dark = 0; dark <= 1 && !result; ++dark) {
                const ui_builtin_theme theme = (ui_builtin_theme)id;
                result = ui_app_set_builtin_theme(code, theme, dark, 0) != UI_OK ||
                    ui_app_set_builtin_theme(fleck, theme, dark, 0) != UI_OK ||
                    ui_node_set_value(code_nodes.handles[SHOWCASE_THEME], id) != UI_OK ||
                    ui_node_set_value(fleck_nodes.handles[SHOWCASE_THEME], id) != UI_OK ||
                    ui_node_set_value(code_nodes.handles[SHOWCASE_DARK], dark) != UI_OK ||
                    ui_node_set_value(fleck_nodes.handles[SHOWCASE_DARK], dark) != UI_OK ||
                    compare_pages(code, fleck, &code_nodes, &fleck_nodes);
            }
        }
    }
    ui_app_destroy(fleck);
    ui_app_destroy(code);
    return result;
}
