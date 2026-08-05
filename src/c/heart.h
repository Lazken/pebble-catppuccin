#pragma once

#include <pebble.h>
#include "catppuccin.h"

// Create/destroy UI for heart-rate indicator
void heart_window_load(Window *window, GFont heart_font, const CatppuccinPalette *palette, bool enabled);
void heart_window_unload(void);

// Update palette (called when flavor/palette changes)
void heart_set_palette(const CatppuccinPalette *palette);

// Enable/disable the heart indicator (subscribe/unsubscribe)
void heart_set_enabled(bool enabled);

// Manually update the displayed bpm (useful for tests)
void heart_update_rate(int bpm);
