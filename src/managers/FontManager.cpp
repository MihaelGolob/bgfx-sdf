//
// Created by MihaelGolob on 22. 04. 2024.
//

/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#define STB_TRUETYPE_IMPLEMENTATION

#include <bx/bx.h>
#include <bx/math.h>
#include <cwchar> 
#include <tinystl/allocator.h>
#include <tinystl/unordered_map.h>
#include "FontManager.h"
#include "../font_processing/CubeAtlas.h"
#include "../font_processing/TrueTypeFont.h"
#include "../utilities.h"

namespace stl = tinystl;
typedef stl::unordered_map<CodePoint, GlyphInfo> GlyphHashMap;

// cache font data
struct FontManager::CachedFont {
    CachedFont() : trueTypeFont(nullptr) {
        masterFontHandle.idx = bx::kInvalidHandle;
    }

    FontInfo fontInfo{};
    GlyphHashMap cachedGlyphs;
    TrueTypeFont *trueTypeFont;
    // a handle to a master font in case of sub distance field font
    FontHandle masterFontHandle{};
    int16_t padding{};
};

#define MAX_FONT_BUFFER_SIZE (512 * 512 * 4)

FontManager::FontManager(Atlas *_atlas) : m_ownAtlas(false), m_atlas(_atlas) {
    init();
}

FontManager::FontManager(uint16_t _textureSideWidth) : m_ownAtlas(true), m_atlas(new Atlas(_textureSideWidth)) {
    init();
}

void FontManager::init() {
    m_cachedFiles = new CachedFile[MAX_OPENED_FILES];
    m_cachedFonts = new CachedFont[MAX_OPENED_FONT];
    m_buffer = new uint8_t[MAX_FONT_BUFFER_SIZE];
}

FontManager::~FontManager() {
    BX_ASSERT(m_fontHandles.getNumHandles() == 0, "All the fonts must be destroyed before destroying the manager")
    delete[] m_cachedFonts;

    BX_ASSERT(m_filesHandles.getNumHandles() == 0, "All the font files must be destroyed before destroying the manager")
    delete[] m_cachedFiles;
    
    BX_ASSERT(m_ftFaces.empty(), "All the font faces must be destroyed before destroying the manager")
    for (FT_Face face : m_ftFaces) {
        FT_Done_Face(face);
    }
    m_ftFaces.clear();

    delete[] m_buffer;

    if (m_ownAtlas) {
        delete m_atlas;
    }
}

TrueTypeHandle FontManager::createTtf(const uint8_t *_buffer, uint32_t _size) {
    uint16_t id = m_filesHandles.alloc();
    BX_ASSERT(id != bx::kInvalidHandle, "Invalid handle used")
    m_cachedFiles[id].buffer = new uint8_t[_size];
    m_cachedFiles[id].bufferSize = _size;
    bx::memCopy(m_cachedFiles[id].buffer, _buffer, _size);

    TrueTypeHandle ret = {id};
    return ret;
}

FreeTypeHandle FontManager::createFtFace(FT_Library *ft, const char* file_path) {
    uint16_t id = m_ftFaces.size();
    FT_Face face;
    if (FT_New_Face(*ft, file_path, 0, &face)) {
        PrintError("Failed to load font face");
        return {bx::kInvalidHandle};
    }
    m_ftFaces.push_back(face);
    
    return {id};
}

void FontManager::destroyTtf(TrueTypeHandle _handle) {
    BX_ASSERT(isValid(_handle), "Invalid handle used")
    delete[] m_cachedFiles[_handle.idx].buffer;
    m_cachedFiles[_handle.idx].bufferSize = 0;
    m_cachedFiles[_handle.idx].buffer = nullptr;
    m_filesHandles.free(_handle.idx);
}

void FontManager::destroyFtFace(FreeTypeHandle _handle) {
    BX_ASSERT(isValid(_handle), "Invalid handle used")
    for (FT_Face face : m_ftFaces) {
        FT_Done_Face(face);
    }
    m_ftFaces.clear();
}

FontHandle FontManager::createFontByPixelSize(TrueTypeHandle _ttfHandle, uint32_t _typefaceIndex, uint32_t _pixelSize, FontType _fontType, uint16_t _glyphPadding) {
    BX_ASSERT(isValid(_ttfHandle), "Invalid handle used")

    auto ttf = new TrueTypeFont();
    if (!ttf->init(m_cachedFiles[_ttfHandle.idx].buffer, m_cachedFiles[_ttfHandle.idx].bufferSize, _typefaceIndex, _pixelSize, _glyphPadding)) {
        delete ttf;
        FontHandle invalid = {bx::kInvalidHandle};
        return invalid;
    }

    uint16_t fontIdx = m_fontHandles.alloc();
    BX_ASSERT(fontIdx != bx::kInvalidHandle, "Invalid handle used")

    CachedFont &font = m_cachedFonts[fontIdx];
    font.trueTypeFont = ttf;
    font.fontInfo = ttf->getFontInfo();
    font.fontInfo.fontType = _fontType;
    font.fontInfo.pixelSize = uint16_t(_pixelSize);
    font.cachedGlyphs.clear();
    font.masterFontHandle.idx = bx::kInvalidHandle;

    FontHandle handle = {fontIdx};
    return handle;
}

