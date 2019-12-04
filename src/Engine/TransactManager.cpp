
#include "Headers/TransactManager.h"

TransactManager::TransactManager() = default;
long TransactManager::GetTransactionSP() {
    std::chrono::time_point tp = std::chrono::system_clock::now();
    long value = 0;
    std::memcpy(&value, &tp, sizeof(tp));
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
    return ErrorConstants::ERR_TRANSACT_CONFLICT;
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
