using Sunfleck;

internal sealed class ShowcaseLayout(App app)
{
    private readonly Dictionary<string, Node> nodes = new(StringComparer.Ordinal);

    public IReadOnlyDictionary<string, Node> Build()
    {
        app.SetWindowOptions("Sunfleck Showcase", 1000, 760);
        var pages = Add(null, NodeKind.NavigationView, id: "pages");
        BuildTheme(pages);
        BuildLayout(pages);
        BuildInputs(pages);
        BuildData(pages);
        BuildOverlays(pages);
        BuildDock(pages);
        app.SetRoot(pages);
        return nodes;
    }

    private Node Add(Node? parent, NodeKind kind, string? text = null, string? id = null)
    {
        var node = app.CreateNode(kind);
        if (text is not null)
        {
            node.Text = text;
        }
        if (id is not null)
        {
            nodes.Add(id, node);
        }
        parent?.Append(node);
        return node;
    }

    private void BuildDock(Node pages)
    {
        var page = Add(pages, NodeKind.TabItem, "Dock");
        var toolbar = Add(page, NodeKind.ToolBar);
        string[] ids = ["dock_reset", "dock_float", "dock_hide", "dock_show", "dock_save", "dock_restore"];
        string[] labels = ["Reset layout", "Float editor", "Auto hide", "Show tools", "Save layout", "Restore layout"];
        for (int index = 0; index < ids.Length; ++index)
        {
            Add(toolbar, NodeKind.Button, labels[index], ids[index]);
        }
        var host = Add(page, NodeKind.DockHost, id: "dock");
        var dock = app.CreateDock(host);
        var a = dock.Register("dock_editor_a", "Editor A", Add(null, NodeKind.TextArea, "Edit this document, then float or move its tab."));
        dock.Register("dock_editor_b", "Editor B", Add(null, NodeKind.TextArea, "A second document with independent content."));
        var explorer = dock.Register("dock_explorer", "Explorer", Add(null, NodeKind.TextArea, "Project files and folders"), DockKind.Tool);
        var output = dock.Register("dock_output", "Output", Add(null, NodeKind.TextArea, "Build completed successfully."), DockKind.Tool);
        output.Move(explorer, DockPosition.Bottom);
        a.Activate();
        dock.CaptureDefault();
    }

    private void BuildTheme(Node pages)
    {
        var tabItem = Add(pages, NodeKind.TabItem, "Theme");
        var scrollView = Add(tabItem, NodeKind.ScrollView);
        Add(scrollView, NodeKind.TextBlock, "Sunfleck 组件展厅 · 主题和命令");
        var appearance = Add(scrollView, NodeKind.Card);
        Add(appearance, NodeKind.Label, "ComboBox / Switch - Appearance");
        var theme = Add(appearance, NodeKind.ComboBox, id: "theme");
        Add(theme, NodeKind.ListItem, "Fluent 2");
        Add(theme, NodeKind.ListItem, "Material 3");
        Add(theme, NodeKind.ListItem, "Radix Themes");
        Add(theme, NodeKind.ListItem, "Apple Liquid Glass");
        Add(theme, NodeKind.ListItem, "Sunfleck Lite");
        Add(appearance, NodeKind.Switch, "Dark appearance", id: "dark");
        Add(appearance, NodeKind.Switch, "Compact density", id: "compact");
        var card = Add(scrollView, NodeKind.Card);
        Add(card, NodeKind.Label, "Card / Button - Counter commands");
        Add(card, NodeKind.TextBlock, "Count: 0", id: "count");
        Add(card, NodeKind.Button, "Increment via Command", id: "increment");
        Add(card, NodeKind.Button, "Reset counter", id: "reset");
    }

