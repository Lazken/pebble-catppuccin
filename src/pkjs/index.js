var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

// Ensure FontAwesome heart glyph (U+F004, "") is present in the generated JS bundle
// so the font subsetting picks up the glyph for FONT_CASKAYICONS_24.
var __font_glyph_heart = '';
// Ensure walking glyph (nerdfont private-use U+E213) is included too.
var __font_glyph_walking = '';
