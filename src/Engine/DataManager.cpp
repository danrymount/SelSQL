//
// Created by quiks on 14.12.2019.
//
#include "Headers/DataManager.h"
#include <mutex>
#include "Headers/Cursor.h"

std::mutex data_mutex;

std::shared_ptr<DataBlock> DataManager::GetDataBlock(const std::string &table_name, int block_id, bool with_alloc) {
    std::lock_guard<std::mutex> g(data_mutex);
    auto position = std::make_pair(table_name, block_id);
    if (cached_block.find(position) == cached_block.end()) {
        std::shared_ptr<DataBlock> data_block;
        data_block = fm_->ReadDataBlock(table_name, block_id);
        if (data_block == nullptr and with_alloc) {
            data_block = std::make_shared<DataBlock>();
            char *new_data = new char[C::DATA_BLOCK_SIZE];
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

void DataManager::FreeDataBlock(const std::string &table_name, int block_id) {
    std::lock_guard<std::mutex> g(data_mutex);
    auto position = std::make_pair(table_name, block_id);
    if (cached_block.find(position) != cached_block.end() and cached_block[position].first != nullptr and
        cached_block[position].first->was_changed) {
        fm_->WriteDataBlock(table_name, cached_block[position].first, block_id);
    }
}

void DataManager::CommitDataBlock(std::string &table_name, int block_id,
                                  const std::vector<std::pair<int, int>> &positions, int tr_id, int record_size) {
    std::lock_guard<std::mutex> g(data_mutex);
    Cursor::MakeCommited(cached_block[std::make_pair(table_name, block_id)].first, tr_id, positions, record_size);
}

void DataManager::ClearAll() { cached_block.clear(); }
std::multimap<std::string, int> DataManager::GetIndexes(const std::string &table_name) {
    return indexes[table_name]->GetIndexes();
}
void DataManager::InsertIndex(std::string table_name, std::string value, int pos) {
    indexes[table_name]->Insert(value, pos);
}
void DataManager::CreateIndex(std::string table_name, Type type) {
    indexes[table_name] = std::make_shared<Indexes>(type);
}
