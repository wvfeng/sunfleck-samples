#ifndef SUNFLECK_COUNTER_SOURCE_MAP_HELPER
#define SUNFLECK_COUNTER_SOURCE_MAP_HELPER
#include <sunfleck/ui.h>
#include <stdio.h>
#include <stdlib.h>

/* Development sidecar generated with the page; never used to reconstruct UI. */
static ui_result attach_counter_source_map(ui_app* app)
{
    FILE* input = fopen(SUNFLECK_COUNTER_SOURCE_MAP, "rb");
    if (!input) { return UI_ERROR_PLATFORM; }
    if (fseek(input, 0, SEEK_END) != 0) { fclose(input); return UI_ERROR_PLATFORM; }
    const long length = ftell(input);
    if (length < 0 || fseek(input, 0, SEEK_SET) != 0) { fclose(input); return UI_ERROR_PLATFORM; }
    char* text = (char*)malloc((size_t)length + 1);
    if (!text) { fclose(input); return UI_ERROR_OUT_OF_MEMORY; }
    const size_t read = fread(text, 1, (size_t)length, input);
    fclose(input);
    const ui_result result = read == (size_t)length ? ui_app_attach_fleck_source_map(app, text, read, NULL) : UI_ERROR_PLATFORM;
    free(text);
    return result;
}
#endif
