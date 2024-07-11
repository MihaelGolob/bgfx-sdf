/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#pragma once

/// Inspired from texture-atlas from freetype-gl (http://code.google.com/p/freetype-gl/)
/// by Nicolas Rougier (Nicolas.Rougier@inria.fr)
/// The actual implementation is based on the article by Jukka Jylänki : "A
/// Thousand Ways to Pack the Bin - A Practical Approach to Two-Dimensional
/// Rectangle Bin Packing", February 27, 2010.
/// More precisely, this is an implementation of the Skyline Bottom-Left
/// algorithm based on C++ sources provided by Jukka Jylänki at:
/// http://clb.demon.fi/files/RectangleBinPack/

#include <bgfx/bgfx.h>
#include "AtlasRegion.h"

// Atlas represents a texture atlas, which uses the RectanglePacker
// to efficiently manage the placement of regions in the atlas.
class Atlas {
public:
    /// create an empty dynamic atlas (region can be updated and added)
    /// @param textureSize an atlas creates a texture cube of 6 faces with size equal to (textureSize*textureSize * sizeof(RGBA) )
    /// @param maxRegionCount maximum number of region allowed in the atlas
    explicit Atlas(uint16_t texture_size, uint16_t max_regions_count = 4096);

    /// initialize a static atlas with serialized data	(region can be updated but not added)
    /// @param textureSize an atlas creates a texture cube of 6 faces with size equal to (textureSize*textureSize * sizeof(RGBA) )
    /// @param textureBuffer buffer of size 6*textureSize*textureSize*sizeof(uint32_t) (will be copied)
    /// @param regionCount number of region in the Atlas
    /// @param regionBuffer buffer containing the region (will be copied)
    /// @param maxRegionCount maximum number of region allowed in the atlas
    Atlas(uint16_t texture_size, const uint8_t *texture_buffer, uint16_t region_count, const uint8_t *region_buffer, uint16_t max_regions_count = 4096);
    ~Atlas();

    /// add a region to the atlas, and copy the content of mem to the underlying texture
    /// returns the number of regions after insertion or UINT16_MAX if the region could not be added
    uint16_t AddRegion(uint16_t width, uint16_t height, const uint8_t *bitmap_buffer,
                       AtlasRegion::Type type = AtlasRegion::TypeBgra8, uint16_t outline = 0);

    /// update a pre allocated region
    void UpdateRegion(const AtlasRegion &region, const uint8_t *bitmap_buffer, AtlasRegion::Type type);

    /// Pack the UV coordinates of the four corners of a region to a vertex buffer using the supplied vertex format.
    /// v0 -- v3
    /// |     |     encoded in that order:  v0,v1,v2,v3
    /// v1 -- v2
    /// @remark the UV are four signed short normalized components.
    /// @remark the x,y,z components encode cube uv coordinates. The w component encode the color channel if any.
    /// @param handle handle to the region we are interested in
    /// @param vertexBuffer address of the first vertex we want to update. Must be valid up to vertexBuffer + offset + 3*stride + 4*sizeof(int16_t), which means the buffer must contains at least 4 vertex includind the first.
    /// @param offset byte offset to the first uv coordinate of the vertex in the buffer
    /// @param stride stride between tho UV coordinates, usually size of a Vertex.
    void PackUv(uint16_t region_handle, uint8_t *vertex_buffer, uint32_t offset, uint32_t stride) const;
    void PackUv(const AtlasRegion &region, uint8_t *vertex_buffer, uint32_t offset, uint32_t stride) const;

    /// Same as PackUv but pack a whole face of the atlas cube, mostly used for debugging and visualizing atlas
    void PackFaceLayerUv(uint32_t idx, uint8_t *vertex_buffer, uint32_t offset, uint32_t stride) const;

    /// return the TextureHandle (cube) of the atlas
    [[nodiscard]] bgfx::TextureHandle GetTextureHandle() const {
        return texture_handle_;
    }

    //retrieve a region info
    [[nodiscard]] const AtlasRegion &GetRegion(uint16_t handle) const {
        return regions_[handle];
    }

    /// retrieve the size of side of a texture in pixels
    [[nodiscard]] uint16_t GetTextureSize() const {
        return texture_size_;
    }

    /// retrieve the usage ratio of the atlas
    //float GetUsageRatio() const { return 0.0f; }

    /// retrieve the numbers of region in the atlas
    [[nodiscard]] uint16_t GetRegionCount() const {
        return region_count_;
    }

    /// retrieve a pointer to the region buffer (in order to serialize it)
    [[nodiscard]] const AtlasRegion *GetRegionBuffer() const {
        return regions_;
    }

    /// retrieve the byte size of the texture
    [[nodiscard]] uint32_t GetTextureBufferSize() const {
        return 6 * texture_size_ * texture_size_ * 4;
    }

    /// retrieve the mirrored texture buffer (to serialize it)
    [[nodiscard]] const uint8_t *GetTextureBuffer() const {
        return texture_buffer_;
    }

private:
    struct PackedLayer;
    PackedLayer *layers_{};
    AtlasRegion *regions_;
    uint8_t *texture_buffer_;

    uint32_t used_layers_;
    uint32_t used_faces_;

    bgfx::TextureHandle texture_handle_{};
    uint16_t texture_size_;
    float texel_size_;

    uint16_t region_count_;
    uint16_t max_region_count_;
};
