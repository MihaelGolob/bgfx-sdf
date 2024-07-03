//
// Created by MihaelGolob on 30. 04. 2024.
//
#pragma once

#include <bgfx/bgfx.h>

// AtlasRegion struct represents a region in the atlas texture.
struct AtlasRegion {
    enum Type {
        TYPE_GRAY = 1, // 1 component
        TYPE_BGRA8 = 4  // 4 components
    };

    [[nodiscard]] Type getType() const; 
    [[nodiscard]] uint32_t getFaceIndex() const; 
    [[nodiscard]] uint32_t getComponentIndex() const;
    void setMask(Type _type, uint32_t _faceIndex, uint32_t _componentIndex); 
    
    uint16_t x, y;
    uint16_t width, height;
    uint32_t mask; //encode the region type, the face index and the component index in case of a gray region
};
