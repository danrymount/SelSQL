
#include "Headers/TransactManager.h"

// TODO PROBLEMS IN MANAGER
TransactManager::TransactManager() = default;
long TransactManager::GetTransactionSP() {
    std::fstream temp("CLOCK_TR_ID", std::ios::in | std::ios::out);
    if (!temp.is_open()) {
        temp = std::fstream("CLOCK_TR_ID", std::ios::in | std::ios::out | std::ios::trunc);
    }
    long value = 0;
    temp >> value;
    std::cout << value << std::endl;
    temp.clear();
    temp.seekp(std::ios::beg);
    temp << (value + 1);
    return value;
}
void TransactManager::ClearUsed(long transaction_sp) {
    //    std::cerr << "ClearUsed " << transaction_sp << std::endl;
    for (auto& table : in_use) {
        for (auto position = table.second.begin(); position != table.second.end();) {
            if (position->second == transaction_sp) {
                position = table.second.erase(position);
            } else {
                ++position;
            }
        }
    }
}
int TransactManager::SetUsed(const std::string& table_name, Position position, long transaction_sp) {
    //    std::cerr << "SetUsed " << transaction_sp << std::endl;
    if (in_use.find(table_name) == in_use.end()) {
        std::map<Position, long> new_;
        new_[position] = transaction_sp;
        in_use[table_name] = new_;
        return 0;
    }
    if (in_use[table_name].find(position) == in_use[table_name].end()) {
        in_use[table_name][position] = transaction_sp;
        return 0;
    }
    if (in_use[table_name][position] == transaction_sp) {
        return 0;
    }
    RestrictTransaction(transaction_sp);
    return 0;
}
void TransactManager::RestrictTransaction(long transaction_sp) { restricted.emplace_back(transaction_sp); }
int TransactManager::IsSuccessful(long transaction_sp) {
    for (auto i : restricted) {
        if (i == transaction_sp) {
            return 0;
        }
    }
    return 1;
}
void TransactManager::SetNewPos(const std::string& table_name, int pos, long tr) {
    ignore[table_name].emplace_back(std::make_pair(pos, tr));
}
void TransactManager::Clear(const std::string& table_name, long tr) {
    for (auto position = ignore[table_name].begin(); position != ignore[table_name].end();) {
        if (position->second == tr) {
            position = ignore[table_name].erase(position);
        } else {
            ++position;
        }
    }
}
// std::shared_ptr<DataBlock> TransactManager::GetDataBlock(std::string table_name, int block_id) {
//    return in_use_block[table_name][block_id];
//}
// void TransactManager::SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block,
//                                   long tr_p) {
//    in_use_block[table_name][block_id] = data_block;
//    trans_usage[tr_p].emplace_back(std::make_pair(table_name, block_id));
//}
