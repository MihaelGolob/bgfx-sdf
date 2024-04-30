//
// Created by MihaelGolob on 30. 04. 2024.
//

#include "AtlasRegion.h"

AtlasRegion::Type AtlasRegion::getType() const {
    return (Type) ((mask >> 0) & 0x0000000F);
}

uint32_t AtlasRegion::getFaceIndex() const {
    return (mask >> 4) & 0x0000000F;
}

uint32_t AtlasRegion::getComponentIndex() const {
    return (mask >> 8) & 0x0000000F;
}

void AtlasRegion::setMask(Type _type, uint32_t _faceIndex, uint32_t _componentIndex) {
    mask = (_componentIndex << 8) + (_faceIndex << 4) + (uint32_t) _type;
}