
#ifndef SELSQL_FILEMANAGER_H
#define SELSQL_FILEMANAGER_H

#include <cstdio>
#include <filesystem>
#include <fstream>

#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "../../Utils/Headers/CommonUtils.h"
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/DataBlock.h"
#include "../../Utils/Structures/Data/Table.h"
#include "EngineUtils.h"

class FileManager {
    std::map<std::string, std::shared_ptr<DB_FILE>> files_;
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
