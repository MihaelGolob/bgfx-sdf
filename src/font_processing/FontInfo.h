//
// Created by MihaelGolob on 30. 04. 2024.
//

#pragma once

#include <string>

enum class FontType {
    Bitmap, // normal
    SdfFromBitmap, // signed distance field
    SdfFromVector,
    Msdf, // multichannel signed distance field
    Color, // multichannel color bitmap
};

struct FontInfo {
    /// The font height in pixel.
    unsigned short pixel_size;
    /// Rendering type used for the font.
    FontType font_type;

    /// The pixel extents above the baseline in pixels (typically positive).
    float ascender;
    /// The extents below the baseline in pixels (typically negative).
    float descender;
    /// The spacing in pixels between one row's descent and the next row's ascent.
    float line_gap;
    /// This field gives the maximum horizontal cursor advance for all glyphs in the font.
    float max_advance_width;
    /// The thickness of the under/hover/strike-trough line in pixels.
    float underline_thickness;
    /// The position of the underline relatively to the baseline.
    float underline_position;

    /// Scale to apply to glyph data.
    float scale;
    
    std::string FontTypeToString(FontType type) {
        switch (type) {
            case FontType::Bitmap:
                return "Bitmap";
            case FontType::SdfFromBitmap:
                return "SdfFromBitmap";
            case FontType::SdfFromVector:
                return "SdfFromVector";
            case FontType::Msdf:
                return "Msdf";
            case FontType::Color:
                return "Color";
        }
    }
};
