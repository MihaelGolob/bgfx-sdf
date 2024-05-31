//
// Created by MihaelGolob on 22. 04. 2024.
//

#include "utilities.h"

// logging

void PrintError(const char* _error) {
    std::cerr << "ERROR: " << _error << std::endl;
}

void PrintError(const std::string& _error) {
    PrintError(_error.c_str());
}

void PrintInfo(const char* _info) {
    std::cout << "INFO: " << _info << std::endl;
}

void PrintInfo(const std::string& _info) {
    PrintInfo(_info.c_str());
}

// file reading 

bx::AllocatorI* getDefaultAllocator() {
    BX_PRAGMA_DIAGNOSTIC_PUSH()
    BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459) // warning C4459: declaration of "s_allocator" hides global declaration
    BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow")
    static bx::DefaultAllocator s_allocator;
    return &s_allocator;
    BX_PRAGMA_DIAGNOSTIC_POP()
}

void* load(const bx::FilePath& filePath, uint32_t* size) {
    const auto allocator = getDefaultAllocator();
    const auto reader = BX_NEW(allocator, bx::FileReader);
    
    if (bx::open(reader, filePath) ) {
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
        error_message += std::string(filePath.getCPtr()); 
        PrintError(error_message.c_str());
    }

    if (size != nullptr) {
        *size = 0;
    }

    return nullptr;
}