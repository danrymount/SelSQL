//
// Created by quiks on 31.10.2019.
//

#ifndef SELSQL_ENGINEUTILS_H
#define SELSQL_ENGINEUTILS_H

#include <filesystem>
#include <fstream>
#include <memory>
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Table.h"
#include <cstring>
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
//int CheckFileHashSum(const std::shared_ptr<DB_FILE>& file);
void WriteIntToFile(std::fstream* file, int value);
int ReadIntFromFile(std::fstream* file);
int GetFileSize(std::fstream* file);
buffer_data GetTableBuffer(Table* table);
std::shared_ptr<Table> ReadTableFromBuffer(char* data);

//double CalcHashSum(const std::shared_ptr<std::fstream>& file);
//std::shared_ptr<DB_FILE> FindLastVersion(const std::string& table_name, int max_v);
//int GetVersion(const std::string& file_name);


class FileNotOpened{};
#endif  // SELSQL_ENGINEUTILS_H
