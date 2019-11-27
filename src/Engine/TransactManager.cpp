
#include "Headers/TransactManager.h"

TransactManager::TransactManager() = default;
size_t TransactManager::GetTransactionId() { return new_transaction_id++; }
void TransactManager::ClearUsed(size_t transaction_id) {
    //    std::cerr << "ClearUsed " << transaction_id << std::endl;
    for (auto& table : in_use) {
        for (auto position = table.second.begin(); position != table.second.end();) {
            if (position->second == transaction_id) {
                position = table.second.erase(position);
            } else {
                ++position;
            }
        }
    }
}
int TransactManager::SetUsed(const std::string& table_name, Position position, size_t transaction_id) {
    //    std::cerr << "SetUsed " << transaction_id << std::endl;
    if (in_use.find(table_name) == in_use.end()) {
        std::map<Position, size_t> new_;
        new_[position] = transaction_id;
        in_use[table_name] = new_;
        return 0;
    }
    if (in_use[table_name].find(position) == in_use[table_name].end()) {
        in_use[table_name][position] = transaction_id;
        return 0;
    }
    if (in_use[table_name][position] == transaction_id) {
        return 0;
    }
    RestrictTransaction(transaction_id);
    return ErrorConstants::ERR_TRANSACT_CONFLICT;
}
void TransactManager::RestrictTransaction(size_t transaction_id) { restricted.emplace_back(transaction_id); }
int TransactManager::IsSuccessful(size_t transaction_id) {
    for (auto i : restricted) {
        if (i == transaction_id) {
            return 0;
        }
    }
    return 1;
}
