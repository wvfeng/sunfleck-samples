#include "showcase.h"

#include <stdio.h>

static ui_result set_fixed_width(ui_node* node, double pixels) {
    ui_binding_value value = {UI_VALUE_LENGTH};
    value.integer = UI_LENGTH_PIXELS; value.number = pixels;
    return ui_node_set_property(node, UI_PROPERTY_WIDTH, &value);
}


typedef struct layout_builder {
    ui_app* app;
    showcase_nodes* nodes;
    ui_result status;
} layout_builder;

static void check_result(layout_builder* layout, ui_result status)
{
    if (layout->status == UI_OK) {
        layout->status = status;
    }
}

static ui_node* add(layout_builder* layout, ui_node* parent, ui_node_kind kind, const char* text)
{
    if (layout->status != UI_OK) {
        return NULL;
    }
    ui_node* node = NULL;
    check_result(layout, ui_node_create(layout->app, kind, &node));
    if (layout->status == UI_OK && text) {
        check_result(layout, ui_node_set_text(node, text));
    }
    if (layout->status == UI_OK && parent) {
        check_result(layout, ui_node_append(parent, node));
    }
    return node;
}

static ui_node* page(layout_builder* layout, const char* title)
{
    ui_node* tab = add(layout, layout->nodes->handles[SHOWCASE_PAGES], UI_TAB_ITEM, title);
    return add(layout, tab, UI_SCROLL_VIEW, NULL);
}

static void build_theme(layout_builder* layout)
{
    ui_node** handles = layout->nodes->handles;
    ui_node* content = page(layout, "Theme");
    add(layout, content, UI_TEXT_BLOCK,
        "Sunfleck 组件展厅 · 主题和命令");
    ui_node* appearance = add(layout, content, UI_CARD, NULL);
    add(layout, appearance, UI_LABEL, "ComboBox / Switch - Appearance");
    ui_node* theme = add(layout, appearance, UI_COMBO_BOX, NULL);
    handles[SHOWCASE_THEME] = theme;
    for (int i = 0; i < UI_BUILTIN_THEME_COUNT; ++i) {
        add(layout, theme, UI_LIST_ITEM, ui_builtin_theme_name((ui_builtin_theme)i));
    }
    handles[SHOWCASE_DARK] = add(layout, appearance, UI_SWITCH, "Dark appearance");
    handles[SHOWCASE_COMPACT] = add(layout, appearance, UI_SWITCH, "Compact density");

    ui_node* card = add(layout, content, UI_CARD, NULL);
    add(layout, card, UI_LABEL, "Card / Button - Counter commands");
    handles[SHOWCASE_COUNT] = add(layout, card, UI_TEXT_BLOCK, "Count: 0");
    handles[SHOWCASE_INCREMENT] = add(layout, card, UI_BUTTON, "Increment via Command");
    handles[SHOWCASE_RESET] = add(layout, card, UI_BUTTON, "Reset counter");
}

