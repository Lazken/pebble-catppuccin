#include "catppuccin.h"
#include <string.h>

const CatppuccinPalette CATPPUCCIN_PALETTE_LATTE = {
  .rosewater = GColorFromHEX(0xDC8A78),
  .flamingo = GColorFromHEX(0xDD7878),
  .pink = GColorFromHEX(0xEA76CB),
  .mauve = GColorFromHEX(0x8839EF),
  .red = GColorFromHEX(0xD20F39),
  .maroon = GColorFromHEX(0xE64553),
  .peach = GColorFromHEX(0xFE640B),
  .yellow = GColorFromHEX(0xDF8E1D),
  .green = GColorFromHEX(0x40A02B),
  .teal = GColorFromHEX(0x179299),
  .sky = GColorFromHEX(0x04A5E5),
  .sapphire = GColorFromHEX(0x209FB5),
  .blue = GColorFromHEX(0x1E66F5),
  .lavender = GColorFromHEX(0x7287FD),
  .text = GColorFromHEX(0x4C4F69),
  .subtext1 = GColorFromHEX(0x5C5F77),
  .subtext0 = GColorFromHEX(0x6C6F85),
  .overlay2 = GColorFromHEX(0x7C7F93),
  .overlay1 = GColorFromHEX(0x8C8FA1),
  .overlay0 = GColorFromHEX(0x9CA0B0),
  .surface2 = GColorFromHEX(0xACB0BE),
  .surface1 = GColorFromHEX(0xBCC0CC),
  .surface0 = GColorFromHEX(0xCCD0DA),
  .base = GColorFromHEX(0xEFF1F5),
  .mantle = GColorFromHEX(0xE6E9EF),
  .crust = GColorFromHEX(0xDCE0E8),
};

const CatppuccinPalette CATPPUCCIN_PALETTE_FRAPPE = {
  .rosewater = GColorFromHEX(0xF2D5CF),
  .flamingo = GColorFromHEX(0xEEBEBE),
  .pink = GColorFromHEX(0xF4B8E4),
  .mauve = GColorFromHEX(0xCA9EE6),
  .red = GColorFromHEX(0xE78284),
  .maroon = GColorFromHEX(0xEA999C),
  .peach = GColorFromHEX(0xEF9F76),
  .yellow = GColorFromHEX(0xE5C890),
  .green = GColorFromHEX(0xA6D189),
  .teal = GColorFromHEX(0x81C8BE),
  .sky = GColorFromHEX(0x99D1DB),
  .sapphire = GColorFromHEX(0x85C1DC),
  .blue = GColorFromHEX(0x8CAAEE),
  .lavender = GColorFromHEX(0xBABBF1),
  .text = GColorFromHEX(0xC6D0F5),
  .subtext1 = GColorFromHEX(0xB5BFE2),
  .subtext0 = GColorFromHEX(0xA5ADCE),
  .overlay2 = GColorFromHEX(0x949CBB),
  .overlay1 = GColorFromHEX(0x838BA7),
  .overlay0 = GColorFromHEX(0x737994),
  .surface2 = GColorFromHEX(0x626880),
  .surface1 = GColorFromHEX(0x51576D),
  .surface0 = GColorFromHEX(0x414559),
  .base = GColorFromHEX(0x303446),
  .mantle = GColorFromHEX(0x292C3C),
  .crust = GColorFromHEX(0x232634),
};

const CatppuccinPalette *palette_for_flavor(CatppuccinFlavor flavor) {
  switch (flavor) {
    case CATPPUCCIN_FLAVOR_LATTE: return &CATPPUCCIN_PALETTE_LATTE;
    case CATPPUCCIN_FLAVOR_FRAPPE: return &CATPPUCCIN_PALETTE_FRAPPE;
    case CATPPUCCIN_FLAVOR_MACCHIATO: return &CATPPUCCIN_PALETTE_MACCHIATO;
    case CATPPUCCIN_FLAVOR_MOCHA:
    default: return &CATPPUCCIN_PALETTE_MOCHA;
  }
}

