//
// Created by User on 16.12.2019.
//

#include "Generator.h"
#include <iostream>
std::string Generator::run() {
    initialize();
    return request;
}

void Generator::initialize() {
    std::vector<std::function<void()>> requests = {std::bind(&Generator::selectRequest, this),
                                                   std::bind(&Generator::createRequest, this),
                                                   std::bind(&Generator::dropRequest, this),
                                                   std::bind(&Generator::deleteRequest, this),
                                                   std::bind(&Generator::showRequest, this),
                                                   std::bind(&Generator::insertRequest, this),
                                                   std::bind(&Generator::updateRequest, this)};
    distCol = std::uniform_int_distribution<>(0, colnames.size() - 1);
    distFunc = std::uniform_int_distribution<> (0, requests.size()-1);
    distRand = std::uniform_int_distribution<> (0, 1);
    distTable = std::uniform_int_distribution<> (0, tablenames.size() - 1);
    dist0_9 = std::uniform_int_distribution<> (0, 9);
    dist1_9 = std::uniform_int_distribution<> (1, 9);
    distSign = std::uniform_int_distribution<> (0, signs.size() - 1);
    distComp = std::uniform_int_distribution<> (0, compSigns.size() - 1);
    distLogic = std::uniform_int_distribution<> (0, logicSigns.size() - 1);
    distJoin = std::uniform_int_distribution<> (0, joinTypes.size() - 1);
    distTypes = std::uniform_int_distribution<> (0, types.size() - 1);
    distConstr = std::uniform_int_distribution<> (0, constraints.size() - 1);
    distVal = std::uniform_int_distribution<> (0, values.size() - 1);
    gen.seed(rd());
    request = "";
    requests[distFunc(gen)]();
}

int Generator::randomChoise() {
    return distRand(gen) == 0 ? 0 : 1;
}

std::string Generator::choiceColnames(int checkStar = 1) {
    std::string names = colnames[distCol(gen)];
    while (randomChoise()) {
        names += ", " + colnames[distCol(gen)];
    }
    if (randomChoise() && checkStar) {
        names += ", *";
    }
    return names;
}

std::string Generator::choiceColname() {
    return colnames[distCol(gen)];
}

std::string Generator::choiceTablename() {
    return tablenames[distTable(gen)];
}

std::string Generator::randNum() {
    std::string num = std::to_string(dist1_9(gen));
    while (randomChoise() and num.length() < 9) {
        num += std::to_string(dist0_9(gen));
    }
    return num;
}

std::string Generator::setExpr() {
    std::string setExpr;
    randomChoise() ? setExpr += colnames[distCol(gen)] : setExpr += randNum();
    while (randomChoise()) {
        setExpr += +" " + signs[distSign(gen)] + " ";
        randomChoise() ? setExpr += colnames[distCol(gen)] : setExpr += randNum();
    }
    return setExpr;
}

std::string Generator::whereExpr() {
    std::string whereExpr;
    if (randomChoise())
        whereExpr += "not ";
    whereExpr += setExpr() + " " + compSigns[distComp(gen)] + " " + setExpr();
    return whereExpr;
}

std::string Generator::where() {
    std::string where = whereExpr();
    while (randomChoise()) {
        where += " " + logicSigns[distLogic(gen)] + " " + whereExpr();
    }
    return where;
}

std::string Generator::join() {
    std::string joinExpr;
    randomChoise() ? joinExpr += join() + " " : joinExpr += choiceTablename() + " ";
    joinExpr += joinTypes[distJoin(gen)] + " ";
    randomChoise() ? joinExpr += join() + " " : joinExpr += choiceTablename() + " ";
    joinExpr += "on " + where();
    return joinExpr;
}

std::string Generator::tableCols() {
    int type = distTypes(gen);
    std::string cols = colnames[distCol(gen)] + " " + types[type];
    if (type == 2) {
        cols += "(" + randNum() + ")";
    }
    if (randomChoise()) {
        cols += " " + constraints[distConstr(gen)];
    }
    if (randomChoise()) {
        cols += ", " + tableCols();
    }
    return cols;
}

std::string Generator::valuesChoice() {
    std::string vals = values[distVal(gen)];
    if (randomChoise()) {
        vals += ", " + values[distVal(gen)];
    }
    return vals;
}

void Generator::selectRequest() {
    request += "select " + choiceColnames() + " from ";
    randomChoise() ? request += choiceTablename() : request += join();
    if (randomChoise()) {
        request += " where " + where();
    }
    request += ";";
}

void Generator::createRequest() { request += "create table " + choiceTablename() + " (" + tableCols() + ");"; }

void Generator::dropRequest() { request += "drop table " + choiceTablename() + ";"; }

void Generator::deleteRequest() {
    request += "delete from " + choiceTablename();
    if (randomChoise()) {
        request += " where " + where();
    }
    request += ";";
}

void Generator::showRequest() { request += "show create table " + choiceTablename() + ";"; }

void Generator::insertRequest() {
    request += "insert into " + choiceTablename();
    if (randomChoise()) {
        request += "(" + choiceColnames(0) + ")";
    }
    request += " values(" + valuesChoice() + ");";
}

void Generator::updateRequest() {
    request += "update " + choiceTablename() + " set " + choiceColname() + " = " + setExpr();
    while (randomChoise()) {
        request += ", " + choiceColname() + " = " + setExpr();
    }
    if (randomChoise()) {
        request += " where " + where();
    }
    request += ";";
}