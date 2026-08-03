#include <pebble.h>
#include <stdio.h>
#include <string.h>

#define SETTINGS_KEY 1

typedef enum {
  CATPPUCCIN_FLAVOR_LATTE = 0,
  CATPPUCCIN_FLAVOR_FRAPPE = 1,
  CATPPUCCIN_FLAVOR_MACCHIATO = 2,
  CATPPUCCIN_FLAVOR_MOCHA = 3,
} CatppuccinFlavor;

typedef struct {
  GColor rosewater;
  GColor flamingo;
  GColor pink;
  GColor mauve;
  GColor red;
  GColor maroon;
  GColor peach;
  GColor yellow;
  GColor green;
  GColor teal;
  GColor sky;
  GColor sapphire;
  GColor blue;
  GColor lavender;
  GColor text;
  GColor subtext1;
  GColor subtext0;
  GColor overlay2;
  GColor overlay1;
  GColor overlay0;
  GColor surface2;
  GColor surface1;
  GColor surface0;
  GColor base;
  GColor mantle;
  GColor crust;
} CatppuccinPalette;

static const CatppuccinPalette CATPPUCCIN_PALETTE_LATTE = {
  .rosewater = GColorFromHEX(0xDC8A78),
  .flamingo = GColorFromHEX(0xDD7878),
  .pink = GColorFromHEX(0xEA76CB),
  .mauve = GColorFromHEX(0x8839EF),
  .red = GColorFromHEX(0xD20F39),
  .maroon = GColorFromHEX(0xE64553),
  .peach = GColorFromHEX(0xFE640B),
  .yellow = GColorFromHEX(0xDF8E1D),
  .green = GColorFromHEX(0x40A02B),
  .teal = GColorFromHEX(0x179299),
  .sky = GColorFromHEX(0x04A5E5),
  .sapphire = GColorFromHEX(0x209FB5),
  .blue = GColorFromHEX(0x1E66F5),
  .lavender = GColorFromHEX(0x7287FD),
  .text = GColorFromHEX(0x4C4F69),
  .subtext1 = GColorFromHEX(0x5C5F77),
  .subtext0 = GColorFromHEX(0x6C6F85),
  .overlay2 = GColorFromHEX(0x7C7F93),
  .overlay1 = GColorFromHEX(0x8C8FA1),
  .overlay0 = GColorFromHEX(0x9CA0B0),
  .surface2 = GColorFromHEX(0xACB0BE),
  .surface1 = GColorFromHEX(0xBCC0CC),
  .surface0 = GColorFromHEX(0xCCD0DA),
  .base = GColorFromHEX(0xEFF1F5),
  .mantle = GColorFromHEX(0xE6E9EF),
  .crust = GColorFromHEX(0xDCE0E8),
};

static const CatppuccinPalette CATPPUCCIN_PALETTE_FRAPPE = {
  .rosewater = GColorFromHEX(0xF2D5CF),
  .flamingo = GColorFromHEX(0xEEBEBE),
  .pink = GColorFromHEX(0xF4B8E4),
  .mauve = GColorFromHEX(0xCA9EE6),
  .red = GColorFromHEX(0xE78284),
  .maroon = GColorFromHEX(0xEA999C),
  .peach = GColorFromHEX(0xEF9F76),
  .yellow = GColorFromHEX(0xE5C890),
  .green = GColorFromHEX(0xA6D189),
  .teal = GColorFromHEX(0x81C8BE),
  .sky = GColorFromHEX(0x99D1DB),
  .sapphire = GColorFromHEX(0x85C1DC),
  .blue = GColorFromHEX(0x8CAAEE),
  .lavender = GColorFromHEX(0xBABBF1),
  .text = GColorFromHEX(0xC6D0F5),
  .subtext1 = GColorFromHEX(0xB5BFE2),
  .subtext0 = GColorFromHEX(0xA5ADCE),
  .overlay2 = GColorFromHEX(0x949CBB),
  .overlay1 = GColorFromHEX(0x838BA7),
  .overlay0 = GColorFromHEX(0x737994),
  .surface2 = GColorFromHEX(0x626880),
  .surface1 = GColorFromHEX(0x51576D),
  .surface0 = GColorFromHEX(0x414559),
  .base = GColorFromHEX(0x303446),
  .mantle = GColorFromHEX(0x292C3C),
  .crust = GColorFromHEX(0x232634),
};

