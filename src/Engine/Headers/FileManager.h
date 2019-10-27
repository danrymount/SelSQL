
#ifndef SELSQL_FILEMANAGER_H
#define SELSQL_FILEMANAGER_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "../../Utils/Headers/CommonUtils.h"
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/DataBlock.h"
#include "../../Utils/Structures/Data/Table.h"

void WriteIntToFile(const std::shared_ptr<std::fstream>& file, int value);
int ReadIntoFromFile(const std::shared_ptr<std::fstream>& file);

class FileManager {
    std::map<std::string, std::shared_ptr<std::fstream>> files_;
    std::map<std::string, std::shared_ptr<Table>> table_data;

    void ReadTableMetaData(const std::string& table_name);
    void WriteTableMetaData(const std::shared_ptr<Table>& table);
    void WriteDataBlocks(const std::string& table_name, const std::vector<std::shared_ptr<DataBlock>>& data);

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
