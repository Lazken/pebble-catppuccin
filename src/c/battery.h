#pragma once

#include <pebble.h>
#include "catppuccin.h"

void battery_layer_update_text(TextLayer *s_battery_layer, int s_battery_level);

void battery_layer_update_color(TextLayer *s_battery_layer, int s_battery_level, const CatppuccinPalette *s_palette);

void battery_set_palette(const CatppuccinPalette *s_palette);

void battery_callback(BatteryChargeState state);

void battery_window_load(Window *window, GFont battery_font, const CatppuccinPalette *palette);

void battery_window_unload(void);
