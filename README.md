# Sunfleck Samples

[计数器](counter)与[组件展厅](showcase)各有 C、C++、C# 的纯代码版和 Fleck 版。需要已安装的桌面 SDK。

```powershell
$sdk = (Resolve-Path ../sdk/local/windows-x64).Path
cmake -S . -B out/local -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$sdk"
cmake --build out/local
ctest --test-dir out/local --output-on-failure
dotnet build counter/csharp/fleck/CounterFleck.csproj "-p:SunfleckSdkRoot=$sdk"
```

C/C++ 也可把 `-S` 指向具体项目目录。C# Fleck 项目构建时调用同一 SDK 的 `fleckc`，可用 `--test` 运行无窗口验证。


## VS Code 调试

在 Sunfleck 多根工作区的 CMake 视图中选中 samples，使用 `vs2026-debug` 作为配置、构建及测试预设。选择可执行目标，例如 `counter_cpp_code`，然后运行 CMake 的调试命令。

预设从 `sdk/local/windows-x64-debug` 加载 Debug SDK。在工作区根目录执行以下命令即可构建并安装。

```powershell
cmake -S core -B sdk/local/build-windows-debug -G "Visual Studio 18 2026" -A x64 -DBUILD_TESTING=OFF
cmake --build sdk/local/build-windows-debug --config Debug --target sunfleck fleckc
cmake --install sdk/local/build-windows-debug --config Debug --prefix sdk/local/windows-x64-debug
```