static void build_layout(layout_builder* layout)
{
    ui_node* content = page(layout, "Layout");
    ui_node* stack = add(layout, content, UI_STACK_PANEL, NULL);
    check_result(layout, ui_node_set_layout(stack, 10, 6));

    ui_node* panel = add(layout, stack, UI_PANEL, NULL);
    add(layout, panel, UI_TEXT_BLOCK, "StackPanel → Panel → TextBlock");

    ui_node* flex_example = add(layout, stack, UI_CARD, NULL);
    add(layout, flex_example, UI_LABEL, "Flex / Badge / Button");
    ui_node* flex = add(layout, flex_example, UI_FLEX, NULL);
    ui_node* fixed = add(layout, flex, UI_BADGE, "Fixed");
    check_result(layout, set_fixed_width(fixed, 100));
    add(layout, flex, UI_BUTTON, "Growing Flex item");

    ui_node* grid_example = add(layout, stack, UI_CARD, NULL);
    add(layout, grid_example, UI_LABEL, "Grid / Card");
    ui_node* grid = add(layout, grid_example, UI_FLEX, NULL);
    ui_node* card = add(layout, grid, UI_CARD, NULL);
    check_result(layout, set_fixed_width(card, 130));
    add(layout, card, UI_TEXT_BLOCK, "Fixed track");
    add(layout, grid, UI_TEXT_BLOCK, "Star track");

    ui_node* wrap_example = add(layout, stack, UI_CARD, NULL);
    add(layout, wrap_example, UI_LABEL, "WrapPanel");
    ui_node* wrap = add(layout, wrap_example, UI_WRAP_PANEL, NULL);
    const char* const labels[] = {"Wrap A", "Wrap B", "Wrap C"};
    for (unsigned i = 0; i < 3; ++i) {
        ui_node* button = add(layout, wrap, UI_BUTTON, labels[i]);
        check_result(layout, set_fixed_width(button, 300));
    }

    ui_node* dock_example = add(layout, stack, UI_CARD, NULL);
    add(layout, dock_example, UI_LABEL, "DockPanel");
    ui_node* dock = add(layout, dock_example, UI_DOCK_PANEL, NULL);
    ui_node* bottom = add(layout, dock, UI_BADGE, "Dock Bottom");
    check_result(layout, ui_node_set_dock(bottom, UI_DOCK_BOTTOM));
    add(layout, dock, UI_TEXT_BLOCK, "Remaining area");

    ui_node* split_example = add(layout, stack, UI_CARD, NULL);
    add(layout, split_example, UI_LABEL, "SplitView");
    ui_node* split = add(layout, split_example, UI_SPLIT_VIEW, NULL);
    add(layout, split, UI_TEXT_BLOCK, "Drag the divider");
    add(layout, split, UI_TEXT_BLOCK, "Right pane");
    check_result(layout, ui_node_set_value(split, 40));

    ui_node* overlay_example = add(layout, stack, UI_CARD, NULL);
    add(layout, overlay_example, UI_LABEL, "Overlay / ProgressBar");
    ui_node* overlay = add(layout, overlay_example, UI_OVERLAY, NULL);
    ui_node* progress = add(layout, overlay, UI_PROGRESS_BAR, NULL);
    check_result(layout, ui_node_set_value(progress, 65));
    add(layout, overlay, UI_TEXT_BLOCK, "Overlay");
}

