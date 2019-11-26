
#ifndef SELSQL_TRANSACTMANAGER_H
#define SELSQL_TRANSACTMANAGER_H
#include "../../Utils/Headers/ErrorConstants.h"
#include "EngineUtils.h"

class TransactManager {
    size_t new_transaction_id = 0;
    std::map<std::string, std::map<Position, size_t>> in_use;

   public:
    TransactManager();
    size_t GetTransactionId();
    void ClearUsed(size_t transaction_id);
    int SetUsed(const std::string& table_name, Position position, size_t transaction_id);
};
#endif  // SELSQL_TRANSACTMANAGER_H
