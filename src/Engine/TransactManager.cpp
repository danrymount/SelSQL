
#include "Headers/TransactManager.h"
#include <mutex>

std::mutex mutex;

TransactManager::TransactManager() {
    std::fstream file("TR_TABLE", std::ios::in | std::ios::out);
    if (!file.is_open()) {
        std::fstream file("TR_TABLE", std::ios::in | std::ios::out | std::ios::trunc);
        file << 0 << " " << 0 << " " << INT64_MAX << std::endl;
        transaction_table[0] = std::make_pair(0, INT64_MAX);
        return;
    }

    while (file.good()) {
        int tr_id = 0;
        int64_t time_s = 0;
        int64_t time_e = 0;
        file >> tr_id >> time_s >> time_e;
        if (tr_id == time_s and time_s == 0 and time_e != INT64_MAX) {
            transaction_table[0] = std::make_pair(0, INT64_MAX);
            continue;
        }
        transaction_table[tr_id] = std::make_pair(time_s, time_e);
    }
};
int64_t TransactManager::GetTransactionSP() {
    std::lock_guard l(mutex);
    active_tr++;

    std::fstream temp("CLOCK_TR_ID", std::ios::in | std::ios::out);
    if (!temp.is_open()) {
        temp = std::fstream("CLOCK_TR_ID", std::ios::in | std::ios::out | std::ios::trunc);
    }
    int64_t value = 1;
    temp >> value;
    temp.clear();
    temp.seekp(std::ios::beg);
    temp << (value + 1);
    int64_t start_time = 0;
    std::chrono::time_point s_time = std::chrono::system_clock::now();
    std::memcpy(&start_time, &s_time, sizeof(s_time));
    transaction_table[value] = std::make_pair(start_time, INT64_MAX);
    return value;
}
void TransactManager::ClearUsed(int64_t transaction_id) {
    for (auto& table : in_use) {
        for (auto position = table.second.begin(); position != table.second.end();) {
            int flag = 0;
            for (auto i : position->second) {
                if (i.first == transaction_id) {
                    flag = 1;
                    position = table.second.erase(position);
                }
                break;
            }
            if (!flag) {
                ++position;
            }
        }
    }
}
int TransactManager::SetUsed(const std::string& table_name, Position position, int64_t transaction_id, int operation) {
    if (in_use.find(table_name) == in_use.end()) {
        std::map<Position, std::vector<std::pair<int64_t, int>>> new_;
        new_[position].emplace_back(std::make_pair(transaction_id, operation));
        in_use[table_name] = new_;
        return 0;
    }
    if (in_use[table_name].find(position) == in_use[table_name].end()) {
        in_use[table_name][position].emplace_back(std::make_pair(transaction_id, operation));
        return 0;
    }
    for (auto i : in_use[table_name][position]) {
        if (i.first == transaction_id) {
            in_use[table_name][position].emplace_back(std::make_pair(transaction_id, operation));
            return 0;
        }
    }

    RestrictTransaction(transaction_id);
    return ErrorConstants::ERR_TRANSACT_CONFLICT;
}

void TransactManager::RestrictTransaction(int64_t transaction_id) { restricted.emplace_back(transaction_id); }

int TransactManager::IsSuccessful(int64_t transaction_id) {
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
            for (auto p : pos.second) {
                if (p.first == tr_id and pos.first.first == block_id) {
                    positions.emplace_back(std::make_pair(pos.first.second, p.second));
                }
            }
        }
    }
    return positions;
}

void TransactManager::EndTransaction(int64_t tr_id) {
    --active_tr;
    int64_t value = 0;
    int64_t end_time = 0;
    std::chrono::time_point e_time = std::chrono::system_clock::now();
    std::memcpy(&end_time, &e_time, sizeof(e_time));
    transaction_table[tr_id].second = end_time;
    std::fstream file("TR_TABLE", std::ios::in | std::ios::out);
    file.seekp(0, std::ios::end);
    file << tr_id << " " << transaction_table[tr_id].first << " " << transaction_table[tr_id].second << std::endl;
}

// std::shared_ptr<DataBlock> TransactManager::GetDataBlock(std::string table_name, int block_id) {
//    return in_use_block[table_name][block_id];
//}
// void TransactManager::SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block,
//                                   long tr_p) {
//    in_use_block[table_name][block_id] = data_block;
//    trans_usage[tr_p].emplace_back(std::make_pair(table_name, block_id));
//}
