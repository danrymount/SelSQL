//
// Created by sapiest on 20.10.2019.
//

#ifndef SELSQL_TREEVISITOR_H
#define SELSQL_TREEVISITOR_H

#include <memory>
#include "../../Utils/Structures/BigResponse.h"
#include "Visitor.h"
class TreeVisitor : public Visitor {
   public:
    TreeVisitor() = default;

    void visit(RootNode* node) override;
    void visit(CreateNode* node) override;
    void visit(DropNode* node) override;
    void visit(ShowCreateNode* node) override;
    void visit(ConstraintNode* node) override;
    void visit(VariableNode* node) override;

    std::shared_ptr<BigResponse> getResponse() { return response; }

   private:
    std::shared_ptr<BigRequest> request;
    std::shared_ptr<BigResponse> response;
};

#endif  // SELSQL_TREEVISITOR_H