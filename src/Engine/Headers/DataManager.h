//
// Created by quiks on 14.12.2019.
//

#ifndef SELSQL_DATAMANAGER_H
#define SELSQL_DATAMANAGER_H
#include "FileManager.h"

class DataManager {
    std::map<std::pair<std::string, int>, std::pair<std::shared_ptr<DataBlock>, int>> cached_block;

   public:
    std::shared_ptr<DataBlock> GetDataBlock(const std::string& table_name, int block_id, bool with_alloc);
    //    SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block);
    void FreeDataBlock(const std::string& table_name, int block_id);
};
#endif  // SELSQL_DATAMANAGER_H
