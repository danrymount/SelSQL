//
// Created by quiks on 14.12.2019.
//
#include "Headers/DataManager.h"
#include <mutex>
#include "Headers/Cursor.h"

std::mutex data_mutex;

std::shared_ptr<DataBlock> DataManager::GetDataBlock(const std::string &table_name, int block_id, bool with_alloc) {
    if (with_alloc) {
        i_o_count++;
    }

    auto position = std::make_pair(table_name, block_id);
    auto block = blocks.GetBlock(position);
    if (block == nullptr) {
        block = fm_->ReadDataBlock(position.first, position.second);
        if (block == nullptr and with_alloc) {
            block = std::make_shared<DataBlock>();
            char *new_data = new char[C::DATA_BLOCK_SIZE];
            std::memset(new_data, '0', C::DATA_BLOCK_SIZE);
            block->setData(new_data);
            blocks.InsertBlock(block, position);
            return block;
        }
        if (block == nullptr) {
            return nullptr;
        }
    }
    return block;
}
void DataManager::CommitDataBlock(std::string &table_name, int block_id,
                                  const std::vector<std::pair<int, int>> &positions, int tr_id, int record_size) {
    Cursor::MakeCommited(blocks.GetBlock(BlockPos(table_name, block_id)), tr_id, positions, record_size);
};
void DataManager::FreeDataBlock(const std::string &table_name, int block_id) {
    auto block = blocks.GetBlock(BlockPos(table_name, block_id));
    if (block != nullptr and block->was_changed) {
        fm_->WriteDataBlock(table_name, block, block_id);
    }
};
std::multimap<std::string, int, cmp> DataManager::GetIndexes(const std::string &table_name) {
    return indexes[table_name]->GetIndexes();
}
void DataManager::InsertIndex(std::string table_name, std::string value, int pos) {
    indexes[table_name]->Insert(value, pos);
}
void DataManager::CreateIndex(std::string table_name, Type type) {
    indexes[table_name] = std::make_shared<Indexes>(type);
}
void DataManager::ClearCached(std::string table_name) { blocks.ClearTable(table_name); }
void DataManager::IncreaseBlockHeat(BlockPos pos) { blocks.HeatIncrease(pos); }
DataManager::DataManager(std::shared_ptr<FileManager> f) : fm_(f) {}
