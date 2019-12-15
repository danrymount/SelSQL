
#ifndef SELSQL_TRANSACTMANAGER_H
#define SELSQL_TRANSACTMANAGER_H
#include "../../Utils/Headers/ErrorConstants.h"
#include "EngineUtils.h"

class TransactManager {
    std::map<std::string, std::map<Position, int64_t>> in_use;
    std::vector<size_t> restricted;

   public:
    std::map<int64_t, std::vector<std::pair<std::string, int>>> trans_usage;
    std::map<std::string, std::vector<std::pair<int, int64_t>>> ignore;
    TransactManager();
    static int64_t GetTransactionSP();
    void ClearUsed(int64_t transaction_id);
    int SetUsed(const std::string& table_name, Position position, int64_t transaction_id);
    int IsSuccessful(int64_t transaction_id);
    void RestrictTransaction(int64_t transaction_id);
    void SetNewPos(const std::string& table_name, int pos, int64_t transaction_id);
    void Clear(const std::string& table_name, int64_t transaction_id);
    //    std::shared_ptr<DataBlock> GetDataBlock(std::string table_name, int block_id);
    //    void SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block, long tr_p);
};
#endif  // SELSQL_TRANSACTMANAGER_H
