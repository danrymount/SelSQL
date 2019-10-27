//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_SELECTACTION_H
#define SELSQL_SELECTACTION_H

#include <sstream>
#include "BaseAction.h"
class SelectAction : public BaseAction {
   public:
    std::stringstream stringstream;

    explicit SelectAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Message execute(std::shared_ptr<BaseActionNode>) override;

   private:
    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    std::vector<std::vector<std::pair<std::string, std::string>>> records;
};

#endif  // SELSQL_SELECTACTION_H
