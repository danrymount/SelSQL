
#ifndef SELSQL_TRANSACTMANAGER_H
#define SELSQL_TRANSACTMANAGER_H
#include "EngineUtils.h"

class TransactManager {
    size_t new_transaction_id = 0;
    std::map<std::string, std::map<Position, int>> in_use;

   public:
    TransactManager();
    size_t GetTransactionId();
    void ClearUsed(size_t transaction_id);
    int SetUsed(Position);
};
#endif  // SELSQL_TRANSACTMANAGER_H
