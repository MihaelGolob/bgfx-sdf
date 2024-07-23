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
#include "FontManager.h"
#include "../font_processing/CubeAtlas.h"
#include "../utilities.h"

#define MAX_FONT_BUFFER_SIZE (512 * 512 * 4)

FontManager::FontManager(Atlas *atlas) : own_atlas_(false), atlas_(atlas) {
    Init();
}

FontManager::FontManager(uint16_t texture_side_width) : own_atlas_(true), atlas_(new Atlas(texture_side_width)) {
    Init();
}

void FontManager::Init() {
    cached_files_ = new CachedFile[MAX_OPENED_FILES];
    cached_fonts_ = new CachedFont[MAX_OPENED_FONT];
    cached_faces_ = new FT_Face[MAX_OPENED_FONT];
    cached_msdf_generators_ = new MsdfGenerator[MAX_OPENED_MSDF_GEN];
    buffer_ = new uint8_t[MAX_FONT_BUFFER_SIZE];
    
    if (FT_Init_FreeType(&ft_library_)) {
        BX_ASSERT(false, "Failed to initialize freetype library")
    }
}

FontManager::~FontManager() {
    BX_ASSERT(font_handles_.getNumHandles() == 0, "All the fonts must be destroyed before destroying the manager")
    delete[] cached_fonts_;

    BX_ASSERT(files_handles_.getNumHandles() == 0, "All the font files must be destroyed before destroying the manager")
    delete[] cached_files_;
    
    delete[] cached_faces_;
    delete[] buffer_;
    delete[] cached_msdf_generators_;

    if (own_atlas_) {
        delete atlas_;
    }
}

TrueTypeHandle FontManager::CreateTtf(const char* file_path) {
    // Load the file
    uint32_t size;
    void* data = Load(file_path, &size);
    
    uint16_t id = files_handles_.alloc();
    BX_ASSERT(id != bx::kInvalidHandle, "Invalid handle used")
    cached_files_[id].buffer = new uint8_t[size];
    cached_files_[id].buffer_size = size;
    cached_files_[id].path = file_path;
    bx::memCopy(cached_files_[id].buffer, data, size);

    TrueTypeHandle ret = {id};
    return ret;
}

void FontManager::DestroyTtf(TrueTypeHandle handle) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    delete[] cached_files_[handle.idx].buffer;
    cached_files_[handle.idx].buffer_size = 0;
    cached_files_[handle.idx].buffer = nullptr;
    cached_files_[handle.idx].path = nullptr;
    files_handles_.free(handle.idx);
}

FontHandle FontManager::CreateFontByPixelSize(TrueTypeHandle ttf_handle, uint32_t typeface_index, uint32_t pixel_size, FontType font_type, uint16_t glyph_padding) {
    BX_ASSERT(isValid(ttf_handle), "Invalid handle used")

    auto ttf = new TrueTypeFont();
    ttf->Init(cached_files_[ttf_handle.idx].buffer, cached_files_[ttf_handle.idx].buffer_size, typeface_index, pixel_size, glyph_padding);

    uint16_t font_idx = font_handles_.alloc();
    BX_ASSERT(font_idx != bx::kInvalidHandle, "Invalid handle used")

    CachedFont &font = cached_fonts_[font_idx];
    font.true_type_font = ttf;
    font.font_info = ttf->GetFontInfo();
    font.font_info.font_type = font_type;
    font.font_info.pixel_size = uint16_t(pixel_size);
    font.cached_glyphs.clear();
    font.master_font_handle.idx = bx::kInvalidHandle;
    if (FontTypeNeedsMsdfGeneration(font_type)) {
        font.face_handle = CreateFace(&cached_files_[ttf_handle.idx]);
        font.msdf_gen_handle = CreateMsdfGenerator(font.face_handle, pixel_size, glyph_padding);
    } else {
        font.face_handle.idx = bx::kInvalidHandle;
        font.msdf_gen_handle.idx = bx::kInvalidHandle;
    }

    FontHandle handle = {font_idx};
    return handle;
}

