//
// Created by quiks on 25.12.2019.
//
#include "Headers/CachedBlocks.h"
bool cmp(std::tuple<std::shared_ptr<DataBlock>, BlockPos, int> left,
         std::tuple<std::shared_ptr<DataBlock>, BlockPos, int> right) {
    return std::get<2>(left) >= std::get<2>(right);
}
void CachedBlocks::InsertBlock(const std::shared_ptr<DataBlock>& data, const BlockPos& pos) {
    if (blocks_.size() < MAX_SIZE) {
        blocks_.insert(blocks_.begin(), std::make_tuple(data, pos, GetAvgHeat()));
        return;
    }
    // TODO Restrict erase block that is in use - SIGSEG POSSIBLE
    blocks_.erase(blocks_.end() - 1);
    blocks_.insert(blocks_.begin(), std::make_tuple(data, pos, GetAvgHeat()));
}
std::shared_ptr<DataBlock> CachedBlocks::GetBlock(const BlockPos& pos) {
    std::cerr << blocks_.size() << std::endl;
    for (auto i : blocks_) {
        if (std::get<1>(i) == pos) {
            ++std::get<2>(i);
            return std::get<0>(i);
        }
    }
    return nullptr;
}
void CachedBlocks::ClearTable(const std::string& table_name) {
    for (auto i = blocks_.begin(); i != blocks_.end();) {
        if (std::get<1>(*i).first == table_name) {
            i = blocks_.erase(i);
        } else {
            ++i;
        }
    }
}
void CachedBlocks::HeatIncrease(BlockPos pos) {
    for (auto& i : blocks_) {
        if (std::get<1>(i) == pos) {
            ++std::get<2>(i);
        }
    }
    std::sort(blocks_.begin(), blocks_.end(), cmp);
}
int CachedBlocks::GetAvgHeat() {
    int sum = 0;
    int res = 1;
    for (auto i : blocks_) {
        sum += std::get<2>(i);
    }
    if (!blocks_.empty()) {
        res = sum / blocks_.size();
    }
    return res;
}
