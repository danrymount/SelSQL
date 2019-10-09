
#ifndef SELSQL_FILEMANAGER_H
#define SELSQL_FILEMANAGER_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "../../Utils/Headers/CommonUtils.h"
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Table.h"

void write_int(std::fstream* file, int value);
int read_int(std::fstream* file);

class FileManager {
    std::map<std::string, std::fstream*> files_;
    std::map<std::string, Table> table_data;

    void ReadMetaData(std::string table_name);
    void WriteMetaData(Table* table);
    void WriteData(Table* table, unsigned char* src);

   public:
    int OpenFile(std::string table_name);
    int CreateFile(Table* table);
    Table* GetTableData(std::string table_name);
    int DeleteTable(std::string table_name);
    unsigned char* GetData(std::string table_name);
    explicit FileManager() = default;
    int UpdateFile(Table* table, unsigned char* src);
};

#endif  // SELSQL_FILEMANAGER_H
