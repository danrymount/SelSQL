//
// Created by quiks on 01.10.2019.
//

#ifndef SELSQL_MAINENGINE_H
#define SELSQL_MAINENGINE_H
#include "../../Utils/Structures/Data/Table.h"
#include "../Headers/APIEngine.h"
#include "../Headers/FileManager.h"
class MainEngine : public APIEngine {
   public:
    FileManager* file_manager_;

    explicit MainEngine();

    int CreateTable(Table* table) override;
    Table* ShowCreateTable(std::string table_name) override;
    int DropTable(std::string table_name) override;
};

#endif  // SELSQL_MAINENGINE_H
