#include <pebble.h>
#include <stdio.h>
#include <string.h>
#include "catppuccin.h"
#include "battery.h"
#include "heart.h"
#define SETTINGS_KEY 1
#define SETTINGS_KEY_HEARTRATE 2

// MESSAGE_KEY_* symbols are generated at build-time into build/include/message_keys.auto.h
// Some build systems expose them as extern uint32_t variables. Declare the SHOW_HEARTRATE key
// here as extern to ensure compilation if the auto-generated header hasn't been updated yet.
extern uint32_t MESSAGE_KEY_SHOW_HEARTRATE;
#define SETTINGS_KEY 1

typedef struct {
  CatppuccinFlavor flavor;
  bool show_heartrate;
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
}

static void load_settings(void) {
  s_settings.flavor = CATPPUCCIN_FLAVOR_MOCHA;
  s_settings.show_heartrate = false;
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
    bool enabled = false;
    if (hr_tuple->type == TUPLE_CSTRING) {
      enabled = (strcmp(hr_tuple->value->cstring, "true") == 0 || strcmp(hr_tuple->value->cstring, "1") == 0);
    } else if (hr_tuple->type == TUPLE_INT) {
      enabled = (hr_tuple->value->int32 != 0);
    }
    if (enabled != s_settings.show_heartrate) {
      s_settings.show_heartrate = enabled;
      persist_write_int(SETTINGS_KEY_HEARTRATE, s_settings.show_heartrate ? 1 : 0);
      heart_set_enabled(s_settings.show_heartrate);
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
  heart_window_load(window, s_battery_font, s_palette, s_settings.show_heartrate);

  apply_palette();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  battery_window_unload();
  heart_window_unload();
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
  // Unsubscribe services and close app message before destroying windows
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  // Deregister app message callbacks and close the channel
  // app_message_deregister_callbacks is available in the SDK; call it if present
  app_message_deregister_callbacks();

  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
