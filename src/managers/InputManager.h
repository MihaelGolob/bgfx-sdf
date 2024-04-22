//
// Created by MihaelGolob on 22. 04. 2024.
//

#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include "../utilities.h"

class InputManager {
public:
    InputManager(GLFWwindow* window, const std::vector<int>& keys_to_track);
    
    static bool IsKeyPressed(int key); 
    
private:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static std::map<int, bool> keys_map_;
};
