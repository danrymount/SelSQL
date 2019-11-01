//
// Created by quiks on 31.10.2019.
//

#ifndef SELSQL_ENGINEUTILS_H
#define SELSQL_ENGINEUTILS_H

#include <filesystem>
#include <fstream>
#include <memory>
#include "../../Utils/Headers/Constants.h"

#ifdef __WIN32
#define DIR_SEPARATOR '\\'
#elif __linux
#define DIR_SEPARATOR  '/'
#endif

namespace fs = std::filesystem;
struct DB_FILE {
    int version = 0;
    std::shared_ptr<std::fstream> meta_file;
    std::shared_ptr<std::fstream> data_file;

    DB_FILE(std::shared_ptr<std::fstream> m_file, std::shared_ptr<std::fstream> d_file, int ver);

    void close();
    int isOpen();
};
int CheckFileHashSum(const std::shared_ptr<DB_FILE>& file);
void WriteIntToFile(const std::shared_ptr<std::fstream>& file, int value);
int ReadIntoFromFile(const std::shared_ptr<std::fstream>& file);
double CalcHashSum(const std::shared_ptr<std::fstream>& file);
std::shared_ptr<DB_FILE> FindLastVersion(const std::string& table_name, int max_v);
int GetVersion(const std::string& file_name);

#endif  // SELSQL_ENGINEUTILS_H
