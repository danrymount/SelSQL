
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
#include "TransactManager.h"

typedef std::pair<std::shared_ptr<std::fstream>, std::shared_ptr<std::fstream>> files;

class FileManager {
//    std::map<std::string, std::shared_ptr<std::fstream>> meta_files_;
    std::shared_ptr<std::fstream> temp;
    std::map<std::string, std::shared_ptr<Table>> table_data;
    std::shared_ptr<TransactManager> transact_manager_;

public:
    void ReadTableMetaData(const std::string &table_name, const std::shared_ptr<std::fstream> &meta_file);

    void WriteTableMetaData(std::shared_ptr<std::fstream> meta_file, const std::shared_ptr<Table> &table);

    explicit FileManager(std::shared_ptr<TransactManager> manager);

    ~FileManager();

    files OpenFile(const std::string &table_name);

    int CreateFile(const std::shared_ptr<Table> &table);

    static int DeleteFile(const std::string &table_name);

    std::shared_ptr<Table> GetTable(const std::string &table_name);

    static std::shared_ptr<DataBlock> ReadDataBlock(const std::string &table_name, int block_id);

    //    static void WriteDataBlockToTemp(const std::string& table_name, std::shared_ptr<DataBlock> data, int block_id,
    //    const std::shared_ptr<std::fstream>& dist);
    static int WriteDataBlock(const std::string &table_name, std::shared_ptr<DataBlock> data, int block_id);

    int UpdateFile(const std::string &table_name, const std::shared_ptr<std::fstream> &src);

    static void Clear(size_t transact_id);
};

#endif  // SELSQL_FILEMANAGER_H
