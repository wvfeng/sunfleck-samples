using Sunfleck;

bool snapshot = args.Length == 2 && args[0] == "--snapshot";
bool test = args.Length == 1 && args[0] == "--test";
if (args.Length != 0 && !test && !snapshot)
{
    return 1;
}
try
{
    using (var app = new App(test || snapshot ? Renderer.Software : Renderer.Auto, headless: test || snapshot, showRenderer: true))
    {
        using (var count = app.CreateTextSource("Count: 0"))
        {
            using (var shared = app.CreateTextSource("Shared text"))
            {
                var controller = new ShowcaseController(app, app.FindFleckNode, count, shared);
                var resource = Path.Combine(AppContext.BaseDirectory, "showcase.flkb");
                app.LoadFleckResource(File.ReadAllBytes(resource), controller.ExportBindings());
                controller.InitializeResources();
                app.OpenWindow();
                if (test)
                {
                    return controller.Verify() ? 0 : 1;
                }
                if (snapshot)
                {
                    controller.Snapshot(args[1]);
                    return 0;
                }
                app.Run();
                return 0;
            }
        }
    }
}
catch (Exception error)
{
    Console.Error.WriteLine(error);
    return 1;
}
