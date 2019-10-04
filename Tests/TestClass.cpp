//
// Created by sapiest on 03.10.2019.
//
#include <gtest/gtest.h>
#include "parser.cpp"

Response res;

TEST(CREATE_TEST, TEST2) {
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name1", res.table.name);
    EXPECT_EQ("ID", res.table.fields[0].name);
    EXPECT_EQ(1, res.table.fields[0].type);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST3) {
    std::string str = "CREATE TABLE name2(col1 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name2", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(2, res.table.fields[0].type);
    EXPECT_EQ(0, res.table.fields[0].constraints[0]);
    str = "DROP TABLE name2;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST4) {
    std::string str = "CREATE TABLE name3(col1 BOOLEAN UNIQUE, col2 INT PRIMARY KEY);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name3", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(3, res.table.fields[0].type);
    EXPECT_EQ(2, res.table.fields[0].constraints[0]);
    EXPECT_EQ("col2", res.table.fields[1].name);
    EXPECT_EQ(1, res.table.fields[1].type);
    EXPECT_EQ(1, res.table.fields[1].constraints[0]);
    str = "DROP TABLE name3;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST5) {
    std::string str = "CREATE TABLE name4(col1 INT UNIQUE NOT NULL, col2 CHAR, col3 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name4", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(1, res.table.fields[0].type);
    EXPECT_EQ(2, res.table.fields[0].constraints[0]);
    EXPECT_EQ(0, res.table.fields[0].constraints[1]);
    EXPECT_EQ("col2", res.table.fields[1].name);
    EXPECT_EQ(4, res.table.fields[1].type);
    EXPECT_EQ("col3", res.table.fields[2].name);
    EXPECT_EQ(2, res.table.fields[2].type);
    EXPECT_EQ(0, res.table.fields[2].constraints[0]);
    str = "DROP TABLE name4;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST6) {
    std::string str = "CREATE TABLE name5(col1 FLOAT UNIQUE PRIMARY KEY, col2 CHAR NOT NULL PRIMARY KEY UNIQUE, col3 "
                      "BOOLEAN);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name5", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(2, res.table.fields[0].type);
    EXPECT_EQ(2, res.table.fields[0].constraints[0]);
    EXPECT_EQ(1, res.table.fields[0].constraints[1]);
    EXPECT_EQ("col2", res.table.fields[1].name);
    EXPECT_EQ(4, res.table.fields[1].type);
    EXPECT_EQ(0, res.table.fields[1].constraints[0]);
    EXPECT_EQ(1, res.table.fields[1].constraints[1]);
    EXPECT_EQ(2, res.table.fields[1].constraints[2]);
    EXPECT_EQ("col3", res.table.fields[2].name);
    EXPECT_EQ(3, res.table.fields[2].type);
    str = "DROP TABLE name5;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST7) {
    std::string str = "CREATE TABLE name6(col1 BOOLEAN , col2 CHAR , col3 INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name6", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(3, res.table.fields[0].type);
    EXPECT_EQ("col2", res.table.fields[1].name);
    EXPECT_EQ(4, res.table.fields[1].type);
    EXPECT_EQ("col3", res.table.fields[2].name);
    EXPECT_EQ(1, res.table.fields[2].type);
    str = "DROP TABLE name6;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST8) {
    std::string str = "CREATE TABLE name7(col1 CHAR UNIQUE, col2 INT NOT NULL, col3 CHAR PRIMARY KEY); ";
    res = parse_request(str.c_str());
    EXPECT_EQ("name7", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(4, res.table.fields[0].type);
    EXPECT_EQ(2, res.table.fields[0].constraints[0]);
    EXPECT_EQ("col2", res.table.fields[1].name);
    EXPECT_EQ(1, res.table.fields[1].type);
    EXPECT_EQ(0, res.table.fields[1].constraints[0]);
    EXPECT_EQ("col3", res.table.fields[2].name);
    EXPECT_EQ(4, res.table.fields[2].type);
    EXPECT_EQ(1, res.table.fields[2].constraints[0]);
    str = "DROP TABLE name7;";
    parse_request(str.c_str());
}
TEST(CREATE_TEST, TEST9) {
    std::string str = "CREATE TABLE name8(col1 CHAR UNIQUE, col2 CHAR UNIQUE, col3 CHAR UNIQUE, col4 CHAR UNIQUE); ";
    res = parse_request(str.c_str());
    EXPECT_EQ("name8", res.table.name);
    EXPECT_EQ("col1", res.table.fields[0].name);
    EXPECT_EQ(4, res.table.fields[0].type);
    EXPECT_EQ(2, res.table.fields[0].constraints[0]);
    EXPECT_EQ("col2", res.table.fields[1].name);
    EXPECT_EQ(4, res.table.fields[1].type);
    EXPECT_EQ(2, res.table.fields[1].constraints[0]);
    EXPECT_EQ("col3", res.table.fields[2].name);
    EXPECT_EQ(4, res.table.fields[2].type);
    EXPECT_EQ(2, res.table.fields[2].constraints[0]);
    EXPECT_EQ("col4", res.table.fields[3].name);
    EXPECT_EQ(4, res.table.fields[3].type);
    EXPECT_EQ(2, res.table.fields[3].constraints[0]);
    str = "DROP TABLE name8;";
    parse_request(str.c_str());
}

TEST(ERROR_TEST, TEST1){
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ(Constants::ERR_TABLE_EXISTS, res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}

TEST(ERROR_TEST, TEST2){
    std::string str = "CREAT TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected STRING, expecting DDLCREATE or DDLSHOW or DDLDROP", res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}

TEST(ERROR_TEST, TEST3){
    std::string str = "CREATE TABLE name1(1565 INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected TYPE, expecting STRING", res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}

TEST(ERROR_TEST, TEST4){
    std::string str = "CREATE TABLE name1(ID IN);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected STRING, expecting TYPE", res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}

TEST(ERROR_TEST, TEST5){
    std::string str = "CREATE TABLE name1(ID INT NOT NU);";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected STRING, expecting COMMA or BRACKET or CONSTRAINT", res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}

TEST(ERROR_TEST, TEST6){
    std::string str = "CREATE TABLE name1( );";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, res.code);
    EXPECT_EQ("syntax error, unexpected BRACKET, expecting STRING", res.errorMsg);
    str = "DROP TABLE name1;";
    parse_request(str.c_str());
}

TEST(SHOW_CREATE, TEST1){
    std::string str = "CREATE TABLE name2(ID INT);";
    parse_request(str.c_str());
    str = "SHOW CREATE TABLE name2;";
    res = parse_request(str.c_str());
    EXPECT_EQ(0, res.code);
    EXPECT_EQ("CREATE TABLE name2(ID INT );", res.returnMsg);
    str = "DROP TABLE name2;";
    parse_request(str.c_str());
}

TEST(SHOW_CREATE, TEST2){
    std::string str = "CREATE TABLE name3(ID INT PRIMARY KEY);";
    res = parse_request(str.c_str());
    str = "SHOW CREATE TABLE name3;";
    Response resShow = parse_request(str.c_str());
    EXPECT_EQ(0, resShow.code);
    str = "DROP TABLE name3;";
    parse_request(str.c_str());
    Response result = parse_request(resShow.returnMsg.c_str());
    EXPECT_EQ(res.table.name, result.table.name);
}

