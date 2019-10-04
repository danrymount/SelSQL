//
// Created by toxaxab on 30.09.2019.
//

#include "Headers/MainLogic.h"
#include <string>
void MainLogic::addColumn(char* name, char* type) {
    //    if (currentVariable.type != none) {
    //        response.table.fields.push_back(currentVariable);
    //        currentVariable.type = none;
    //    }
    //    if (checkName.find(ParserUtils::chrToString(name)) == checkName.end()) {
    //        currentVariable.constraints.erase(currentVariable.constraints.begin(), currentVariable.constraints.end());
    //        checkName[ParserUtils::chrToString(name)] = 1;
    //        currentVariable.name = ParserUtils::chrToString(name);
    //        currentVariable.type = parserUtils.stringToType(ParserUtils::chrToString(type));
    //    } else {
    //        response.code = 1;
    //        showErrorName();
    //    }

    if (checkName.find(ParserUtils::chrToString(name)) == checkName.end()) {
        response.table.addField(string(name), parserUtils.stringToType(string(type)));
        checkName[string(name)] = 1;
    } else {
        response.code = 1;
        response.errorMsg = Constants::ERR_SAME_FIELD_NAME;
    }

    // printf("VALNAME = %s, Type = %s\n", name, type);
}

void MainLogic::addConstraint(char* name) {
    response.code = response.table.addConstraint(parserUtils.stringToConstraint(string(name)));
    if(response.code)
        response.errorMsg = Constants::ERR_SAME_CONSTRAINT;

    //    Constraint tempConstraint = parserUtils.stringToConstraint(ParserUtils::chrToString(name));
    //    for (const auto& elem : currentVariable.constraints) {
    //        if (elem == tempConstraint) {
    //            response.code = 1;
    //            // response.errorMsg = Constants::ERR_TABLE_EXISTS;
    //            // showError();
    //        }
    //    }
    //    if (!response.code)
    //        currentVariable.constraints.push_back(tempConstraint);
    //}
}

Response MainLogic::finish() {
//    if (currentVariable.type != none) {
//        response.table.fields.push_back(currentVariable);
//        currentVariable.type = none;
//    }

    if (response.code)
        return response;

//    switch (currentAction) {
//        case CREATE: {
//            errorCode = engineApi.CreateTable(&response.table);
//            if (errorCode) {
//                std::cout << Constants::ERR_TABLE_EXISTS << std::endl;
//                response.code = 1;
//                response.errorMsg = Constants::ERR_TABLE_EXISTS;
//                return response;
//            }
//            break;
//        }
//        case SHOW_CREATE: {
//            response.table = *engineApi.ShowCreateTable(response.table.name);
//            if (&(response.table) == nullptr) {
//                // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
//                response.code = 1;
//                response.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
//                return response;
//            }
//            response.returnMsg = actionsUtils.makeRequestCreateFromTable(response.table);
//            // std::cout << response.returnMsg  << std::endl;
//            break;
//        }
//        case DROP: {
//            errorCode = engineApi.DropTable(response.table.name);
//            if (errorCode) {
//                // std::cout << Constants::ERR_TABLE_NOT_EXISTS << std::endl;
//                response.code = 1;
//                response.errorMsg = Constants::ERR_TABLE_NOT_EXISTS;
//                return response;
//            }
//            break;
//        }
//    }
    return response;
}

void MainLogic::addTableName(char* name) {
    response.clear();
    checkName.erase(checkName.begin(), checkName.end());

    response.table.name = string(name);
}

void MainLogic::addActionName(char* name) {
    currentAction = parserUtils.stringToAction(ParserUtils::chrToString(name));
}
