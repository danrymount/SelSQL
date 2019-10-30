//
// Created by sapiest on 05.10.2019.
//

#include "Headers/CreateAction.h"
#include "../../Parser/Headers/CreateVisitor.h"
// BigResponse CreateAction::execute(std::shared_ptr<BigRequest> _request, MainEngine* mainEngine) {
//    response = mainEngine->CreateTable(_request.get());
//    requestToResponse(_request);
//    if (!response.error.getErrorCode())
//        return response;
//    // std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
//    response.error = Message(ErrorConstants::ERR_TABLE_EXISTS);
//
//    return response;
//}

Message CreateAction::execute(std::shared_ptr<BaseActionNode> root) {
    root->accept(getTreeVisitor().get());
    auto v = static_cast<CreateVisitor*>(getTreeVisitor().get());
    auto t = v->getTable();
    auto error = v->getError();
    if (error.getErrorCode()) {
        return error;
    }
    return getEngine().CreateTable(std::make_shared<Table>(v->getTable()));
}