FontHandle FontManager::createScaledFontToPixelSize(FontHandle _baseFontHandle, uint32_t _pixelSize) {
    BX_ASSERT(isValid(_baseFontHandle), "Invalid handle used")
    CachedFont &baseFont = m_cachedFonts[_baseFontHandle.idx];
    FontInfo &fontInfo = baseFont.fontInfo;

    FontInfo newFontInfo = fontInfo;
    newFontInfo.pixelSize = uint16_t(_pixelSize);
    newFontInfo.scale = (float) _pixelSize / (float) fontInfo.pixelSize;
    newFontInfo.ascender = (newFontInfo.ascender * newFontInfo.scale);
    newFontInfo.descender = (newFontInfo.descender * newFontInfo.scale);
    newFontInfo.lineGap = (newFontInfo.lineGap * newFontInfo.scale);
    newFontInfo.maxAdvanceWidth = (newFontInfo.maxAdvanceWidth * newFontInfo.scale);
    newFontInfo.underlineThickness = (newFontInfo.underlineThickness * newFontInfo.scale);
    newFontInfo.underlinePosition = (newFontInfo.underlinePosition * newFontInfo.scale);

    uint16_t fontIdx = m_fontHandles.alloc();
    BX_ASSERT(fontIdx != bx::kInvalidHandle, "Invalid handle used")

    CachedFont &font = m_cachedFonts[fontIdx];
    font.cachedGlyphs.clear();
    font.fontInfo = newFontInfo;
    font.trueTypeFont = nullptr;
    font.masterFontHandle = _baseFontHandle;

    FontHandle handle = {fontIdx};
    return handle;
}

void FontManager::destroyFont(FontHandle _handle) {
    BX_ASSERT(isValid(_handle), "Invalid handle used")

    CachedFont &font = m_cachedFonts[_handle.idx];

    if (font.trueTypeFont != nullptr) {
        delete font.trueTypeFont;
        font.trueTypeFont = nullptr;
    }

    font.cachedGlyphs.clear();
    m_fontHandles.free(_handle.idx);
}

bool FontManager::preloadGlyph(FontHandle _handle, const wchar_t *_string) {
    BX_ASSERT(isValid(_handle), "Invalid handle used")
    CachedFont &font = m_cachedFonts[_handle.idx];

    if (nullptr == font.trueTypeFont) {
        return false;
    }

    for (uint32_t ii = 0, end = (uint32_t) wcslen(_string); ii < end; ++ii) {
        CodePoint codePoint = _string[ii];
        if (!preloadGlyph(_handle, codePoint)) {
            return false;
        }
    }

    return true;
}

bool FontManager::preloadGlyph(FontHandle _handle, CodePoint _codePoint) {
    BX_ASSERT(isValid(_handle), "Invalid handle used")
    CachedFont &font = m_cachedFonts[_handle.idx];
    FontInfo &fontInfo = font.fontInfo;

    GlyphHashMap::iterator iter = font.cachedGlyphs.find(_codePoint);
    if (iter != font.cachedGlyphs.end()) {
        return true;
    }

    if (font.trueTypeFont != nullptr) {
        GlyphInfo glyphInfo {};

        // todo mihael: add support for distance field font
        switch (font.fontInfo.fontType) {
            case FontType::Bitmap:
                font.trueTypeFont->bakeGlyphAlpha(_codePoint, glyphInfo, m_buffer);
                break;
                
            case FontType::SDF:
                font.trueTypeFont->BakeGlyphSDF(_codePoint, glyphInfo, m_buffer);
                break;

            default:
                BX_ASSERT(false, "TextureType not supported yet")
        }

        addBitmap(glyphInfo, m_buffer);

        glyphInfo.advance_x = (glyphInfo.advance_x * fontInfo.scale);
        glyphInfo.advance_y = (glyphInfo.advance_y * fontInfo.scale);
        glyphInfo.offset_x = (glyphInfo.offset_x * fontInfo.scale);
        glyphInfo.offset_y = (glyphInfo.offset_y * fontInfo.scale);
        glyphInfo.height = (glyphInfo.height * fontInfo.scale);
        glyphInfo.width = (glyphInfo.width * fontInfo.scale);

        font.cachedGlyphs[_codePoint] = glyphInfo;
        return true;
    }

    if (isValid(font.masterFontHandle)
        && preloadGlyph(font.masterFontHandle, _codePoint)) {
        const GlyphInfo *glyph = getGlyphInfo(font.masterFontHandle, _codePoint);

        GlyphInfo glyphInfo = *glyph;
        glyphInfo.advance_x = (glyphInfo.advance_x * fontInfo.scale);
        glyphInfo.advance_y = (glyphInfo.advance_y * fontInfo.scale);
        glyphInfo.offset_x = (glyphInfo.offset_x * fontInfo.scale);
        glyphInfo.offset_y = (glyphInfo.offset_y * fontInfo.scale);
        glyphInfo.height = (glyphInfo.height * fontInfo.scale);
        glyphInfo.width = (glyphInfo.width * fontInfo.scale);

        font.cachedGlyphs[_codePoint] = glyphInfo;
        return true;
    }

    return false;
}

