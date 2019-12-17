
#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include <functional>
#include <memory>
#include <utility>
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Record.h"
#include "DataManager.h"
#include "FileManager.h"
#include "TransactManager.h"

class Cursor {
    std::shared_ptr<Table> table_;

    std::shared_ptr<DataManager> data_manager_;
    std::shared_ptr<TransactManager> transact_manager_;
    std::shared_ptr<DataBlock> data_block_;
    std::vector<std::pair<std::string, std::string>> values_;
    std::shared_ptr<std::fstream> data_file_;

    int block_id_ = 0;
    int pos_in_block_ = 0;
    int64_t current_tr_id_ = 0;
    static void SaveFieldData(std::string val, Type type, char* dist, int start_pos);
    static void GetFieldData(std::string* dist, Type type, char* src, int start_pos);

    int EmplaceBack(Record* record);
    int NextDataBlock();

   public:
    Cursor(const std::shared_ptr<Table>& table, const std::shared_ptr<DataManager>& data_manager,
           const std::shared_ptr<TransactManager>& transact_manager, std::shared_ptr<std::fstream> data_file,
           int64_t tr_id);
    Cursor();
    int Insert(const std::vector<std::string>& cols, const std::vector<std::string>& new_data);
    int NextRecord();
    int Delete();
    int Update(std::vector<std::string> cols, std::vector<std::string> new_data);
    int Reset();
    std::vector<std::pair<std::string, std::string>> Fetch();
    static void MakeCommited(const std::shared_ptr<DataBlock>& block, int64_t tr_id,
                             const std::vector<int>& need_commit, int record_size);
};
#endif  // SELSQL_CURSOR_H