static const CatppuccinPalette CATPPUCCIN_PALETTE_MACCHIATO = {
  .rosewater = GColorFromHEX(0xF4DBD6),
  .flamingo = GColorFromHEX(0xF0C6C6),
  .pink = GColorFromHEX(0xF5BDE6),
  .mauve = GColorFromHEX(0xC6A0F6),
  .red = GColorFromHEX(0xED8796),
  .maroon = GColorFromHEX(0xEE99A0),
  .peach = GColorFromHEX(0xF5A97F),
  .yellow = GColorFromHEX(0xEED49F),
  .green = GColorFromHEX(0xA6DA95),
  .teal = GColorFromHEX(0x8BD5CA),
  .sky = GColorFromHEX(0x91D7E3),
  .sapphire = GColorFromHEX(0x7DC4E4),
  .blue = GColorFromHEX(0x8AADF4),
  .lavender = GColorFromHEX(0xB7BDF8),
  .text = GColorFromHEX(0xCAD3F5),
  .subtext1 = GColorFromHEX(0xB8C0E0),
  .subtext0 = GColorFromHEX(0xA5ADCB),
  .overlay2 = GColorFromHEX(0x939AB7),
  .overlay1 = GColorFromHEX(0x8087A2),
  .overlay0 = GColorFromHEX(0x6E738D),
  .surface2 = GColorFromHEX(0x5B6078),
  .surface1 = GColorFromHEX(0x494D64),
  .surface0 = GColorFromHEX(0x363A4F),
  .base = GColorFromHEX(0x24273A),
  .mantle = GColorFromHEX(0x1E2030),
  .crust = GColorFromHEX(0x181926),
};

static const CatppuccinPalette CATPPUCCIN_PALETTE_MOCHA = {
  .rosewater = GColorFromHEX(0xF5E0DC),
  .flamingo = GColorFromHEX(0xF2CDCD),
  .pink = GColorFromHEX(0xF5C2E7),
  .mauve = GColorFromHEX(0xCBA6F7),
  .red = GColorFromHEX(0xF38BA8),
  .maroon = GColorFromHEX(0xEBA0AC),
  .peach = GColorFromHEX(0xFAB387),
  .yellow = GColorFromHEX(0xF9E2AF),
  .green = GColorFromHEX(0xA6E3A1),
  .teal = GColorFromHEX(0x94E2D5),
  .sky = GColorFromHEX(0x89DCEB),
  .sapphire = GColorFromHEX(0x74C7EC),
  .blue = GColorFromHEX(0x89B4FA),
  .lavender = GColorFromHEX(0xB4BEFE),
  .text = GColorFromHEX(0xCDD6F4),
  .subtext1 = GColorFromHEX(0xBAC2DE),
  .subtext0 = GColorFromHEX(0xA6ADC8),
  .overlay2 = GColorFromHEX(0x9399B2),
  .overlay1 = GColorFromHEX(0x7F849C),
  .overlay0 = GColorFromHEX(0x6C7086),
  .surface2 = GColorFromHEX(0x585B70),
  .surface1 = GColorFromHEX(0x45475A),
  .surface0 = GColorFromHEX(0x313244),
  .base = GColorFromHEX(0x1E1E2E),
  .mantle = GColorFromHEX(0x181825),
  .crust = GColorFromHEX(0x11111B),
};

typedef struct {
  CatppuccinFlavor flavor;
} AppSettings;

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_battery_layer;
static GFont s_time_font;
static GFont s_date_font;
static int s_battery_level;
static AppSettings s_settings;
static const CatppuccinPalette *s_palette;

