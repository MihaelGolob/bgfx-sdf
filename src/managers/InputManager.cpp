//
// Created by MihaelGolob on 22. 04. 2024.
//

#include "InputManager.h"

// define static member
std::map<int, bool> InputManager::keys_map_;
std::vector<FunctionHandler<void (int)>> InputManager::key_pressed_callbacks_;
std::vector<FunctionHandler<void (int)>> InputManager::key_released_callbacks_;
unsigned int InputManager::id_counter_ = 0;

InputManager::InputManager(GLFWwindow *window, const std::vector<int>& keys_to_track) {
    for (int key : keys_to_track) {
        keys_map_[key] = false;
    }
    
    glfwSetKeyCallback(window, InputManager::KeyCallback);
}

InputManager::~InputManager() {
    if (!key_pressed_callbacks_.empty()) {
        PrintError("There are still some key pressed callbacks subscribed!");
    }
    
    if (!key_released_callbacks_.empty()) {
        PrintError("There are still some key released callbacks subscribed!");
    }
}

void InputManager::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        keys_map_[key] = true;
        for (const auto& callback : key_pressed_callbacks_) {
            callback.function(key);
        }
    } else if (action == GLFW_RELEASE) {
        keys_map_[key] = false;
        for (const auto& callback : key_released_callbacks_) {
            callback.function(key);
        }
    }
}

bool InputManager::IsKeyPressed(int key) {
    if (keys_map_.find(key) == keys_map_.end()) {
        PrintError("Key not found in keys_map_!");
        return false;
    }
    return keys_map_[key];
}

FunctionId InputManager::SubscribeKeyPressed(const std::function<void (int)>& callback) {
    key_pressed_callbacks_.push_back({callback, id_counter_});
    return {id_counter_++ };
}

FunctionId InputManager::SubscribeKeyReleased(const std::function<void (int)>& callback) {
    key_released_callbacks_.push_back({callback, id_counter_});
    return {id_counter_++ };
}

void InputManager::UnsubscribeKeyPressed(const FunctionId handler) {
    for (int i = 0; i < key_pressed_callbacks_.size(); i++) {
        if (key_pressed_callbacks_[i].id == handler) {
            key_pressed_callbacks_.erase(key_pressed_callbacks_.begin() + i);
            return;
        }
    }
}

void InputManager::UnsubscribeKeyReleased(const FunctionId handler) {
    for (int i = 0; i < key_released_callbacks_.size(); i++) {
        if (key_released_callbacks_[i].id == handler) {
            key_released_callbacks_.erase(key_released_callbacks_.begin() + i);
            return;
        }
    }
}

char InputManager::GetKeyChar(int key) {
    switch(key) {
        case GLFW_KEY_A: return 'a';
        case GLFW_KEY_B: return 'b';
        case GLFW_KEY_C: return 'c';
        case GLFW_KEY_D: return 'd';
        case GLFW_KEY_E: return 'e';
        case GLFW_KEY_F: return 'f';
        case GLFW_KEY_G: return 'g';
        case GLFW_KEY_H: return 'h';
        case GLFW_KEY_I: return 'i';
        case GLFW_KEY_J: return 'j';
        case GLFW_KEY_K: return 'k';
        case GLFW_KEY_L: return 'l';
        case GLFW_KEY_M: return 'm';
        case GLFW_KEY_N: return 'n';
        case GLFW_KEY_O: return 'o';
        case GLFW_KEY_P: return 'p';
        case GLFW_KEY_Q: return 'q';
        case GLFW_KEY_R: return 'r';
        case GLFW_KEY_S: return 's';
        case GLFW_KEY_T: return 't';
        case GLFW_KEY_U: return 'u';
        case GLFW_KEY_V: return 'v';
        case GLFW_KEY_W: return 'w';
        case GLFW_KEY_X: return 'x';
        case GLFW_KEY_Y: return 'y';
        case GLFW_KEY_Z: return 'z';
        case GLFW_KEY_0: return '0';
        case GLFW_KEY_1: return '1';
        case GLFW_KEY_2: return '2';
        case GLFW_KEY_3: return '3';
        case GLFW_KEY_4: return '4';
        case GLFW_KEY_5: return '5';
        case GLFW_KEY_6: return '6';
        case GLFW_KEY_7: return '7';
        case GLFW_KEY_8: return '8';
        case GLFW_KEY_9: return '9';
        case GLFW_KEY_SPACE: return ' ';
        default: {
            PrintError("Key not found in GetKeyChar!");
            return '\0';
        }
    }
}
