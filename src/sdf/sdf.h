//
// Created by MihaelGolob on 13. 05. 2024.
//

#pragma once

#define MAX_FLOAT 1e+37f
#define MAX_PASSES 10
#define SLACK 0.001f
#define SQRT2 1.41421356237f

struct SdfPoint {
    float x, y;
    float distance;
};

float GetSquareDistance(SdfPoint* p1, SdfPoint* p2) {
    float dx = p1->x - p2->x;
    float dy = p1->y - p2->y;
    return dx * dx + dy * dy;
}

float GetEdgeDistance(float gradient_x, float gradient_y, float value) {
    if ((gradient_x == 0.0f) || (gradient_y == 0.0f)) return 0.5f - value; // if gradients are very small, pixel is either inside or outside 
    
    // use symmetry
    gradient_x = bx::abs(gradient_x);
    gradient_y = bx::abs(gradient_y);
    if (gradient_x < gradient_y) std::swap(gradient_x, gradient_y);
    
    // calculate distance
    float value1 = 0.5f * gradient_y / gradient_x;
    if (value < value1) return 0.5f * (gradient_x + gradient_y) - bx::sqrt(2.0f * gradient_x * gradient_y * value);
    else if (value < (1.0f - value1)) return (0.5f - value) * gradient_x;
    return -0.5f * (gradient_x + gradient_y) + bx::sqrt(2.0f * gradient_x * gradient_y * (1.0f - value)); 
}

void CalculateBoundary(const uint8_t* glyph, int width, int height, SdfPoint* points) {
    for (int y = 1; y < height-1; y++) {
        for (int x = 1; x < width-1; x++) {
            auto index = y * width + x;
            SdfPoint point = {(float)x, (float)y, 0.0f};
            
            // skip non-boundary pixels
            if (glyph[index] == 255) continue;
            if (glyph[index] == 0) {
                bool horizontal_check = glyph[index-1] == 255 || glyph[index+1] == 255;
                bool vertical_check = glyph[index-width] == 255 || glyph[index+width] == 255;
                if (!horizontal_check && !vertical_check) continue;
            }
            
            // calculate gradient
            float gradient_x = -(float)glyph[index - width - 1] - SQRT2 * (float)glyph[index - 1] - (float)glyph[index + width - 1]
                               +(float)glyph[index - width + 1] + SQRT2 * (float)glyph[index + 1] + (float)glyph[index + width + 1];
            float gradient_y = -(float)glyph[index - width - 1] - SQRT2 * (float)glyph[index - width] - (float)glyph[index - width + 1]
                               +(float)glyph[index + width - 1] + SQRT2 * (float)glyph[index + width] + (float)glyph[index + width + 1];
            
            if (bx::abs(gradient_x) < SLACK && bx::abs(gradient_y) < SLACK) continue;
            float gradient_len = gradient_x * gradient_x + gradient_y * gradient_y;
            // normalize gradient
            if (gradient_len > SLACK) {
                gradient_len = 1.0f / bx::sqrt(gradient_len);
                gradient_x *= gradient_len;
                gradient_y *= gradient_len;
            }
            
            auto tk = x + y * width;
            float distance = GetEdgeDistance(gradient_x, gradient_y, (float)glyph[index] / 255.0f);
            points[index].x = (float)x + distance * gradient_x;
            points[index].y = (float)y + distance * gradient_y;
            points[index].distance = GetSquareDistance(&point, &points[tk]);
        }
    }
}

// use the sweep & update algorithm to build antialiased signed distance field
bool BuildSignedDistanceField(uint8_t* out_buffer, uint8_t* glyph_bitmap, int width, int height, int spread = 6) {
    auto points = (SdfPoint*)malloc(width * height * sizeof(SdfPoint));
    if (!points) return false;
    
    // initialize points
    for (int i = 0; i < width * height; i++) {
        points[i].x = 0;
        points[i].y = 0;
        points[i].distance = MAX_FLOAT;
    }

    CalculateBoundary(glyph_bitmap, width, height, points); 
    
    // Calculate distances with sweep & update
    for (int pass = 0; pass < MAX_PASSES; pass++) {
        bool changed = false;
        
        // bottom-left to top right
        for (int y = 1; y < height-1; y++) {
            for (int x = 1; x < width-1; x++) {
                auto index = y * width + x;
                SdfPoint point = {(float)x, (float)y, points[index].distance};
                SdfPoint result = {};
                bool changed_inner = false;
                
                auto neighbor = index - width - 1; // (x-1, y-1)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                neighbor = index - width; // (x, y-1)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                neighbor = index - width + 1; // (x+1, y-1)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                neighbor = index - 1; // (x-1, y)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                if (changed_inner) {
                    points[index] = result;
                    changed = true;
                }
            }
        }
        
        // top-right to bottom-left
        for (int y = height-2; y > 0; y--) {
            for (int x = width-2; x > 0; x--) {
                auto index = y * width + x;
                SdfPoint point = {(float)x, (float)y, points[index].distance};
                SdfPoint result = {};
                bool changed_inner = false;

                auto neighbor = index + 1; // (x+1, y)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                neighbor = index + width - 1; // (x-1, y+1)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                neighbor = index + width; // (x, y+1)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }
                
                neighbor = index + width + 1; // (x+1, y+1)
                if (points[neighbor].distance < point.distance) {
                    auto distance = GetSquareDistance(&point, &points[neighbor]);
                    if (distance + SLACK < point.distance) {
                        result = points[neighbor];
                        result.distance = distance;
                        point.distance = distance;
                        changed_inner = true;
                    }
                }

                if (changed_inner) {
                    points[index] = result;
                    changed = true;
                }
            }
        } 
        
        if (!changed) break;
    }
    
    // map to good range
    float scale = 1.0f / (float)spread;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float distance = bx::sqrt(points[y * width + x].distance) * scale;
            if (glyph_bitmap[y * width + x] > 127) distance = -distance;
            out_buffer[y * width + x] = (uint8_t)(bx::clamp(0.5f - distance * 0.5f, 0.0f, 1.0f) * 255.0f);
        }
    }
    
    // clean up
    free(points);
    return true;
}