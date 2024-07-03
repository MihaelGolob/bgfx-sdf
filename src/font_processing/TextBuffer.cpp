//
// Created by MihaelGolob on 30. 04. 2024.
//

#include "TextBuffer.h"
#include "CubeAtlas.h"
#include "utf8.h"
#include "FontInfo.h"
#include "../managers/FontManager.h"

#include <bgfx/embedded_shader.h>

#define MAX_BUFFERED_CHARACTERS (8192 - 5)

TextBuffer::TextBuffer(FontManager *font_manager)
        : style_flags_(StyleNormal), text_color_(UINT32_MAX), background_color_(UINT32_MAX),
          overline_color_(UINT32_MAX), underline_color_(UINT32_MAX), strike_through_color_(UINT32_MAX),
          outline_width_(3.0f), outline_color_(0x000000ff), drop_shadow_color_(0x0000005a), drop_shadow_softener_(1.0f),
          pen_x_(0), pen_y_(0), origin_x_(0), origin_y_(0), line_ascender_(0), line_descender_(0), line_gap_(0),
          previous_code_point_(0), font_manager_(font_manager),
          vertex_buffer_(new TextVertex[MAX_BUFFERED_CHARACTERS * 4]),
          index_buffer_(new uint16_t[MAX_BUFFERED_CHARACTERS * 6]),
          style_buffer_(new uint8_t[MAX_BUFFERED_CHARACTERS * 4]), index_count_(0), line_start_index_(0),
          vertex_count_(0) {
    rectangle_.width = 0;
    rectangle_.height = 0;
    drop_shadow_offset_[0] = 0.00f;
    drop_shadow_offset_[1] = 0.00f;
}

TextBuffer::~TextBuffer() {
    delete[] vertex_buffer_;
    delete[] index_buffer_;
    delete[] style_buffer_;
}

void TextBuffer::AppendText(FontHandle font_handle, const char *string, const char *end) {
    if (vertex_count_ == 0) {
        origin_x_ = pen_x_;
        origin_y_ = pen_y_;
        line_descender_ = 0;
        line_ascender_ = 0;
        line_gap_ = 0;
        previous_code_point_ = 0;
    }

    CodePoint codepoint = 0;
    uint32_t state = 0;

    if (end == nullptr) {
        end = string + bx::strLen(string);
    }
    BX_ASSERT(end >= string, "")

    for (; *string && string < end; ++string) {
        if (Utf8Decode(&state, (uint32_t *) &codepoint, *string) == UTF8_ACCEPT) {
            AppendGlyph(font_handle, codepoint, false);
        }
    }

    BX_ASSERT(state == UTF8_ACCEPT, "The string is not well-formed")
}

void TextBuffer::AppendText(FontHandle font_handle, const wchar_t *string, const wchar_t *end) {
    if (vertex_count_ == 0) {
        origin_x_ = pen_x_;
        origin_y_ = pen_y_;
        line_descender_ = 0;
        line_ascender_ = 0;
        line_gap_ = 0;
        previous_code_point_ = 0;
    }

    if (end == nullptr) {
        end = string + wcslen(string);
    }
    BX_ASSERT(end >= string, "")

    for (const wchar_t *current = string; current < end; ++current) {
        uint32_t code_point = *current;
        AppendGlyph(font_handle, code_point, false);
    }
}

void TextBuffer::AppendAtlasFace(uint16_t face_index) {
    if (vertex_count_ / 4 >= MAX_BUFFERED_CHARACTERS) {
        return;
    }

    float x0 = pen_x_;
    float y0 = pen_y_;
    float x1 = x0 + (float) font_manager_->GetAtlas()->GetTextureSize();
    float y1 = y0 + (float) font_manager_->GetAtlas()->GetTextureSize();

    font_manager_->GetAtlas()->PackFaceLayerUv(face_index, (uint8_t *) vertex_buffer_,
                                               sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u),
                                               sizeof(TextVertex)
    );

    SetVertex(vertex_count_ + 0, x0, y0, background_color_);
    SetVertex(vertex_count_ + 1, x0, y1, background_color_);
    SetVertex(vertex_count_ + 2, x1, y1, background_color_);
    SetVertex(vertex_count_ + 3, x1, y0, background_color_);

    index_buffer_[index_count_ + 0] = vertex_count_ + 0;
    index_buffer_[index_count_ + 1] = vertex_count_ + 1;
    index_buffer_[index_count_ + 2] = vertex_count_ + 2;
    index_buffer_[index_count_ + 3] = vertex_count_ + 0;
    index_buffer_[index_count_ + 4] = vertex_count_ + 2;
    index_buffer_[index_count_ + 5] = vertex_count_ + 3;
    vertex_count_ += 4;
    index_count_ += 6;
}

