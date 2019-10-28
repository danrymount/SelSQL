//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_SHOWCREATEACTION_H
#define SELSQL_SHOWCREATEACTION_H

#include "BaseAction.h"
class ShowCreateAction : public BaseAction {
   public:
    explicit ShowCreateAction(std::shared_ptr<TreeVisitor> _visitor) : BaseAction(std::move(_visitor)) {}

    Message execute(std::shared_ptr<BaseActionNode>) override;
    //BigResponse execute(std::shared_ptr<BigRequest>, MainEngine* mainEngine) override;
   private:
    std::string getTableName(IdentNode* node);
};

#endif  // SELSQL_SHOWCREATEACTION_H
