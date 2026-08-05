#pragma once

#include <pebble.h>
#include "catppuccin.h"

void steps_window_load(Window *window, GFont icon_font, const CatppuccinPalette *palette, bool enabled);
void steps_window_unload(void);

void steps_set_palette(const CatppuccinPalette *palette);

void steps_set_enabled(bool enabled);

void steps_update_count(int steps);
