#include <pebble.h>
#include <stdio.h>
#include "catppuccin.h"
#include "battery.h"

static TextLayer *g_battery_layer = NULL;
static const CatppuccinPalette *g_palette = NULL;
static int g_battery_level = 0;

void battery_set_palette(const CatppuccinPalette *s_palette) {
  g_palette = s_palette;
}

void battery_callback(BatteryChargeState state) {
  g_battery_level = state.charge_percent;
  if (g_battery_layer) {
    battery_layer_update_text(g_battery_layer, g_battery_level);
    battery_layer_update_color(g_battery_layer, g_battery_level, g_palette);
  }
}

void battery_window_load(Window *window, GFont battery_font, const CatppuccinPalette *palette) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int margin = 2;
  int battery_width = 36;

  if (!g_battery_layer) {
    g_battery_layer = text_layer_create(GRect(bounds.size.w - battery_width - margin, margin, battery_width, 24));
    text_layer_set_background_color(g_battery_layer, GColorClear);
    text_layer_set_font(g_battery_layer, battery_font);
    layer_add_child(window_layer, text_layer_get_layer(g_battery_layer));
  }

  battery_set_palette(palette);
  battery_layer_update_text(g_battery_layer, g_battery_level);
  battery_layer_update_color(g_battery_layer, g_battery_level, g_palette);
}

void battery_window_unload(void) {
  if (g_battery_layer) {
    text_layer_destroy(g_battery_layer);
    g_battery_layer = NULL;
  }
  g_palette = NULL;
}

static GColor battery_color_for_level(const CatppuccinPalette *palette, int level) {
  // Defensive: if palette is not set yet, return a safe default color.
  if (!palette) return GColorWhite;
  if (level <= 10) {
    return palette->green;
  }
  if (level <= 30) {
    return palette->green;
  }
  return palette->green;
}

void battery_layer_update_color(TextLayer *s_battery_layer, int s_battery_level, const CatppuccinPalette *s_palette) {
  if (s_battery_layer) {
    const CatppuccinPalette *p = s_palette ? s_palette : g_palette;
    text_layer_set_text_color(s_battery_layer, battery_color_for_level(p, s_battery_level));
  }
}

void battery_layer_update_text(TextLayer *s_battery_layer, int s_battery_level) {
  if (s_battery_layer) {
    static char s_battery_buffer[8];
    if (s_battery_level < 10) {
      snprintf(s_battery_buffer, sizeof(s_battery_buffer), "");
    }
    else if (s_battery_level < 30) {
      snprintf(s_battery_buffer, sizeof(s_battery_buffer), "");
    }
    else if (s_battery_level < 60) {
      snprintf(s_battery_buffer, sizeof(s_battery_buffer), "");
    }
    else if (s_battery_level < 80) {
      snprintf(s_battery_buffer, sizeof(s_battery_buffer), "");
    }
    else {
      snprintf(s_battery_buffer, sizeof(s_battery_buffer), "");
    }
    text_layer_set_text(s_battery_layer, s_battery_buffer);
  }
}
