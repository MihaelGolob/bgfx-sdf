//
// Created by MihaelGolob on 13. 05. 2024.
//

#pragma once

int GetPixel(uint8_t* bitmap, int width, int height, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return 0;
    
    return bitmap[y * width + x] == 0 ? 0 : 1;
}

float FindDistanceToGlyph(uint8_t* glyphBitmap, int width, int height, int x, int y, int spread = 6) {
    const int state = GetPixel(glyphBitmap, width, height, x, y);
    const int minX = std::max(x - spread, 0);
    const int minY = std::max(y - spread, 0);
    const int maxX = std::min(x + spread, width - 1);
    const int maxY = std::min(y + spread, height - 1);
    
    float minDistance = spread*spread;
    for (int j = minY; j <= maxY; j++) {
        for (int i = minX; i <= maxX; i++) {
            const int pixelState = GetPixel(glyphBitmap, width, height, i, j);
            if (pixelState == state) {
                continue;
            }
            
            float dxSquared = (i - x) * (i - x);
            float dySquared = (j - y) * (j - y);
            float dSquared = dxSquared + dySquared;
        
            minDistance = std::min(minDistance, dSquared);
        }
    }
    
    minDistance = bx::sqrt(minDistance);
    minDistance = std::min(minDistance, (float) spread);
    // map from [0, spread] to [0, 1]
    float output = (minDistance - 0.5f) / (spread - 0.5f);
    
    output *= state == 0 ? -1.0f : 1.0f;
    
    // map from [-1, 1] to [0, 1]
    output = (output + 1.0f) / 2.0f;
    return output;
}

bool BuildSignedDistanceField(uint8_t* outBuffer, uint8_t* glyphBitmap, int width, int height, int spread = 6) {
    // Compute the distance field
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float dist = FindDistanceToGlyph(glyphBitmap, width, height, x, y, spread);
            outBuffer[y * width + x] = (uint8_t) (bx::floor(dist * 255.0f));
        }
    }
    
    return true;
}