#include "persist.h"
#include "config.h"

enum key {
  TEMP_LO,
  TEMP_HI,
  TEMP_TREND,
  DAYS_TREND,
  DAYS_ICON,
  PRECIP_DAYS,
  PRECIP_TREND,
  FORECAST_START,
  CITY,
  SUN_EVENT_START_TYPE,
  SUN_EVENT_TIMES,
  NUM_ENTRIES,
  NUM_DAYS,
  CURRENT_TEMP,
  UVI,
  BATTERY_LEVEL,
  CONFIG,
  ADVICE,
  HOLIDAYS
}; // Deprecated: BATTERY_LEVEL

void persist_init() {
  if (!persist_exists(TEMP_LO)) {
    persist_write_int(TEMP_LO, 2);
  }
  if (!persist_exists(TEMP_HI)) {
    persist_write_int(TEMP_HI, 12);
  }
  if (!persist_exists(TEMP_TREND)) {
    int16_t data[] = {2, 2, 2, 4, 7, 9, 11, 12, 12, 12, 11, 9};
    persist_write_data(TEMP_TREND, (void *)data, 12 * sizeof(int16_t));
  }
  if (!persist_exists(DAYS_TREND)) {
    int16_t data[] = {2, 2, 2, 4, 7, 9, 11};
    persist_write_data(DAYS_TREND, (void *)data, 7 * sizeof(int16_t));
  }
  if (!persist_exists(DAYS_ICON)) {
    int16_t data[] = {2, 2, 2, 4, 7, 9, 11};
    persist_write_data(DAYS_ICON, (void *)data, 7 * sizeof(int16_t));
  }
  if (!persist_exists(PRECIP_DAYS)) {
    uint8_t data[] = {0, 0, 0, 0, 0, 0, 0};
    persist_write_data(PRECIP_DAYS, (void *)data, 7 * sizeof(uint8_t));
  }
  if (!persist_exists(PRECIP_TREND)) {
    uint8_t data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    persist_write_data(PRECIP_TREND, (void *)data, 12 * sizeof(uint8_t));
  }
  if (!persist_exists(ADVICE)) {
    persist_write_int(ADVICE, 0);
  }
  if (!persist_exists(HOLIDAYS)) {
    persist_write_int(HOLIDAYS, 0);
  }
  if (!persist_exists(FORECAST_START)) {
    persist_write_int(FORECAST_START, 0);
  }
  if (!persist_exists(NUM_ENTRIES)) {
    persist_write_int(NUM_ENTRIES, 12);
  }
  if (!persist_exists(NUM_DAYS)) {
    persist_write_int(NUM_DAYS, 7);
  }
  if (!persist_exists(CURRENT_TEMP)) {
    persist_write_int(CURRENT_TEMP, 1);
  }
  if (!persist_exists(UVI)) {
    persist_write_int(UVI, 0);
  }
  if (!persist_exists(CITY)) {
    persist_write_string(CITY, "Koji");
  }
  if (!persist_exists(SUN_EVENT_START_TYPE)) {
    persist_write_int(SUN_EVENT_START_TYPE, 0);
  }
  if (!persist_exists(SUN_EVENT_TIMES)) {
    uint32_t data[] = {0, 0};
    persist_write_data(SUN_EVENT_TIMES, (void *)data, 2 * sizeof(uint32_t));
  }
  if (!persist_exists(CONFIG)) {
    Config config = (Config){.celsius = false,
                             .time_lead_zero = false,
                             .axis_12h = false,
                             .time_font = 0,
                             .show_qt = true,
                             .show_bt = true,
                             .show_bt_disconnect = true,
                             .vibe = false,
                             .show_am_pm = false,
                             .color_saturday = GColorWhite,
                             .color_sunday = GColorWhite,
                             .color_us_federal = GColorWhite,
                             .color_time = GColorWhite};
    persist_set_config(config);
  }
}

int persist_get_temp_lo() { return persist_read_int(TEMP_LO); }

int persist_get_temp_hi() { return persist_read_int(TEMP_HI); }

int persist_get_temp_trend(int16_t *buffer, const size_t buffer_size) {
  return persist_read_data(TEMP_TREND, (void *)buffer,
                           buffer_size * sizeof(int16_t));
}

