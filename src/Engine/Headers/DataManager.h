//
// Created by quiks on 14.12.2019.
//

#ifndef SELSQL_DATAMANAGER_H
#define SELSQL_DATAMANAGER_H
#include "CachedBlocks.h"
#include "FileManager.h"
#include "Index.h"
class DataManager {
    std::map<std::pair<std::string, int>, std::pair<std::shared_ptr<DataBlock>, int>> cached_block;
    std::shared_ptr<FileManager> fm_;
    std::map<std::string, std::shared_ptr<Indexes>> indexes;
    CachedBlocks blocks;

   public:
    int i_o_count = 0;
    std::multimap<std::string, int, cmp> GetIndexes(const std::string& table_name);
    void CreateIndex(std::string table_name, Type type);
    void InsertIndex(std::string table_name, std::string value, int pos);
    DataManager(std::shared_ptr<FileManager> f) : fm_(f){};
    std::shared_ptr<DataBlock> GetDataBlock(const std::string& table_name, int block_id, bool with_alloc);
    //    SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block);
    void FreeDataBlock(const std::string& table_name, int block_id);
    void CommitDataBlock(std::string& table_name, int block_id, const std::vector<std::pair<int, int>>& positions,
                         int tr_id, int record_size);

    void ClearAll();
    void ClearCached(std::string table_name);
    void IncreaseBlockHeat(BlockPos pos);
};
#endif  // SELSQL_DATAMANAGER_H
