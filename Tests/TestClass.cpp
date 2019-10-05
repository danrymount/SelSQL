//
// Created by sapiest on 03.10.2019.
//
#include <gtest/gtest.h>
#include "../src/Logic/Headers/MainLogic.h"
#include "Headers/TestUtils.h"
#include "parser.cpp"

Response res;

TEST(CREATE_TEST, TEST2) {
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name1", res.table.name);
    auto field = *(res.table.getFields().begin());
    EXPECT_EQ("ID", field.first);
    EXPECT_EQ(0, field.second.type);
}
TEST(CREATE_TEST, TEST3) {
    std::string str = "CREATE TABLE name2(col1 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name2", res.table.name);
    auto field = *(res.table.getFields().begin());
    EXPECT_EQ("col1", field.first);
    EXPECT_EQ(1, field.second.type);
    EXPECT_EQ(0, field.second.getConstraints()[0]);
}
TEST(CREATE_TEST, TEST4) {
    std::string str = "CREATE TABLE name3(col1 BOOLEAN UNIQUE, col2 INT PRIMARY KEY);";
    res = parse_request(str.c_str());
    std::map<std::string, Variable> fields = {{"col1", Variable(BOOLEAN, std::vector<Constraint>{UNIQUE})},
                                              {"col2", Variable(INT, std::vector<Constraint>{PRIMARY_KEY})}};
    Response obj(CREATE, Table("name3", fields), 0, "", "");
    TestUtils::comparetTables(obj, res);
}
TEST(CREATE_TEST, TEST5) {
    std::string str = "CREATE TABLE name4(col1 INT UNIQUE NOT NULL, col2 CHAR, col3 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    std::map<std::string, Variable> fields = {{"col1", Variable(INT, std::vector<Constraint>{UNIQUE, NOT_NULL})},
                                              {"col2", Variable(CHAR, std::vector<Constraint>{})},
                                              {"col3", Variable(FLOAT, std::vector<Constraint>{NOT_NULL})}};
    Response obj(CREATE, Table("name4", fields), 0, "", "");
    TestUtils::comparetTables(obj, res);
}
TEST(CREATE_TEST, TEST6) {
    std::string str = "CREATE TABLE name5(col1 FLOAT UNIQUE PRIMARY KEY, col2 CHAR NOT NULL PRIMARY KEY UNIQUE, col3 "
                      "BOOLEAN);";
    res = parse_request(str.c_str());
    std::map<std::string, Variable> fields = {{"col1", Variable(FLOAT, std::vector<Constraint>{UNIQUE, PRIMARY_KEY})},
                                              {"col2",
                                               Variable(CHAR, std::vector<Constraint>{NOT_NULL, PRIMARY_KEY, UNIQUE})},
                                              {"col3", Variable(BOOLEAN, std::vector<Constraint>{})}};
    Response obj(CREATE, Table("name5", fields), 0, "", "");
    TestUtils::comparetTables(obj, res);
}
TEST(CREATE_TEST, TEST7) {
    std::string str = "CREATE TABLE name6(col1 BOOLEAN , col2 CHAR , col3 INT);";
    res = parse_request(str.c_str());
    std::map<std::string, Variable> fields = {{"col1", Variable(BOOLEAN, std::vector<Constraint>{})},
                                              {"col2", Variable(CHAR, std::vector<Constraint>{})},
                                              {"col3", Variable(INT, std::vector<Constraint>{})}};
    Response obj(CREATE, Table("name6", fields), 0, "", "");
    TestUtils::comparetTables(obj, res);
}
TEST(CREATE_TEST, TEST8) {
    std::string str = "CREATE TABLE name7(col1 CHAR UNIQUE, col2 INT NOT NULL, col3 CHAR PRIMARY KEY); ";
    res = parse_request(str.c_str());
    std::map<std::string, Variable> fields = {{"col1", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                              {"col2", Variable(INT, std::vector<Constraint>{NOT_NULL})},
                                              {"col3", Variable(CHAR, std::vector<Constraint>{PRIMARY_KEY})}};
    Response obj(CREATE, Table("name7", fields), 0, "", "");
    TestUtils::comparetTables(obj, res);
}
TEST(CREATE_TEST, TEST9) {
    std::string str = "CREATE TABLE name8(col1 CHAR UNIQUE, col2 CHAR UNIQUE, col3 CHAR UNIQUE, col4 CHAR UNIQUE); ";
    res = parse_request(str.c_str());
    std::map<std::string, Variable> fields = {{"col1", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                              {"col2", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                              {"col3", Variable(CHAR, std::vector<Constraint>{UNIQUE})},
                                              {"col4", Variable(CHAR, std::vector<Constraint>{UNIQUE})}};
    Response obj(CREATE, Table("name8", fields), 0, "", "");
    TestUtils::comparetTables(obj, res);
}


TEST(SYNTAX_ERROR_TEST, TEST1) {
    std::string str = "CREAT TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected STRING, expecting DDLCREATE or DDLSHOW or DDLDROP (Str num 1, sym num 5): CREAT", res.errorMsg);
}

TEST(SYNTAX_ERROR_TEST, TEST2) {
    std::string str = "CREATE TABLE name1(1565 INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected TYPE, expecting STRING (Str num 1, sym num 20): INT", res.errorMsg);
}

TEST(SYNTAX_ERROR_TEST, TEST3) {
    std::string str = "CREATE TABLE name1(ID IN);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected STRING, expecting TYPE (Str num 1, sym num 21): IN", res.errorMsg);
}

TEST(SYNTAX_ERROR_TEST, TEST4) {
    std::string str = "CREATE TABLE name1(ID INT NOT NU);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected STRING, expecting COMMA or BRACKET or CONSTRAINT (Str num 1, sym num 25): NOT", res.errorMsg);
}

TEST(SYNTAX_ERROR_TEST, TEST5) {
    std::string str = "CREATE TABLE name1( );";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected BRACKET, expecting STRING (Str num 1, sym num 18): )", res.errorMsg);
}

TEST(ERROR_TEST, TEST1) {
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(1, res.code);
    EXPECT_EQ(Constants::ERR_TABLE_EXISTS, res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}


TEST(SHOW_CREATE, TEST1) {
    std::string str = "CREATE TABLE name2(ID INT);";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
    str = "SHOW CREATE TABLE name2;";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(0, res.code);
    EXPECT_EQ("CREATE TABLE name2(ID INT );", res.returnMsg);
    str = "DROP TABLE name2;";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
}

TEST(SHOW_CREATE, TEST2) {
    std::string str = "CREATE TABLE name3(ID INT PRIMARY KEY);";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
    str = "SHOW CREATE TABLE name3;";
    res = parse_request(str.c_str());
    res = MainLogic::executeRequest(res);
    EXPECT_EQ(0, res.code);
    str = "DROP TABLE name3;";
    res = parse_request(str.c_str());
    MainLogic::executeRequest(res);
    Response result = parse_request(res.returnMsg.c_str());
    result = MainLogic::executeRequest(res);
    EXPECT_EQ(res.table.name, result.table.name);
}
