#include <pebble.h>
#include <stdio.h>
#include "catppuccin.h"
#include "heart.h"

static TextLayer *g_heart_layer = NULL;
static const CatppuccinPalette *g_palette = NULL;
static bool g_enabled = false;
static int g_bpm = -1; // -1 = unknown
static GFont g_font = NULL;
static AppTimer *g_periodic_timer = NULL;
static AppTimer *g_sample_timeout_timer = NULL;
static uint32_t g_sample_interval_seconds = 60; // default, can be changed from config
static const uint32_t g_sample_window_seconds = 10; // how long to allow sampling each attempt
static bool g_sampling_active = false;

static void update_layer_text(void) {
  if (!g_heart_layer) return;
  static char buffer[16];
  if (g_bpm >= 0) {
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

static void health_handler(HealthEventType event, void *context) {
  if (event == HealthEventHeartRateUpdate) {
    HealthValue v = health_service_peek_current_value(HealthMetricHeartRateBPM);
    if (v > 0) {
      heart_update_rate((int)v);
    }
  }
}

static void heart_unsubscribe_all(void) {
#if defined(PBL_HEALTH)
  health_service_events_unsubscribe();
#endif
}

static void start_sample_now(void);

static void sampling_expired_cb(void *context) {
#if defined(PBL_HEALTH)
  health_service_set_heart_rate_sample_period(0);
  health_service_events_unsubscribe();
#endif
  g_sampling_active = false;
  if (g_sample_timeout_timer) {
    app_timer_cancel(g_sample_timeout_timer);
    g_sample_timeout_timer = NULL;
  }
}

static void periodic_timer_cb(void *context) {
  if (!g_enabled) return;
  if (g_sampling_active) {
    if (g_periodic_timer) { app_timer_cancel(g_periodic_timer); g_periodic_timer = NULL; }
    g_periodic_timer = app_timer_register(g_sample_interval_seconds * 1000, periodic_timer_cb, NULL);
    return;
  }
  start_sample_now();
  if (g_periodic_timer) { app_timer_cancel(g_periodic_timer); g_periodic_timer = NULL; }
  g_periodic_timer = app_timer_register(g_sample_interval_seconds * 1000, periodic_timer_cb, NULL);
}

static void start_periodic_timer(void) {
  if (g_periodic_timer) return;
  g_periodic_timer = app_timer_register(g_sample_interval_seconds * 1000, periodic_timer_cb, NULL);
}

static void stop_periodic_timer(void) {
  if (g_periodic_timer) {
    app_timer_cancel(g_periodic_timer);
    g_periodic_timer = NULL;
  }
}

static void start_sample_now(void) {
  if (!g_enabled) return;
#if defined(PBL_HEALTH)
  {
    time_t now = time(NULL);
    HealthServiceAccessibilityMask accessible = health_service_metric_accessible(HealthMetricHeartRateBPM, now, now);
    if (!(accessible & HealthServiceAccessibilityMaskAvailable)) return;
  }
  health_service_set_heart_rate_sample_period((uint16_t)g_sample_window_seconds);
  health_service_events_subscribe(health_handler, NULL);
  g_sampling_active = true;
  {
    HealthValue v = health_service_peek_current_value(HealthMetricHeartRateBPM);
    if (v > 0) {
      heart_update_rate((int)v);
    }
  }
  if (g_sample_timeout_timer) { app_timer_cancel(g_sample_timeout_timer); g_sample_timeout_timer = NULL; }
  g_sample_timeout_timer = app_timer_register(g_sample_window_seconds * 1000, sampling_expired_cb, NULL);
#endif
}

static void cancel_active_sampling(void) {
  if (g_sample_timeout_timer) { app_timer_cancel(g_sample_timeout_timer); g_sample_timeout_timer = NULL; }
#if defined(PBL_HEALTH)
  health_service_set_heart_rate_sample_period(0);
  health_service_events_unsubscribe();
#endif
  g_sampling_active = false;
}

void heart_set_enabled(bool enabled) {
  if (g_enabled == enabled) return;
  g_enabled = enabled;
  if (g_enabled) {
    start_sample_now();
    start_periodic_timer();
  } else {
    stop_periodic_timer();
    cancel_active_sampling();
    g_bpm = -1;
    update_layer_text();
  }
}

void heart_update_rate(int bpm) {
  g_bpm = bpm;
  update_layer_text();
}

void heart_set_sample_interval_seconds(uint32_t seconds) {
  if (seconds < 15) seconds = 15;
  g_sample_interval_seconds = seconds;
  if (g_periodic_timer) {
    app_timer_cancel(g_periodic_timer);
    g_periodic_timer = app_timer_register(g_sample_interval_seconds * 1000, periodic_timer_cb, NULL);
  }
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
