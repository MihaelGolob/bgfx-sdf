//
// Created by MihaelGolob on 22. 04. 2024.
//

#include "utilities.h"

// logging

void PrintError(const char* error) {
    std::cerr << "ERROR: " << error << std::endl;
}

void PrintInfo(const char* info) {
    std::cout << "INFO: " << info << std::endl;
}

void PrintInfo(const std::string& info) {
    PrintInfo(info.c_str());
}

void PrintRed(const char* info) {
    std::cout << "\033[0;31m" << info << "\033[0m" << std::endl;
}

void PrintBlue(const char* info) {
    std::cout << "\033[0;34m" << info << "\033[0m" << std::endl;
}

void PrintCyan(const char* info) {
    std::cout << "\033[0;36m" << info << "\033[0m" << std::endl;
}

void PrintGreen(const char* info) {
    std::cout << "\033[0;32m" << info << "\033[0m" << std::endl;
}

// file reading 
bx::AllocatorI* GetDefaultAllocator() {
    BX_PRAGMA_DIAGNOSTIC_PUSH()
    BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459) 
    BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow")
    static bx::DefaultAllocator s_allocator;
    return &s_allocator;
    BX_PRAGMA_DIAGNOSTIC_POP()
}

void* Load(const bx::FilePath& file_path, uint32_t* size) {
    const auto allocator = GetDefaultAllocator();
    const auto reader = BX_NEW(allocator, bx::FileReader);
    
    if (bx::open(reader, file_path) ) {
        auto actual_size = (uint32_t)bx::getSize(reader);
        void* data = bx::alloc(allocator, actual_size);
        bx::read(reader, data, actual_size, bx::ErrorAssert{});
        bx::close(reader);
        
        if (size != nullptr) {
            *size = actual_size;
        }
        return data;
    } else {
        std::string error_message = "Could not open file: ";
        error_message += std::string(file_path.getCPtr()); 
        PrintError(error_message.c_str());
    }

    if (size != nullptr) {
        *size = 0;
    }

    return nullptr;
}