static void build_inputs(layout_builder* layout)
{
    ui_node** handles = layout->nodes->handles;
    ui_node* content = page(layout, "Inputs");
    ui_node* toolbar_example = add(layout, content, UI_CARD, NULL);
    add(layout, toolbar_example, UI_LABEL, "ToolBar / InfoBar - Click an action");
    ui_node* toolbar = add(layout, toolbar_example, UI_TOOL_BAR, NULL);
    handles[SHOWCASE_ICON_BUTTON] = add(layout, toolbar, UI_BUTTON, "Button");
    add(layout, toolbar, UI_TOGGLE_BUTTON, "ToggleButton");
    handles[SHOWCASE_LINK] = add(layout, toolbar, UI_HYPERLINK, "Hyperlink: show message");
    handles[SHOWCASE_NOTICE] = add(layout, toolbar_example, UI_INFO_BAR, "Try the toolbar or submit a search.");
    ui_node* choice_example = add(layout, content, UI_CARD, NULL);
    add(layout, choice_example, UI_LABEL, "CheckBox / Switch / RadioButton");
    add(layout, choice_example, UI_CHECK_BOX, "CheckBox");
    add(layout, choice_example, UI_SWITCH, "Switch");

    ui_node* radio_group = add(layout, choice_example, UI_FLEX, NULL);
    ui_node* first = add(layout, radio_group, UI_RADIO_BUTTON, "First");
    add(layout, radio_group, UI_RADIO_BUTTON, "Second");
    check_result(layout, ui_node_set_value(first, 1));

    ui_node* range_example = add(layout, content, UI_CARD, NULL);
    add(layout, range_example, UI_LABEL, "SpinBox / Slider / ProgressBar");
    add(layout, range_example, UI_LABEL, "SpinBox - Adjust the number");
    ui_node* spin = add(layout, range_example, UI_SPIN_BOX, NULL);
    check_result(layout, ui_node_set_value(spin, 12));
    add(layout, range_example, UI_LABEL, "Slider - Drag to update the ProgressBar");
    handles[SHOWCASE_SLIDER] = add(layout, range_example, UI_SLIDER, NULL);
    check_result(layout, ui_node_set_value(handles[SHOWCASE_SLIDER], 35));
    add(layout, range_example, UI_LABEL, "ProgressBar - Follows the slider");
    handles[SHOWCASE_PROGRESS] = add(layout, range_example, UI_PROGRESS_BAR, NULL);
    check_result(layout, ui_node_set_value(handles[SHOWCASE_PROGRESS], 35));
    add(layout, range_example, UI_LABEL, "Separator");
    add(layout, range_example, UI_SEPARATOR, NULL);

    ui_node* editor_example = add(layout, content, UI_CARD, NULL);
    add(layout, editor_example, UI_LABEL, "Label / TextBox - Click the label to focus");
    ui_node* label = add(layout, editor_example, UI_LABEL, "Click this label to focus the editor");
    handles[SHOWCASE_EDITOR] = add(layout, editor_example, UI_TEXT_BOX, "Edit this text / 编辑");
    check_result(layout, ui_node_set_anchor(label, handles[SHOWCASE_EDITOR]));
    ui_node* binding_example = add(layout, content, UI_CARD, NULL);
    add(layout, binding_example, UI_LABEL, "TextBox - TwoWay and OneTime bindings");
    add(layout, binding_example, UI_TEXT_BLOCK, "TwoWay: edit either field to update both");
    handles[SHOWCASE_BOUND_EDITOR] = add(layout, binding_example, UI_TEXT_BOX, NULL);
    handles[SHOWCASE_BOUND_PEER] = add(layout, binding_example, UI_TEXT_BOX, NULL);
    add(layout, binding_example, UI_TEXT_BLOCK, "OneTime snapshot:");
    handles[SHOWCASE_SNAPSHOT] = add(layout, binding_example, UI_TEXT_BLOCK, NULL);

    ui_node* text_inputs = add(layout, content, UI_CARD, NULL);
    add(layout, text_inputs, UI_LABEL, "PasswordBox / TextArea / SearchBox");
    add(layout, text_inputs, UI_LABEL, "PasswordBox");
    add(layout, text_inputs, UI_PASSWORD_BOX, "Secret");
    add(layout, text_inputs, UI_LABEL, "TextArea");
    add(layout, text_inputs, UI_TEXT_AREA, "First line\n第二行");
    add(layout, text_inputs, UI_LABEL, "SearchBox - Submit to update the InfoBar");
    handles[SHOWCASE_SEARCH] = add(layout, text_inputs, UI_SEARCH_BOX, NULL);
    ui_node* image_example = add(layout, content, UI_CARD, NULL);
    add(layout, image_example, UI_LABEL, "Image / Icon");
    ui_node* images = add(layout, image_example, UI_FLEX, NULL);
    handles[SHOWCASE_SAMPLE_IMAGE] = add(layout, images, UI_IMAGE, NULL);
    check_result(layout, set_fixed_width(handles[SHOWCASE_SAMPLE_IMAGE], 80));
    handles[SHOWCASE_SAMPLE_ICON] = add(layout, images, UI_ICON, NULL);
}

