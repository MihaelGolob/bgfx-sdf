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
    AtlasRegion faceRegion{};
};

Atlas::Atlas(uint16_t _textureSize, uint16_t _maxRegionsCount)
        : m_usedLayers(0), m_usedFaces(0), m_textureSize(_textureSize), m_regionCount(0),
          m_maxRegionCount(_maxRegionsCount) {
    BX_ASSERT(_textureSize >= 64 && _textureSize <= 4096, "Invalid _textureSize %d.", _textureSize);
    BX_ASSERT(_maxRegionsCount >= 64 && _maxRegionsCount <= 32000, "Invalid _maxRegionsCount %d.", _maxRegionsCount);

    m_texelSize = float(UINT16_MAX) / float(m_textureSize);

    m_layers = new PackedLayer[6]; // number of layers is arbitrary
    for (int ii = 0; ii < 6; ++ii) {
        m_layers[ii].packer.init(_textureSize, _textureSize);
    }

    m_regions = new AtlasRegion[_maxRegionsCount];
    m_textureBuffer = new uint8_t[_textureSize * _textureSize * 6 * 4]; // 6 layers * 4 channels (RGBA)
    bx::memSet(m_textureBuffer, 0, _textureSize * _textureSize * 6 * 4);

    m_textureHandle = bgfx::createTextureCube(_textureSize, false, 1, bgfx::TextureFormat::BGRA8);
}

Atlas::Atlas(uint16_t _textureSize, const uint8_t *_textureBuffer, uint16_t _regionCount, const uint8_t *_regionBuffer, uint16_t _maxRegionsCount)
        : m_usedLayers(6), m_usedFaces(6), m_textureSize(_textureSize), m_regionCount(_regionCount),
        m_maxRegionCount(_regionCount < _maxRegionsCount ? _regionCount : _maxRegionsCount) {
    BX_ASSERT(_regionCount <= 64 && _maxRegionsCount <= 4096, "_regionCount %d, _maxRegionsCount %d", _regionCount,
              _maxRegionsCount);

    m_texelSize = float(UINT16_MAX) / float(m_textureSize);

    m_regions = new AtlasRegion[_regionCount];
    m_textureBuffer = new uint8_t[getTextureBufferSize()];

    bx::memCopy(m_regions, _regionBuffer, _regionCount * sizeof(AtlasRegion));
    bx::memCopy(m_textureBuffer, _textureBuffer, getTextureBufferSize());

    m_textureHandle = bgfx::createTextureCube(_textureSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_SAMPLER_NONE,
                                              bgfx::makeRef(m_textureBuffer, getTextureBufferSize())
    );
}

Atlas::~Atlas() {
    bgfx::destroy(m_textureHandle);

    delete[] m_layers;
    delete[] m_regions;
    delete[] m_textureBuffer;
}

uint16_t Atlas::addRegion(uint16_t _width, uint16_t _height, const uint8_t *_bitmapBuffer, AtlasRegion::Type _type,
                          uint16_t outline) {
    if (m_regionCount >= m_maxRegionCount) {
        return UINT16_MAX;
    }

    uint16_t xx = 0;
    uint16_t yy = 0;
    uint32_t idx = 0;
    while (idx < m_usedLayers) {
        if (m_layers[idx].faceRegion.getType() == _type
            && m_layers[idx].packer.addRectangle(_width + 1, _height + 1, xx, yy)) {
            break;
        }

        idx++;
    }

    if (idx >= m_usedLayers) {
        if ((idx + _type) > 24
            || m_usedFaces >= 6) {
            return UINT16_MAX;
        }

        AtlasRegion &region = m_layers[idx].faceRegion;
        region.x = 0;
        region.y = 0;
        region.width = m_textureSize;
        region.height = m_textureSize;
        region.setMask(_type, m_usedFaces, 0);

        m_usedLayers++;
        m_usedFaces++;

        if (!m_layers[idx].packer.addRectangle(_width + 1, _height + 1, xx, yy)) {
            return UINT16_MAX;
        }
    }

    AtlasRegion &region = m_regions[m_regionCount];
    region.x = xx;
    region.y = yy;
    region.width = _width;
    region.height = _height;
    region.mask = m_layers[idx].faceRegion.mask;

    updateRegion(region, _bitmapBuffer);

    region.x += outline;
    region.y += outline;
    region.width -= (outline * 2);
    region.height -= (outline * 2);

    return m_regionCount++;
}

