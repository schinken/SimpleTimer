# SimpleTimer for Arduino

[![CI](https://github.com/schinken/SimpleTimer/actions/workflows/ci.yml/badge.svg)](https://github.com/schinken/SimpleTimer/actions/workflows/ci.yml)

Run callbacks at an interval or after a timeout, polled from `loop()`. No interrupts.

This is a fork of the SimpleTimer library from Marcello Romani (https://playground.arduino.cc/Code/SimpleTimer). Sadly there is no original repository to apply pull requests.

## Additional Features

* Support lambda-expressions / std::function callbacks
* Microsecond resolution with `-DSIMPLE_TIMER_MICROS`

## Install

PlatformIO, in `platformio.ini`:

```ini
lib_deps = https://github.com/schinken/SimpleTimer.git
```

Arduino IDE: download this repository as ZIP, then `Sketch > Include Library > Add .ZIP Library`.

## Usage

```cpp
#include <SimpleTimer.h>

SimpleTimer timer;

void setup() {
    Serial.begin(115200);

    timer.setInterval(1000, [] { Serial.println("every second"); });
    timer.setTimeout(5000, [] { Serial.println("once, after 5 seconds"); });
}

void loop() {
    timer.run();
}
```

A complete sketch is in [examples/Basic](examples/Basic/Basic.ino).

## Supported boards

CI compiles the example for every push:

| Family | Board in CI |
| --- | --- |
| AVR | Arduino Uno |
| megaAVR | Arduino Nano Every |
| SAMD | Arduino MKR Zero |
| Renesas | Arduino Uno R4 Minima |
| RP2040 | Raspberry Pi Pico |
| STM32 | Blue Pill F103C8 |
| ESP8266 | NodeMCU v2 |
| ESP32 | ESP32 Dev Module |

## API

| Function | What it does |
| --- | --- |
| `setInterval(ms, f)` | call `f` every `ms` milliseconds, returns the timer id |
| `setTimeout(ms, f)` | call `f` once after `ms` milliseconds, returns the timer id |
| `setTimer(ms, f, n)` | call `f` every `ms` milliseconds, `n` times, returns the timer id |
| `deleteTimer(id)` | remove a timer |
| `restartTimer(id)` | start the current interval of a timer again |
| `enable(id)` / `disable(id)` / `toggle(id)` | pause and resume a timer |
| `isEnabled(id)` | true if the timer is enabled |
| `getNumTimers()` / `getNumAvailableTimers()` | used and free timer slots |
| `run()` | call this in `loop()` |

## Good to know

* One `SimpleTimer` holds up to 10 timers. `setInterval`, `setTimeout` and `setTimer` return `-1` if all are in use.
* Timer ids are reused after a timer is deleted or has finished.
* If `loop()` is blocked for longer than an interval, the missed runs are skipped, not replayed.
* A callback may delete or re-create its own timer.
* AVR boards (Uno, Mega, ...) have no `<functional>`. Callbacks are plain function pointers there, so lambdas can't capture variables.
* `SIMPLE_TIMER_MICROS` switches all delays to microseconds. It has to be a compiler flag, a `#define` in the sketch has no effect. PlatformIO: `build_flags = -DSIMPLE_TIMER_MICROS`.

## Test

Runs on the host, no board needed:

```sh
c++ -std=c++11 -g -fsanitize=address,undefined -fno-sanitize-recover=all -Itest -I. test/test.cpp SimpleTimer.cpp -o /tmp/simpletimer-test && /tmp/simpletimer-test
```

## License

LGPL-2.1-or-later, see [LICENSE](LICENSE).