void TextBuffer::ClearTextBuffer() {
    pen_x_ = 0;
    pen_y_ = 0;
    origin_x_ = 0;
    origin_y_ = 0;

    vertex_count_ = 0;
    index_count_ = 0;
    line_start_index_ = 0;
    line_ascender_ = 0;
    line_descender_ = 0;
    line_gap_ = 0;
    previous_code_point_ = 0;
    rectangle_.width = 0;
    rectangle_.height = 0;
}

void TextBuffer::AppendGlyph(FontHandle handle, CodePoint code_point, bool shadow) {
    if (code_point == L'\t') {
        for (uint32_t ii = 0; ii < 4; ++ii) {
            AppendGlyph(handle, L' ', shadow);
        }
        return;
    }

    const GlyphInfo *glyph = font_manager_->GetGlyphInfo(handle, code_point);
    BX_WARN(nullptr != glyph, "Glyph not found (font handle %d, code point %d)", handle.idx, code_point)
    if (nullptr == glyph) {
        previous_code_point_ = 0;
        return;
    }

    if (vertex_count_ / 4 >= MAX_BUFFERED_CHARACTERS) {
        previous_code_point_ = 0;
        return;
    }

    const FontInfo &font = font_manager_->GetFontInfo(handle);

    if (code_point == L'\n') {
        pen_x_ = origin_x_;
        pen_y_ += line_gap_ + line_ascender_ - line_descender_;
        line_gap_ = font.line_gap;
        line_descender_ = font.descender;
        line_ascender_ = font.ascender;
        line_start_index_ = vertex_count_;
        previous_code_point_ = 0;
        return;
    }

    //is there a change of font size that require the text on the left to be centered again ?
    if (font.ascender > line_ascender_ || (font.descender < line_descender_)) {
        if (font.descender < line_descender_) {
            line_descender_ = font.descender;
            line_gap_ = font.line_gap;
        }

        float txt_decals = (font.ascender - line_ascender_);
        line_ascender_ = font.ascender;
        line_gap_ = font.line_gap;
        VerticalCenterLastLine((txt_decals), (pen_y_ - line_ascender_), (pen_y_ + line_ascender_ - line_descender_ + line_gap_));
    }

    float kerning = font_manager_->GetKerning(handle, previous_code_point_, code_point);
    pen_x_ += kerning;

    const GlyphInfo &black_glyph = font_manager_->GetBlackGlyph();
    const Atlas *atlas = font_manager_->GetAtlas();
    const AtlasRegion &atlas_region = atlas->GetRegion(glyph->region_index);

    if (shadow) {
        if (atlas_region.GetType() != AtlasRegion::TypeBgra8) {
            float extra_x_offset = drop_shadow_offset_[0];
            float extra_y_offset = drop_shadow_offset_[1];

            float x0 = pen_x_ + (glyph->offset_x) + extra_x_offset;
            float y0 = (pen_y_ + line_ascender_ + (glyph->offset_y) + extra_y_offset);
            float x1 = (x0 + glyph->width);
            float y1 = (y0 + glyph->height);

            bx::memSet(&vertex_buffer_[vertex_count_], 0, sizeof(TextVertex) * 4);

            atlas->PackUv(glyph->region_index, (uint8_t *) vertex_buffer_, sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u2), sizeof(TextVertex));

            uint32_t adjusted_drop_shadow_color = ((((drop_shadow_color_ & 0xff000000) >> 8) * (text_color_ >> 24)) & 0xff000000) | (drop_shadow_color_ & 0x00ffffff);
            SetVertex(vertex_count_ + 0, x0, y0, adjusted_drop_shadow_color);
            SetVertex(vertex_count_ + 1, x0, y1, adjusted_drop_shadow_color);
            SetVertex(vertex_count_ + 2, x1, y1, adjusted_drop_shadow_color);
            SetVertex(vertex_count_ + 3, x1, y0, adjusted_drop_shadow_color);

            index_buffer_[index_count_ + 0] = vertex_count_ + 0;
            index_buffer_[index_count_ + 1] = vertex_count_ + 1;
            index_buffer_[index_count_ + 2] = vertex_count_ + 2;
            index_buffer_[index_count_ + 3] = vertex_count_ + 0;
            index_buffer_[index_count_ + 4] = vertex_count_ + 2;
            index_buffer_[index_count_ + 5] = vertex_count_ + 3;
            vertex_count_ += 4;
            index_count_ += 6;
        }

        pen_x_ += glyph->advance_x;
        if (pen_x_ > rectangle_.width) {
            rectangle_.width = pen_x_;
        }

        if ((pen_y_ + line_ascender_ - line_descender_ + line_gap_) > rectangle_.height) {
            rectangle_.height = (pen_y_ + line_ascender_ - line_descender_ + line_gap_);
        }

        previous_code_point_ = code_point;

        return;
    }

    if (style_flags_ & StyleBackground
        && background_color_ & 0xff000000) {
        float x0 = (pen_x_ - kerning);
        float y0 = (pen_y_);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = (pen_y_ + line_ascender_ - line_descender_ + line_gap_);

        atlas->PackUv(black_glyph.region_index, (uint8_t *) vertex_buffer_, sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u), sizeof(TextVertex));

        const uint16_t vertex_count = vertex_count_;
        SetVertex(vertex_count + 0, x0, y0, background_color_, StyleBackground);
        SetVertex(vertex_count + 1, x0, y1, background_color_, StyleBackground);
        SetVertex(vertex_count + 2, x1, y1, background_color_, StyleBackground);
        SetVertex(vertex_count + 3, x1, y0, background_color_, StyleBackground);

        index_buffer_[index_count_ + 0] = vertex_count + 0;
        index_buffer_[index_count_ + 1] = vertex_count + 1;
        index_buffer_[index_count_ + 2] = vertex_count + 2;
        index_buffer_[index_count_ + 3] = vertex_count + 0;
        index_buffer_[index_count_ + 4] = vertex_count + 2;
        index_buffer_[index_count_ + 5] = vertex_count + 3;
        vertex_count_ += 4;
        index_count_ += 6;
    }

    if (style_flags_ & StyleUnderline && underline_color_ & 0xFF000000) {
        float x0 = (pen_x_ - kerning);
        float y0 = (pen_y_ + line_ascender_ - line_descender_ * 0.5f);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = y0 + font.underline_thickness;

        atlas->PackUv(black_glyph.region_index, (uint8_t *) vertex_buffer_, sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u), sizeof(TextVertex));

        SetVertex(vertex_count_ + 0, x0, y0, underline_color_, StyleUnderline);
        SetVertex(vertex_count_ + 1, x0, y1, underline_color_, StyleUnderline);
        SetVertex(vertex_count_ + 2, x1, y1, underline_color_, StyleUnderline);
        SetVertex(vertex_count_ + 3, x1, y0, underline_color_, StyleUnderline);

        index_buffer_[index_count_ + 0] = vertex_count_ + 0;
        index_buffer_[index_count_ + 1] = vertex_count_ + 1;
        index_buffer_[index_count_ + 2] = vertex_count_ + 2;
        index_buffer_[index_count_ + 3] = vertex_count_ + 0;
        index_buffer_[index_count_ + 4] = vertex_count_ + 2;
        index_buffer_[index_count_ + 5] = vertex_count_ + 3;
        vertex_count_ += 4;
        index_count_ += 6;
    }

    if (style_flags_ & StyleOverline && overline_color_ & 0xFF000000) {
        float x0 = (pen_x_ - kerning);
        float y0 = (pen_y_);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = y0 + font.underline_thickness;

        font_manager_->GetAtlas()->PackUv(black_glyph.region_index, (uint8_t *) vertex_buffer_,
                                          sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u),
                                          sizeof(TextVertex)
        );

        SetVertex(vertex_count_ + 0, x0, y0, overline_color_, StyleOverline);
        SetVertex(vertex_count_ + 1, x0, y1, overline_color_, StyleOverline);
        SetVertex(vertex_count_ + 2, x1, y1, overline_color_, StyleOverline);
        SetVertex(vertex_count_ + 3, x1, y0, overline_color_, StyleOverline);

        index_buffer_[index_count_ + 0] = vertex_count_ + 0;
        index_buffer_[index_count_ + 1] = vertex_count_ + 1;
        index_buffer_[index_count_ + 2] = vertex_count_ + 2;
        index_buffer_[index_count_ + 3] = vertex_count_ + 0;
        index_buffer_[index_count_ + 4] = vertex_count_ + 2;
        index_buffer_[index_count_ + 5] = vertex_count_ + 3;
        vertex_count_ += 4;
        index_count_ += 6;
    }

    if (style_flags_ & StyleStrikeThrough && strike_through_color_ & 0xFF000000) {
        float x0 = (pen_x_ - kerning);
        float y0 = (pen_y_ + 0.666667f * font.ascender);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = y0 + font.underline_thickness;

        atlas->PackUv(black_glyph.region_index, (uint8_t *) vertex_buffer_, sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u), sizeof(TextVertex));
        SetVertex(vertex_count_ + 0, x0, y0, strike_through_color_, StyleStrikeThrough);
        SetVertex(vertex_count_ + 1, x0, y1, strike_through_color_, StyleStrikeThrough);
        SetVertex(vertex_count_ + 2, x1, y1, strike_through_color_, StyleStrikeThrough);
        SetVertex(vertex_count_ + 3, x1, y0, strike_through_color_, StyleStrikeThrough);

        index_buffer_[index_count_ + 0] = vertex_count_ + 0;
        index_buffer_[index_count_ + 1] = vertex_count_ + 1;
        index_buffer_[index_count_ + 2] = vertex_count_ + 2;
        index_buffer_[index_count_ + 3] = vertex_count_ + 0;
        index_buffer_[index_count_ + 4] = vertex_count_ + 2;
        index_buffer_[index_count_ + 5] = vertex_count_ + 3;
        vertex_count_ += 4;
        index_count_ += 6;
    }

    if (!shadow && atlas_region.GetType() == AtlasRegion::TypeBgra8) { bx::memSet(&vertex_buffer_[vertex_count_], 0, sizeof(TextVertex) * 4);

        atlas->PackUv(glyph->region_index, (uint8_t *) vertex_buffer_, sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u1), sizeof(TextVertex));

        float glyph_scale = glyph->bitmap_scale;
        float glyph_width = glyph->width * glyph_scale;
        float glyph_height = glyph->height * glyph_scale;
        float x0 = pen_x_ + (glyph->offset_x);
        float y0 = (pen_y_ + (font.ascender + -font.descender - glyph_height) / 2);
        float x1 = (x0 + glyph_width);
        float y1 = (y0 + glyph_height);

        SetVertex(vertex_count_ + 0, x0, y0, text_color_);
        SetVertex(vertex_count_ + 1, x0, y1, text_color_);
        SetVertex(vertex_count_ + 2, x1, y1, text_color_);
        SetVertex(vertex_count_ + 3, x1, y0, text_color_);
    } else if (!shadow) {
        bx::memSet(&vertex_buffer_[vertex_count_], 0, sizeof(TextVertex) * 4);

        atlas->PackUv(glyph->region_index, (uint8_t *) vertex_buffer_, sizeof(TextVertex) * vertex_count_ + offsetof(TextVertex, u), sizeof(TextVertex));
        float x0 = pen_x_ + (glyph->offset_x);
        float y0 = (pen_y_ + line_ascender_ + (glyph->offset_y));
        float x1 = (x0 + glyph->width);
        float y1 = (y0 + glyph->height);

        SetVertex(vertex_count_ + 0, x0, y0, text_color_);
        SetVertex(vertex_count_ + 1, x0, y1, text_color_);
        SetVertex(vertex_count_ + 2, x1, y1, text_color_);
        SetVertex(vertex_count_ + 3, x1, y0, text_color_);

        SetOutlineColor(vertex_count_ + 0, outline_color_);
        SetOutlineColor(vertex_count_ + 1, outline_color_);
        SetOutlineColor(vertex_count_ + 2, outline_color_);
        SetOutlineColor(vertex_count_ + 3, outline_color_);
    }

    index_buffer_[index_count_ + 0] = vertex_count_ + 0;
    index_buffer_[index_count_ + 1] = vertex_count_ + 1;
    index_buffer_[index_count_ + 2] = vertex_count_ + 2;
    index_buffer_[index_count_ + 3] = vertex_count_ + 0;
    index_buffer_[index_count_ + 4] = vertex_count_ + 2;
    index_buffer_[index_count_ + 5] = vertex_count_ + 3;
    vertex_count_ += 4;
    index_count_ += 6;

    pen_x_ += glyph->advance_x;
    if (pen_x_ > rectangle_.width) {
        rectangle_.width = pen_x_;
    }

    if ((pen_y_ + line_ascender_ - line_descender_ + line_gap_) > rectangle_.height) {
        rectangle_.height = (pen_y_ + line_ascender_ - line_descender_ + line_gap_);
    }

    previous_code_point_ = code_point;
}

void TextBuffer::VerticalCenterLastLine(float txt_decal_y, float top, float bottom) {
    for (uint32_t ii = line_start_index_; ii < vertex_count_; ii += 4) {
        if (style_buffer_[ii] == StyleBackground) {
            vertex_buffer_[ii + 0].y = top;
            vertex_buffer_[ii + 1].y = bottom;
            vertex_buffer_[ii + 2].y = bottom;
            vertex_buffer_[ii + 3].y = top;
        } else {
            vertex_buffer_[ii + 0].y += txt_decal_y;
            vertex_buffer_[ii + 1].y += txt_decal_y;
            vertex_buffer_[ii + 2].y += txt_decal_y;
            vertex_buffer_[ii + 3].y += txt_decal_y;
        }
    }
}