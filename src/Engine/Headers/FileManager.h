
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
    std::map<std::string, DB_FILE> files_;
    std::fstream* temp;
    std::map<std::string, std::shared_ptr<Table>> table_data;

    void ReadTableMetaData(const std::string& table_name);
    void WriteTableMetaData(const std::shared_ptr<Table>& table);
    void WriteDataBlockToTemp(const std::string& table_name, DataBlock* data, int block_id);

   public:
    explicit FileManager();
    int OpenFile(const std::string& table_name);
    int CreateFile(const std::shared_ptr<Table>& table);
    void CloseAllFiles();
    int DeleteFile(const std::string& table_name);
    std::shared_ptr<Table> GetTable(const std::string& table_name);
    DataBlock* ReadDataBlock(const std::string& table_name, int block_id);
    int UpdateBlock(const std::shared_ptr<Table>& table, DataBlock* data, int block_id);
    int UpdateFile(std::string table_name);
};

#endif  // SELSQL_FILEMANAGER_H
