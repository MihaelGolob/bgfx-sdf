//
// Created by MihaelGolob on 30. 04. 2024.
//

#include "AtlasRegion.h"

AtlasRegion::Type AtlasRegion::GetType() const {
    return (Type) ((mask >> 0) & 0x0000000F);
}

uint32_t AtlasRegion::GetFaceIndex() const {
    return (mask >> 4) & 0x0000000F;
}

uint32_t AtlasRegion::GetComponentIndex() const {
    return (mask >> 8) & 0x0000000F;
}

void AtlasRegion::SetMask(Type type, uint32_t face_index, uint32_t component_index) {
    mask = (component_index << 8) + (face_index << 4) + (uint32_t) type;
}