void Atlas::updateRegion(const AtlasRegion &_region, const uint8_t *_bitmapBuffer) {
    uint32_t size = _region.width * _region.height * 4;
    if (0 < size) {
        const bgfx::Memory *mem = bgfx::alloc(size);
        bx::memSet(mem->data, 0, mem->size);
        if (_region.getType() == AtlasRegion::TYPE_BGRA8) {
            const uint8_t *inLineBuffer = _bitmapBuffer;
            uint8_t *outLineBuffer = &m_textureBuffer[_region.getFaceIndex() * (m_textureSize * m_textureSize * 4) +
                                     (((_region.y * m_textureSize) + _region.x) * 4)];

            // copy the region to the texture buffer
            for (int yy = 0; yy < _region.height; ++yy) {
                bx::memCopy(outLineBuffer, inLineBuffer, _region.width * 4);
                inLineBuffer += _region.width * 4;
                outLineBuffer += m_textureSize * 4;
            }

            bx::memCopy(mem->data, _bitmapBuffer, mem->size);
        } else {
            uint32_t layer = _region.getComponentIndex();
            const uint8_t *inLineBuffer = _bitmapBuffer;
            uint8_t *outLineBuffer = (m_textureBuffer + _region.getFaceIndex() * (m_textureSize * m_textureSize * 4) +
                                      (((_region.y * m_textureSize) + _region.x) * 4));

            for (int yy = 0; yy < _region.height; ++yy) {
                for (int xx = 0; xx < _region.width; ++xx) {
                    outLineBuffer[(xx * 4) + layer] = inLineBuffer[xx];
                }

                bx::memCopy(mem->data + yy * _region.width * 4, outLineBuffer, _region.width * 4);
                inLineBuffer += _region.width;
                outLineBuffer += m_textureSize * 4;
            }
        }

        bgfx::updateTextureCube(m_textureHandle, 0, (uint8_t) _region.getFaceIndex(), 0, _region.x, _region.y,
                                _region.width, _region.height, mem);
    }
}

void Atlas::packFaceLayerUV(uint32_t _idx, uint8_t *_vertexBuffer, uint32_t _offset, uint32_t _stride) const {
    packUV(m_layers[_idx].faceRegion, _vertexBuffer, _offset, _stride);
}

void Atlas::packUV(uint16_t _regionHandle, uint8_t *_vertexBuffer, uint32_t _offset, uint32_t _stride) const {
    const AtlasRegion &region = m_regions[_regionHandle];
    packUV(region, _vertexBuffer, _offset, _stride);
}

static void writeUV(uint8_t *_vertexBuffer, int16_t _x, int16_t _y, int16_t _z, int16_t _w) {
    uint16_t *xyzw = (uint16_t *) _vertexBuffer;
    xyzw[0] = _x;
    xyzw[1] = _y;
    xyzw[2] = _z;
    xyzw[3] = _w;
}

void Atlas::packUV(const AtlasRegion &_region, uint8_t *_vertexBuffer, uint32_t _offset, uint32_t _stride) const {
    int16_t x0 = (int16_t) (((float) _region.x * m_texelSize) - float(INT16_MAX));
    int16_t y0 = (int16_t) (((float) _region.y * m_texelSize) - float(INT16_MAX));
    int16_t x1 = (int16_t) ((((float) _region.x + _region.width) * m_texelSize) - float(INT16_MAX));
    int16_t y1 = (int16_t) ((((float) _region.y + _region.height) * m_texelSize) - float(INT16_MAX));
    int16_t ww = (int16_t) ((float(INT16_MAX) / 4.0f) * (float) _region.getComponentIndex());

    _vertexBuffer += _offset;
    switch (_region.getFaceIndex()) {
        case 0: // +X
            x0 = -x0;
            x1 = -x1;
            y0 = -y0;
            y1 = -y1;
            writeUV(_vertexBuffer, INT16_MAX, y0, x0, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, INT16_MAX, y1, x0, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, INT16_MAX, y1, x1, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, INT16_MAX, y0, x1, ww);
            _vertexBuffer += _stride;
            break;

        case 1: // -X
            y0 = -y0;
            y1 = -y1;
            writeUV(_vertexBuffer, INT16_MIN, y0, x0, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, INT16_MIN, y1, x0, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, INT16_MIN, y1, x1, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, INT16_MIN, y0, x1, ww);
            _vertexBuffer += _stride;
            break;

        case 2: // +Y
            writeUV(_vertexBuffer, x0, INT16_MAX, y0, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x0, INT16_MAX, y1, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, INT16_MAX, y1, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, INT16_MAX, y0, ww);
            _vertexBuffer += _stride;
            break;

        case 3: // -Y
            y0 = -y0;
            y1 = -y1;
            writeUV(_vertexBuffer, x0, INT16_MIN, y0, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x0, INT16_MIN, y1, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, INT16_MIN, y1, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, INT16_MIN, y0, ww);
            _vertexBuffer += _stride;
            break;

        case 4: // +Z
            y0 = -y0;
            y1 = -y1;
            writeUV(_vertexBuffer, x0, y0, INT16_MAX, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x0, y1, INT16_MAX, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, y1, INT16_MAX, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, y0, INT16_MAX, ww);
            _vertexBuffer += _stride;
            break;

        case 5: // -Z
            x0 = -x0;
            x1 = -x1;
            y0 = -y0;
            y1 = -y1;
            writeUV(_vertexBuffer, x0, y0, INT16_MIN, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x0, y1, INT16_MIN, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, y1, INT16_MIN, ww);
            _vertexBuffer += _stride;
            writeUV(_vertexBuffer, x1, y0, INT16_MIN, ww);
            _vertexBuffer += _stride;
            break;
    }
}
