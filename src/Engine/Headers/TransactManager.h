
#ifndef SELSQL_TRANSACTMANAGER_H
#define SELSQL_TRANSACTMANAGER_H
#include "../../Utils/Headers/ErrorConstants.h"
#include "EngineUtils.h"

class TransactManager {
    std::map<std::string, std::map<Position, long>> in_use;
    std::vector<size_t> restricted;

   public:
    std::map<std::string, std::vector<std::pair<int, long>>> ignore;
    TransactManager();
    long GetTransactionSP();
    void ClearUsed(long transaction_sp);
    int SetUsed(const std::string& table_name, Position position, long transaction_sp);
    int IsSuccessful(long transaction_sp);
    void RestrictTransaction(long transaction_sp);
    void SetNewPos(const std::string& table_name, int pos, long tr);
    void Clear(const std::string& table_name, long tr);
};
#endif  // SELSQL_TRANSACTMANAGER_H
