//
// Created by MihaelGolob on 30. 04. 2024.
//
#pragma once

/// A structure that describe a glyph.
struct GlyphInfo {
    /// Index for faster retrieval.
    int glyphIndex;

    /// Glyph's width in pixels.
    float width;

    /// Glyph's height in pixels.
    float height;

    /// Glyph's left offset in pixels
    float offset_x;

    /// Glyph's top offset in pixels.
    ///
    /// @remark This is the distance from the baseline to the top-most glyph
    ///   scan line, upwards y coordinates being positive.
    float offset_y;

    /// For horizontal text layouts, this is the unscaled horizontal
    /// distance in pixels used to increment the pen position when the
    /// glyph is drawn as part of a string of text.
    float advance_x;

    /// For vertical text layouts, this is the unscaled vertical distance
    /// in pixels used to increment the pen position when the glyph is
    /// drawn as part of a string of text.
    float advance_y;

    /// Amount to scale a bitmap image glyph.
    float bitmapScale;

    /// Region index in the atlas storing textures.
    unsigned short regionIndex;
};

// Glyph metrics:
// --------------
//
//                       xmin                     xmax
//                        |                         |
//                        |<-------- width -------->|
//                        |                         |
//              |         +-------------------------+----------------- ymax
//              |         |    ggggggggg   ggggg    |     ^        ^
//              |         |   g:::::::::ggg::::g    |     |        |
//              |         |  g:::::::::::::::::g    |     |        |
//              |         | g::::::ggggg::::::gg    |     |        |
//              |         | g:::::g     g:::::g     |     |        |
//    offset_x -|-------->| g:::::g     g:::::g     |  offset_y    |
//              |         | g:::::g     g:::::g     |     |        |
//              |         | g::::::g    g:::::g     |     |        |
//              |         | g:::::::ggggg:::::g     |     |        |
//              |         |  g::::::::::::::::g     |     |      height
//              |         |   gg::::::::::::::g     |     |        |
//  baseline ---*---------|---- gggggggg::::::g-----*--------      |
//            / |         |             g:::::g     |              |
//     origin   |         | gggggg      g:::::g     |              |
//              |         | g:::::gg   gg:::::g     |              |
//              |         |  g::::::ggg:::::::g     |              |
//              |         |   gg:::::::::::::g      |              |
//              |         |     ggg::::::ggg        |              |
//              |         |         gggggg          |              v
//              |         +-------------------------+----------------- ymin
//              |                                   |
//              |------------- advance_x ---------->|
