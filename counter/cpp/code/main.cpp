#include <sunfleck/ui.hpp>
#include <cstdio>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

struct Counter {
    sunfleck::Node label;
    unsigned value = 0;
    std::exception_ptr failure;
};

static void increment(ui_node*, void* context) noexcept
{
    auto& state = *static_cast<Counter*>(context);
    try {
        const auto text = "Count: " + std::to_string(++state.value);
        state.label.setText(text.c_str());
    } catch (...) {
        state.failure = std::current_exception();
    }
}

static int run(bool headless, const std::filesystem::path& state_file = {})
{
    try {
        const ui_app_options options{sizeof(options), UI_API_VERSION,
            static_cast<uint32_t>(headless ? UI_SOFTWARE : UI_AUTO), headless ? unsigned{UI_HEADLESS} : unsigned{UI_SHOW_RENDERER | UI_DEBUG_PIPE}};
        sunfleck::App app(options);

        app.setWindowOptions("Sunfleck Counter", 360, 180);
        auto stack = app.createNode(UI_STACK_PANEL);
        stack.setLayout(8, 16);

        auto label = app.createNode(UI_TEXT_BLOCK);
        Counter state{label};
        if (!state_file.empty()) {
            std::ifstream input(state_file);
            unsigned restored = 0;
            if (input >> restored) { state.value = restored; }
        }
        const auto initial_count = state.value;
        label.setText(("Count: " + std::to_string(state.value)).c_str());
        stack.append(label);

        auto button = app.createNode(UI_BUTTON);
        button.setText("Increment");
        stack.append(button);
        app.setRoot(stack);

        button.setAction(increment, &state);
#ifdef _WIN32
#endif
        app.openWindow();
        if (headless) {
            app.poll(0);
            app.activateButton();
            app.poll(0);
        } else {
            app.run();
        }
        if (state.failure) {
            std::rethrow_exception(state.failure);
        }
        if (!state_file.empty()) {
            auto pending = state_file;
            pending += ".tmp";
            std::ofstream output(pending, std::ios::trunc);
            output << state.value << '\n';
            output.close();
            if (!output) { throw std::runtime_error("Could not save counter state"); }
#ifdef _WIN32
            if (!MoveFileExW(pending.c_str(), state_file.c_str(), MOVEFILE_REPLACE_EXISTING)) {
                throw std::runtime_error("Could not publish counter state");
            }
#else
            std::filesystem::rename(pending, state_file);
#endif
        }
        return headless && label.text() != "Count: " + std::to_string(initial_count + 1) ? 1 : 0;
    } catch (const std::exception& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR arguments, int show)
{
    (void)instance;
    (void)previous;
    (void)show;
    const auto required = GetEnvironmentVariableW(L"SUNFLECK_RESTART_STATE_FILE", nullptr, 0);
    std::wstring path(required, L'\0');
    if (required) {
        const auto length = GetEnvironmentVariableW(L"SUNFLECK_RESTART_STATE_FILE", path.data(), required);
        if (length >= required) { return 1; }
        path.resize(length);
    }
    return run(std::strcmp(arguments, "--test") == 0, path);
}
#else
int main(int argc, char** argv)
{
    return run(argc == 2 && std::strcmp(argv[1], "--test") == 0);
}
#endif
