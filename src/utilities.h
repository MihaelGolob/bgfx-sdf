//
// Created by MihaelGolob on 22. 04. 2024.
//

#pragma once

#include <bx/file.h>
#include <bx/readerwriter.h>
#include <iostream>
#include <string>

void PrintError(const char* error);
void PrintError(const std::string& error);
void PrintInfo(const char* info);
void PrintInfo(const std::string& info);

void PrintRed(const char* info);
void PrintBlue(const char* info);
void PrintCyan(const char* info);
void PrintGreen(const char* info);

class FileReader : public bx::FileReader {
    typedef bx::FileReader super;

public:
    bool open(const bx::FilePath& file_path, bx::Error* err) override {
        return super::open(file_path, err);
    }
};

bx::AllocatorI* GetDefaultAllocator();
void* Load(const bx::FilePath& file_path, uint32_t* size);