FontFaceHandle FontManager::CreateFace(CachedFile* font_file) {
    auto handle = FontFaceHandle{face_handles_.alloc()};
    BX_ASSERT(handle.idx != bx::kInvalidHandle, "Invalid handle used")
    
    if (FT_New_Face(ft_library_, font_file->path, 0, &cached_faces_[handle.idx])) {
        face_handles_.free(handle.idx);
        return FontFaceHandle{bx::kInvalidHandle};
    }
    
    return handle;
}

MsdfGenHandle FontManager::CreateMsdfGenerator(FontFaceHandle face_handle, uint32_t pixel_size, uint32_t padding) {
    auto handle = MsdfGenHandle{msdf_gen_handles_.alloc()};
    BX_ASSERT(handle.idx != bx::kInvalidHandle, "Invalid handle used")
    
    cached_msdf_generators_[handle.idx].Init(cached_faces_[face_handle.idx], pixel_size, padding);
    
    return handle;
}

FontHandle FontManager::CreateScaledFontToPixelSize(FontHandle base_font_handle, uint32_t pixel_size) {
    BX_ASSERT(isValid(base_font_handle), "Invalid handle used")
    CachedFont &base_font = cached_fonts_[base_font_handle.idx];
    FontInfo &font_info = base_font.font_info;

    FontInfo new_font_info = font_info;
    new_font_info.pixel_size = uint16_t(pixel_size);
    new_font_info.scale = (float) pixel_size / (float) font_info.pixel_size;
    new_font_info.ascender = (new_font_info.ascender * new_font_info.scale);
    new_font_info.descender = (new_font_info.descender * new_font_info.scale);
    new_font_info.line_gap = (new_font_info.line_gap * new_font_info.scale);
    new_font_info.max_advance_width = (new_font_info.max_advance_width * new_font_info.scale);
    new_font_info.underline_thickness = (new_font_info.underline_thickness * new_font_info.scale);
    new_font_info.underline_position = (new_font_info.underline_position * new_font_info.scale);

    uint16_t font_idx = font_handles_.alloc();
    BX_ASSERT(font_idx != bx::kInvalidHandle, "Invalid handle used")

    CachedFont &font = cached_fonts_[font_idx];
    font.cached_glyphs.clear();
    font.font_info = new_font_info;
    font.true_type_font = nullptr;
    font.master_font_handle = base_font_handle;

    FontHandle handle = {font_idx};
    return handle;
}

void FontManager::DestroyFont(FontHandle handle) {
    BX_ASSERT(isValid(handle), "Invalid handle used")

    CachedFont &font = cached_fonts_[handle.idx];

    if (font.true_type_font != nullptr) {
        delete font.true_type_font;
        font.true_type_font = nullptr;
    }

    font.cached_glyphs.clear();
    font_handles_.free(handle.idx);
    face_handles_.free(font.face_handle.idx);
    msdf_gen_handles_.free(font.msdf_gen_handle.idx);
}

bool FontManager::PreloadGlyph(FontHandle handle, const wchar_t *string) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    CachedFont &font = cached_fonts_[handle.idx];

    if (nullptr == font.true_type_font) {
        return false;
    }

    for (uint32_t ii = 0, end = (uint32_t) wcslen(string); ii < end; ++ii) {
        CodePoint code_point = string[ii];
        if (!PreloadGlyph(handle, code_point)) {
            return false;
        }
    }

    return true;
}