void FontManager::addGlyphBitmap(FontHandle _handle, CodePoint _codePoint, uint16_t _width, uint16_t _height,
                                 uint16_t _pitch, float extraScale, const uint8_t *_bitmapBuffer, float glyphOffsetX, float glyphOffsetY) {
    BX_ASSERT(isValid(_handle), "Invalid handle used")
    CachedFont &font = m_cachedFonts[_handle.idx];

    GlyphHashMap::iterator iter = font.cachedGlyphs.find(_codePoint);
    if (iter != font.cachedGlyphs.end()) {
        return;
    }

    GlyphInfo glyphInfo {};

    float glyphScale = extraScale;
    glyphInfo.offset_x = glyphOffsetX * glyphScale;
    glyphInfo.offset_y = glyphOffsetY * glyphScale;
    glyphInfo.width = (float) _width;
    glyphInfo.height = (float) _height;
    glyphInfo.advance_x = (float) _width * glyphScale;
    glyphInfo.advance_y = (float) _height * glyphScale;
    glyphInfo.bitmapScale = glyphScale;

    uint32_t dstPitch = _width * 4;

    uint8_t *dst = m_buffer;
    const uint8_t *src = _bitmapBuffer;
    uint32_t srcPitch = _pitch;

    for (int32_t ii = 0; ii < _height; ++ii) {
        bx::memCopy(dst, src, dstPitch);

        dst += dstPitch;
        src += srcPitch;
    }

    glyphInfo.regionIndex = m_atlas->addRegion(
        (uint16_t) bx::ceil(glyphInfo.width), (uint16_t) bx::ceil(glyphInfo.height), m_buffer,
        AtlasRegion::TYPE_BGRA8
    );

    font.cachedGlyphs[_codePoint] = glyphInfo;
}

const FontInfo &FontManager::getFontInfo(FontHandle _handle) const {
    BX_ASSERT(isValid(_handle), "Invalid handle used")
    return m_cachedFonts[_handle.idx].fontInfo;
}

float FontManager::getKerning(FontHandle _handle, CodePoint _prevCodePoint, CodePoint _codePoint) {
    const CachedFont &cachedFont = m_cachedFonts[_handle.idx];
    if (isValid(cachedFont.masterFontHandle)) {
        CachedFont &baseFont = m_cachedFonts[cachedFont.masterFontHandle.idx];
        return baseFont.trueTypeFont->m_scale
               * stbtt_GetCodepointKernAdvance(&baseFont.trueTypeFont->m_font, _prevCodePoint, _codePoint)
               * cachedFont.fontInfo.scale;
    } else {
        return cachedFont.trueTypeFont->m_scale *
               stbtt_GetCodepointKernAdvance(&cachedFont.trueTypeFont->m_font, _prevCodePoint, _codePoint);
    }
}

const GlyphInfo *FontManager::getGlyphInfo(FontHandle _handle, CodePoint _codePoint) {
    const GlyphHashMap &cachedGlyphs = m_cachedFonts[_handle.idx].cachedGlyphs;
    GlyphHashMap::const_iterator it = cachedGlyphs.find(_codePoint);

    if (it == cachedGlyphs.end()) {
        if (!preloadGlyph(_handle, _codePoint)) {
            return nullptr;
        }

        it = cachedGlyphs.find(_codePoint);
    }

    BX_ASSERT(it != cachedGlyphs.end(), "Failed to preload glyph.")
    return &it->second;
}

bool FontManager::addBitmap(GlyphInfo &_glyphInfo, const uint8_t *_data) {
    _glyphInfo.regionIndex = m_atlas->addRegion((uint16_t) bx::ceil(_glyphInfo.width), (uint16_t) bx::ceil(_glyphInfo.height), _data, AtlasRegion::TYPE_GRAY);
    return true;
}
