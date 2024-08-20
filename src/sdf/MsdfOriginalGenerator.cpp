//
// Created by MihaelGolob on 16. 08. 2024.
//

#include "MsdfOriginalGenerator.h"
#include "../utilities.h"

MsdfOriginalGenerator::MsdfOriginalGenerator() {
    ft_ = msdfgen::initializeFreetype();
}

void MsdfOriginalGenerator::Init(const std::string &font_path, int font_size) {
    font_size_ = font_size;
    font_ = msdfgen::loadFont(ft_, font_path.c_str());
    if (!font_) PrintError("Failed to load font");
}

void MsdfOriginalGenerator::BakeGlyphMsdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output) {
    msdfgen::Shape shape;
    if (!msdfgen::loadGlyph(shape, font_, code_point, msdfgen::FONT_SCALING_EM_NORMALIZED)) {
        PrintError("Failed to load glyph");
        return;
    }
    
    msdfgen::FontMetrics metrics{};
    msdfgen::getFontMetrics(metrics, font_);
    
    const auto bounds = shape.getBounds();

    // TODO: temp
    glyph_info.width = font_size_;
    glyph_info.height = font_size_;
    glyph_info.advance_x = 20;
    glyph_info.advance_y = 0;
    glyph_info.offset_x = bounds.l;
    glyph_info.offset_y = -bounds.b;

    shape.normalize();
    msdfgen::edgeColoringSimple(shape, 3.0);
    msdfgen::Bitmap<float, 3> msdf(font_size_, font_size_);
    msdfgen::SDFTransformation t(msdfgen::Projection(font_size_, msdfgen::Vector2(0.0, 0.0)), msdfgen::Range(1.0));
    msdfgen::generateMSDF(msdf, shape, t);

    for (int y = 0; y < msdf.height(); y++) {
        for (int x = 0; x < msdf.width(); x++) {
            const auto pixel = msdf(x, msdf.width() - y);
            const auto index = (y * msdf.width() + x) * 4;
            
            output[index + 0] = static_cast<uint8_t>(pixel[2] * 255);
            output[index + 1] = static_cast<uint8_t>(pixel[1] * 255);
            output[index + 2] = static_cast<uint8_t>(pixel[0] * 255);
            output[index + 3] = 255;
        }
    }
}

