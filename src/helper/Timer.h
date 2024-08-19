//
// Created by MihaelGolob on 12. 08. 2024.
//

#include <chrono>
#include <string>
#include <functional>

class Timer {
public:
    explicit Timer(const std::string& name, const std::function<void(double)>& callback = nullptr, bool print = false);
    ~Timer();
    
private:
    void Stop();
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint_;
    std::string name_;
    std::function<void(double)> callback_;
    bool print_;
};
