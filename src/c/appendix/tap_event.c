#include "tap_event.h"
#include "c/layers/weather_status_layer.h"
#include "c/layers/forecast_layer.h"
#include "c/layers/loading_layer.h"
#include "c/layers/bike_layer.h"
#include <stdlib.h>

#define FLICK_THRESHOLD 2000 // milli-G; normal wear is <1500, a flick is 2000-4000
#define DEBOUNCE_SECONDS 1

static bool s_weather_visible = false;
static time_t s_last_flick = 0;

static void toggle_page(void) {
    s_weather_visible = !s_weather_visible;
    weather_status_layer_set_hidden(!s_weather_visible);
    forecast_layer_set_hidden(!s_weather_visible);
    bike_layer_set_hidden(s_weather_visible);
    loading_layer_refresh();
}

static void accel_handler(AccelData *data, uint32_t num_samples) {
    for (uint32_t i = 0; i < num_samples; i++) {
        if (abs(data[i].x) > FLICK_THRESHOLD ||
            abs(data[i].y) > FLICK_THRESHOLD ||
            abs(data[i].z) > FLICK_THRESHOLD) {
            time_t now = time(NULL);
            if (now - s_last_flick >= DEBOUNCE_SECONDS) {
                s_last_flick = now;
                APP_LOG(APP_LOG_LEVEL_INFO, "flick! x=%d y=%d z=%d",
                        data[i].x, data[i].y, data[i].z);
                toggle_page();
            }
            return; // one flick per batch is enough
        }
    }
}

void tap_init()
{
    accel_data_service_subscribe(10, accel_handler);
    accel_service_set_sampling_rate(ACCEL_SAMPLING_50HZ);
    APP_LOG(APP_LOG_LEVEL_INFO, "accel flick detection started");
}

void tap_deinit()
{
    accel_data_service_unsubscribe();
}

bool tap_is_weather_visible(void) {
    return s_weather_visible;
}
