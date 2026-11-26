#include "showcase.h"

#include <stdio.h>
#include <string.h>

static void remember(showcase_state* demo, ui_result result)
{
    if (demo->status == UI_OK) {
        demo->status = result;
    }
}

static void set_count(showcase_state* demo, unsigned value)
{
    char text[48];
    demo->count = value;
    snprintf(text, sizeof(text), "Count: %u", value);
    remember(demo, ui_text_source_set(demo->source, text));
}

static void increment(ui_node* node, void* context)
{
    (void)node;
    showcase_state* demo = context;
    set_count(demo, demo->count + 1);
}

static void reset(ui_node* node, void* context)
{
    (void)node;
    set_count(context, 0);
}

static void theme(ui_node* node, void* context)
{
    (void)node;
    showcase_state* demo = context;
    uint32_t preset = 0, compact = 0;
    uint32_t dark = 0;
    remember(demo, ui_node_get_value(demo->theme, &preset));
    remember(demo, ui_node_get_value(demo->dark, &dark));
    remember(demo, ui_node_get_value(demo->compact, &compact));
    remember(demo, ui_app_set_builtin_theme(demo->app, (ui_builtin_theme)preset, dark, compact));
}

static void slider(ui_node* node, void* context)
{
    showcase_state* demo = context;
    uint32_t value = 0;
    remember(demo, ui_node_get_value(node, &value));
    remember(demo, ui_node_set_value(demo->progress, value));
}

static void link(ui_node* node, void* context)
{
    (void)node;
    showcase_state* demo = context;
    remember(demo, ui_node_set_text(demo->notice, "Action received from the toolbar."));
}

static void search(ui_node* node, void* context)
{
    showcase_state* demo = context;
    char text[16385];
    uint32_t length = sizeof(text);
    remember(demo, ui_node_get_text(node, text, &length));
    if (demo->status == UI_OK) {
        remember(demo, ui_node_set_text(demo->notice, text));
    }
}

static void toggle(ui_node* node, void* context)
{
    (void)node;
    ui_node* popup = context;
    uint32_t value = 0;
    if (ui_node_get_value(popup, &value) == UI_OK) {
        ui_node_set_value(popup, !value);
    }
}

static void menu(ui_node* node, void* context)
{
    showcase_state* demo = context;
    uint32_t choice = 0;
    remember(demo, ui_node_get_value(node, &choice));
    remember(demo, ui_node_set_value(choice ? demo->popup : demo->dialog, 1));
}

static void command(showcase_state* demo, ui_node* node, ui_node_action action, void* context)
{
    if (node) {
        ui_command* handle = NULL;
        remember(demo, ui_command_create(demo->app, action, context, &handle));
        remember(demo, ui_node_bind_command(node, handle));
    }
}