    private void BuildLayout(Node pages)
    {
        var tabItem = Add(pages, NodeKind.TabItem, "Layout");
        var scrollView = Add(tabItem, NodeKind.ScrollView);
        var stackPanel = Add(scrollView, NodeKind.StackPanel);
        stackPanel.SetLayout(10, 6);
        var panel = Add(stackPanel, NodeKind.Panel);
        Add(panel, NodeKind.TextBlock, "StackPanel → Panel → TextBlock");
        var flexExample = Add(stackPanel, NodeKind.Card);
        Add(flexExample, NodeKind.Label, "Flex / Badge / Button");
        var flex = Add(flexExample, NodeKind.Flex);
        var badge = Add(flex, NodeKind.Badge, "Fixed");
        badge.Width = 100;
        Add(flex, NodeKind.Button, "Growing Flex item");
        var gridExample = Add(stackPanel, NodeKind.Card);
        Add(gridExample, NodeKind.Label, "Grid / Card");
        var grid = Add(gridExample, NodeKind.Flex);
        var card = Add(grid, NodeKind.Card);
        card.Width = 130;
        Add(card, NodeKind.TextBlock, "Fixed track");
        Add(grid, NodeKind.TextBlock, "Star track");
        var wrapExample = Add(stackPanel, NodeKind.Card);
        Add(wrapExample, NodeKind.Label, "WrapPanel");
        var wrapPanel = Add(wrapExample, NodeKind.WrapPanel);
        var button = Add(wrapPanel, NodeKind.Button, "Wrap A");
        button.Width = 300;
        var button2 = Add(wrapPanel, NodeKind.Button, "Wrap B");
        button2.Width = 300;
        var button3 = Add(wrapPanel, NodeKind.Button, "Wrap C");
        button3.Width = 300;
        var dockExample = Add(stackPanel, NodeKind.Card);
        Add(dockExample, NodeKind.Label, "DockPanel");
        var dockPanel = Add(dockExample, NodeKind.DockPanel);
        var badge2 = Add(dockPanel, NodeKind.Badge, "Dock Bottom");
        badge2.SetDock(DockEdge.Bottom);
        Add(dockPanel, NodeKind.TextBlock, "Remaining area");
        var splitExample = Add(stackPanel, NodeKind.Card);
        Add(splitExample, NodeKind.Label, "SplitView");
        var splitView = Add(splitExample, NodeKind.SplitView);
        splitView.Value = 40;
        Add(splitView, NodeKind.TextBlock, "Drag the divider");
        Add(splitView, NodeKind.TextBlock, "Right pane");
        var overlayExample = Add(stackPanel, NodeKind.Card);
        Add(overlayExample, NodeKind.Label, "Overlay / ProgressBar");
        var overlay = Add(overlayExample, NodeKind.Overlay);
        var progressBar = Add(overlay, NodeKind.ProgressBar);
        progressBar.Value = 65;
        Add(overlay, NodeKind.TextBlock, "Overlay");
    }