bool FontManager::PreloadGlyph(FontHandle handle, CodePoint code_point) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    CachedFont &font = cached_fonts_[handle.idx];
    FontInfo &font_info = font.font_info;

    GlyphHashMap::iterator iter = font.cached_glyphs.find(code_point);
    if (iter != font.cached_glyphs.end()) {
        return true;
    }

    if (font.true_type_font != nullptr) {
        GlyphInfo glyph_info {};
        AtlasRegion::Type bitmap_type = AtlasRegion::TypeGray;

        switch (font.font_info.font_type) {
            case FontType::Bitmap:
                font.true_type_font->BakeGlyphAlpha(code_point, glyph_info, buffer_);
                break;
            case FontType::SdfFromBitmap:
                font.true_type_font->BakeGlyphSdf(code_point, glyph_info, buffer_);
                break;
            case FontType::SdfFromVector:
                cached_msdf_generators_[font.msdf_gen_handle.idx].BakeGlyphSdf(code_point, glyph_info, buffer_);
                bitmap_type = AtlasRegion::TypeBgra8; // todo check how to do this with single channel texture
                break;
            case FontType::Msdf:
                cached_msdf_generators_[font.msdf_gen_handle.idx].BakeGlyphMsdf(code_point, glyph_info, buffer_);
                bitmap_type = AtlasRegion::TypeBgra8;
                break;
            default:
                BX_ASSERT(false, "TextureType not supported yet")
        }

        AddBitmap(glyph_info, buffer_, bitmap_type);

        glyph_info.advance_x = (glyph_info.advance_x * font_info.scale);
        glyph_info.advance_y = (glyph_info.advance_y * font_info.scale);
        glyph_info.offset_x = (glyph_info.offset_x * font_info.scale);
        glyph_info.offset_y = (glyph_info.offset_y * font_info.scale);
        glyph_info.height = (glyph_info.height * font_info.scale);
        glyph_info.width = (glyph_info.width * font_info.scale);

        font.cached_glyphs[code_point] = glyph_info;
        return true;
    }

    if (isValid(font.master_font_handle)
        && PreloadGlyph(font.master_font_handle, code_point)) {
        const GlyphInfo *glyph = GetGlyphInfo(font.master_font_handle, code_point);

        GlyphInfo glyph_info = *glyph;
        glyph_info.advance_x = (glyph_info.advance_x * font_info.scale);
        glyph_info.advance_y = (glyph_info.advance_y * font_info.scale);
        glyph_info.offset_x = (glyph_info.offset_x * font_info.scale);
        glyph_info.offset_y = (glyph_info.offset_y * font_info.scale);
        glyph_info.height = (glyph_info.height * font_info.scale);
        glyph_info.width = (glyph_info.width * font_info.scale);

        font.cached_glyphs[code_point] = glyph_info;
        return true;
    }

    return false;
}

const FontInfo &FontManager::GetFontInfo(FontHandle handle) const {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    return cached_fonts_[handle.idx].font_info;
}

float FontManager::GetKerning(FontHandle handle, CodePoint prev_code_point, CodePoint code_point) {
    const CachedFont &cached_font = cached_fonts_[handle.idx];
    if (isValid(cached_font.master_font_handle)) {
        CachedFont &base_font = cached_fonts_[cached_font.master_font_handle.idx];
        return base_font.true_type_font->scale_ * stbtt_GetCodepointKernAdvance(&base_font.true_type_font->font_, prev_code_point, code_point) * cached_font.font_info.scale;
    } else {
        return cached_font.true_type_font->scale_ * stbtt_GetCodepointKernAdvance(&cached_font.true_type_font->font_, prev_code_point, code_point);
    }
}

const GlyphInfo *FontManager::GetGlyphInfo(FontHandle handle, CodePoint code_point) {
    const GlyphHashMap &cached_glyphs = cached_fonts_[handle.idx].cached_glyphs;
    GlyphHashMap::const_iterator it = cached_glyphs.find(code_point);

    if (it == cached_glyphs.end()) {
        if (!PreloadGlyph(handle, code_point)) {
            return nullptr;
        }

        it = cached_glyphs.find(code_point);
    }

    BX_ASSERT(it != cached_glyphs.end(), "Failed to preload glyph.")
    return &it->second;
}

bool FontManager::AddBitmap(GlyphInfo &glyph_info, const uint8_t *data, const AtlasRegion::Type bitmap_type) {
    glyph_info.region_index = atlas_->AddRegion((uint16_t) bx::ceil(glyph_info.width), (uint16_t) bx::ceil(glyph_info.height), data, bitmap_type);
    return true;
}

bool FontManager::FontTypeNeedsMsdfGeneration(FontType font_type) {
    return font_type == FontType::Msdf || font_type == FontType::SdfFromVector;
}
