#include <sunfleck/ui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../source_map.h"

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct counter {
    ui_node* label;
    ui_app* app;
    unsigned value;
    ui_result status;
    ui_model* model;
    const char* caption;
} counter;

static void pause_ms(unsigned milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    const struct timespec pause = {milliseconds / 1000, (milliseconds % 1000) * 1000000};
    nanosleep(&pause, NULL);
#endif
}
static void show_progress(void* context)
{
    counter* state = context;
    state->caption = "Incrementing...";
    state->status = ui_model_notify_property_changed(state->model, "caption");
}
static ui_result increment(void* context, const ui_cancellation_token* cancellation)
{
    counter* state = context;
    ui_result result = ui_app_post_cancellable(state->app, cancellation, show_progress, state, NULL);
    if (result != UI_OK) { return result; }
    pause_ms(80);
    return ui_cancellation_requested(cancellation) ? UI_CANCELLED : UI_OK;
}
static void completed(void* context, ui_result result)
{
    counter* state = context;
    if (result == UI_OK) { ++state->value; }
    state->caption = "Increment";
    state->status = ui_model_notify_property_changed(state->model, NULL);
    if (result != UI_OK && result != UI_CANCELLED) { state->status = result; }
}

static ui_result get_count(void* context, ui_binding_value* value)
{
    value->kind = UI_VALUE_INT;
    value->integer = ((counter*)context)->value;
    return UI_OK;
}
static ui_result get_caption(void* context, ui_binding_value* value)
{
    value->kind = UI_VALUE_STRING;
    value->string = ((counter*)context)->caption;
    return UI_OK;
}

ui_result load_counter(ui_app* app, ui_fleck_diagnostic* diagnostic);
ui_result load_counter_with_model(ui_app* app, ui_model* model, const ui_fleck_data_context* context, ui_fleck_diagnostic* diagnostic);

static ui_result build_page(ui_app* app, counter* state, ui_node** label, ui_node** button)
{
    state->app = app;
    ui_command* command = NULL;
    ui_result status = ui_model_create(app, &state->model);
    const ui_model_property property = {sizeof(property), 1, "count", UI_VALUE_INT, get_count, NULL, state};
    const ui_model_property caption = {sizeof(caption), 1, "caption", UI_VALUE_STRING, get_caption, NULL, state};
    const ui_async_command_options options = {sizeof(options), 2, increment, NULL, completed, state};
    if (status == UI_OK) { status = ui_model_add_property(state->model, &property); }
    if (status == UI_OK) { status = ui_model_add_property(state->model, &caption); }
    if (status == UI_OK) { status = ui_command_create_async(app, &options, &command); }
    if (status == UI_OK) { status = ui_model_add_command(state->model, "increment", command); }
    if (status == UI_OK) { status = load_counter_with_model(app, state->model, NULL, NULL); }
    if (status == UI_OK) { status = attach_counter_source_map(app); }
    if (status == UI_OK) {
        status = ui_app_find_fleck_node(app, "count", label);
    }
    if (status == UI_OK) {
        status = ui_app_find_fleck_node(app, "increment", button);
    }
    return status;
}

static int verify(ui_app* app, const counter* state, ui_node* button)
{
    char text[32];
    uint32_t size = sizeof(text);
    if (ui_app_poll(app, 0) != UI_OK || ui_app_activate_button(app) != UI_OK || ui_app_activate_button(app) != UI_OK) { return 0; }
    int saw_progress = 0;
    for (unsigned i = 0; i < 3000 && state->value == 0; ++i) {
        const ui_result polled = ui_app_poll(app, 0);
        if (polled != UI_OK && polled != UI_TIMEOUT) { fprintf(stderr, "Counter poll failed: %d\n", (int)polled); return 0; }
        size = sizeof(text);
        if (ui_node_get_text(button, text, &size) != UI_OK) { return 0; }
        saw_progress |= strcmp(text, "Incrementing...") == 0;
        pause_ms(1);
    }
    size = sizeof(text);
    return saw_progress && state->value == 1 &&
        ui_node_get_text(state->label, text, &size) == UI_OK &&
        strcmp(text, "Count: 1") == 0;
}

static int run(int headless)
{
    const ui_app_options options = {sizeof(options), UI_API_VERSION,
        headless ? UI_SOFTWARE : UI_AUTO, headless ? UI_HEADLESS : UI_SHOW_RENDERER | UI_DEBUG_PIPE};
    ui_app* app = NULL;
    if (ui_app_create(&options, &app) != UI_OK) {
        return 1;
    }

    counter state = {0};
    state.caption = "Increment";
    ui_node* button = NULL;
    int result = 1;
    const ui_result built = build_page(app, &state, &state.label, &button);
    if (built == UI_OK && ui_app_open_window(app) == UI_OK) {
        result = headless ? !verify(app, &state, button) : ui_app_run(app) != UI_EXITED;
    } else {
        fprintf(stderr, "Unable to load counter: %d\n", (int)built);
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
