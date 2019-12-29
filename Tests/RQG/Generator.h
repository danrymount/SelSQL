//
// Created by User on 16.12.2019.
//
#pragma once

#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <functional>

class Generator {
   public:
    std::string run();

   private:
    void initialize();
    std::string request;
    std::vector<std::string> colnames = {"id", "name", "city", "age", "phone", "address", "status", "preferences",
                                         "car", "education", "job", "children", "money", "house", "pets"};
    std::vector<std::string> tablenames = {"peoples", "employees", "children", "men", "women", "teachers", "students",
                                           "doctors", "directors", "friends"};
    std::vector<std::string> signs = {"+", "-", "*", "/"};
    std::vector<std::string> compSigns = {"=", "!=", ">", "<", ">=", "<="};
    std::vector<std::string> logicSigns = {"and", "or"};
    std::vector<std::string> joinTypes = {"join", "left join", "right join", "full join"};
    std::vector<std::string> types = {"int", "float", "char"};
    std::vector<std::string> constraints = {"unique", "primary key", "not null"};
    std::vector<std::string> values = {"1", "'Vasya'", "'Gorod'", "42", "8957", "'sdf5545f'", "'chel'", "'vip'", "'auto'"
                                       , "'student'", "'no'", "5", "7895476", "88", "17"};
    int randomChoise();
    std::string choiceColnames(int);
    std::string choiceColname();
    std::string choiceTablename();
    std::string join();
    std::string where();
    std::string whereExpr();
    std::string setExpr();
    std::string tableCols();
    std::string randNum();
    std::string valuesChoice();
    void selectRequest();
    void createRequest();
    void dropRequest();
    void deleteRequest();
    void showRequest();
    void insertRequest();
    void updateRequest();
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> distCol, distFunc, distRand, distTable, dist1_9, dist0_9, distSign, distComp, distLogic,
    distJoin, distTypes, distConstr, distVal ;
};
