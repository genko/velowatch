#include "velo_layer.h"
#include "c/appendix/config.h"
#include "c/appendix/persist.h"

#define NUM_WEEKS 1
#define DAYS_PER_WEEK 7
#define FONT_OFFSET 5
#define ICON_SIZE 25

static Layer *s_velo_layer;
static TextLayer *s_calendar_text_layers[NUM_WEEKS * DAYS_PER_WEEK];
static GBitmap *s_weather_icons[NUM_WEEKS * DAYS_PER_WEEK];
static BitmapLayer *s_weather_icon_layers[NUM_WEEKS * DAYS_PER_WEEK];

// Map OpenWeatherMap weather condition IDs to Pebble bitmap resource IDs
static uint32_t owm_to_icon_resource(int owm_code) {
  // OWM condition codes: https://openweathermap.org/weather-conditions
  if (owm_code >= 200 && owm_code < 300) {
    return RESOURCE_ID_IMAGE_WEATHER_THUNDERSTORM;
  } else if (owm_code >= 300 && owm_code < 400) {
    return RESOURCE_ID_IMAGE_WEATHER_DRIZZLE;
  } else if (owm_code >= 500 && owm_code < 600) {
    return RESOURCE_ID_IMAGE_WEATHER_RAIN;
  } else if (owm_code >= 600 && owm_code < 700) {
    return RESOURCE_ID_IMAGE_WEATHER_SNOW;
  } else if (owm_code >= 700 && owm_code < 800) {
    return RESOURCE_ID_IMAGE_WEATHER_MIST;
  } else if (owm_code == 800) {
    return RESOURCE_ID_IMAGE_WEATHER_CLEAR_DAY;
  } else if (owm_code == 801 || owm_code == 802) {
    return RESOURCE_ID_IMAGE_WEATHER_FEW_CLOUDS;
  } else if (owm_code >= 803) {
    return RESOURCE_ID_IMAGE_WEATHER_CLOUDS;
  }
  // Default: clear day
  return RESOURCE_ID_IMAGE_WEATHER_CLEAR_DAY;
}

static void velo_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  float box_w = (float)bounds.size.w / DAYS_PER_WEEK;

  // Load data from storage
  const int num_days = persist_get_num_days();
  int16_t temps[num_days];
  int16_t icons[num_days];
  uint8_t precips[num_days];
  static char s_calendar_box_buffers[NUM_WEEKS * DAYS_PER_WEEK][4];

  persist_get_days_trend(temps, num_days);
  persist_get_days_icon(icons, num_days);
  persist_get_precip_days(precips, num_days);

  int temp_h = bounds.size.h - ICON_SIZE - 4;

  // Draw blue background for rainy entries (precipitation > 50%)
  graphics_context_set_fill_color(ctx, GColorBlue);
  for (int i = 0; i < NUM_WEEKS * DAYS_PER_WEEK; ++i) {
    if (i < num_days && precips[i] > 50) {
      GRect bg_rect = GRect((int)(i * box_w), 0,
                            (int)((i + 1) * box_w) - (int)(i * box_w),
                            temp_h + 4);
      graphics_fill_rect(ctx, bg_rect, 0, GCornerNone);
    }
  }

  // Fill each box with temperature and update weather icons
  for (int i = 0; i < NUM_WEEKS * DAYS_PER_WEEK; ++i) {
    char *buffer = s_calendar_box_buffers[i];

    text_layer_set_text_color(s_calendar_text_layers[i], GColorWhite);

    bool bold = true;
    text_layer_set_font(s_calendar_text_layers[i],
                        fonts_get_system_font(bold ? FONT_KEY_GOTHIC_24_BOLD
                                                   : FONT_KEY_GOTHIC_24));

    if (i < num_days) {
      snprintf(buffer, 4, "%d", config_localize_temp(temps[i]));
    } else {
      snprintf(buffer, 4, "--");
    }
    text_layer_set_text(s_calendar_text_layers[i], buffer);

    // Update weather icon
    if (i < num_days && i < 7) {
      uint32_t icon_res = owm_to_icon_resource(icons[i]);
      if (s_weather_icons[i] != NULL) {
        gbitmap_destroy(s_weather_icons[i]);
      }
      s_weather_icons[i] = gbitmap_create_with_resource(icon_res);
      bitmap_layer_set_bitmap(s_weather_icon_layers[i], s_weather_icons[i]);
      layer_set_hidden(
          bitmap_layer_get_layer(s_weather_icon_layers[i]), false);
    } else {
      layer_set_hidden(
          bitmap_layer_get_layer(s_weather_icon_layers[i]), true);
    }
  }
}

void velo_layer_create(Layer *parent_layer, GRect frame) {
  s_velo_layer = layer_create(frame);
  GRect bounds = layer_get_bounds(s_velo_layer);

  int w = bounds.size.w;
  int temp_h = bounds.size.h - ICON_SIZE - 4;
  float box_w = (float)w / DAYS_PER_WEEK;

  for (int i = 0; i < NUM_WEEKS * DAYS_PER_WEEK; ++i) {
    // Temperature text layer (top portion)
    TextLayer *s_box_text_layer = text_layer_create(GRect(
        (i % DAYS_PER_WEEK) * box_w, -FONT_OFFSET,
        box_w, temp_h + FONT_OFFSET));
    text_layer_set_background_color(s_box_text_layer, GColorClear);
    text_layer_set_text_alignment(s_box_text_layer, GTextAlignmentCenter);
    s_calendar_text_layers[i] = s_box_text_layer;
    layer_add_child(s_velo_layer, text_layer_get_layer(s_box_text_layer));

    // Weather icon layer (bottom portion)
    s_weather_icons[i] = NULL;
    BitmapLayer *icon_layer = bitmap_layer_create(GRect(
        (i % DAYS_PER_WEEK) * box_w + (box_w - ICON_SIZE) / 2,
        temp_h + 4,
        ICON_SIZE, ICON_SIZE));
    bitmap_layer_set_compositing_mode(icon_layer, GCompOpSet);
    s_weather_icon_layers[i] = icon_layer;
    layer_add_child(s_velo_layer, bitmap_layer_get_layer(icon_layer));
  }

  layer_set_update_proc(s_velo_layer, velo_update_proc);

  // Add it as a child layer to the Window's root layer
  layer_add_child(parent_layer, s_velo_layer);
}

void velo_layer_refresh() { layer_mark_dirty(s_velo_layer); }

void velo_layer_destroy() {
  for (int i = 0; i < NUM_WEEKS * DAYS_PER_WEEK; ++i) {
    text_layer_destroy(s_calendar_text_layers[i]);
    if (s_weather_icons[i] != NULL) {
      gbitmap_destroy(s_weather_icons[i]);
    }
    bitmap_layer_destroy(s_weather_icon_layers[i]);
  }
  layer_destroy(s_velo_layer);
}
