#include "showcase.h"
#include "showcase_model.hpp"
#include "showcase_bindings.hpp"

#include <cstring>

int main()
{
    using sunfleck::detail::check;
    const ui_app_options options{sizeof(options), UI_API_VERSION, UI_SOFTWARE, UI_HEADLESS};
    sunfleck::App code(options), fleck(options);

    showcase_nodes codeNodes{};
    showcase_state codeDemo{};
    check(build_showcase(code.nativeHandle(), &codeNodes));
    check(showcase_connect(&codeDemo, code.nativeHandle(), &codeNodes));

    ShowcaseModel model(fleck);
    auto page = sunfleck::generated::load_showcase::load(fleck, model);
    model.initializeResources();

    code.openWindow();
    fleck.openWindow();
    const auto fleckPages = fleck.findFleckNode("pages");
    for (uint32_t index = 0; index < 6; ++index) {
        check(ui_node_set_value(codeNodes.handles[SHOWCASE_PAGES], index));
        check(ui_node_set_value(fleckPages.nativeHandle(), index));
        code.poll(0);
        page->poll(0);

        uint32_t width = 0, height = 0, otherWidth = 0, otherHeight = 0;
        const auto* pixels = ui_app_pixels(code.nativeHandle(), &width, &height);
        const auto* other = ui_app_pixels(fleck.nativeHandle(), &otherWidth, &otherHeight);
        if (!pixels || !other || width != otherWidth || height != otherHeight ||
            std::memcmp(pixels, other, static_cast<size_t>(width) * height * 4) != 0) {
            return 1;
        }
    }
    return 0;
}
