
#include "Headers/TransactManager.h"

TransactManager::TransactManager() = default;
size_t TransactManager::GetTransactionId() { return new_transaction_id++; }
void TransactManager::ClearUsed(size_t transaction_id) {
    for (auto table : in_use) {
        for (auto position = table.second.begin(); position != table.second.end();) {
            if (position->second == transaction_id) {
                table.second.erase(position);
            } else {
                ++position;
            }
        }
    }
}
int TransactManager::SetUsed(const std::string& table_name, Position position, size_t transaction_id) {
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

    return ErrorConstants::ERR_TRANSACT_CONFLICT;
}
