/*
* Copyright 2013 Jeremie Roy. All rights reserved.
* License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
*/

#include <bgfx/bgfx.h>
#include <bx/bx.h>

#include <vector>

#include "CubeAtlas.h"
#include "RectanglePacker.h"

struct Atlas::PackedLayer {
    RectanglePacker packer;
    AtlasRegion face_region{};
};

Atlas::Atlas(uint16_t texture_size, uint16_t max_regions_count) : used_layers_(0), used_faces_(0), texture_size_(texture_size), region_count_(0), max_region_count_(max_regions_count) {
    BX_ASSERT(texture_size >= 64 && texture_size <= 4096, "Invalid _textureSize %d.", texture_size)
    BX_ASSERT(max_regions_count >= 64 && max_regions_count <= 32000, "Invalid _maxRegionsCount %d.", max_regions_count)

    texel_size_ = float(UINT16_MAX) / float(texture_size_);

    layers_ = new PackedLayer[6]; // number of layers is arbitrary
    for (int ii = 0; ii < 6; ++ii) {
        layers_[ii].packer.Init(texture_size, texture_size);
    }

    regions_ = new AtlasRegion[max_regions_count];
    texture_buffer_ = new uint8_t[texture_size * texture_size * 6 * 4]; // 6 layers * 4 channels (RGBA)
    bx::memSet(texture_buffer_, 0, texture_size * texture_size * 6 * 4);

    texture_handle_ = bgfx::createTextureCube(texture_size, false, 1, bgfx::TextureFormat::BGRA8);
}

Atlas::Atlas(uint16_t texture_size, const uint8_t *texture_buffer, uint16_t region_count, const uint8_t *region_buffer, uint16_t max_regions_count)
        : used_layers_(6), used_faces_(6), texture_size_(texture_size), region_count_(region_count), max_region_count_(region_count < max_regions_count ? region_count : max_regions_count) {
    BX_ASSERT(region_count <= 64 && max_regions_count <= 4096, "_regionCount %d, _maxRegionsCount %d", region_count, max_regions_count)

    texel_size_ = float(UINT16_MAX) / float(texture_size_);

    regions_ = new AtlasRegion[region_count];
    texture_buffer_ = new uint8_t[GetTextureBufferSize()];

    bx::memCopy(regions_, region_buffer, region_count * sizeof(AtlasRegion));
    bx::memCopy(texture_buffer_, texture_buffer, GetTextureBufferSize());

    texture_handle_ = bgfx::createTextureCube(texture_size, false, 1, bgfx::TextureFormat::BGRA8, BGFX_SAMPLER_NONE,
                                              bgfx::makeRef(texture_buffer_, GetTextureBufferSize())
    );
}

Atlas::~Atlas() {
    bgfx::destroy(texture_handle_);

    delete[] layers_;
    delete[] regions_;
    delete[] texture_buffer_;
}

uint16_t Atlas::AddRegion(uint16_t width, uint16_t height, const uint8_t *bitmap_buffer, AtlasRegion::Type type, uint16_t outline) {
    if (region_count_ >= max_region_count_) {
        return UINT16_MAX;
    }

    uint16_t xx = 0;
    uint16_t yy = 0;
    uint32_t idx = 0;
    while (idx < used_layers_) {
        if (layers_[idx].face_region.GetType() == type
            && layers_[idx].packer.AddRectangle(width + 1, height + 1, xx, yy)) {
            break;
        }

        idx++;
    }

    if (idx >= used_layers_) {
        if ((idx + type) > 24
            || used_faces_ >= 6) {
            return UINT16_MAX;
        }

        AtlasRegion &region = layers_[idx].face_region;
        region.x = 0;
        region.y = 0;
        region.width = texture_size_;
        region.height = texture_size_;
        region.SetMask(type, used_faces_, 0);

        used_layers_++;
        used_faces_++;

        if (!layers_[idx].packer.AddRectangle(width + 1, height + 1, xx, yy)) {
            return UINT16_MAX;
        }
    }

    AtlasRegion &region = regions_[region_count_];
    region.x = xx;
    region.y = yy;
    region.width = width;
    region.height = height;
    region.mask = layers_[idx].face_region.mask;

    UpdateRegion(region, bitmap_buffer);

    region.x += outline;
    region.y += outline;
    region.width -= (outline * 2);
    region.height -= (outline * 2);

    return region_count_++;
}

