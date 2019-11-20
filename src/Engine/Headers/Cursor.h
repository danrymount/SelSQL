
#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include <memory>
#include <utility>
#include "../../Utils/Headers/Constants.h"
#include "FileManager.h"
class Cursor {
    std::shared_ptr<Table> table_;
    std::shared_ptr<FileManager> file_manager_;
    std::shared_ptr<DataBlock> data_block_;
    std::vector<std::pair<std::string, std::string>> values_;


    int read_block_id = 0;
    int write_block_id = 0;
    int current_pos = 0;
    int readed_data = 0;
    int current_session_deleted_ = 0;

    static void SaveFieldData(std::string val, Type type, unsigned char* dist, int start_pos);
    static void GetFieldData(std::string* dist, Type type, unsigned char* src, int start_pos);
    void Allocate();
   public:
    Cursor(const std::shared_ptr<Table>& table, const std::shared_ptr<FileManager>& file_manager);
    Cursor();
    int Insert(std::vector<std::string> cols, std::vector<std::string> new_data);
    int UpdateDataBlock();
    int NextRecord();
    int NextDataBlock();
    int Delete();
    int Update(std::vector<std::string> cols, std::vector<std::string> new_data);
    int Reset();
    std::vector<std::pair<std::string, std::string>> Fetch();
    ~Cursor();
};
#endif  // SELSQL_CURSOR_H
