#include <SimpleTimer.h>

SimpleTimer timer;
int tickId;

void tick() {
    Serial.println("tick");
}

void setup() {
    Serial.begin(115200);

    tickId = timer.setInterval(1000, tick);
    timer.setTimeout(5000, [] { Serial.println("5 seconds are over"); });

    // pause the tick after 10 seconds, resume it after 20
    timer.setTimer(10000, [] { timer.toggle(tickId); }, 2);
}

void loop() {
    timer.run();
}
