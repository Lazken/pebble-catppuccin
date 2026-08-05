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

typedef enum { HEART_MODE_PASSIVE = 0, HEART_MODE_LIVE = 1 } HeartMode;

// Set the heart monitoring mode: PASSIVE = peek+subscribe, LIVE = continuous sampling
void heart_set_mode(HeartMode mode);
