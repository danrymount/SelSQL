
#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include <memory>
#include <utility>
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Record.h"
#include "FileManager.h"
#include "TransactManager.h"
class Cursor {
    std::shared_ptr<Table> table_;
    std::shared_ptr<FileManager> file_manager_;
    std::shared_ptr<TransactManager> transact_manager_;
    std::shared_ptr<DataBlock> data_block_;
    std::vector<std::pair<std::string, std::string>> values_;
    std::shared_ptr<std::fstream> data_file_;

    int changed = 0;
    int read_block_id = 0;
    int write_block_id = 0;
    int current_pos = 0;
    int readed_data = 0;
    int current_session_deleted_ = 0;

    static void SaveFieldData(std::string val, Type type, unsigned char* dist, int start_pos);
    static void GetFieldData(std::string* dist, Type type, unsigned char* src, int start_pos);
    void Allocate();

   public:
    Cursor(const std::shared_ptr<Table>& table, const std::shared_ptr<FileManager>& file_manager,
           const std::shared_ptr<TransactManager>& transact_manager, std::shared_ptr<std::fstream> data_file);
    Cursor();
    int Insert(std::vector<std::string> cols, std::vector<std::string> new_data, int transact_id = 0);
    int UpdateDataBlock();
    int NextRecord();
    int NextDataBlock();
    int Delete(int transact_id = 0);
    int Update(std::vector<std::string> cols, std::vector<std::string> new_data, int transact_id = 0);
    int Reset();

    void Commit();
    std::vector<std::pair<std::string, std::string>> Fetch();
    ~Cursor();
};
#endif  // SELSQL_CURSOR_H
