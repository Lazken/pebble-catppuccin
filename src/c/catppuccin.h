#pragma once

#include <pebble.h>
#include <stdio.h>

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
extern const CatppuccinPalette CATPPUCCIN_PALETTE_LATTE;
extern const CatppuccinPalette CATPPUCCIN_PALETTE_FRAPPE;
extern const CatppuccinPalette CATPPUCCIN_PALETTE_MACCHIATO;
extern const CatppuccinPalette CATPPUCCIN_PALETTE_MOCHA;

const CatppuccinPalette *palette_for_flavor(CatppuccinFlavor flavor);
CatppuccinFlavor flavor_from_string(const char *value);
