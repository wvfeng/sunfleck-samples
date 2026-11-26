#pragma once

#include <sunfleck/property_model.hpp>
#include <chrono>
#include <thread>

class MainModel {
public:
    unsigned count() const noexcept { return count_; }
    std::string buttonText() const { return "Increment"; }
    bool canIncrement() const noexcept { return count_ < 10; }
    auto subscribePropertyChanged(sunfleck::PropertyChangedHandler handler)
    {
        return changed_.subscribe(std::move(handler));
    }
    explicit MainModel(sunfleck::App& app) : app_(app) {}
    void increment(sunfleck::CancellationToken cancellation)
    {
        // Simulate a slow business operation.
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        app_.invokeAsync([this] {
            ++count_;
            changed_.notify("count");
            changed_.notify("canIncrement");
        }, cancellation).get();
    }

private:
    sunfleck::App& app_;
    unsigned count_ = 0;
    sunfleck::PropertyChangedEvent changed_;
};
