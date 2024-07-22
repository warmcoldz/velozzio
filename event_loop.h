#pragma once

#include "timer.h"
#include "event_loop_internal_controller.h"

#include <map>
#include <unordered_map>
#include <chrono>

namespace event_loop {

class EventLoop : private IEventLoopInternalController
{
public:
    EventLoop();

    Timer CreateTimer(std::shared_ptr<ITimerHandler> handler);
    void Run();

private:
    std::chrono::system_clock::time_point CurrentTime() const override final;
    void AddTimer(ITimerInternalController* timer) override final;
    void RemoveTimer(ITimerInternalController* timer) override final;

private:
    void CheckTimersExpired();
    bool HasTimers() const;
    std::chrono::system_clock::time_point EarliestTimerExpirationTime() const;

private:
    std::chrono::system_clock::time_point time_;

    using TimersTree = std::multimap<std::chrono::system_clock::time_point /*expirationTime*/, ITimerInternalController*>;

    TimersTree timers_;
    std::unordered_map<ITimerInternalController*, TimersTree::iterator> timerPositions_;
};

} // namespace event_loop
