//
// Created by MihaelGolob on 22. 04. 2024.
//

#pragma once

#include <GLFW/glfw3.h>
#include <map>
#include <vector>
#include <functional>
#include "../utilities.h"
#include "../window/Window.h"

struct FunctionId {
    unsigned int id;
    
    bool operator==(const FunctionId& other) const {
        return id == other.id;
    }
    
    bool operator!=(const FunctionId& other) const {
        return id != other.id;
    }
};

template<typename T>
struct FunctionHandler {
    
    std::function<T> function;
    FunctionId id;
};

class InputManager {
public:
    InputManager(Window* window, const std::vector<int>& keys_to_track);
    ~InputManager();
    
    static bool IsKeyPressed(int key);
    
    static FunctionId SubscribeKeyPressed(const std::function<void (int)>& callback);
    static FunctionId SubscribeKeyReleased(const std::function<void (int)>& callback);
    
    static void UnsubscribeKeyPressed(FunctionId);
    static void UnsubscribeKeyReleased(FunctionId);
    
    static char GetKeyChar(int key);
    
private:
    static void KeyCallback(int key, WindowKeyAction action);
    static std::map<int, bool> keys_map_;

    static std::vector<FunctionHandler<void (int)>> key_pressed_callbacks_;
    static std::vector<FunctionHandler<void (int)>> key_released_callbacks_; 
    
    static unsigned int id_counter_;
};
