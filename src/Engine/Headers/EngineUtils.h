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
#define DIR_SEPARATOR '/'
#endif

typedef std::pair<char*, int> buffer_data;
typedef std::pair<int, int> Position;
typedef Constants C;
namespace fs = std::filesystem;

int GetFileSize(std::fstream* file);
buffer_data GetTableBuffer(Table* table);
buffer_data GetDataBlockBuffer(DataBlock* data_block);
std::shared_ptr<DataBlock> ReadDataBlockFromBuffer(char* data);
std::shared_ptr<Table> ReadTableFromBuffer(char* data);
void RestoreFromTemp(std::fstream* src, std::fstream* dist, int record_size);
std::string ConstructFileName(const std::string& table_name, size_t transaction_id);
std::string FindTempFile(const std::string& table_name, size_t transaction_id);

class FileNotOpened {};
#endif  // SELSQL_ENGINEUTILS_H
