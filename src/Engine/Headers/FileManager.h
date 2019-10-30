
#ifndef SELSQL_FILEMANAGER_H
#define SELSQL_FILEMANAGER_H

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "../../Utils/Headers/CommonUtils.h"
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/DataBlock.h"
#include "../../Utils/Structures/Data/Table.h"

namespace fs = std::filesystem;
struct DB_FILE {
    int version = 0;
    std::shared_ptr<std::fstream> meta_file;
    std::shared_ptr<std::fstream> data_file;

    DB_FILE(std::shared_ptr<std::fstream> m_file, std::shared_ptr<std::fstream> d_file, int ver)
                                                                                                        : meta_file(std::move(m_file)),
                                                                                                          data_file(std::move(d_file)),
                                                                                                          version(ver){};

    void close() {
        meta_file->close();
        data_file->close();
    };
    int isOpen() { return meta_file->is_open() and data_file->is_open(); }
};
double CheckFileHashSum(const std::shared_ptr<DB_FILE>& file);
void WriteIntToFile(const std::shared_ptr<std::fstream>& file, int value);
int ReadIntoFromFile(const std::shared_ptr<std::fstream>& file);

int GetVersion(const std::string& file_name);

class FileManager {
    std::map<std::string, std::shared_ptr<DB_FILE>> files_;
    std::map<std::string, std::shared_ptr<Table>> table_data;

    void ReadTableMetaData(const std::string& table_name);
    void WriteTableMetaData(const std::shared_ptr<Table>& table);
    void WriteDataBlocks(const std::string& table_name, const std::vector<std::shared_ptr<DataBlock>>& data);
    std::shared_ptr<DB_FILE> FindLastVersion(const std::string& table_name);

   public:
    explicit FileManager() = default;
    int OpenFile(const std::string& table_name);
    int CreateFile(const std::shared_ptr<Table>& table);
    void CloseAllFiles();
    int DeleteFile(const std::string& table_name);
    std::shared_ptr<Table> GetTable(const std::string& table_name);
    std::vector<std::shared_ptr<DataBlock>> ReadDataBlocks(const std::string& table_name);
    int UpdateFile(const std::shared_ptr<Table>& table, const std::vector<std::shared_ptr<DataBlock>>& data);
};

#endif  // SELSQL_FILEMANAGER_H
