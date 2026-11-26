#pragma once
#include <sunfleck/ui.h>

/* The six showcase entry points share the same IDs, layout, and actions. */
typedef struct sunfleck_dock_demo {
    ui_dock* dock;
    ui_node* buttons[6];
    char saved[16384];
    uint32_t saved_size;
    ui_result status;
} sunfleck_dock_demo;

static inline void sunfleck_dock_demo_action(ui_node* node, void* context)
{
    sunfleck_dock_demo* demo = (sunfleck_dock_demo*)context;
    ui_dock_item *editor = NULL, *explorer = NULL, *output = NULL;
    demo->status = ui_dock_find(demo->dock, "dock_editor_a", &editor);
    if (demo->status != UI_OK) { return; }
    demo->status = ui_dock_find(demo->dock, "dock_explorer", &explorer);
    if (demo->status != UI_OK) { return; }
    demo->status = ui_dock_find(demo->dock, "dock_output", &output);
    if (demo->status != UI_OK) { return; }
    if (node == demo->buttons[0]) { demo->status = ui_dock_reset(demo->dock); }
    else if (node == demo->buttons[1]) {
        const ui_rect bounds = {160, 140, 480, 320}; demo->status = ui_dock_float(editor, &bounds, 0);
    } else if (node == demo->buttons[2]) { demo->status = ui_dock_auto_hide(explorer, UI_DOCK_LEFT); }
    else if (node == demo->buttons[3]) {
        ui_dock_state state;
        demo->status = ui_dock_get_state(explorer, &state);
        if (demo->status == UI_OK) { demo->status = state == UI_DOCK_AUTO_HIDDEN ? ui_dock_pin(explorer) : ui_dock_show(explorer); }
        if (demo->status == UI_OK) { demo->status = ui_dock_show(output); }
    } else if (node == demo->buttons[4]) {
        demo->saved_size = sizeof(demo->saved); demo->status = ui_dock_save(demo->dock, demo->saved, &demo->saved_size);
    } else if (node == demo->buttons[5] && demo->saved_size) { demo->status = ui_dock_restore(demo->dock, demo->saved, demo->saved_size - 1, NULL); }
}
static inline ui_result sunfleck_dock_demo_connect(ui_node* host, ui_node* const* buttons, sunfleck_dock_demo* demo)
{
    ui_result status = ui_dock_create(host, &demo->dock);
    if (status != UI_OK) { return status; }
    for (unsigned i = 0; i < 6; ++i) {
        demo->buttons[i] = buttons[i];
        status = ui_node_set_action(buttons[i], sunfleck_dock_demo_action, demo);
        if (status != UI_OK) { return status; }
    }
    return UI_OK;
}
static inline ui_result sunfleck_dock_demo_build(ui_app* app, ui_node* pages, ui_node** host, ui_node** buttons)
{
    ui_node *page = NULL, *toolbar = NULL;
    ui_result status = ui_node_create(app, UI_TAB_ITEM, &page);
    if (status != UI_OK) { return status; }
    status = ui_node_set_text(page, "Dock"); if (status != UI_OK) { return status; }
    status = ui_node_append(pages, page); if (status != UI_OK) { return status; }
    status = ui_node_create(app, UI_TOOL_BAR, &toolbar); if (status != UI_OK) { return status; }
    status = ui_node_append(page, toolbar); if (status != UI_OK) { return status; }
    const char* labels[] = {"Reset layout", "Float editor", "Auto hide", "Show tools", "Save layout", "Restore layout"};
    for (unsigned i = 0; i < 6; ++i) {
        status = ui_node_create(app, UI_BUTTON, &buttons[i]); if (status != UI_OK) { return status; }
        status = ui_node_set_text(buttons[i], labels[i]); if (status != UI_OK) { return status; }
        status = ui_node_append(toolbar, buttons[i]); if (status != UI_OK) { return status; }
    }
    status = ui_node_create(app, UI_DOCK_HOST, host); if (status != UI_OK) { return status; }
    status = ui_node_append(page, *host); if (status != UI_OK) { return status; }
    ui_dock* dock = NULL; status = ui_dock_create(*host, &dock); if (status != UI_OK) { return status; }
    const char* ids[] = {"dock_editor_a", "dock_editor_b", "dock_explorer", "dock_output"};
    const char* titles[] = {"Editor A", "Editor B", "Explorer", "Output"};
    const char* texts[] = {"Edit this document, then float or move its tab.", "A second document with independent content.", "Project files and folders", "Build completed successfully."};
    ui_dock_item* items[4] = {NULL, NULL, NULL, NULL};
    for (unsigned i = 0; i < 4; ++i) {
        ui_node* content = NULL;
        status = ui_node_create(app, UI_TEXT_AREA, &content); if (status != UI_OK) { return status; }
        status = ui_node_set_text(content, texts[i]); if (status != UI_OK) { return status; }
        const ui_dock_item_options options = {sizeof(options), 1, ids[i], titles[i], content,
            i < 2 ? UI_DOCK_DOCUMENT_KIND : UI_DOCK_TOOL_KIND, UI_DOCK_DEFAULT_CAPABILITIES, 96, 64};
        status = ui_dock_register(dock, &options, &items[i]); if (status != UI_OK) { return status; }
    }
    status = ui_dock_move(items[3], items[2], UI_DOCK_POSITION_BOTTOM, UINT32_MAX); if (status != UI_OK) { return status; }
    status = ui_dock_activate(items[0]); if (status != UI_OK) { return status; }
    return ui_dock_capture_default(dock);
}