CatppuccinFlavor flavor_from_string(const char *value) {
  if (!value) return CATPPUCCIN_FLAVOR_MOCHA;
  if (strcmp(value, "latte") == 0) return CATPPUCCIN_FLAVOR_LATTE;
  if (strcmp(value, "frappe") == 0) return CATPPUCCIN_FLAVOR_FRAPPE;
  if (strcmp(value, "macchiato") == 0) return CATPPUCCIN_FLAVOR_MACCHIATO;
  return CATPPUCCIN_FLAVOR_MOCHA;
}

const CatppuccinPalette CATPPUCCIN_PALETTE_MACCHIATO = {
  .rosewater = GColorFromHEX(0xF4DBD6),
  .flamingo = GColorFromHEX(0xF0C6C6),
  .pink = GColorFromHEX(0xF5BDE6),
  .mauve = GColorFromHEX(0xC6A0F6),
  .red = GColorFromHEX(0xED8796),
  .maroon = GColorFromHEX(0xEE99A0),
  .peach = GColorFromHEX(0xF5A97F),
  .yellow = GColorFromHEX(0xEED49F),
  .green = GColorFromHEX(0xA6DA95),
  .teal = GColorFromHEX(0x8BD5CA),
  .sky = GColorFromHEX(0x91D7E3),
  .sapphire = GColorFromHEX(0x7DC4E4),
  .blue = GColorFromHEX(0x8AADF4),
  .lavender = GColorFromHEX(0xB7BDF8),
  .text = GColorFromHEX(0xCAD3F5),
  .subtext1 = GColorFromHEX(0xB8C0E0),
  .subtext0 = GColorFromHEX(0xA5ADCB),
  .overlay2 = GColorFromHEX(0x939AB7),
  .overlay1 = GColorFromHEX(0x8087A2),
  .overlay0 = GColorFromHEX(0x6E738D),
  .surface2 = GColorFromHEX(0x5B6078),
  .surface1 = GColorFromHEX(0x494D64),
  .surface0 = GColorFromHEX(0x363A4F),
  .base = GColorFromHEX(0x24273A),
  .mantle = GColorFromHEX(0x1E2030),
  .crust = GColorFromHEX(0x181926),
};

const CatppuccinPalette CATPPUCCIN_PALETTE_MOCHA = {
  .rosewater = GColorFromHEX(0xF5E0DC),
  .flamingo = GColorFromHEX(0xF2CDCD),
  .pink = GColorFromHEX(0xF5C2E7),
  .mauve = GColorFromHEX(0xCBA6F7),
  .red = GColorFromHEX(0xF38BA8),
  .maroon = GColorFromHEX(0xEBA0AC),
  .peach = GColorFromHEX(0xFAB387),
  .yellow = GColorFromHEX(0xF9E2AF),
 // .green = GColorFromHEX(0xA6E3A1),
  .green = GColorGreen,
  .teal = GColorFromHEX(0x94E2D5),
  .sky = GColorFromHEX(0x89DCEB),
  .sapphire = GColorFromHEX(0x74C7EC),
  .blue = GColorFromHEX(0x89B4FA),
  .lavender = GColorFromHEX(0xB4BEFE),
  .text = GColorFromHEX(0xCDD6F4),
  .subtext1 = GColorFromHEX(0xBAC2DE),
  .subtext0 = GColorFromHEX(0xA6ADC8),
  .overlay2 = GColorFromHEX(0x9399B2),
  .overlay1 = GColorFromHEX(0x7F849C),
  .overlay0 = GColorFromHEX(0x6C7086),
  .surface2 = GColorFromHEX(0x585B70),
  .surface1 = GColorFromHEX(0x45475A),
  .surface0 = GColorFromHEX(0x313244),
  .base = GColorFromHEX(0x1E1E2E),
  .mantle = GColorFromHEX(0x181825),
  .crust = GColorFromHEX(0x11111B),
};
