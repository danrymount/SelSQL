//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_SELECTACTION_H
#define SELSQL_SELECTACTION_H

#include <sstream>
#include "../../../Parser/Headers/ExpressionOptimizerVisitor.h"
#include "../../../Parser/Headers/ExpressionVisitor.h"
#include "BaseAction.h"
class SelectAction : public BaseAction {
   public:
    std::stringstream stringstream;

    explicit SelectAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {
        exprVisitor = new ExpressionVisitor();
        optimizerExprVisitor = new ExpressionOptimizerVisitor();
    }

    ~SelectAction() {
        delete exprVisitor;
        delete optimizerExprVisitor;
    }

    Message execute(std::shared_ptr<BaseActionNode>) override;

   private:
    ExpressionVisitor *exprVisitor;
    ExpressionOptimizerVisitor *optimizerExprVisitor;
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    std::vector<std::vector<std::pair<std::pair<std::string, std::string>, std::string>>> records;

    static std::string getDateTime(int64_t dateTime) {
        std::chrono::system_clock::time_point now{};
        std::memcpy(&now, &dateTime, sizeof(dateTime));
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_c);

        char buff[70];
        strftime(buff, sizeof buff, "%d-%m-%Y %H:%M:%S", &now_tm);
        auto res = std::string(buff);

        return res;
    }
};

#endif  // SELSQL_SELECTACTION_H
