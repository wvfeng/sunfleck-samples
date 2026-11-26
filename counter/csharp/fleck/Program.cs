using System.ComponentModel;
using System.Globalization;
using System.Reflection.Metadata;
using System.Text.Json;
using Sunfleck;

[assembly: MetadataUpdateHandler(typeof(CounterHotReload))]

bool headless = args.Contains("--test");
bool smoke = args.Contains("--smoke");
int reloadArgument = Array.IndexOf(args, "--watch-fleck");
string? reloadPath = reloadArgument >= 0 && reloadArgument + 1 < args.Length
    ? Path.GetFullPath(args[reloadArgument + 1]) : null;
int metadataArgument = Array.IndexOf(args, "--live-model-file");
string? metadataPath = metadataArgument >= 0 && metadataArgument + 1 < args.Length
    ? Path.GetFullPath(args[metadataArgument + 1]) : null;
using (var app = new App(headless ? Renderer.Software : Renderer.Auto, headless, showRenderer: true))
{
    string? statePath = Environment.GetEnvironmentVariable("SUNFLECK_RESTART_STATE_FILE");
    int initialCount = 0;
    if (!string.IsNullOrEmpty(statePath) && File.Exists(statePath))
    {
        int.TryParse(File.ReadAllText(statePath).Trim(), NumberStyles.None, CultureInfo.InvariantCulture, out initialCount);
    }
    var model = new CounterModel(initialCount);
    using (var context = app.CreatePropertyModel(model))
    {
        using (var increment = app.CreateAsyncCommand(async cancellationToken =>
        {
            await model.IncrementAsync(app, cancellationToken);
            await app.InvokeAsync(() =>
            {
                if (!string.IsNullOrEmpty(statePath))
                {
                    string pending = statePath + ".tmp";
                    File.WriteAllText(pending, model.Count.ToString(CultureInfo.InvariantCulture));
                    File.Move(pending, statePath, overwrite: true);
                }
            });
        }))
        {
            model.IncrementCount = increment;
            PublishManagedMetadata(metadataPath);
            var resource = Path.Combine(AppContext.BaseDirectory, "counter.flkb");
            string? initialSource = null;
            if (reloadPath is null)
            {
                app.LoadFleckResource(File.ReadAllBytes(resource), context);
                string sourceMap = resource + ".fleckmap";
                if (File.Exists(sourceMap)) { app.AttachFleckSourceMap(File.ReadAllText(sourceMap)); }
            }
            else
            {
                initialSource = File.ReadAllText(reloadPath);
                app.LoadFleck(initialSource, context);
                app.SetFleckSourceUri(new Uri(reloadPath).AbsoluteUri);
            }
            app.OpenWindow();
            if (args.Contains("--pick") && !headless)
            {
                app.SetDebugPicking(true);
            }
            if (smoke && args.Length == 2)
            {
                app.Poll();
                File.WriteAllText(args[1], "ready");
            }
            if (headless)
            {
                app.Poll();
                app.ActivateButton();
                bool sawProgress = false;
                var deadline = DateTime.UtcNow.AddSeconds(5);
                while (increment.IsRunning && DateTime.UtcNow < deadline)
                {
                    app.Poll();
                    sawProgress |= app.FindFleckNode("increment").Text == "Incrementing...";
                    Thread.Sleep(1);
                }
                if (!sawProgress || increment.IsRunning)
                {
                    return 1;
                }
            }
            else if (reloadPath is not null && initialSource is not null)
            {
                WatchFleck(app, context, reloadPath, initialSource, metadataPath);
            }
            else if (smoke && args.Length == 2)
            {
                bool reportedClick = false;
                while (app.Poll(16) != Result.Exited)
                {
                    if (!reportedClick && app.FindFleckNode("count").Text == $"Count: {initialCount + 1}")
                    {
                        File.WriteAllText(args[1] + ".clicked", "ready");
                        reportedClick = true;
                    }
                }
            }
            else
            {
                while (app.Poll(16) != Result.Exited)
                {
                    RefreshManagedBindings(context, metadataPath);
                }
            }
            return !headless && !smoke || app.FindFleckNode("count").Text == $"Count: {initialCount + 1}" ? 0 : 1;
        }
    }
}

static void WatchFleck(App app, PropertyModel model, string path, string initialSource, string? metadataPath)
{
    var lastAttempt = initialSource;
    var pending = initialSource;
    long stableSince = 0;
    long nextRead = 0;
    ulong reportedRefresh = app.GetFleckRefreshStatus().Sequence;
    while (app.Poll(16) != Result.Exited)
    {
        RefreshManagedBindings(model, metadataPath);
        var refresh = app.GetFleckRefreshStatus();
        if (refresh.Sequence != reportedRefresh)
        {
            reportedRefresh = refresh.Sequence;
            if (refresh.Result != Result.Ok)
            {
                Console.Error.WriteLine(refresh.Diagnostic?.Message ?? refresh.Result.ToString());
            }
        }
        long now = Environment.TickCount64;
        if (now < nextRead)
        {
            continue;
        }
        nextRead = now + 150;
        try
        {
            var source = File.ReadAllText(path);
            if (source != pending)
            {
                pending = source;
                stableSince = now;
                continue;
            }
            if (source == lastAttempt || now - stableSince < 200)
            {
                continue;
            }
            app.QueueFleckRefresh(source, model);
            lastAttempt = source;
        }
        catch (IOException)
        {
            // Editors may replace the file during save. Retry on the next scan.
        }
    }
}

static void PublishManagedMetadata(string? path)
{
    if (path is null)
    {
        return;
    }
    using var metadata = JsonDocument.Parse(PropertyModel.DescribeType<CounterModel>());
    string pending = path + ".tmp";
    File.WriteAllText(pending, JsonSerializer.Serialize(new { version = 1, models = new[] { metadata.RootElement } }));
    File.Move(pending, path, overwrite: true);
}

static void RefreshManagedBindings(PropertyModel model, string? metadataPath)
{
    if (!CounterHotReload.TakeUpdate())
    {
        return;
    }
    try
    {
        model.NotifyPropertyChanged();
        PublishManagedMetadata(metadataPath);
    }
    catch (SunfleckException error) when (error.Result == Result.InvalidState)
    {
        CounterHotReload.UpdateApplication(null);
    }
}

internal static class CounterHotReload
{
    private static int pending;
    internal static bool TakeUpdate() => Interlocked.Exchange(ref pending, 0) != 0;
    public static void UpdateApplication(Type[]? _) => Interlocked.Exchange(ref pending, 1);
}

internal sealed class CounterModel(int count) : INotifyPropertyChanged
{
    public event PropertyChangedEventHandler? PropertyChanged;
    public int Count { get; private set; } = count;
    public string ButtonText { get; private set; } = "Increment";
    public string Caption => "Current count";
    public bool CanIncrement => Count < 10;
    [CommandModes(CommandExecutionModes.Async)]
    public Command IncrementCount { get; set; } = null!;
    public async Task IncrementAsync(App app, CancellationToken cancellationToken)
    {
        await app.InvokeAsync(() =>
        {
            ButtonText = "Incrementing...";
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(nameof(ButtonText)));
        });
        await Task.Delay(80, cancellationToken);
        await app.InvokeAsync(() =>
        {
            ++Count;
            ButtonText = "Increment";
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(null));
        });
    }
}
