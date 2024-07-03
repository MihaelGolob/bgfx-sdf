//
// Created by MihaelGolob on 30. 04. 2024.
//
#pragma once

#include <bgfx/bgfx.h>

// AtlasRegion struct represents a region in the atlas texture.
struct AtlasRegion {
    enum Type {
        TypeGray = 1, // 1 component
        TypeBgra8 = 4  // 4 components
    };

    [[nodiscard]] Type GetType() const; 
    [[nodiscard]] uint32_t GetFaceIndex() const; 
    [[nodiscard]] uint32_t GetComponentIndex() const;
    void SetMask(Type type, uint32_t face_index, uint32_t component_index); 
    
    uint16_t x, y;
    uint16_t width, height;
    uint32_t mask; //encode the region type, the face index and the component index in case of a gray region
};
