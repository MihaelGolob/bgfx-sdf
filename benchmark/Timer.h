//
// Created by MihaelGolob on 12. 08. 2024.
//

#include <chrono>
#include <string>

class Timer {
public:
    explicit Timer(const std::string& name);
    ~Timer();
    
private:
    void Stop();
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint_;
    std::string name_;
};