    private void BuildInputs(Node pages)
    {
        var tabItem = Add(pages, NodeKind.TabItem, "Inputs");
        var scrollView = Add(tabItem, NodeKind.ScrollView);
        var toolbarExample = Add(scrollView, NodeKind.Card);
        Add(toolbarExample, NodeKind.Label, "ToolBar / InfoBar - Click an action");
        var toolBar = Add(toolbarExample, NodeKind.ToolBar);
        Add(toolBar, NodeKind.Button, "Button", id: "icon_button");
        Add(toolBar, NodeKind.ToggleButton, "ToggleButton");
        Add(toolBar, NodeKind.Hyperlink, "Hyperlink: show message", id: "link");
        Add(toolbarExample, NodeKind.InfoBar, "Try the toolbar or submit a search.", id: "notice");
        var choiceExample = Add(scrollView, NodeKind.Card);
        Add(choiceExample, NodeKind.Label, "CheckBox / Switch / RadioButton");
        Add(choiceExample, NodeKind.CheckBox, "CheckBox");
        Add(choiceExample, NodeKind.Switch, "Switch");
        var flex = Add(choiceExample, NodeKind.Flex);
        var radioButton = Add(flex, NodeKind.RadioButton, "First");
        radioButton.Value = 1;
        Add(flex, NodeKind.RadioButton, "Second");
        var rangeExample = Add(scrollView, NodeKind.Card);
        Add(rangeExample, NodeKind.Label, "SpinBox / Slider / ProgressBar");
        Add(rangeExample, NodeKind.Label, "SpinBox - Adjust the number");
        var spinBox = Add(rangeExample, NodeKind.SpinBox);
        spinBox.Value = 12;
        Add(rangeExample, NodeKind.Label, "Slider - Drag to update the ProgressBar");
        var slider = Add(rangeExample, NodeKind.Slider, id: "slider");
        slider.Value = 35;
        Add(rangeExample, NodeKind.Label, "ProgressBar - Follows the slider");
        var progress = Add(rangeExample, NodeKind.ProgressBar, id: "progress");
        progress.Value = 35;
        Add(rangeExample, NodeKind.Label, "Separator");
        Add(rangeExample, NodeKind.Separator);
        var editorExample = Add(scrollView, NodeKind.Card);
        Add(editorExample, NodeKind.Label, "Label / TextBox - Click the label to focus");
        var label = Add(editorExample, NodeKind.Label, "Click this label to focus the editor");
        Add(editorExample, NodeKind.TextBox, "Edit this text / 编辑", id: "editor");
        var bindingExample = Add(scrollView, NodeKind.Card);
        Add(bindingExample, NodeKind.Label, "TextBox - TwoWay and OneTime bindings");
        Add(bindingExample, NodeKind.TextBlock, "TwoWay: edit either field to update both");
        Add(bindingExample, NodeKind.TextBox, id: "bound_editor");
        Add(bindingExample, NodeKind.TextBox, id: "bound_peer");
        Add(bindingExample, NodeKind.TextBlock, "OneTime snapshot:");
        Add(bindingExample, NodeKind.TextBlock, id: "snapshot");
        var textInputs = Add(scrollView, NodeKind.Card);
        Add(textInputs, NodeKind.Label, "PasswordBox / TextArea / SearchBox");
        Add(textInputs, NodeKind.Label, "PasswordBox");
        Add(textInputs, NodeKind.PasswordBox, "Secret");
        Add(textInputs, NodeKind.Label, "TextArea");
        Add(textInputs, NodeKind.TextArea, "First line\n第二行");
        Add(textInputs, NodeKind.Label, "SearchBox - Submit to update the InfoBar");
        Add(textInputs, NodeKind.SearchBox, id: "search");
        var imageExample = Add(scrollView, NodeKind.Card);
        Add(imageExample, NodeKind.Label, "Image / Icon");
        var flex2 = Add(imageExample, NodeKind.Flex);
        var sampleImage = Add(flex2, NodeKind.Image, id: "sample_image");
        sampleImage.Width = 80;
        Add(flex2, NodeKind.Image, id: "sample_icon");
        label.SetAnchor(nodes["editor"]);
    }

    private void BuildData(Node pages)
    {
        var tabItem = Add(pages, NodeKind.TabItem, "Data");
        var scrollView = Add(tabItem, NodeKind.ScrollView);
        var comboExample = Add(scrollView, NodeKind.Card);
        Add(comboExample, NodeKind.Label, "ComboBox / ListItem");
        var comboBox = Add(comboExample, NodeKind.ComboBox);
        Add(comboBox, NodeKind.ListItem, "Alpha");
        Add(comboBox, NodeKind.ListItem, "Beta");
        Add(comboBox, NodeKind.ListItem, "Gamma");
        var listExample = Add(scrollView, NodeKind.Card);
        Add(listExample, NodeKind.Label, "ListView / ListItem");
        var listView = Add(listExample, NodeKind.ListView);
        Add(listView, NodeKind.ListItem, "List Alpha");
        Add(listView, NodeKind.ListItem, "List Beta");
        Add(listView, NodeKind.ListItem, "List Gamma");
        var itemsExample = Add(scrollView, NodeKind.Card);
        Add(itemsExample, NodeKind.Label, "ItemsControl");
        Add(itemsExample, NodeKind.ItemsControl, id: "items");
        var treeExample = Add(scrollView, NodeKind.Card);
        Add(treeExample, NodeKind.Label, "TreeView / TreeItem");
        var treeView = Add(treeExample, NodeKind.TreeView);
        var treeItem = Add(treeView, NodeKind.TreeItem, "level 1, item 1");
        Add(treeItem, NodeKind.TreeItem, "level 2, item 1");
        Add(treeItem, NodeKind.TreeItem, "level 2, item 2");
        Add(treeItem, NodeKind.TreeItem, "level 2, item 3");
        treeItem.Value = 1;
        var collapsed = Add(treeView, NodeKind.TreeItem, "level 1, item 2");
        Add(collapsed, NodeKind.TreeItem, "level 2, item 1");
        Add(treeView, NodeKind.TreeItem, "level 1, item 3");
        var tableExample = Add(scrollView, NodeKind.Card);
        Add(tableExample, NodeKind.Label, "DataGrid / Badge / TextBox");
        Add(tableExample, NodeKind.TextBlock, "DataGrid: scroll and click a value to edit");
        var table = Add(tableExample, NodeKind.DataGrid, id: "table");
        for (int index = 1; index <= 12; index++)
        {
            var row = Add(table, NodeKind.Grid);
            row.SetGridTracks([], [new(GridUnit.Pixels,90), new(GridUnit.Star)]);
            Add(row, NodeKind.Badge, $"Row {index:00}").Width = 90;
            Add(row, NodeKind.TextBox, $"Editable value {index}", index == 1 ? "cell" : null).SetGridCell(0,1);
        }
    }

