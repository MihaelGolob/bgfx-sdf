//
// Created by MihaelGolob on 22. 04. 2024.
//

#include "InputManager.h"

// define static member
std::map<int, bool> InputManager::keys_map_;

InputManager::InputManager(GLFWwindow *window, const std::vector<int>& keys_to_track) {
    for (int key : keys_to_track) {
        keys_map_[key] = false;
    }
    
    glfwSetKeyCallback(window, InputManager::KeyCallback);
}

void InputManager::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys_map_[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys_map_[key] = false;
    }
}

bool InputManager::IsKeyPressed(int key) {
    if (keys_map_.find(key) == keys_map_.end()) {
        PrintError("Key not found in keys_map_!");
        return false;
    }
    return keys_map_[key];
}