ui_result showcase_connect(showcase_state* demo, ui_app* app, const showcase_nodes* nodes)
{
    demo->app = app;
    demo->pages = nodes->handles[SHOWCASE_PAGES];
    demo->theme = nodes->handles[SHOWCASE_THEME];
    demo->dark = nodes->handles[SHOWCASE_DARK];
    demo->compact = nodes->handles[SHOWCASE_COMPACT];
    remember(demo, ui_node_set_value(demo->theme, UI_BUILTIN_SUNFLECK_LITE));
    demo->progress = nodes->handles[SHOWCASE_PROGRESS];
    demo->notice = nodes->handles[SHOWCASE_NOTICE];
    demo->popup = nodes->handles[SHOWCASE_POPUP];
    demo->dialog = nodes->handles[SHOWCASE_DIALOG];
    demo->label = nodes->handles[SHOWCASE_COUNT];
    ui_node* context = nodes->handles[SHOWCASE_CONTEXT];
    const char* const items[] = {"One", "Two", "Three"};
    const uint8_t rgb[] = {36, 107, 206, 255, 211, 105, 28, 125, 81, 36, 107, 206};
    remember(demo, ui_node_set_items(nodes->handles[SHOWCASE_ITEMS], items, 3));
    remember(demo, ui_node_set_image(nodes->handles[SHOWCASE_SAMPLE_IMAGE], rgb, 2, 2));
    remember(demo, ui_node_set_image(nodes->handles[SHOWCASE_SAMPLE_ICON], rgb, 2, 2));
    remember(demo, ui_text_source_create(app, "Count: 0", &demo->source));
    remember(demo, ui_node_bind_text(demo->label, demo->source, UI_BIND_ONE_WAY));
    ui_text_source* shared_text = NULL;
    remember(demo, ui_text_source_create(app, "Shared text", &shared_text));
    if (demo->status == UI_OK) {
        remember(demo, ui_node_bind_text(nodes->handles[SHOWCASE_BOUND_EDITOR], shared_text, UI_BIND_TWO_WAY));
        remember(demo, ui_node_bind_text(nodes->handles[SHOWCASE_BOUND_PEER], shared_text, UI_BIND_TWO_WAY));
        remember(demo, ui_node_bind_text(nodes->handles[SHOWCASE_SNAPSHOT], shared_text, UI_BIND_ONE_TIME));
    }
    command(demo, nodes->handles[SHOWCASE_INCREMENT], increment, demo);
    command(demo, nodes->handles[SHOWCASE_RESET], reset, demo);
    command(demo, nodes->handles[SHOWCASE_THEME], theme, demo);
    command(demo, nodes->handles[SHOWCASE_DARK], theme, demo);
    command(demo, nodes->handles[SHOWCASE_COMPACT], theme, demo);
    command(demo, nodes->handles[SHOWCASE_SLIDER], slider, demo);
    command(demo, nodes->handles[SHOWCASE_ICON_BUTTON], link, demo);
    command(demo, nodes->handles[SHOWCASE_LINK], link, demo);
    command(demo, nodes->handles[SHOWCASE_SEARCH], search, demo);
    command(demo, nodes->handles[SHOWCASE_SHOW_POPUP], toggle, demo->popup);
    command(demo, nodes->handles[SHOWCASE_SHOW_DIALOG], toggle, demo->dialog);
    command(demo, nodes->handles[SHOWCASE_CLOSE_DIALOG], toggle, demo->dialog);
    command(demo, nodes->handles[SHOWCASE_CLOSE_CONTEXT], toggle, context);
    command(demo, nodes->handles[SHOWCASE_MENU], menu, demo);
    remember(demo, sunfleck_dock_demo_connect(nodes->handles[SHOWCASE_DOCK], &nodes->handles[SHOWCASE_DOCK_RESET], &demo->dock_demo));
    return demo->status;
}

ui_result showcase_test(showcase_state* demo)
{
    char label[32];
    uint32_t length = sizeof(label);
    remember(demo, ui_app_activate_button(demo->app));
    remember(demo, ui_app_poll(demo->app, 0));
    remember(demo, ui_node_get_text(demo->label, label, &length));
    if (demo->status != UI_OK || strcmp(label, "Count: 1") != 0) {
        return UI_ERROR_INVALID_STATE;
    }
    remember(demo, ui_node_set_value(demo->theme, UI_BUILTIN_MATERIAL_3));
    remember(demo, ui_node_set_value(demo->dark, 1));
    theme(NULL, demo);
    remember(demo, ui_app_poll(demo->app, 0));
    uint32_t width = 0, height = 0;
    const uint8_t* pixels = ui_app_pixels(demo->app, &width, &height);
    if (!pixels || width < 16 || height < 8) {
        return UI_ERROR_PLATFORM;
    }
    /* Stay inside the window padding and clear of the antialiased corner rim. */
    const size_t background = ((size_t)(height - 8) * width + width - 16) * 4;
    if (pixels[background] != 0x17 || pixels[background + 1] != 0x12 || pixels[background + 2] != 0x1b) {
        return UI_ERROR_PLATFORM;
    }
    /* Page zero has already rendered; selecting it again correctly stays idle. */
    for (uint32_t page = 1; page < 5 && demo->status == UI_OK; ++page) {
        remember(demo, ui_node_set_value(demo->pages, page));
        remember(demo, ui_app_poll(demo->app, 0));
    }
    remember(demo, ui_app_activate_button(demo->app));
    remember(demo, ui_app_poll(demo->app, 0));
    uint32_t visible = 0;
    remember(demo, ui_node_get_value(demo->popup, &visible));
    return demo->status == UI_OK && visible == 1 ? UI_OK : UI_ERROR_INVALID_STATE;
}

int showcase_run(ui_app* app, const showcase_nodes* nodes, int headless)
{
    showcase_state demo = {0};
    ui_result status = showcase_connect(&demo, app, nodes);
    if (status == UI_OK) {
        status = ui_app_open_window(app);
    }
    if (status == UI_OK && headless) {
        status = ui_app_poll(app, 0);
        uint32_t width = 0, height = 0;
        if (status == UI_OK) {
            status = ui_app_pixels(app, &width, &height) && width == 1000 && height == 760
                ? showcase_test(&demo) : UI_ERROR_PLATFORM;
        }
    } else if (status == UI_OK) {
        status = ui_app_run(app);
    }
    return status == (headless ? UI_OK : UI_EXITED) ? 0 : 1;
}
