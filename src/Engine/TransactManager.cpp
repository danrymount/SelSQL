
#include "Headers/TransactManager.h"

// TODO PROBLEMS IN MANAGER
TransactManager::TransactManager() {
    std::fstream file("TR_TABLE", std::ios::in | std::ios::out);
    if (!file.is_open()) {
        return;
    }

    while (file.good()) {
        int tr_id = 0;
        int64_t time_s = 0;
        int64_t time_e = 0;
        file >> tr_id >> time_s >> time_e;
        if (tr_id == time_s and time_s == 0) {
            continue;
        }
        transaction_table[tr_id] = std::make_pair(time_s, time_e);
    }
};
int64_t TransactManager::GetTransactionSP() {
    std::fstream temp("CLOCK_TR_ID", std::ios::in | std::ios::out);
    if (!temp.is_open()) {
        temp = std::fstream("CLOCK_TR_ID", std::ios::in | std::ios::out | std::ios::trunc);
    }
    int64_t value = 0;
    temp >> value;
    std::cout << value << std::endl;
    temp.clear();
    temp.seekp(std::ios::beg);
    temp << (value + 1);
    int64_t start_time = 0;
    std::chrono::time_point s_time = std::chrono::system_clock::now();
    std::memcpy(&start_time, &s_time, sizeof(s_time));
    transaction_table[value] = std::make_pair(start_time, 0);
    return value;
}
void TransactManager::ClearUsed(int64_t transaction_id) {
    for (auto& table : in_use) {
        for (auto position = table.second.begin(); position != table.second.end();) {
            if (position->second.first == transaction_id) {
                position = table.second.erase(position);
            } else {
                ++position;
            }
        }
    }
}
int TransactManager::SetUsed(const std::string& table_name, Position position, int64_t transaction_id, int operation) {
    if (in_use.find(table_name) == in_use.end()) {
        std::map<Position, std::pair<long, int>> new_;
        new_[position] = std::make_pair(transaction_id, operation);
        in_use[table_name] = new_;
        return 0;
    }
    if (in_use[table_name].find(position) == in_use[table_name].end()) {
        in_use[table_name][position] = std::make_pair(transaction_id, operation);
        return 0;
    }
    if (in_use[table_name][position].first == transaction_id) {
        return 0;
    }
    RestrictTransaction(transaction_id);
    return 0;
}
void TransactManager::RestrictTransaction(int64_t transaction_id) { restricted.emplace_back(transaction_id); }
int TransactManager::IsSuccessful(long transaction_id) {
    for (auto i : restricted) {
        if (i == transaction_id) {
            return 0;
        }
    }
    return 1;
}
void TransactManager::SetNewPos(const std::string& table_name, int pos, int64_t transaction_id) {
    ignore[table_name].emplace_back(std::make_pair(pos, transaction_id));
}
void TransactManager::Clear(const std::string& table_name, int64_t transaction_id) {
    for (auto position = ignore[table_name].begin(); position != ignore[table_name].end();) {
        if (position->second == transaction_id) {
            position = ignore[table_name].erase(position);
        } else {
            ++position;
        }
    }
}
std::vector<std::pair<int, int>> TransactManager::GetPositionsNeedCommit(std::string table_name, int block_id,
                                                                         int64_t tr_id) {
    std::vector<std::pair<int, int>> positions;
    for (auto table : in_use) {
        for (auto pos : table.second) {
            if (pos.second.first == tr_id and pos.first.first == block_id) {
                positions.emplace_back(std::make_pair(pos.first.second, pos.second.second));
            }
        }
    }
    return positions;
}
void TransactManager::UpdateTransactionTable() {
    std::fstream file("TR_TABLE", std::ios::in | std::ios::out | std::ios::trunc);
    for (auto tr : transaction_table) {
        file << tr.first << " " << tr.second.first << " " << tr.second.second << std::endl;
    }
}
void TransactManager::EndTransaction(int64_t tr_id) {
    int64_t value = 0;
    int64_t end_time = 0;
    std::chrono::time_point e_time = std::chrono::system_clock::now();
    std::memcpy(&end_time, &e_time, sizeof(e_time));
    transaction_table[tr_id].second = end_time;
}

// std::shared_ptr<DataBlock> TransactManager::GetDataBlock(std::string table_name, int block_id) {
//    return in_use_block[table_name][block_id];
//}
// void TransactManager::SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block,
//                                   long tr_p) {
//    in_use_block[table_name][block_id] = data_block;
//    trans_usage[tr_p].emplace_back(std::make_pair(table_name, block_id));
//}
