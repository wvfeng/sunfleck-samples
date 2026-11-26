using Sunfleck;
using System.Globalization;

bool headless = args.Contains("--test");
bool smoke = args.Contains("--smoke");
string? stateFile = Environment.GetEnvironmentVariable("SUNFLECK_RESTART_STATE_FILE");
int count = 0;
if (!string.IsNullOrEmpty(stateFile) && File.Exists(stateFile))
{
    int.TryParse(File.ReadAllText(stateFile).Trim(), NumberStyles.None, CultureInfo.InvariantCulture, out count);
}
int initialCount = count;
using (var app = new App(headless ? Renderer.Software : Renderer.Auto, headless, showRenderer: true))
{
    app.SetWindowOptions("Sunfleck C# Counter", 360, 180);
    var stack = app.CreateNode(NodeKind.StackPanel);
    stack.SetLayout(8, 16);
    var label = app.CreateNode(NodeKind.TextBlock);
    label.Text = $"Count: {count}";
    var button = app.CreateNode(NodeKind.Button);
    button.Text = "Increment";
    stack.Append(label);
    stack.Append(button);
    app.SetRoot(stack);

    button.SetAction(_ => label.Text = $"Count: {++count}");
    app.OpenWindow();
    if (smoke && args.Length == 2)
    {
        app.Poll();
        File.WriteAllText(args[1], "ready");
    }
    if (headless)
    {
        app.Poll();
        app.ActivateButton();
        app.Poll();
    }
    else
    {
        app.Run();
    }
    if ((headless || smoke) && label.Text != $"Count: {initialCount + 1}")
    {
        return 1;
    }
    if (!string.IsNullOrEmpty(stateFile))
    {
        string pending = stateFile + ".tmp";
        File.WriteAllText(pending, count.ToString(CultureInfo.InvariantCulture));
        File.Move(pending, stateFile, overwrite: true);
    }
    return 0;
}
