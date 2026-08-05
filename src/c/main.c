#include <pebble.h>
#include <stdio.h>
#include <string.h>
#include "catppuccin.h"
#include "battery.h"
#include "heart.h"
#include "steps.h"
#define SETTINGS_KEY 1
#define SETTINGS_KEY_HEARTRATE 2
#define SETTINGS_KEY_HEARTRATE_MODE 4
#define SETTINGS_KEY_STEPS 5

extern uint32_t MESSAGE_KEY_SHOW_HEARTRATE;
// Use numeric message key IDs (matching package.json) to avoid relying on
// auto-generated symbols during build.
#define MESSAGE_KEY_SHOW_HEARTRATE_MODE 10002
#define MESSAGE_KEY_SHOW_STEPS 10003

typedef struct {
  CatppuccinFlavor flavor;
  bool show_heartrate;
  bool show_steps;
  int hr_mode;
} AppSettings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;
static GFont s_date_font;
static GFont s_battery_font;
static AppSettings s_settings;
static const CatppuccinPalette *s_palette;

static void save_settings(void) {
  persist_write_int(SETTINGS_KEY, s_settings.flavor);
  persist_write_int(SETTINGS_KEY_HEARTRATE, s_settings.show_heartrate ? 1 : 0);
  persist_write_int(SETTINGS_KEY_HEARTRATE_MODE, s_settings.hr_mode);
  persist_write_int(SETTINGS_KEY_STEPS, s_settings.show_steps ? 1 : 0);
}

static void load_settings(void) {
  s_settings.flavor = CATPPUCCIN_FLAVOR_MOCHA;
  s_settings.show_heartrate = false;
  s_settings.show_steps = false;
  s_settings.hr_mode = 0;
  if (persist_exists(SETTINGS_KEY)) {
    int stored = persist_read_int(SETTINGS_KEY);
    if (stored >= CATPPUCCIN_FLAVOR_LATTE && stored <= CATPPUCCIN_FLAVOR_MOCHA) {
      s_settings.flavor = (CatppuccinFlavor)stored;
    }
  }

  
  if (persist_exists(SETTINGS_KEY_HEARTRATE)) {
    int stored = persist_read_int(SETTINGS_KEY_HEARTRATE);
    s_settings.show_heartrate = (stored != 0);
  }
  if (persist_exists(SETTINGS_KEY_HEARTRATE_MODE)) {
    int stored = persist_read_int(SETTINGS_KEY_HEARTRATE_MODE);
    if (stored != 0 && stored != 1) stored = 0;
    s_settings.hr_mode = stored;
  }
  if (persist_exists(SETTINGS_KEY_STEPS)) {
    int stored = persist_read_int(SETTINGS_KEY_STEPS);
    s_settings.show_steps = (stored != 0);
  }
  s_palette = palette_for_flavor(s_settings.flavor);
}

