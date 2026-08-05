#include <pebble.h>
#include <stdio.h>
#include "catppuccin.h"
#include "steps.h"

static TextLayer *g_steps_layer = NULL;
static const CatppuccinPalette *g_palette = NULL;
static bool g_enabled = false;
static int g_steps = -1; // -1 = unknown
static GFont g_font = NULL;

static void update_layer_text(void) {
  if (!g_steps_layer) return;
  static char buffer[32];
  /* Use the walking glyph (private use area U+E213) followed by the steps count. */
  if (g_steps >= 0) {
    snprintf(buffer, sizeof(buffer), " %d", g_steps);
  } else {
    snprintf(buffer, sizeof(buffer), " --");
  }
  text_layer_set_text(g_steps_layer, buffer);
}

void steps_set_palette(const CatppuccinPalette *palette) {
  g_palette = palette;
  if (g_steps_layer && g_palette) {
    // Steps should use the palette's peach color (not red). Use peach.
    text_layer_set_text_color(g_steps_layer, g_palette->peach);
  }
}

#if defined(PBL_HEALTH)
static void health_handler(HealthEventType event, void *context) {
  // On any health event, peek the current step count and update if available
  HealthValue v = health_service_peek_current_value(HealthMetricStepCount);
  if (v >= 0) {
    steps_update_count((int)v);
  }
}
#endif

void steps_set_enabled(bool enabled) {
  if (g_enabled == enabled) return;
  g_enabled = enabled;
  if (g_enabled) {
#if defined(PBL_HEALTH)
    time_t now = time(NULL);
    HealthServiceAccessibilityMask accessible = health_service_metric_accessible(HealthMetricStepCount, now, now);
    if (accessible & HealthServiceAccessibilityMaskAvailable) {
      HealthValue v = health_service_peek_current_value(HealthMetricStepCount);
      if (v >= 0) steps_update_count((int)v);
      health_service_events_subscribe(health_handler, NULL);
    }
#endif
  } else {
#if defined(PBL_HEALTH)
    health_service_events_unsubscribe();
#endif
    g_steps = -1;
    update_layer_text();
  }
}

void steps_update_count(int steps) {
  g_steps = steps;
  update_layer_text();
}

void steps_window_load(Window *window, GFont icon_font, const CatppuccinPalette *palette, bool enabled) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  int margin = 2;
  int height = 24;

  g_font = icon_font;
  g_palette = palette;

  if (!g_steps_layer) {
    g_steps_layer = text_layer_create(GRect(0, bounds.size.h - height - margin, bounds.size.w, height));
    text_layer_set_background_color(g_steps_layer, GColorClear);
    if (g_font) text_layer_set_font(g_steps_layer, g_font);
    text_layer_set_text_alignment(g_steps_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(g_steps_layer));
  }

  if (g_palette) text_layer_set_text_color(g_steps_layer, g_palette->peach);
  else text_layer_set_text_color(g_steps_layer, GColorWhite); // debug fallback

  g_steps = -1;
  update_layer_text();

  (void)g_steps_layer; // debug logging removed

  g_enabled = false;
  steps_set_enabled(enabled);
}

void steps_window_unload(void) {
  if (g_steps_layer) {
    text_layer_destroy(g_steps_layer);
    g_steps_layer = NULL;
  }
#if defined(PBL_HEALTH)
  health_service_events_unsubscribe();
#endif
  g_palette = NULL;
  g_enabled = false;
  g_steps = -1;
  g_font = NULL;
}
