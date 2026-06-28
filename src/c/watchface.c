#include "appendix/app_message.h"
#include "appendix/config.h"
#include "appendix/persist.h"
#include "appendix/tap_event.h"
#include "windows/main_window.h"
#include <pebble.h>

static void init() {
  app_message_init();
  persist_init();
  config_load();
  tap_init();
  main_window_create();
}

static void deinit() {
  config_unload();
  tap_deinit();
  main_window_destroy();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