    private void BuildOverlays(Node pages)
    {
        var tabItem = Add(pages, NodeKind.TabItem, "Overlays");
        var scrollView = Add(tabItem, NodeKind.ScrollView);
        var menuExample = Add(scrollView, NodeKind.Card);
        Add(menuExample, NodeKind.Label, "MenuBar / ComboBox - Open Actions");
        var menuBar = Add(menuExample, NodeKind.MenuBar);
        var menu = Add(menuBar, NodeKind.ComboBox, "Actions", id: "menu");
        Add(menu, NodeKind.ListItem, "Show dialog");
        Add(menu, NodeKind.ListItem, "Show popup");
        var popupExample = Add(scrollView, NodeKind.Card);
        Add(popupExample, NodeKind.Label, "Popup - Click the button to toggle");
        Add(popupExample, NodeKind.Button, "Toggle popup", id: "show_popup");
        var popup = Add(popupExample, NodeKind.Popup, id: "popup");
        Add(popup, NodeKind.InfoBar, "Popup content");
        var contextExample = Add(scrollView, NodeKind.Card);
        Add(contextExample, NodeKind.Label, "Tooltip / ContextMenu - Hover or right-click");
        Add(contextExample, NodeKind.Button, "Hover or right-click", id: "target");
        var tooltip = Add(contextExample, NodeKind.Tooltip);
        Add(tooltip, NodeKind.TextBlock, "Tooltip from an anchor reference");
        var context = Add(contextExample, NodeKind.ContextMenu, id: "context");
        Add(context, NodeKind.Button, "Close context menu", id: "close_context");
        var dialogExample = Add(scrollView, NodeKind.Card);
        Add(dialogExample, NodeKind.Label, "Dialog - Open and dismiss");
        Add(dialogExample, NodeKind.Button, "Open modal dialog", id: "show_dialog");
        var dialog = Add(dialogExample, NodeKind.Dialog, id: "dialog");
        Add(dialog, NodeKind.TextBlock, "Modal dialog");
        Add(dialog, NodeKind.Button, "Close", id: "close_dialog");
        var spinnerExample = Add(scrollView, NodeKind.Card);
        Add(spinnerExample, NodeKind.Label, "Spinner");
        Add(spinnerExample, NodeKind.Spinner);
        var tabsExample = Add(scrollView, NodeKind.Card);
        Add(tabsExample, NodeKind.Label, "TabView / TabItem");
        var tabs = Add(tabsExample, NodeKind.TabView, id: "tabs");
        var tabItem2 = Add(tabs, NodeKind.TabItem, "Overview");
        Add(tabItem2, NodeKind.TextBlock, "First tab page");
        var tabItem3 = Add(tabs, NodeKind.TabItem, "Details");
        Add(tabItem3, NodeKind.TextBlock, "Second tab page");
        popup.SetAnchor(nodes["show_popup"]);
        tooltip.SetAnchor(nodes["target"]);
        context.SetAnchor(nodes["target"]);
    }
}
