#include "showcase.h"

ui_result showcase_fleck_handles(ui_app* app, showcase_nodes* nodes)
{
    static const char* const ids[SHOWCASE_HANDLE_COUNT] = {
        [SHOWCASE_PAGES] = "pages",
        [SHOWCASE_THEME] = "theme",
        [SHOWCASE_DARK] = "dark",
        [SHOWCASE_COMPACT] = "compact",
        [SHOWCASE_COUNT] = "count",
        [SHOWCASE_INCREMENT] = "increment",
        [SHOWCASE_RESET] = "reset",
        [SHOWCASE_ICON_BUTTON] = "icon_button",
        [SHOWCASE_LINK] = "link",
        [SHOWCASE_NOTICE] = "notice",
        [SHOWCASE_SLIDER] = "slider",
        [SHOWCASE_PROGRESS] = "progress",
        [SHOWCASE_EDITOR] = "editor",
        [SHOWCASE_BOUND_EDITOR] = "bound_editor",
        [SHOWCASE_BOUND_PEER] = "bound_peer",
        [SHOWCASE_SNAPSHOT] = "snapshot",
        [SHOWCASE_SEARCH] = "search",
        [SHOWCASE_SAMPLE_IMAGE] = "sample_image",
        [SHOWCASE_SAMPLE_ICON] = "sample_icon",
        [SHOWCASE_ITEMS] = "items",
        [SHOWCASE_TABLE] = "table",
        [SHOWCASE_CELL] = "cell",
        [SHOWCASE_MENU] = "menu",
        [SHOWCASE_SHOW_POPUP] = "show_popup",
        [SHOWCASE_POPUP] = "popup",
        [SHOWCASE_TARGET] = "target",
        [SHOWCASE_CONTEXT] = "context",
        [SHOWCASE_CLOSE_CONTEXT] = "close_context",
        [SHOWCASE_SHOW_DIALOG] = "show_dialog",
        [SHOWCASE_DIALOG] = "dialog",
        [SHOWCASE_CLOSE_DIALOG] = "close_dialog",
        [SHOWCASE_TABS] = "tabs",
        [SHOWCASE_DOCK] = "dock",
        [SHOWCASE_DOCK_RESET] = "dock_reset",
        [SHOWCASE_DOCK_FLOAT] = "dock_float",
        [SHOWCASE_DOCK_HIDE] = "dock_hide",
        [SHOWCASE_DOCK_SHOW] = "dock_show",
        [SHOWCASE_DOCK_SAVE] = "dock_save",
        [SHOWCASE_DOCK_RESTORE] = "dock_restore",
    };
    for (unsigned i = 1; i < SHOWCASE_HANDLE_COUNT; ++i) {
        const ui_result status = ui_app_find_fleck_node(app, ids[i], &nodes->handles[i]);
        if (status != UI_OK) {
            return status;
        }
    }
    return UI_OK;
}
