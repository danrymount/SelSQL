
#ifndef SELSQL_TRANSACTMANAGER_H
#define SELSQL_TRANSACTMANAGER_H
#include "../../Utils/Headers/ErrorConstants.h"
#include "EngineUtils.h"

class TransactManager {
    std::map<std::string, std::map<Position, std::vector<std::pair<int64_t, int>>>> in_use;
    std::vector<size_t> restricted;

   public:
    int active_tr = 0;
    std::map<int, std::pair<int64_t, int64_t>> transaction_table;
    std::map<int64_t, std::vector<std::pair<std::string, int>>> block_usage_by_trans;
    std::map<std::string, std::vector<std::pair<int, int64_t>>> ignore;
    TransactManager();
    int64_t GetTransactionSP();
    void ClearUsed(int64_t transaction_id);
    int SetUsed(const std::string& table_name, Position position, int64_t transaction_id, int operation);
    int IsSuccessful(int64_t transaction_id);
    void RestrictTransaction(int64_t transaction_id);
    void SetNewPos(const std::string& table_name, int pos, int64_t transaction_id);
    void Clear(const std::string& table_name, int64_t transaction_id);
    std::vector<std::pair<int, int>> GetPositionsNeedCommit(std::string table_name, int block_id, int64_t tr_id);
    void EndTransaction(int64_t tr_id);
    void SetBlockUsage(int64_t tr_id, const std::string& table_name, int block_id);

    //    std::shared_ptr<DataBlock> GetDataBlock(std::string table_name, int block_id);
    //    void SetDataBlock(std::string table_name, int block_id, std::shared_ptr<DataBlock> data_block, long tr_p);
};
#endif  // SELSQL_TRANSACTMANAGER_H
