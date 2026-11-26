using Sunfleck;

internal sealed class ShowcaseController(App app, Func<string, Node> find, TextSource countText, TextSource sharedText)
{
    private int count;
    private string? savedDockLayout;

    private void DockAction(string action)
    {
        var dock = app.CreateDock(find("dock"));
        switch (action)
        {
            case "reset":
                dock.Reset();
                break;
            case "float":
                dock.Find("dock_editor_a").Float(new DockRectangle(160, 140, 480, 320));
                break;
            case "hide":
                dock.Find("dock_explorer").AutoHide();
                break;
            case "show":
                var explorer = dock.Find("dock_explorer");
                if (explorer.State == DockState.AutoHidden)
                {
                    explorer.Pin();
                }
                else
                {
                    explorer.Show();
                }
                dock.Find("dock_output").Show();
                break;
            case "save":
                savedDockLayout = dock.Save();
                break;
            case "restore":
                if (savedDockLayout is not null)
                {
                    dock.Restore(savedDockLayout);
                }
                break;
        }
    }

    public void Connect()
    {
        find("count").BindText(countText);
        find("bound_editor").BindText(sharedText, BindingMode.TwoWay);
        find("bound_peer").BindText(sharedText, BindingMode.TwoWay);
        find("snapshot").BindText(sharedText, BindingMode.OneTime);
        foreach (var (id, action) in Actions())
        {
            find(id).BindCommand(app.CreateCommand(action));
        }
    }

    public FleckDataContext ExportBindings()
    {
        var context = new FleckDataContext().AddText("CountText", countText).AddText("SharedText", sharedText);
        foreach (var (id, action) in Actions())
        {
            context.AddCommand(id, app.CreateCommand(action));
        }
        return context;
    }

    public void InitializeResources()
    {
        find("theme").Value = (uint)BuiltinTheme.SunfleckLite;
        find("items").SetItems("One", "Two", "Three");
        byte[] rgb = [36, 107, 206, 255, 211, 105, 28, 125, 81, 36, 107, 206];
        foreach (var id in new[] { "sample_image", "sample_icon" })
        {
            find(id).SetImage(rgb, 2, 2);
        }
    }

    private (string Id, Action<Node> Action)[] Actions() =>
    [
        ("dock_reset", _ => DockAction("reset")),
        ("dock_float", _ => DockAction("float")),
        ("dock_hide", _ => DockAction("hide")),
        ("dock_show", _ => DockAction("show")),
        ("dock_save", _ => DockAction("save")),
        ("dock_restore", _ => DockAction("restore")),
        ("increment", _ => countText.Set($"Count: {++count}")),
        ("reset", _ =>
        {
            count = 0;
            countText.Set("Count: 0");
        }),
        ("theme", _ => ApplyTheme()),
        ("dark", _ => ApplyTheme()),
        ("compact", _ => ApplyTheme()),
        ("slider", node => find("progress").Value = node.Value),
        ("icon_button", _ => ShowNotice()),
        ("link", _ => ShowNotice()),
        ("search", node => find("notice").Text = node.Text),
        ("show_popup", _ => Toggle("popup")),
        ("show_dialog", _ => Toggle("dialog")),
        ("close_dialog", _ => Toggle("dialog")),
        ("close_context", _ => Toggle("context")),
        ("menu", node => find(node.Value == 0 ? "dialog" : "popup").Value = 1)
    ];

    private void ApplyTheme()
    {
        var theme = (BuiltinTheme)find("theme").Value;
        app.SetTheme(theme, find("dark").Value != 0, find("compact").Value != 0);
    }

    private void Toggle(string id)
    {
        var node = find(id);
        node.Value = node.Value == 0 ? 1u : 0u;
    }

    private void ShowNotice() => find("notice").Text = "Action received from the toolbar.";

    public void Snapshot(string path)
    {
        using (var stream = File.Create(path))
        {
            for (uint page = 0; page < 6; page++)
            {
                find("pages").Value = page;
                app.Poll();
                stream.Write(app.ReadPixels().Rgba);
            }
        }
    }

    public bool Verify()
    {
        app.Poll();
        app.ActivateButton();
        if (find("count").Text != "Count: 1")
        {
            return false;
        }

        sharedText.Set("Updated by C#");
        if (find("bound_editor").Text != "Updated by C#" ||
            find("bound_peer").Text != "Updated by C#" || find("snapshot").Text != "Shared text")
        {
            return false;
        }

        find("theme").Value = (uint)BuiltinTheme.Material3;
        find("dark").Value = 1;
        ApplyTheme();
        app.Poll();
        var frame = app.ReadPixels();
        // Sample the padding, away from the antialiased corner rim.
        int background = frame.Width >= 16 && frame.Height >= 8
            ? checked((int)(((frame.Height - 8) * frame.Width + frame.Width - 16) * 4)) : 0;
        if (frame.Width != 1000 || frame.Height != 760 ||
            frame.Rgba[background] != 0x17 || frame.Rgba[background + 1] != 0x12 || frame.Rgba[background + 2] != 0x1b)
        {
            return false;
        }

        for (uint page = 0; page < 5; page++)
        {
            find("pages").Value = page;
            app.Poll();
        }
        app.ActivateButton();
        return find("popup").Value == 1;
    }
}