static void apply_palette(void) {
  if (!s_palette) {
    s_palette = palette_for_flavor(CATPPUCCIN_FLAVOR_MOCHA);
  }
  if (s_main_window) window_set_background_color(s_main_window, s_palette->surface0);
  if (s_time_layer) text_layer_set_text_color(s_time_layer, s_palette->text);
  if (s_date_layer) text_layer_set_text_color(s_date_layer, s_palette->blue);
  battery_set_palette(s_palette);
  heart_set_palette(s_palette);
  steps_set_palette(s_palette);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  Tuple *flavor_tuple = dict_find(iterator, MESSAGE_KEY_CATPPUCCIN_FLAVOR);
  if (flavor_tuple && flavor_tuple->type == TUPLE_CSTRING) {
    CatppuccinFlavor flavor = flavor_from_string(flavor_tuple->value->cstring);
    if (flavor != s_settings.flavor) {
      s_settings.flavor = flavor;
      s_palette = palette_for_flavor(s_settings.flavor);
      save_settings();
      apply_palette();
    }
  }

  Tuple *hr_tuple = dict_find(iterator, MESSAGE_KEY_SHOW_HEARTRATE);
  if (hr_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox: SHOW_HEARTRATE tuple type=%d", hr_tuple->type);
    bool enabled = false;
    if (hr_tuple->type == TUPLE_CSTRING) {
      enabled = (strcmp(hr_tuple->value->cstring, "true") == 0 || strcmp(hr_tuple->value->cstring, "1") == 0);
    } else if (hr_tuple->type == TUPLE_INT) {
      enabled = (hr_tuple->value->int32 != 0);
    }
    if (enabled != s_settings.show_heartrate) {
      s_settings.show_heartrate = enabled;
      persist_write_int(SETTINGS_KEY_HEARTRATE, s_settings.show_heartrate ? 1 : 0);
      // If the main window is loaded, create/destroy the UI immediately. Otherwise
      // defer to main_window_load which will create the UI if needed.
      if (s_main_window) {
        Layer *root = window_get_root_layer(s_main_window);
        if (root) {
          if (s_settings.show_heartrate) {
            heart_window_load(s_main_window, s_battery_font, s_palette, true);
            heart_set_mode((HeartMode)s_settings.hr_mode);
          } else {
            heart_window_unload();
          }
        } else {
          heart_set_enabled(s_settings.show_heartrate);
        }
      } else {
        heart_set_enabled(s_settings.show_heartrate);
      }
    }
  }

  Tuple *hr_mode_tuple = dict_find(iterator, MESSAGE_KEY_SHOW_HEARTRATE_MODE);
  if (hr_mode_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox: SHOW_HEARTRATE_MODE tuple type=%d", hr_mode_tuple->type);
    int mode = 0;
    if (hr_mode_tuple->type == TUPLE_CSTRING) {
      mode = atoi(hr_mode_tuple->value->cstring);
    } else if (hr_mode_tuple->type == TUPLE_INT) {
      mode = hr_mode_tuple->value->int32;
    }
    if (mode != 0 && mode != 1) mode = 0;
    if (mode != s_settings.hr_mode) {
      s_settings.hr_mode = mode;
      persist_write_int(SETTINGS_KEY_HEARTRATE_MODE, s_settings.hr_mode);
      // If heart is currently enabled, apply mode immediately
      if (s_settings.show_heartrate) {
        heart_set_mode((HeartMode)s_settings.hr_mode);
      }
    }
  }

  Tuple *steps_tuple = dict_find(iterator, MESSAGE_KEY_SHOW_STEPS);
  if (steps_tuple) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox: SHOW_STEPS tuple type=%d", steps_tuple->type);
    bool enabled = false;
    if (steps_tuple->type == TUPLE_CSTRING) {
      enabled = (strcmp(steps_tuple->value->cstring, "true") == 0 || strcmp(steps_tuple->value->cstring, "1") == 0);
    } else if (steps_tuple->type == TUPLE_INT) {
      enabled = (steps_tuple->value->int32 != 0);
    }
    if (enabled != s_settings.show_steps) {
      s_settings.show_steps = enabled;
      persist_write_int(SETTINGS_KEY_STEPS, s_settings.show_steps ? 1 : 0);
      if (s_main_window) {
        Layer *root = window_get_root_layer(s_main_window);
        if (root) {
          if (s_settings.show_steps) {
            steps_window_load(s_main_window, s_battery_font, s_palette, true);
          } else {
            steps_window_unload();
          }
        } else {
          steps_set_enabled(s_settings.show_steps);
        }
      } else {
        steps_set_enabled(s_settings.show_steps);
      }
    }
  }
}

static void update_time(void) {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_time_buffer[8];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, s_time_buffer);

  static char s_date_buffer[16];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %b %d", tick_time);
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  if (!s_palette) {
    s_palette = palette_for_flavor(CATPPUCCIN_FLAVOR_MOCHA);
  }
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CASKAYBOLD_56));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CASKAY_24));
  s_battery_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CASKAYICONS_24));

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int date_height = 30;
  int block_height = 56 + date_height;
  int time_y = (bounds.size.h / 2) - (block_height / 2) - 10;
  int date_y = time_y + 56;

  s_time_layer = text_layer_create(GRect(0, time_y, bounds.size.w, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, s_palette->text);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  s_date_layer = text_layer_create(GRect(0, date_y, bounds.size.w, 30));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, s_palette->mauve);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  battery_window_load(window, s_battery_font, s_palette);
  if (s_settings.show_heartrate) {
    heart_window_load(window, s_battery_font, s_palette, true);
    heart_set_mode((HeartMode)s_settings.hr_mode);
  }
  if (s_settings.show_steps) {
    steps_window_load(window, s_battery_font, s_palette, true);
  }

  apply_palette();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  battery_window_unload();
  heart_window_unload();
  steps_window_unload();
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  fonts_unload_custom_font(s_battery_font);
}

static void init(void) {
  load_settings();
  s_main_window = window_create();
  window_set_background_color(s_main_window, s_palette->base);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });

  window_stack_push(s_main_window, true);
  update_time();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  app_message_register_inbox_received(inbox_received_callback);
  app_message_open(256, 256);
  battery_state_service_subscribe(battery_callback);
  battery_callback(battery_state_service_peek());
}

static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  app_message_deregister_callbacks();

  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
