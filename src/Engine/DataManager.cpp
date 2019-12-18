//
// Created by quiks on 14.12.2019.
//
#include "Headers/DataManager.h"
#include "Headers/Cursor.h"
std::shared_ptr<DataBlock> DataManager::GetDataBlock(const std::string& table_name, int block_id, bool with_alloc) {
    auto position = std::make_pair(table_name, block_id);
    if (cached_block.find(position) == cached_block.end()) {
        std::shared_ptr<DataBlock> data_block;
        data_block = FileManager::ReadDataBlock(table_name, block_id);
        if (data_block == nullptr and with_alloc) {
            data_block = std::make_shared<DataBlock>();
            char* new_data = new char[C::DATA_BLOCK_SIZE];
            std::memset(new_data, '0', C::DATA_BLOCK_SIZE);
            data_block->setData(new_data);
        }
        if (data_block == nullptr) {
            return nullptr;
        }
        cached_block[position] = std::pair(data_block, 1);
        return data_block;
    }

    cached_block[position].second++;
    return cached_block[position].first;
}
void DataManager::FreeDataBlock(const std::string& table_name, int block_id) {
    auto position = std::make_pair(table_name, block_id);
    cached_block[std::make_pair(table_name, block_id)].second--;
    if (cached_block[position].second == 0) {
        FileManager::WriteDataBlock(table_name, cached_block[position].first, block_id);
        cached_block.erase(position);
    }
}
void DataManager::CommitDataBlock(std::string& table_name, int block_id,
                                  const std::vector<std::pair<int, int>>& positions, int tr_id, int record_size) {
    Cursor::MakeCommited(cached_block[std::make_pair(table_name, block_id)].first, tr_id, positions, record_size);
}
