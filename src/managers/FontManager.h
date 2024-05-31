//
// Created by Mihael Golob on 22. 04. 2024.
//
/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#pragma once

#include <bx/handlealloc.h>
#include <bx/string.h>

#include "../FontAtlas/GlyphInfo.h"
#include "../FontAtlas/FontInfo.h"
#include "../FontAtlas/FontHandles.h"

class Atlas;

#define MAX_OPENED_FILES 64
#define MAX_OPENED_FONT  64
#define FONT_TYPE_ALPHA UINT32_C(0x00000100)

/// Unicode value of a character
typedef int32_t CodePoint;

class FontManager {
public:
    /// Create the font manager using an external cube atlas (doesn't take ownership of the atlas).
    FontManager(Atlas *_atlas);

    /// Create the font manager and create the texture cube as BGRA8 with linear filtering.
    FontManager(uint16_t _textureSideWidth = 512);

    ~FontManager();

    /// Retrieve the atlas used by the font manager (e.g. to add stuff to it)
    const Atlas *getAtlas() const {
        return m_atlas;
    }

    /// Load a TrueType font from a given buffer. The buffer is copied and
    /// thus can be freed or reused after this call.
    ///
    /// @return invalid handle if the loading fail
    TrueTypeHandle createTtf(const uint8_t *_buffer, uint32_t _size);

    /// Unload a TrueType font (free font memory) but keep loaded glyphs.
    void destroyTtf(TrueTypeHandle _handle);

    /// Return a font whose height is a fixed pixel size.
    FontHandle createFontByPixelSize(TrueTypeHandle _handle, uint32_t _typefaceIndex, uint32_t _pixelSize,
                                     FontType _fontType = FontType::Bitmap, uint16_t _glyphPadding = 6);

    /// Return a scaled child font whose height is a fixed pixel size.
    FontHandle createScaledFontToPixelSize(FontHandle _baseFontHandle, uint32_t _pixelSize);

    /// destroy a font (truetype or baked)
    void destroyFont(FontHandle _handle);

    /// Preload a set of glyphs from a TrueType file.
    ///
    /// @return True if every glyph could be preloaded, false otherwise if
    ///   the Font is a baked font, this only do validation on the characters.
    bool preloadGlyph(FontHandle _handle, const wchar_t *_string);

    /// Preload a single glyph, return true on success.
    bool preloadGlyph(FontHandle _handle, CodePoint _character);

    bool addGlyphBitmap(FontHandle _handle, CodePoint _character, uint16_t _width, uint16_t height, uint16_t _pitch,
                        float extraScale, const uint8_t *_bitmapBuffer, float glyphOffsetX, float glyphOffsetY);

    /// Return the font descriptor of a font.
    ///
    /// @remark the handle is required to be valid
    const FontInfo &getFontInfo(FontHandle _handle) const;

    /// Return the rendering information about the glyph region. Load the
    /// glyph from a TrueType font if possible
    ///
    const GlyphInfo *getGlyphInfo(FontHandle _handle, CodePoint _codePoint);

    float getKerning(FontHandle _handle, CodePoint _prevCodePoint, CodePoint _codePoint);

    const GlyphInfo &getBlackGlyph() const {
        return m_blackGlyph;
    }

private:
    struct CachedFont;
    struct CachedFile {
        uint8_t *buffer;
        uint32_t bufferSize;
    };

    void init();

    bool addBitmap(GlyphInfo &_glyphInfo, const uint8_t *_data);

    bool m_ownAtlas;
    Atlas *m_atlas;

    bx::HandleAllocT<MAX_OPENED_FONT> m_fontHandles;
    CachedFont *m_cachedFonts;

    bx::HandleAllocT<MAX_OPENED_FILES> m_filesHandles;
    CachedFile *m_cachedFiles;

    GlyphInfo m_blackGlyph;

    //temporary buffer to raster glyph
    uint8_t *m_buffer;
};
