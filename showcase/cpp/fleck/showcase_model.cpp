#include "showcase_model.hpp"

using sunfleck::detail::check;

ShowcaseModel::ShowcaseModel(sunfleck::App& app)
    : app_(app), sharedText_(app.createTextSource("Shared text")) {}

void ShowcaseModel::initializeResources()
{
    ui_node* buttons[6]{};
    const char* dock_ids[] = {"dock_reset", "dock_float", "dock_hide", "dock_show", "dock_save", "dock_restore"};
    for (unsigned i = 0; i < 6; ++i) { buttons[i] = app_.findFleckNode(dock_ids[i]).nativeHandle(); }
    check(sunfleck_dock_demo_connect(app_.findFleckNode("dock").nativeHandle(), buttons, &dock_demo_));
    app_.findFleckNode("theme").setValue(UI_BUILTIN_SUNFLECK_LITE);
    const char* const items[] = {"One", "Two", "Three"};
    app_.findFleckNode("items").setItems(items, 3);
    const uint8_t rgb[] = {36, 107, 206, 255, 211, 105, 28, 125, 81, 36, 107, 206};
    for (const auto* id : {"sample_image", "sample_icon"}) {
        app_.findFleckNode(id).setImage(rgb, 2, 2);
    }
}

void ShowcaseModel::applyTheme(sunfleck::Node)
{
    const auto preset = app_.findFleckNode("theme").value();
    const auto compact = app_.findFleckNode("compact").value();
    const auto dark = app_.findFleckNode("dark").value();
    app_.setTheme(static_cast<ui_builtin_theme>(preset), dark != 0, compact != 0);
}

void ShowcaseModel::followSlider(sunfleck::Node slider)
{
    app_.findFleckNode("progress").setValue(slider.value());
}

void ShowcaseModel::showNotice(sunfleck::Node)
{
    app_.findFleckNode("notice").setText("Action received from the toolbar.");
}

void ShowcaseModel::submitSearch(sunfleck::Node editor)
{
    const auto text = editor.text();
    app_.findFleckNode("notice").setText(text.c_str());
}

void ShowcaseModel::toggle(const char* id)
{
    auto node = app_.findFleckNode(id);
    node.setValue(!node.value());
}

void ShowcaseModel::togglePopup(sunfleck::Node) { toggle("popup"); }
void ShowcaseModel::toggleDialog(sunfleck::Node) { toggle("dialog"); }
void ShowcaseModel::toggleContext(sunfleck::Node) { toggle("context"); }

void ShowcaseModel::selectMenu(sunfleck::Node menu)
{
    app_.findFleckNode(menu.value() ? "popup" : "dialog").setValue(1);
}