static const CatppuccinPalette *palette_for_flavor(CatppuccinFlavor flavor) {
  switch (flavor) {
    case CATPPUCCIN_FLAVOR_LATTE: return &CATPPUCCIN_PALETTE_LATTE;
    case CATPPUCCIN_FLAVOR_FRAPPE: return &CATPPUCCIN_PALETTE_FRAPPE;
    case CATPPUCCIN_FLAVOR_MACCHIATO: return &CATPPUCCIN_PALETTE_MACCHIATO;
    case CATPPUCCIN_FLAVOR_MOCHA:
    default: return &CATPPUCCIN_PALETTE_MOCHA;
  }
}

static CatppuccinFlavor flavor_from_string(const char *value) {
  if (!value) return CATPPUCCIN_FLAVOR_MOCHA;
  if (strcmp(value, "latte") == 0) return CATPPUCCIN_FLAVOR_LATTE;
  if (strcmp(value, "frappe") == 0) return CATPPUCCIN_FLAVOR_FRAPPE;
  if (strcmp(value, "macchiato") == 0) return CATPPUCCIN_FLAVOR_MACCHIATO;
  return CATPPUCCIN_FLAVOR_MOCHA;
}

static void save_settings(void) {
  persist_write_int(SETTINGS_KEY, s_settings.flavor);
}

static void load_settings(void) {
  s_settings.flavor = CATPPUCCIN_FLAVOR_MOCHA;
  if (persist_exists(SETTINGS_KEY)) {
    int stored = persist_read_int(SETTINGS_KEY);
    if (stored >= CATPPUCCIN_FLAVOR_LATTE && stored <= CATPPUCCIN_FLAVOR_MOCHA) {
      s_settings.flavor = (CatppuccinFlavor)stored;
    }
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
}

static GColor battery_color_for_level(int level) {
  if (level <= 10) {
    return s_palette->red;
  }
  if (level <= 25) {
    return s_palette->yellow;
  }
  return s_palette->green;
}

static void battery_layer_update_color(void) {
  if (s_battery_layer) {
    text_layer_set_text_color(s_battery_layer, battery_color_for_level(s_battery_level));
  }
}

static void battery_layer_update_text(void) {
  if (s_battery_layer) {
    static char s_battery_buffer[8];
    snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", s_battery_level);
    text_layer_set_text(s_battery_layer, s_battery_buffer);
  }
}

static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
  if (s_battery_layer) {
    battery_layer_update_text();
    battery_layer_update_color();
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  Tuple *flavor_tuple = dict_find(iterator, MESSAGE_KEY_CATPPUCCIN_FLAVOR);
  if (!flavor_tuple || flavor_tuple->type != TUPLE_CSTRING) {
    return;
  }

  CatppuccinFlavor flavor = flavor_from_string(flavor_tuple->value->cstring);
  if (flavor != s_settings.flavor) {
    s_settings.flavor = flavor;
    s_palette = palette_for_flavor(s_settings.flavor);
    save_settings();
    apply_palette();
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
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CASKAY_56));
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_CASKAY_24));

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int date_height = 30;
  int block_height = 56 + date_height;
  int time_y = (bounds.size.h / 2) - (block_height / 2) - 10;
  int date_y = time_y + 56;

  s_time_layer = text_layer_create(GRect(0, time_y, bounds.size.w, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, s_palette->sky);
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  s_date_layer = text_layer_create(GRect(0, date_y, bounds.size.w, 30));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, s_palette->mauve);
  text_layer_set_font(s_date_layer, s_date_font);
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));

  int margin = 2;
  int battery_width = 44;
  s_battery_layer = text_layer_create(GRect(bounds.size.w - battery_width - margin, margin, battery_width, 24));
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_text_color(s_battery_layer, s_palette->text);
  text_layer_set_font(s_battery_layer, s_date_font);
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
  battery_layer_update_text();
  battery_layer_update_color();

  apply_palette();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_battery_layer);
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
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
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
