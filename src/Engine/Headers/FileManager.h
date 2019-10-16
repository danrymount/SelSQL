
#ifndef SELSQL_FILEMANAGER_H
#define SELSQL_FILEMANAGER_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "../../Utils/Headers/CommonUtils.h"
#include "../../Utils/Headers/Constants.h"
#include "../../Utils/Structures/Data/Table.h"

void write_int(std::fstream* file, int value);
int read_int(std::fstream* file);

class FileManager {
    std::map<std::string, std::fstream*> files_;
    std::map<std::string, Table> table_data;

    void ReadTableMetaData(std::string table_name);
    void WriteTableMetaData(const std::shared_ptr<Table>& table);
    void WriteDataBlock(const std::shared_ptr<Table>& table);
    void WriteData(const std::shared_ptr<Table>& table, unsigned char* src);
    void ReadDataBlock(std::string table_name);

   public:
    int OpenFile(std::string table_name);
    int CreateFile(const std::shared_ptr<Table>& table);
    Table* GetTableData(std::string table_name);
    int DeleteTable(std::string table_name);
    unsigned char* GetData(std::string table_name);
    explicit FileManager() = default;
    int UpdateFile(const std::shared_ptr<Table>& table, unsigned char* src);
};

#endif  // SELSQL_FILEMANAGER_H
