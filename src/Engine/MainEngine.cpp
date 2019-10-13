//
// Created by quiks on 01.10.2019.
//

#include "Headers/MainEngine.h"

BigResponse MainEngine::CreateTable(BigRequest* request) {
    BigResponse bigResponse;
    int error = file_manager_->CreateFile(std::make_shared<Table>(request->ddlData.table));
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_EXISTS);
    }
    return bigResponse;
}

BigResponse MainEngine::ShowCreateTable(BigRequest request) {
    BigResponse bigResponse;
    if (file_manager_->OpenFile(request.ddlData.table.name)) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    } else {
        bigResponse.ddlData.table = *file_manager_->GetTableData(request.ddlData.table.name);
    }

    return bigResponse;
}

BigResponse MainEngine::DropTable(BigRequest* request) {
    BigResponse bigResponse;
    int error = file_manager_->DeleteTable(request->ddlData.table.name);
    if (error) {
        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    }

    return bigResponse;
}  // returns 0 if table is dropped and non-zero if table doesnt exist

MainEngine::MainEngine() { file_manager_ = std::make_shared<FileManager>(); }
// BigResponse MainEngine::Insert(BigRequest* bigRequest) {
//    int error = file_manager_->OpenFile(bigRequest->tableName);
//    BigResponse bigResponse;
//    if (error) {
//        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
//        return bigResponse;
//    }
//    Table* t = file_manager_->GetTableData(bigRequest->tableName);
//
//    cursor = new Cursor(t, file_manager_);
//
//    cursor->Insert(bigRequest->dmlData.columns, bigRequest->dmlData.values);
//    cursor->Commit();
//
//    return BigResponse();
//}
BigResponse MainEngine::Select(BigRequest* bigRequest) {
    //    int error = file_manager_->OpenFile(bigRequest->tableName);
    //    BigResponse bigResponse;
    //    if (error) {
    //        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    //        return bigResponse;
    //    }
    //    Table* t = file_manager_->GetTableData(bigRequest->tableName);
    //
    //    cursor = new Cursor(t, file_manager_);
    //    std::vector<std::pair<std::string, std::string>> record;
    //    if (t->record_amount == 0) {
    //        return BigResponse();
    //    }
    //    do {
    //        record = cursor->Fetch();
    //        for (auto i : record) {
    //            std::cout << i.first << "    =    " << i.second << std::endl;
    //        }
    //
    //    } while (!cursor->Next());
    //
    //    return BigResponse();
}
BigResponse MainEngine::Delete(BigRequest* bigRequest) {
    //    int error = file_manager_->OpenFile(bigRequest->tableName);
    //    BigResponse bigResponse;
    //    if (error) {
    //        bigResponse.error = Error(ErrorConstants::ERR_TABLE_NOT_EXISTS);
    //        return bigResponse;
    //    }
    //    Table* t = file_manager_->GetTableData(bigRequest->tableName);
    //    std::cout << t->record_amount << std ::endl;
    //    cursor = new Cursor(t, file_manager_);
    //    std::map<std::string, Condition> cond = bigRequest->dmlData.conditions;
    //    std::vector<std::pair<std::string, std::string>> record;
    //    int delete_count = 0;
    //
    //    int no_such_cols_table = 1;
    //    for (auto i : t->fields) {
    //        if (cond.find(i.first) != cond.end()) {
    //            no_such_cols_table = 0;
    //            break;
    //        }
    //    }
    //
    //    if (no_such_cols_table) {
    //        bigResponse.error = Error(ErrorConstants::ERR_NO_SUCH_FIELD);
    //        return bigResponse;
    //    }
    //
    //    do {
    //        record = cursor->Fetch();
    //        if (record.empty()) {
    //            continue;
    //        }
    //        for (int i = 0; i < record.size(); ++i) {
    //            std::string field_name = record[i].first;
    //            if (cond.find(field_name) != cond.end()) {
    //                if (check_condition(record[i].second, cond[field_name])) {
    //                    delete_count++;
    //                    cursor->Delete();
    //                }
    //            }
    //        }
    //    } while (!cursor->Next());
    //    cursor->table->record_amount -= delete_count;
    //    cursor->Commit();
    //    std::cout << cursor->table->record_amount << std ::endl;
    //    return BigResponse();
}

int MainEngine::check_condition(std::string rec_val, Condition cond_val) {
    int res = 0;
    switch (cond_val.sign) {
        case GREATEREQUALS:
            if (rec_val >= cond_val.value) {
                res = 1;
            }
            break;
        case GREATER:
            if (rec_val > cond_val.value) {
                res = 1;
            }
            break;
        case NOEQUALS:
            if (rec_val != cond_val.value) {
                res = 1;
            }
            break;
        case EQUALS: {
            if (rec_val == cond_val.value) {
                res = 1;
            }
            break;
        }
        case LOWER:
            if (rec_val < cond_val.value) {
                res = 1;
            }
            break;
        case LOWEREQUALS:
            if (rec_val <= cond_val.value) {
                res = 1;
            }
            break;
    }
    return res;
}
std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> MainEngine::GetCursor(std::string table_name) {
    std::shared_ptr<Table> table(new Table());
    std::shared_ptr<Cursor> cursor1(new Cursor);

    if (file_manager_->OpenFile(table_name)) {
        return std::make_pair(table, cursor1);
    };
    table = std::make_shared<Table>(*file_manager_->GetTableData(table_name));
    cursor1 = std::make_shared<Cursor>(table, file_manager_);
    return std::make_pair(table, cursor1);
}
