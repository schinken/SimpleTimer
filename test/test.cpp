// Host check, no board needed. Run from the repo root:
//   c++ -std=c++11 -g -fsanitize=address,undefined -fno-sanitize-recover=all -Itest -I. test/test.cpp SimpleTimer.cpp -o /tmp/simpletimer-test && /tmp/simpletimer-test

#include "SimpleTimer.h"
#include <cassert>
#include <cstdio>

static unsigned long now = 0;
unsigned long millis() { return now; }
unsigned long micros() { return now * 1000; }

int main() {
    // runs missed while loop() was blocked are skipped, the phase is kept
    {
        SimpleTimer t;
        int fired = 0;
        now = 0;
        t.setInterval(1000, [&] { fired++; });
        now = 5300;
        for (int i = 0; i < 5; i++) { t.run(); now++; }
        assert(fired == 1);
        for (; now < 6000; now++) t.run();
        assert(fired == 1);
        t.run();
        assert(fired == 2);
    }

    // a timeout callback may delete its own id and re-arm itself
    {
        SimpleTimer t;
        int fired = 0;
        int id = -1;
        std::function<void()> tick = [&] { fired++; t.deleteTimer(id); id = t.setTimeout(100, tick); };
        now = 0;
        id = t.setTimeout(100, tick);
        for (; now <= 1000; now += 10) t.run();
        assert(fired == 10);
        assert(t.getNumTimers() == 1);
    }

    // an interval callback may delete itself and keep using its captures
    {
        SimpleTimer t;
        int fired = 0;
        int id = -1;
        int a = 1, b = 2, c = 3;
        now = 0;
        id = t.setInterval(100, [&] { t.deleteTimer(id); fired += a + b + c; });
        for (; now <= 1000; now += 10) t.run();
        assert(fired == 6);
        assert(t.getNumTimers() == 0);
    }

    // a failed set*() returns -1, which must be safe to pass back in
    {
        SimpleTimer t;
        int id = 0;
        for (int i = 0; i <= SimpleTimer::MAX_TIMERS; i++) id = t.setInterval(1000, [] {});
        assert(id == -1);
        t.restartTimer(id);
        t.enable(id);
        t.disable(id);
        t.toggle(id);
        t.deleteTimer(id);
        assert(!t.isEnabled(id));
        assert(t.getNumTimers() == SimpleTimer::MAX_TIMERS);
    }

    puts("ok");
    return 0;
}
