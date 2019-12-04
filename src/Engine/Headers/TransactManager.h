
#ifndef SELSQL_TRANSACTMANAGER_H
#define SELSQL_TRANSACTMANAGER_H
#include "../../Utils/Headers/ErrorConstants.h"
#include "EngineUtils.h"

class TransactManager {
    std::map<std::string, std::map<Position, long>> in_use;
    std::vector<size_t> restricted;

   public:
    TransactManager();
    long GetTransactionSP();
    void ClearUsed(long transaction_sp);
    int SetUsed(const std::string& table_name, Position position, long transaction_sp);
    int IsSuccessful(long transaction_sp);
    void RestrictTransaction(long transaction_sp);
};
#endif  // SELSQL_TRANSACTMANAGER_H
