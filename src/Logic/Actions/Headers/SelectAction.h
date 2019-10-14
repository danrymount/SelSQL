//
// Created by sapiest on 05.10.2019.
//

#ifndef SELSQL_SELECTACTION_H
#define SELSQL_SELECTACTION_H

#include "BaseAction.h"
class SelectAction : public BaseAction {
   public:
    BigResponse execute(BigRequest& _request, MainEngine* mainEngine) override;

   private:
    void printAll(BigResponse& response);

    void printAllHeader(std::shared_ptr<Table> table);

    void printField(std::string field);

    std::pair<std::shared_ptr<Table>, std::shared_ptr<Cursor>> cursor;
    // std::vector<std::pair<std::string, std::string>> record;
};

#endif  // SELSQL_SELECTACTION_H