int persist_get_days_trend(int16_t *buffer, const size_t buffer_size) {
  return persist_read_data(DAYS_TREND, (void *)buffer,
                           buffer_size * sizeof(int16_t));
}

int persist_get_days_icon(int16_t *buffer, const size_t buffer_size) {
  return persist_read_data(DAYS_ICON, (void *)buffer,
                           buffer_size * sizeof(int16_t));
}

int persist_get_precip_days(uint8_t *buffer, const size_t buffer_size) {
  return persist_read_data(PRECIP_DAYS, (void *)buffer,
                           buffer_size * sizeof(uint8_t));
}

int persist_get_advice() { return persist_read_int(ADVICE); }

int persist_get_holidays() { return persist_read_int(HOLIDAYS); }

int persist_get_precip_trend(uint8_t *buffer, const size_t buffer_size) {
  return persist_read_data(PRECIP_TREND, (void *)buffer,
                           buffer_size * sizeof(uint8_t));
}

time_t persist_get_forecast_start() {
  return (time_t)persist_read_int(FORECAST_START);
}

int persist_get_num_entries() { return persist_read_int(NUM_ENTRIES); }

int persist_get_num_days() { return persist_read_int(NUM_DAYS); }

int persist_get_current_temp() { return persist_read_int(CURRENT_TEMP); }

int persist_get_current_uvi() { return persist_read_int(UVI); }

int persist_get_city(char *buffer, const size_t buffer_size) {
  return persist_read_string(CITY, buffer, buffer_size);
}

int persist_get_sun_event_start_type() {
  return persist_read_int(SUN_EVENT_START_TYPE);
}

int persist_get_sun_event_times(time_t *buffer, const size_t buffer_size) {
  return persist_read_data(SUN_EVENT_TIMES, (void *)buffer,
                           buffer_size * sizeof(time_t));
}

int persist_get_config(Config *config) {
  return persist_read_data(CONFIG, config, sizeof(Config));
}

void persist_set_temp_lo(int val) { persist_write_int(TEMP_LO, val); }

void persist_set_temp_hi(int val) { persist_write_int(TEMP_HI, val); }

void persist_set_temp_trend(int16_t *data, const size_t size) {
  persist_write_data(TEMP_TREND, (void *)data, size * sizeof(int16_t));
}

void persist_set_days_trend(int16_t *data, const size_t size) {
  persist_write_data(DAYS_TREND, (void *)data, size * sizeof(int16_t));
}

void persist_set_days_icon(int16_t *data, const size_t size) {
  persist_write_data(DAYS_ICON, (void *)data, size * sizeof(int16_t));
}

void persist_set_precip_days(uint8_t *data, const size_t size) {
  persist_write_data(PRECIP_DAYS, (void *)data, size * sizeof(uint8_t));
}

void persist_set_precip_trend(uint8_t *data, const size_t size) {
  persist_write_data(PRECIP_TREND, (void *)data, size * sizeof(uint8_t));
}

void persist_set_advice(int val) { persist_write_int(ADVICE, val); }

void persist_set_holidays(int val) { persist_write_int(HOLIDAYS, val); }

void persist_set_forecast_start(time_t val) {
  persist_write_int(FORECAST_START, (int)val);
}

void persist_set_num_entries(int val) { persist_write_int(NUM_ENTRIES, val); }

void persist_set_num_days(int val) { persist_write_int(NUM_DAYS, val); }

void persist_set_current_temp(int val) { persist_write_int(CURRENT_TEMP, val); }

void persist_set_current_uvi(int val) { persist_write_int(UVI, val); }

void persist_set_city(char *val) { persist_write_string(CITY, val); }

void persist_set_sun_event_start_type(int val) {
  persist_write_int(SUN_EVENT_START_TYPE, val);
}

void persist_set_sun_event_times(time_t *data, const size_t size) {
  persist_write_data(SUN_EVENT_TIMES, (void *)data, size * sizeof(time_t));
}

void persist_set_config(Config config) {
  persist_write_data(CONFIG, &config, sizeof(Config));
  config_refresh(); // Refresh global config variable
}
