//
// Created by MihaelGolob on 30. 04. 2024.
//

#pragma once

enum class FontType {
    Bitmap, // normal
    SDF, // signed distance field
    MSDF, // multichannel signed distance field
};

struct FontInfo {
    /// The font height in pixel.
    unsigned short pixelSize;
    /// Rendering type used for the font.
    FontType fontType;

    /// The pixel extents above the baseline in pixels (typically positive).
    float ascender;
    /// The extents below the baseline in pixels (typically negative).
    float descender;
    /// The spacing in pixels between one row's descent and the next row's ascent.
    float lineGap;
    /// This field gives the maximum horizontal cursor advance for all glyphs in the font.
    float maxAdvanceWidth;
    /// The thickness of the under/hover/strike-trough line in pixels.
    float underlineThickness;
    /// The position of the underline relatively to the baseline.
    float underlinePosition;

    /// Scale to apply to glyph data.
    float scale;
};
