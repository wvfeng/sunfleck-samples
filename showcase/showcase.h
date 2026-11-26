#pragma once
#include <sunfleck/ui.h>
#include "dock_demo.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum showcase_handle {
    SHOWCASE_NONE,
    SHOWCASE_PAGES,
    SHOWCASE_THEME,
    SHOWCASE_DARK,
    SHOWCASE_COMPACT,
    SHOWCASE_COUNT,
    SHOWCASE_INCREMENT,
    SHOWCASE_RESET,
    SHOWCASE_ICON_BUTTON,
    SHOWCASE_LINK,
    SHOWCASE_NOTICE,
    SHOWCASE_SLIDER,
    SHOWCASE_PROGRESS,
    SHOWCASE_EDITOR,
    SHOWCASE_BOUND_EDITOR,
    SHOWCASE_BOUND_PEER,
    SHOWCASE_SNAPSHOT,
    SHOWCASE_SEARCH,
    SHOWCASE_SAMPLE_IMAGE,
    SHOWCASE_SAMPLE_ICON,
    SHOWCASE_ITEMS,
    SHOWCASE_TABLE,
    SHOWCASE_CELL,
    SHOWCASE_MENU,
    SHOWCASE_SHOW_POPUP,
    SHOWCASE_POPUP,
    SHOWCASE_TARGET,
    SHOWCASE_CONTEXT,
    SHOWCASE_CLOSE_CONTEXT,
    SHOWCASE_SHOW_DIALOG,
    SHOWCASE_DIALOG,
    SHOWCASE_CLOSE_DIALOG,
    SHOWCASE_TABS,
    SHOWCASE_DOCK, SHOWCASE_DOCK_RESET, SHOWCASE_DOCK_FLOAT, SHOWCASE_DOCK_HIDE,
    SHOWCASE_DOCK_SHOW, SHOWCASE_DOCK_SAVE, SHOWCASE_DOCK_RESTORE,
    SHOWCASE_HANDLE_COUNT
} showcase_handle;

typedef struct showcase_nodes {
    ui_node* handles[SHOWCASE_HANDLE_COUNT];
} showcase_nodes;

typedef struct showcase_state {
    ui_app* app;
    ui_node *pages, *theme, *dark, *compact, *progress, *notice, *popup, *dialog, *label;
    ui_text_source* source;
    sunfleck_dock_demo dock_demo;
    unsigned count;
    ui_result status;
} showcase_state;

ui_result showcase_connect(showcase_state* demo, ui_app* app, const showcase_nodes* nodes);
ui_result showcase_test(showcase_state* demo);

ui_result build_showcase(ui_app* app, showcase_nodes* nodes);
ui_result load_showcase(ui_app* app, ui_fleck_diagnostic* diagnostic);
ui_result showcase_fleck_handles(ui_app* app, showcase_nodes* nodes);
int showcase_run(ui_app* app, const showcase_nodes* nodes, int headless);
#ifdef __cplusplus
}
#endif
