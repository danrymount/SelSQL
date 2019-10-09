//
// Created by quiks on 07.10.2019.
//

#ifndef SELSQL_CURSOR_H
#define SELSQL_CURSOR_H
#include "../../Utils/Headers/Constants.h"
#include "FileManager.h"
class Cursor {
    Table* table;
    FileManager* fileManager;
    char* data;

    std::vector<std::pair<std::string, std::string>> vals;

    void SaveFieldData(std::string val, Type type, char* dist, int start_pos);
    void GetFieldData(std::string* dist, Type type, char* src, int start_pos);

   public:
    int pos = 0;
    Cursor(Table* t, FileManager* fm) {
        table = t;
        fileManager = fm;
        data = new char[Constants::DATA_PAGE_SIZE];
        data = fileManager->GetData(table->name);
        for (const auto& i : table->fields) {
            vals.emplace_back(std::make_pair(i.first, ""));
        }
    };
    int Insert(std::vector<std::string> cols, std::vector<std::string> new_data);
    int Commit();
    int Next();
    std::vector<std::pair<std::string, std::string>> Fetch();
};
#endif  // SELSQL_CURSOR_H
