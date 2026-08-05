#include <pebble.h>
#include <stdio.h>
#include "catppuccin.h"
#include "heart.h"

static TextLayer *g_heart_layer = NULL;
static const CatppuccinPalette *g_palette = NULL;
static bool g_enabled = false;
static int g_bpm = -1; // -1 = unknown
static GFont g_font = NULL;

static void update_layer_text(void) {
  if (!g_heart_layer) return;
  static char buffer[16];
  // FontAwesome heart glyph (U+F004) literal: ""
  if (g_bpm >= 0) {
    /* Use literal FontAwesome heart glyph (U+F004). The glyph is also
       present in the JS bundle so the font subsetting picks it up. */
    snprintf(buffer, sizeof(buffer), " %d", g_bpm);
  } else {
    snprintf(buffer, sizeof(buffer), " --");
  }
  text_layer_set_text(g_heart_layer, buffer);
}

void heart_set_palette(const CatppuccinPalette *palette) {
  g_palette = palette;
  if (g_heart_layer && g_palette) {
    text_layer_set_text_color(g_heart_layer, g_palette->red);
  }
}

// HealthService handler (used on devices with PBL_HEALTH)
static void health_handler(HealthEventType event, void *context) {
  if (event == HealthEventHeartRateUpdate) {
    HealthValue v = health_service_peek_current_value(HealthMetricHeartRateBPM);
    if (v > 0) {
      heart_update_rate((int)v);
    }
  }
}

static void heart_unsubscribe_all(void) {
  // Unsubscribe HealthService events if available
#if defined(PBL_HEALTH)
  health_service_events_unsubscribe();
#endif
}

static void heart_subscribe(void) {
  if (!g_enabled) return;
#if defined(PBL_HEALTH)
  // Subscribe to HealthService events for heart-rate updates.
  health_service_events_subscribe(health_handler, NULL);
  // Try to peek latest heart rate now
  {
    HealthValue v = health_service_peek_current_value(HealthMetricHeartRateBPM);
    if (v > 0) {
      heart_update_rate((int)v);
    }
  }
#endif
  update_layer_text();
}

static void heart_unsubscribe(void) {
  heart_unsubscribe_all();
  g_bpm = -1;
  update_layer_text();
}

void heart_set_enabled(bool enabled) {
  if (g_enabled == enabled) return;
  g_enabled = enabled;
  if (g_enabled) {
    heart_subscribe();
  } else {
    heart_unsubscribe();
  }
}

void heart_update_rate(int bpm) {
  g_bpm = bpm;
  update_layer_text();
}

void heart_window_load(Window *window, GFont heart_font, const CatppuccinPalette *palette, bool enabled) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  int margin = 2;
  int width = 56;
  g_font = heart_font;
  g_palette = palette;

  if (!g_heart_layer) {
    g_heart_layer = text_layer_create(GRect(margin, margin, width, 24));
    text_layer_set_background_color(g_heart_layer, GColorClear);
    if (g_font) text_layer_set_font(g_heart_layer, g_font);
    text_layer_set_text_alignment(g_heart_layer, GTextAlignmentLeft);
    layer_add_child(window_layer, text_layer_get_layer(g_heart_layer));
  }

  if (g_palette) text_layer_set_text_color(g_heart_layer, g_palette->red);

  g_bpm = -1;
  update_layer_text();

  g_enabled = false;
  heart_set_enabled(enabled);
}

void heart_window_unload(void) {
  if (g_heart_layer) {
    text_layer_destroy(g_heart_layer);
    g_heart_layer = NULL;
  }
  heart_unsubscribe_all();
  g_palette = NULL;
  g_enabled = false;
  g_bpm = -1;
  g_font = NULL;
}
