//
// Created by MihaelGolob on 16. 08. 2024.
//

#include <freetype/freetype.h>
#include "MsdfOriginalGenerator.h"
#include "../utilities.h"

MsdfOriginalGenerator::MsdfOriginalGenerator() {
    ft_ = msdfgen::initializeFreetype();
}

void MsdfOriginalGenerator::Init(const std::string &font_path, FT_Face face, int font_size, int padding) {
    font_size_ = font_size;
    face_ = face;
    padding_ = padding;
    texture_width = font_size + 2 * padding;
    texture_height = font_size + 2 * padding;
    
    font_ = msdfgen::loadFont(ft_, font_path.c_str());
    if (!font_) PrintError("Failed to load font");

    // calculate scale
    long max_width = 0;
    long max_height = 0;

    for (int i = 'A'; i <= 'z'; i++) {
        auto glyph_index = FT_Get_Char_Index(face_, i);
        FT_Load_Glyph(face_, glyph_index, FT_LOAD_NO_SCALE);

        max_width = std::max(max_width, face_->glyph->metrics.width);
        max_height = std::max(max_height, face_->glyph->metrics.height);
    }

    scale_ = (double) std::max(max_width, max_height) / font_size_;
}

void MsdfOriginalGenerator::BakeGlyphMsdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output) {
    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, font_, code_point, msdfgen::FONT_SCALING_EM_NORMALIZED)) {
        PrintError("Failed to load glyph");
        return;
    }

    CalculateGlyphInfo(glyph_info, code_point);
   
    shape.normalize();
    msdfgen::edgeColoringSimple(shape, 3.0);
    msdfgen::Bitmap<float, 3> msdf(texture_width, texture_height);
    msdfgen::SDFTransformation t(msdfgen::Projection(font_size_, msdfgen::Vector2(0.1, 0.2)), msdfgen::Range(1.8));
    msdfgen::generateMSDF(msdf, shape, t);

    for (int y = 0; y < texture_height; y++) {
        for (int x = 0; x < texture_width; x++) {
            const auto pixel = msdf(x, msdf.width() - y);
            const auto index = (y * msdf.width() + x) * 4;
            
            output[index + 0] = static_cast<uint8_t>(pixel[0] * 255);
            output[index + 1] = static_cast<uint8_t>(pixel[1] * 255);
            output[index + 2] = static_cast<uint8_t>(pixel[2] * 255);
            output[index + 3] = 255;
        }
    }
}

void MsdfOriginalGenerator::CalculateGlyphInfo(GlyphInfo &out_glyph_info, CodePoint code_point) {
    FT_Load_Glyph(face_, FT_Get_Char_Index(face_, code_point), FT_LOAD_NO_SCALE);
    
    const auto glyph = face_->glyph;
    const auto metrics = face_->glyph->metrics;
    
    out_glyph_info.width = texture_width;
    out_glyph_info.height = texture_height;
    out_glyph_info.advance_x = std::floor(glyph->advance.x * (1.0 / scale_));
    out_glyph_info.advance_y = std::floor(glyph->advance.y * (1.0 / scale_));
    out_glyph_info.offset_x = metrics.horiBearingX * (1.0 / scale_) - padding_;
    out_glyph_info.offset_y = -(metrics.height - metrics.horiBearingY) * (1.0 / scale_) - padding_;
}