static void build_data(layout_builder* layout)
{
    ui_node** handles = layout->nodes->handles;
    ui_node* content = page(layout, "Data");
    ui_node* combo_example = add(layout, content, UI_CARD, NULL);
    add(layout, combo_example, UI_LABEL, "ComboBox / ListItem");
    ui_node* combo = add(layout, combo_example, UI_COMBO_BOX, NULL);
    add(layout, combo, UI_LIST_ITEM, "Alpha");
    add(layout, combo, UI_LIST_ITEM, "Beta");
    add(layout, combo, UI_LIST_ITEM, "Gamma");

    ui_node* list_example = add(layout, content, UI_CARD, NULL);
    add(layout, list_example, UI_LABEL, "ListView / ListItem");
    ui_node* list = add(layout, list_example, UI_LIST_VIEW, NULL);
    add(layout, list, UI_LIST_ITEM, "List Alpha");
    add(layout, list, UI_LIST_ITEM, "List Beta");
    add(layout, list, UI_LIST_ITEM, "List Gamma");
    ui_node* items_example = add(layout, content, UI_CARD, NULL);
    add(layout, items_example, UI_LABEL, "ItemsControl");
    handles[SHOWCASE_ITEMS] = add(layout, items_example, UI_ITEMS_CONTROL, NULL);

    ui_node* tree_example = add(layout, content, UI_CARD, NULL);
    add(layout, tree_example, UI_LABEL, "TreeView / TreeItem");
    ui_node* tree = add(layout, tree_example, UI_TREE_VIEW, NULL);
    ui_node* branch = add(layout, tree, UI_TREE_ITEM, "level 1, item 1");
    add(layout, branch, UI_TREE_ITEM, "level 2, item 1");
    add(layout, branch, UI_TREE_ITEM, "level 2, item 2");
    add(layout, branch, UI_TREE_ITEM, "level 2, item 3");
    check_result(layout, ui_node_set_value(branch, 1));
    ui_node* collapsed = add(layout, tree, UI_TREE_ITEM, "level 1, item 2");
    add(layout, collapsed, UI_TREE_ITEM, "level 2, item 1");
    add(layout, tree, UI_TREE_ITEM, "level 1, item 3");

    ui_node* table_example = add(layout, content, UI_CARD, NULL);
    add(layout, table_example, UI_LABEL, "DataGrid / Badge / TextBox");
    add(layout, table_example, UI_TEXT_BLOCK, "DataGrid: scroll and click a value to edit");
    ui_node* table = add(layout, table_example, UI_DATA_GRID, NULL);
    handles[SHOWCASE_TABLE] = table;
    for (unsigned i = 1; i <= 12; ++i) {
        char title[16], value[24];
        snprintf(title, sizeof(title), "Row %02u", i);
        snprintf(value, sizeof(value), "Editable value %u", i);
        ui_node* row = add(layout, table, UI_GRID, NULL);
        const ui_grid_track columns[] = {{UI_GRID_PIXELS,90},{UI_GRID_STAR,1}};
        check_result(layout, ui_grid_set_tracks(row, NULL, 0, columns, 2));
        ui_node* badge = add(layout, row, UI_BADGE, title);
        check_result(layout, set_fixed_width(badge, 90));
        ui_node* cell = add(layout, row, UI_TEXT_BOX, value);
        check_result(layout, ui_node_set_grid_cell(cell,0,1,1,1));
        if (i == 1) {
            handles[SHOWCASE_CELL] = cell;
        }
    }
}

