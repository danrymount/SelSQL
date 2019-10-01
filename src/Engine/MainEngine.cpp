//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"
int MainEngine::CreateTable(Table* table) { return file_manager_->CreateFile(table); }

Table* MainEngine::ShowCreateTable(std::string table_name) {
    if (file_manager_->OpenFile(table_name)) {
        return nullptr;
    }
    return file_manager_->GetTableData(table_name);
}

int MainEngine::DropTable(std::string table_name) { return 0; }

MainEngine::MainEngine() { file_manager_ = new FileManager(); }