void Atlas::UpdateRegion(const AtlasRegion &region, const uint8_t *bitmap_buffer) {
    uint32_t size = region.width * region.height * 4;
    if (0 < size) {
        const bgfx::Memory *mem = bgfx::alloc(size);
        bx::memSet(mem->data, 0, mem->size);
        if (region.GetType() == AtlasRegion::TypeBgra8) {
            const uint8_t *in_line_buffer = bitmap_buffer;
            uint8_t *out_line_buffer = &texture_buffer_[region.GetFaceIndex() * (texture_size_ * texture_size_ * 4) + (((region.y * texture_size_) + region.x) * 4)];

            // copy the region to the texture buffer
            for (int yy = 0; yy < region.height; ++yy) {
                bx::memCopy(out_line_buffer, in_line_buffer, region.width * 4);
                in_line_buffer += region.width * 4;
                out_line_buffer += texture_size_ * 4;
            }

            bx::memCopy(mem->data, bitmap_buffer, mem->size);
        } else {
            uint32_t layer = region.GetComponentIndex();
            const uint8_t *in_line_buffer = bitmap_buffer;
            uint8_t *out_line_buffer = (texture_buffer_ + region.GetFaceIndex() * (texture_size_ * texture_size_ * 4) + (((region.y * texture_size_) + region.x) * 4));

            for (int yy = 0; yy < region.height; ++yy) {
                for (int xx = 0; xx < region.width; ++xx) {
                    out_line_buffer[(xx * 4) + layer] = in_line_buffer[xx];
                }

                bx::memCopy(mem->data + yy * region.width * 4, out_line_buffer, region.width * 4);
                in_line_buffer += region.width;
                out_line_buffer += texture_size_ * 4;
            }
        }

        bgfx::updateTextureCube(texture_handle_, 0, (uint8_t) region.GetFaceIndex(), 0, region.x, region.y, region.width, region.height, mem);
    }
}

void Atlas::PackFaceLayerUv(uint32_t idx, uint8_t *vertex_buffer, uint32_t offset, uint32_t stride) const {
    PackUv(layers_[idx].face_region, vertex_buffer, offset, stride);
}

void Atlas::PackUv(uint16_t region_handle, uint8_t *vertex_buffer, uint32_t offset, uint32_t stride) const {
    const AtlasRegion &region = regions_[region_handle];
    PackUv(region, vertex_buffer, offset, stride);
}

static void WriteUv(uint8_t *vertex_buffer, int16_t x, int16_t y, int16_t z, int16_t w) {
    auto xyzw = (uint16_t *) vertex_buffer;
    xyzw[0] = x;
    xyzw[1] = y;
    xyzw[2] = z;
    xyzw[3] = w;
}

void Atlas::PackUv(const AtlasRegion &region, uint8_t *vertex_buffer, uint32_t offset, uint32_t stride) const {
    auto x0 = (int16_t) (((float) region.x * texel_size_) - float(INT16_MAX));
    auto y0 = (int16_t) (((float) region.y * texel_size_) - float(INT16_MAX));
    auto x1 = (int16_t) ((((float) region.x + region.width) * texel_size_) - float(INT16_MAX));
    auto y1 = (int16_t) ((((float) region.y + region.height) * texel_size_) - float(INT16_MAX));
    auto ww = (int16_t) ((float(INT16_MAX) / 4.0f) * (float) region.GetComponentIndex());

    vertex_buffer += offset;
    switch (region.GetFaceIndex()) {
        case 0: // +X
            x0 = -x0;
            x1 = -x1;
            y0 = -y0;
            y1 = -y1;
            WriteUv(vertex_buffer, INT16_MAX, y0, x0, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, INT16_MAX, y1, x0, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, INT16_MAX, y1, x1, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, INT16_MAX, y0, x1, ww);
            break;

        case 1: // -X
            y0 = -y0;
            y1 = -y1;
            WriteUv(vertex_buffer, INT16_MIN, y0, x0, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, INT16_MIN, y1, x0, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, INT16_MIN, y1, x1, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, INT16_MIN, y0, x1, ww);
            break;

        case 2: // +Y
            WriteUv(vertex_buffer, x0, INT16_MAX, y0, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x0, INT16_MAX, y1, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, INT16_MAX, y1, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, INT16_MAX, y0, ww);
            break;

        case 3: // -Y
            y0 = -y0;
            y1 = -y1;
            WriteUv(vertex_buffer, x0, INT16_MIN, y0, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x0, INT16_MIN, y1, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, INT16_MIN, y1, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, INT16_MIN, y0, ww);
            break;

        case 4: // +Z
            y0 = -y0;
            y1 = -y1;
            WriteUv(vertex_buffer, x0, y0, INT16_MAX, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x0, y1, INT16_MAX, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, y1, INT16_MAX, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, y0, INT16_MAX, ww);
            break;

        case 5: // -Z
            x0 = -x0;
            x1 = -x1;
            y0 = -y0;
            y1 = -y1;
            WriteUv(vertex_buffer, x0, y0, INT16_MIN, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x0, y1, INT16_MIN, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, y1, INT16_MIN, ww);
            vertex_buffer += stride;
            WriteUv(vertex_buffer, x1, y0, INT16_MIN, ww);
            break;
    }
}
