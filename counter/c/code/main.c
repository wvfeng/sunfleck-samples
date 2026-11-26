#include <sunfleck/ui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct counter {
    ui_node* label;
    unsigned value;
    ui_result status;
} counter;

static void increment(ui_node* button, void* context)
{
    (void)button;
    counter* state = context;
    char text[32];
    snprintf(text, sizeof(text), "Count: %u", ++state->value);
    state->status = ui_node_set_text(state->label, text);
}

static ui_result build_page(ui_app* app, ui_node** label, ui_node** button)
{
    ui_node* stack = NULL;
    if (ui_app_set_window_options(app, "Sunfleck Counter", 360, 180) != UI_OK ||
        ui_node_create(app, UI_STACK_PANEL, &stack) != UI_OK ||
        ui_node_set_layout(stack, 8, 16) != UI_OK ||
        ui_node_create(app, UI_TEXT_BLOCK, label) != UI_OK ||
        ui_node_append(stack, *label) != UI_OK ||
        ui_node_create(app, UI_BUTTON, button) != UI_OK ||
        ui_node_set_text(*button, "Increment") != UI_OK ||
        ui_node_append(stack, *button) != UI_OK) {
        return UI_ERROR_INVALID_STATE;
    }
    return ui_app_set_root(app, stack);
}

static int verify(ui_app* app, const counter* state, unsigned initial)
{
    char text[32];
    char expected[32];
    snprintf(expected, sizeof(expected), "Count: %u", initial + 1);
    uint32_t size = sizeof(text);
    return ui_app_poll(app, 0) == UI_OK &&
        ui_app_activate_button(app) == UI_OK && state->status == UI_OK &&
        ui_app_poll(app, 0) == UI_OK &&
        ui_node_get_text(state->label, text, &size) == UI_OK &&
        strcmp(text, expected) == 0;
}

#ifdef _WIN32
static int save_counter(const wchar_t* path, unsigned value)
{
    const size_t length = wcslen(path);
    wchar_t* pending = malloc((length + 5) * sizeof(*pending));
    if (!pending) { return 0; }
    memcpy(pending, path, length * sizeof(*pending));
    memcpy(pending + length, L".tmp", 5 * sizeof(*pending));
    FILE* output = _wfopen(pending, L"w");
    int saved = 0;
    if (output) {
        const int written = fprintf(output, "%u\n", value) > 0;
        saved = fclose(output) == 0 && written && MoveFileExW(pending, path, MOVEFILE_REPLACE_EXISTING);
        if (!saved) { DeleteFileW(pending); }
    }
    free(pending);
    return saved;
}
#endif

static int run(int headless)
{
    const ui_app_options options = {sizeof(options), UI_API_VERSION,
        headless ? UI_SOFTWARE : UI_AUTO, headless ? UI_HEADLESS : UI_SHOW_RENDERER | UI_DEBUG_PIPE};
    ui_app* app = NULL;
    if (ui_app_create(&options, &app) != UI_OK) {
        return 1;
    }

    counter state = {0};
#ifdef _WIN32
    const wchar_t* state_file = _wgetenv(L"SUNFLECK_RESTART_STATE_FILE");
    if (state_file && *state_file) {
        FILE* input = _wfopen(state_file, L"r");
        if (input) {
            if (fscanf(input, "%u", &state.value) != 1) { state.value = 0; }
            fclose(input);
        }
    }
#endif
    const unsigned initial = state.value;
    char initial_text[32];
    snprintf(initial_text, sizeof(initial_text), "Count: %u", initial);
    ui_node* button = NULL;
    int result = 1;
    if (build_page(app, &state.label, &button) == UI_OK &&
#ifdef _WIN32
#endif
        ui_node_set_text(state.label, initial_text) == UI_OK &&
        ui_node_set_action(button, increment, &state) == UI_OK &&
        ui_app_open_window(app) == UI_OK) {
        result = headless ? !verify(app, &state, initial) : ui_app_run(app) != UI_EXITED || state.status != UI_OK;
#ifdef _WIN32
        if (result == 0 && state_file && *state_file && !save_counter(state_file, state.value)) {
            result = 1;
        }
#endif
    }
    ui_app_destroy(app);
    return result;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR arguments, int show)
{
    (void)instance;
    (void)previous;
    (void)show;
    return run(strcmp(arguments, "--test") == 0);
}
#else
int main(int argc, char** argv)
{
    return run(argc == 2 && strcmp(argv[1], "--test") == 0);
}
#endif
