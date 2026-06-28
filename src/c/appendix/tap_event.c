#include "tap_event.h"
#include "c/layers/weather_status_layer.h"
#include "c/layers/forecast_layer.h"
#include "c/layers/loading_layer.h"
#include "c/layers/bike_layer.h"

static bool s_weather_visible = false;

void tap_handler(AccelAxisType axis, int32_t direction)
{
    APP_LOG(APP_LOG_LEVEL_INFO, "TAP! axis=%d dir=%ld", (int)axis, (long)direction);
    s_weather_visible = !s_weather_visible;
    weather_status_layer_set_hidden(!s_weather_visible);
    forecast_layer_set_hidden(!s_weather_visible);
    bike_layer_set_hidden(s_weather_visible);
    loading_layer_refresh();
}

void tap_init()
{
    accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ);
    accel_tap_service_subscribe(tap_handler);
    APP_LOG(APP_LOG_LEVEL_INFO, "tap_init: tap service subscribed");
}

void tap_deinit()
{
    accel_tap_service_unsubscribe();
}

bool tap_is_weather_visible(void) {
    return s_weather_visible;
}