static void build_overlays(layout_builder* layout)
{
    ui_node** handles = layout->nodes->handles;
    ui_node* content = page(layout, "Overlays");
    ui_node* menu_example = add(layout, content, UI_CARD, NULL);
    add(layout, menu_example, UI_LABEL, "MenuBar / ComboBox - Open Actions");
    ui_node* menu_bar = add(layout, menu_example, UI_MENU_BAR, NULL);
    ui_node* menu = add(layout, menu_bar, UI_COMBO_BOX, "Actions");
    handles[SHOWCASE_MENU] = menu;
    add(layout, menu, UI_LIST_ITEM, "Show dialog");
    add(layout, menu, UI_LIST_ITEM, "Show popup");

    ui_node* popup_example = add(layout, content, UI_CARD, NULL);
    add(layout, popup_example, UI_LABEL, "Popup - Click the button to toggle");
    ui_node* show_popup = add(layout, popup_example, UI_BUTTON, "Toggle popup");
    handles[SHOWCASE_SHOW_POPUP] = show_popup;
    ui_node* popup = add(layout, popup_example, UI_POPUP, NULL);
    handles[SHOWCASE_POPUP] = popup;
    check_result(layout, ui_node_set_anchor(popup, show_popup));
    add(layout, popup, UI_INFO_BAR, "Popup content");

    ui_node* context_example = add(layout, content, UI_CARD, NULL);
    add(layout, context_example, UI_LABEL, "Tooltip / ContextMenu - Hover or right-click");
    ui_node* target = add(layout, context_example, UI_BUTTON, "Hover or right-click");
    handles[SHOWCASE_TARGET] = target;
    ui_node* tooltip = add(layout, context_example, UI_TOOLTIP, NULL);
    check_result(layout, ui_node_set_anchor(tooltip, target));
    add(layout, tooltip, UI_TEXT_BLOCK, "Tooltip from an anchor reference");

    ui_node* context = add(layout, context_example, UI_CONTEXT_MENU, NULL);
    handles[SHOWCASE_CONTEXT] = context;
    check_result(layout, ui_node_set_anchor(context, target));
    handles[SHOWCASE_CLOSE_CONTEXT] = add(layout, context, UI_BUTTON, "Close context menu");

    ui_node* dialog_example = add(layout, content, UI_CARD, NULL);
    add(layout, dialog_example, UI_LABEL, "Dialog - Open and dismiss");
    handles[SHOWCASE_SHOW_DIALOG] = add(layout, dialog_example, UI_BUTTON, "Open modal dialog");
    ui_node* dialog = add(layout, dialog_example, UI_DIALOG, NULL);
    handles[SHOWCASE_DIALOG] = dialog;
    add(layout, dialog, UI_TEXT_BLOCK, "Modal dialog");
    handles[SHOWCASE_CLOSE_DIALOG] = add(layout, dialog, UI_BUTTON, "Close");

    ui_node* spinner_example = add(layout, content, UI_CARD, NULL);
    add(layout, spinner_example, UI_LABEL, "Spinner");
    add(layout, spinner_example, UI_SPINNER, NULL);
    ui_node* tabs_example = add(layout, content, UI_CARD, NULL);
    add(layout, tabs_example, UI_LABEL, "TabView / TabItem");
    ui_node* tabs = add(layout, tabs_example, UI_TAB_VIEW, NULL);
    handles[SHOWCASE_TABS] = tabs;
    ui_node* overview = add(layout, tabs, UI_TAB_ITEM, "Overview");
    add(layout, overview, UI_TEXT_BLOCK, "First tab page");
    ui_node* details = add(layout, tabs, UI_TAB_ITEM, "Details");
    add(layout, details, UI_TEXT_BLOCK, "Second tab page");
}

ui_result build_showcase(ui_app* app, showcase_nodes* nodes)
{
    layout_builder layout = {app, nodes, ui_app_set_window_options(app, "Sunfleck Showcase", 1000, 760)};
    ui_node* root = add(&layout, NULL, UI_NAVIGATION_VIEW, NULL);
    nodes->handles[SHOWCASE_PAGES] = root;
    build_theme(&layout);
    build_layout(&layout);
    build_inputs(&layout);
    build_data(&layout);
    build_overlays(&layout);
    if (layout.status == UI_OK) { check_result(&layout, sunfleck_dock_demo_build(app, root, &nodes->handles[SHOWCASE_DOCK], &nodes->handles[SHOWCASE_DOCK_RESET])); }
    return layout.status == UI_OK ? ui_app_set_root(app, root) : layout.status;
}
