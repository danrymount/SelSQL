//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"
int MainEngine::CreateTable(Table* table) {
    return file_manager_->CreateFile(table);
}  // returns 0 if table is created and 1 if table exists

Table* MainEngine::ShowCreateTable(std::string table_name) {
    if (file_manager_->OpenFile(table_name)) {
        return nullptr;
    }
    return file_manager_->GetTableData(table_name);
}  // returns Table* if ok or nullptr if table doesnt exist

int MainEngine::DropTable(std::string table_name) {
    return file_manager_->DeleteTable(table_name);
}  // returns 0 if table is dropped and non-zero if table doesnt exist

MainEngine::MainEngine() { file_manager_ = new FileManager(); }
BigResponse MainEngine::Insert(BigRequest* bigRequest) {
    file_manager_->OpenFile(bigRequest->tableName);
    Table* t = file_manager_->GetTableData(bigRequest->tableName);

    cursor = new Cursor(t, file_manager_);

    cursor->Insert(bigRequest->dmlData.columns, bigRequest->dmlData.values);
    cursor->Commit();

    return BigResponse();
}
BigResponse MainEngine::Select(BigRequest* bigRequest) {
    file_manager_->OpenFile(bigRequest->tableName);
    Table* t = file_manager_->GetTableData(bigRequest->tableName);

    cursor = new Cursor(t, file_manager_);
    std::vector<std::pair<std::string, std::string>> record;
    do {
        record = cursor->Fetch();
        for (auto i : record) {
            std::cout << i.first << "    =    " << i.second << std::endl;
        }

    } while (!cursor->Next());

    return BigResponse();
}
BigResponse MainEngine::Delete(BigRequest* bigRequest) {
    file_manager_->OpenFile(bigRequest->tableName);
    Table* t = file_manager_->GetTableData(bigRequest->tableName);
    std::cout << t->record_amount << std ::endl;
    cursor = new Cursor(t, file_manager_);
    std::map<std::string, Condition> cond = bigRequest->dmlData.conditions;
    std::vector<std::pair<std::string, std::string>> record;
    int delete_count = 0;
    do {
        record = cursor->Fetch();
        if (record.empty()) {
            continue;
        }
        for (int i = 0; i < record.size(); ++i) {
            std::string field_name = record[i].first;
            if (cond.find(field_name) != cond.end()) {
                switch (cond[field_name].sign) {
                    case GREATEREQUALS:
                        if (record[i].second >= cond[field_name].value) {
                            cursor->Delete();
                            delete_count++;
                        }
                        break;
                    case GREATER:
                        if (record[i].second > cond[field_name].value) {
                            cursor->Delete();
                            delete_count++;
                        }
                        break;
                    case NOEQUALS:
                        if (record[i].second != cond[field_name].value) {
                            cursor->Delete();
                            delete_count++;
                        }
                        break;
                    case EQUALS: {
                        if (record[i].second == cond[field_name].value) {
                            cursor->Delete();
                            delete_count++;
                        }
                        break;
                    }
                    case LOWER:
                        if (record[i].second < cond[field_name].value) {
                            cursor->Delete();
                            delete_count++;
                        }
                        break;
                    case LOWEREQUALS:
                        if (record[i].second <= cond[field_name].value) {
                            cursor->Delete();
                            delete_count++;
                        }
                        break;
                }
            }
        }
    } while (!cursor->Next());
    cursor->table->record_amount -= delete_count;
    cursor->Commit();
    std::cout << cursor->table->record_amount << std ::endl;
    return BigResponse();
}
