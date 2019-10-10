//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"
BigResponse MainEngine::CreateTable(BigRequest* request) {
    BigResponse bigResponse;
    int error = file_manager_->CreateFile(&request->ddlData.table);
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return bigResponse;

}  // returns 0 if table is created and 1 if table exists

BigResponse MainEngine::ShowCreateTable(BigRequest request) {
    BigResponse bigResponse;
    if (file_manager_->OpenFile(request.ddlData.table.name)) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    } else {
        bigResponse.ddlData.table = *file_manager_->GetTableData(request.ddlData.table.name);
    }

    return bigResponse;
}  // returns Table* if ok or nullptr if table doesnt exist

BigResponse MainEngine::DropTable(BigRequest* request) {
    BigResponse bigResponse;
    int error = file_manager_->DeleteTable(request->ddlData.table.name);
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    return bigResponse;
}  // returns 0 if table is dropped and non-zero if table doesnt exist

MainEngine::MainEngine() { file_manager_ = new FileManager(); }
BigResponse MainEngine::Insert(BigRequest* bigRequest) {
    int error = file_manager_->OpenFile(bigRequest->tableName);
    BigResponse bigResponse;
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return bigResponse;
    }
    Table* t = file_manager_->GetTableData(bigRequest->tableName);

    cursor = new Cursor(t, file_manager_);

    cursor->Insert(bigRequest->dmlData.columns, bigRequest->dmlData.values);
    cursor->Commit();

    return BigResponse();
}
BigResponse MainEngine::Select(BigRequest* bigRequest) {
    int error = file_manager_->OpenFile(bigRequest->tableName);
    BigResponse bigResponse;
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return bigResponse;
    }
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
    int error = file_manager_->OpenFile(bigRequest->tableName);
    BigResponse bigResponse;
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        return bigResponse;
    }
    Table* t = file_manager_->GetTableData(bigRequest->tableName);
    std::cout << t->record_amount << std ::endl;
    cursor = new Cursor(t, file_manager_);
    std::map<std::string, Condition> cond = bigRequest->dmlData.conditions;
    std::vector<std::pair<std::string, std::string>> record;
    int delete_count = 0;

    int no_such_cols_table = 1;
    for (auto i : t->fields) {
        if (cond.find(i.first) != cond.end()) {
            no_such_cols_table = 0;
            break;
        }
    }

    if (no_such_cols_table) {
        bigResponse.error = Error(ErrorConstants::ERR_NO_SUCH_FIELD);
        return bigResponse;
    }

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
