#include <stdio.h>

#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "tusb.h"

#include "bno085_controller.h"

// ---- config ----
static const uint SDA_PIN = 20;
static const uint SCL_PIN = 21;

// ---- helpers ----
static int16_t map_axis(float deg) {
    // deadzone
    if (deg > -2.0f && deg < 2.0f) deg = 0;

    // scale: ±30° → full range
    float scaled = deg * (32767.0f / 30.0f);

    if (scaled > 32767) scaled = 32767;
    if (scaled < -32768) scaled = -32768;

    return (int16_t)scaled;
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    // ---- USB init ----
    tusb_init();

    // ---- IMU init ----
    Bno085Controller imu(i2c0, SDA_PIN, SCL_PIN);

    if (!imu.begin()) {
        while (1) {
            sleep_ms(1000);
        }
    }

    imu.enableRotation(10);
    imu.enableAccelerometer(10);

    ImuAccel accel = {};
    ImuEuler euler = {};

    // ---- axis mapper (int8 for TinyUSB) ----
    auto map_axis = [](float deg) -> int8_t {
        // deadzone
        if (deg > -2.0f && deg < 2.0f) return 0;

        // scale: ±30° → ±127
        float scaled = deg * (127.0f / 30.0f);

        if (scaled > 127) scaled = 127;
        if (scaled < -128) scaled = -128;

        return (int8_t)scaled;
    };

    while (1) {
        // ---- USB task (REQUIRED) ----
        tud_task();

        if (imu.update(accel, euler)) {

            if (tud_hid_ready()) {
                int8_t r = map_axis(euler.r);  // roll
                int8_t p = map_axis(euler.p);  // pitch
                int8_t h = map_axis(euler.h);  // yaw

                tud_hid_gamepad_report(
                    0,  // report id

                    h,  // X
                    p,  // Y
                    0,  // Z
                    0,  // Rz

                    0,  // Rx
                    0,  // Ry

                    0,  // hat
                    0   // buttons
                );
            }

            // optional debug
            printf("H=%.2f P=%.2f R=%.2f\n", euler.h, euler.p, euler.r);
        }

        sleep_ms(5);
    }
}
