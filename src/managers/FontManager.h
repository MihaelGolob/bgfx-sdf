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
#include <freetype/freetype.h>
#include <vector>
#include <tinystl/allocator.h>
#include <tinystl/unordered_map.h>

#include "../font_processing/GlyphInfo.h"
#include "../font_processing/FontInfo.h"
#include "../font_processing/FontHandles.h"
#include "../font_processing/TrueTypeFont.h"
#include "../sdf/MsdfGenerator.h"
#include "../font_processing/AtlasRegion.h"

class Atlas;

#define MAX_OPENED_FILES 64
#define MAX_OPENED_FONT  64
#define MAX_OPENED_FACES 64
#define FONT_TYPE_ALPHA UINT32_C(0x00000100)

namespace stl = tinystl;

/// Unicode value of a character
typedef int32_t CodePoint;
typedef stl::unordered_map<CodePoint, GlyphInfo> GlyphHashMap;

class FontManager {
private:
    struct CachedFont {
        CachedFont() : true_type_font(nullptr) {
            master_font_handle.idx = bx::kInvalidHandle;
        }

        FontInfo font_info{};
        GlyphHashMap cached_glyphs;
        TrueTypeFont *true_type_font;
        // a handle to a master font in case of sub distance field font
        FontHandle master_font_handle{};
        int16_t padding{};
        // used for msdf rendering
        FontFaceHandle face_handle{};
    };
    struct CachedFile {
        uint8_t *buffer;
        uint32_t buffer_size;
        const char* path;
    }; 
     
public:
    /// Create the font manager using an external cube atlas (doesn't take ownership of the atlas).
    explicit FontManager(Atlas *atlas);

    /// Create the font manager and create the texture cube as BGRA8 with linear filtering.
    explicit FontManager(uint16_t texture_side_width = 512);

    ~FontManager();

    /// Retrieve the atlas used by the font manager (e.g. to add stuff to it)
    [[nodiscard]] const Atlas *GetAtlas() const {
        return atlas_;
    }

    /// Load a TrueType font from a given buffer. The buffer is copied and
    /// thus can be freed or reused after this call.
    ///
    /// @return invalid handle if the loading fail
    TrueTypeHandle CreateTtf(const char* font_path);
    
    /// Unload a TrueType font (free font memory) but keep loaded glyphs.
    void DestroyTtf(TrueTypeHandle handle);

    /// Return a font whose height is a fixed pixel size.
    FontHandle CreateFontByPixelSize(TrueTypeHandle ttf_handle, uint32_t typeface_index, uint32_t pixel_size,
                                     FontType font_type = FontType::Bitmap, uint16_t glyph_padding = 6);

    FontFaceHandle CreateFace(CachedFile* font_file);

    /// Return a scaled child font whose height is a fixed pixel size.
    FontHandle CreateScaledFontToPixelSize(FontHandle base_font_handle, uint32_t pixel_size);

    /// destroy a font (truetype or baked)
    void DestroyFont(FontHandle handle);

    /// Preload a set of glyphs from a TrueType file.
    ///
    /// @return True if every glyph could be preloaded, false otherwise if
    ///   the Font is a baked font, this only do validation on the characters.
    bool PreloadGlyph(FontHandle handle, const wchar_t *string);

    /// Preload a single glyph, return true on success.
    bool PreloadGlyph(FontHandle handle, CodePoint code_point);

    /// Return the font descriptor of a font.
    ///
    /// @remark the handle is required to be valid
    [[nodiscard]] const FontInfo &GetFontInfo(FontHandle handle) const;

    /// Return the rendering information about the glyph region. Load the
    /// glyph from a TrueType font if possible
    ///
    const GlyphInfo *GetGlyphInfo(FontHandle handle, CodePoint code_point);

    float GetKerning(FontHandle handle, CodePoint prev_code_point, CodePoint code_point);

    [[nodiscard]] const GlyphInfo &GetBlackGlyph() const {
        return black_glyph_;
    }

private:
    void Init();
    bool AddBitmap(GlyphInfo &glyph_info, const uint8_t *data, AtlasRegion::Type bitmap_type);
    
    FT_Library ft_library_{};
    MsdfGenerator msdf_generator_;

    bool own_atlas_;
    Atlas *atlas_;

    bx::HandleAllocT<MAX_OPENED_FONT> font_handles_;
    CachedFont *cached_fonts_{};

    bx::HandleAllocT<MAX_OPENED_FILES> files_handles_;
    CachedFile *cached_files_{};
    
    bx::HandleAllocT<MAX_OPENED_FACES> face_handles_;
    FT_Face *cached_faces_{};

    GlyphInfo black_glyph_{};

    //temporary buffer to raster glyph
    uint8_t *buffer_{};
};
