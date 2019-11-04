//
// Created by quiks on 31.10.2019.
//

#ifndef SELSQL_ENGINEUTILS_H
#define SELSQL_ENGINEUTILS_H

#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/DataBlock.h"
#include "../../Utils/Structures/Data/Table.h"
#ifdef __WIN32
#define DIR_SEPARATOR '\\'
#elif __linux
#define DIR_SEPARATOR  '/'
#endif

typedef std::pair<char*, int> buffer_data;
namespace fs = std::filesystem;
struct DB_FILE {
    int version = 0;
    std::fstream* meta_file;
    std::fstream* data_file;
    DB_FILE() = default;
    DB_FILE(std::fstream* m_file, std::fstream* d_file);

    void close();
    int isOpen();
};

void WriteIntToFile(std::fstream* file, int value);
int ReadIntFromFile(std::fstream* file);
int GetFileSize(std::fstream* file);

buffer_data GetTableBuffer(Table* table);
buffer_data GetDataBlockBuffer(DataBlock* data_block);
std::shared_ptr<DataBlock> ReadDataBlockFromBuffer(char* data);
std::shared_ptr<Table> ReadTableFromBuffer(char* data);


class FileNotOpened{};
#endif  // SELSQL_ENGINEUTILS_H
