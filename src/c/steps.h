#pragma once

#include <pebble.h>
#include "catppuccin.h"

// Create/destroy UI for steps indicator
void steps_window_load(Window *window, GFont icon_font, const CatppuccinPalette *palette, bool enabled);
void steps_window_unload(void);

// Update palette (called when flavor/palette changes)
void steps_set_palette(const CatppuccinPalette *palette);

// Enable/disable the steps indicator (subscribe/unsubscribe)
void steps_set_enabled(bool enabled);

// Manually update the displayed steps (useful for tests)
void steps_update_count(int steps);
