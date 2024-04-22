//
// Created by MihaelGolob on 22. 04. 2024.
//

#pragma once

#include <bx/file.h>
#include <bx/readerwriter.h>
#include <iostream>
#include <string>

void PrintError(const char* _error);
void PrintInfo(const char* _info);

class FileReader : public bx::FileReader {
    typedef bx::FileReader super;

public:
    bool open(const bx::FilePath& filePath, bx::Error* _err) override {
        return super::open(filePath, _err);
    }
};

bx::AllocatorI* getDefaultAllocator();
void* load(const bx::FilePath& filePath, uint32_t* size);