//
// Created by sapiest on 05.10.2019.
//

#include "Headers/SelectAction.h"
#include <mutex>
#include "../../Parser/Headers/SelectVisitor.h"

Message SelectAction::execute(std::shared_ptr<BaseActionNode> root) {
    std::mutex my;
    std::lock_guard<std::mutex> guard(my);
    root->accept(getTreeVisitor().get());
    auto v = static_cast<SelectVisitor *>(getTreeVisitor().get());
    auto expr = v->getExpr();
    v->setExpressionVisitor(exprVisitor);
    auto source = v->getSource();
    source->accept(getTreeVisitor().get());
    auto message = v->getMessage();
    if (message.getErrorCode()) {
        commitTransaction(root);

        return message;
    }
    auto tableName = v->getTableName();
    if (tableName.empty()) {
        records = v->getRecords();
    } else {
        cursor = v->getEngine()->GetCursor(tableName, root->getId());
        auto table = cursor.first;
        auto columns = v->getColumns();
        std::vector<std::pair<std::string, std::string>> columnValues;

        for (auto &col : columns) {
            columnValues.emplace_back(std::make_pair(col.second, ""));
        }

        if (table->name.empty()) {
            commitTransaction(root);

            return Message(ErrorConstants::ERR_TABLE_NOT_EXISTS);
        }

        message = ActionsUtils::checkFieldsExist(table, columnValues);
        if (message.getErrorCode()) {
            commitTransaction(root);

            return message;
        }

        cursor.second->Reset();
        try {
            expr->accept(optimizerExprVisitor);
        } catch (std::exception &exception) {
            // TODO expception from visitor
        }

        // if (optimizerExprVisitor->getMbIndex()) {
        // auto mbIndentIndex = optimizerExprVisitor->getIndent();
        // auto it = std::find_if(table->getFields().begin(), table->getFields().end(), [](std::pair<std::string,
        // Variable> field) { return field.second.isIndex(); });
        std::string curIndex;
        for (auto &field : table->getFields()) {
            if (field.second.isIndex()) {
                curIndex = field.first;
                break;
            }
        }
        if (!curIndex.empty()) {
            std::cout << "INDEXSES" << std::endl;
            auto data_manager = cursor.second->GetDataManager();
            auto indexes = data_manager->GetIndexes(tableName);
            indexExprVisitor->setValues(indexes);
            expr->accept(indexExprVisitor);
            auto result = indexExprVisitor->gerAns();
            for (auto &res : result) {
                cursor.second->SetPos(res);
                auto _record = cursor.second->Fetch();
                if (_record.first.empty()) {
                    continue;
                }
                std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
                for (auto &col : _record.first) {
                    _newRecord.emplace_back(std::make_pair(std::make_pair("", col.first), col.second));
                }
                records.push_back(_newRecord);
            }
            //            for (auto &field : indexes) {
            //                cursor.second->SetPos(field.second);
            //                //                if (optimizerExprVisitor->getMbIndex()) {
            //                //                    auto mbIndentIndex = optimizerExprVisitor->getIndent();
            //                //                    if(mbIndentIndex == curIndex){
            //                //
            //                //                    }
            //                //                }
            //
            //
            //
            //                auto _record = cursor.second->Fetch();
            //                if (_record.first.empty()) {
            //                    continue;
            //                }
            //                std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
            //                if (_record.first.empty()) {
            //                    continue;
            //                }
            //                for (auto &col : _record.first) {
            //                    _newRecord.emplace_back(std::make_pair(std::make_pair("", col.first), col.second));
            //                }
            //                exprVisitor->setFirstValues(_newRecord);
            //                try {
            //                    expr->accept(exprVisitor);
            //                } catch (std::exception &exception) {
            //                    v->getEngine()->Commit(root->getId());
            //                    std::string exc = exception.what();
            //                    return Message(ErrorConstants::ERR_TYPE_MISMATCH);
            //                }
            //                if (exprVisitor->getResult()) {
            //                    records.push_back(_newRecord);
            //                }
            //            }
        } else {
            //} else {
            do {
                auto start = v->getStartTime();
                auto finish = v->getFinishTime();

                auto _record = cursor.second->Fetch(start == -1 ? 0 : start, finish == -1 ? 0 : finish);
                if (_record.first.empty()) {
                    continue;
                }
                std::vector<std::pair<std::pair<std::string, std::string>, std::string>> _newRecord;
                for (auto &col : _record.first) {
                    _newRecord.emplace_back(std::make_pair(std::make_pair("", col.first), col.second));
                }

                if (start != -1) {
                    auto startTime = getDateTime(_record.second.first);
                    if (_record.second.first == 0) {
                        continue;
                    }
                    auto endTime = !_record.second.second ? "-" : getDateTime(_record.second.second);

                    _newRecord.emplace_back(std::make_pair(std::make_pair("", "start_time"), startTime));
                    _newRecord.emplace_back(std::make_pair(std::make_pair("", "end_time"), endTime));
                }

                exprVisitor->setFirstValues(_newRecord);
                try {
                    expr->accept(exprVisitor);
                } catch (std::exception &exception) {
                    v->getEngine()->Commit(root->getId());
                    std::string exc = exception.what();
                    return Message(ErrorConstants::ERR_TYPE_MISMATCH);
                }
                if (exprVisitor->getResult()) {
                    records.push_back(_newRecord);
                }
            } while (!cursor.second->NextRecord());
        }
    }

    v->setRecords(records);
    return Message(ActionsUtils::checkSelectColumns(records, v->getColumns()));
};