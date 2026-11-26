#pragma once

#include <sunfleck/ui.hpp>
#include "../../dock_demo.h"

class ShowcaseModel {
public:
    explicit ShowcaseModel(sunfleck::App& app);
    unsigned count() const noexcept { return count_; }
    const sunfleck::TextSource& sharedText() const noexcept { return sharedText_; }

    void initializeResources();
    void increment(sunfleck::Node) noexcept { ++count_; }
    void reset(sunfleck::Node) noexcept { count_ = 0; }
    void applyTheme(sunfleck::Node);
    void followSlider(sunfleck::Node slider);
    void showNotice(sunfleck::Node);
    void submitSearch(sunfleck::Node editor);
    void togglePopup(sunfleck::Node);
    void toggleDialog(sunfleck::Node);
    void toggleContext(sunfleck::Node);
    void selectMenu(sunfleck::Node menu);

private:
    void toggle(const char* id);
    sunfleck::App& app_;
    sunfleck::TextSource sharedText_;
    unsigned count_ = 0;
    sunfleck_dock_demo dock_demo_{};
};
