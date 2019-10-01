
#ifndef SELSQL_FILEMANAGER_H
#define SELSQL_FILEMANAGER_H

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "../../Utils/Headers/CommonUtils.h"
#include "../../Utils/Headers/Constants.h"

class FileManager {
    std::map<std::string, std::fstream*> files_;
    std::map<std::string, Table> table_data;

    void ReadMetaData(std::string table_name);
    void WriteMetaData(Table* table);

   public:
    int OpenFile(std::string table_name);
    int CreateFile(Table* table);
    Table* GetTableData(std::string table_name);
    explicit FileManager() = default;
};

#endif  // SELSQL_FILEMANAGER_H
