//
// Created by sapiest on 03.10.2019.
//
#include <gtest/gtest.h>
#include "parser.cpp"

Table* res;

TEST(MAIN_TEST, TEST1) {
    std::string str = "SHOW CREATE TABLE NAME;";
    res = parse_request(str.c_str());
    EXPECT_EQ(1, 1);
}
TEST(MAIN_TEST, TEST2) {
    std::string str = "CREATE TABLE name1(ID INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name1", res->name);
    EXPECT_EQ("ID", res->fields[0].name);
    EXPECT_EQ(1, res->fields[0].type);
}
TEST(MAIN_TEST, TEST3) {
    std::string str = "CREATE TABLE name2(col1 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name2", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(2, res->fields[0].type);
    EXPECT_EQ(0, res->fields[0].constraints[0]);
}
TEST(MAIN_TEST, TEST4) {
    std::string str = "CREATE TABLE name3(col1 BOOLEAN UNIQUE, col2 INT PRIMARY KEY);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name3", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(3, res->fields[0].type);
    EXPECT_EQ(2, res->fields[0].constraints[0]);
    EXPECT_EQ("col2", res->fields[1].name);
    EXPECT_EQ(1, res->fields[1].type);
    EXPECT_EQ(1, res->fields[1].constraints[0]);
}
TEST(MAIN_TEST, TEST5) {
    std::string str = "CREATE TABLE name4(col1 INT UNIQUE NOT NULL, col2 CHAR, col3 FLOAT NOT NULL);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name4", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(1, res->fields[0].type);
    EXPECT_EQ(2, res->fields[0].constraints[0]);
    EXPECT_EQ(0, res->fields[0].constraints[1]);
    EXPECT_EQ("col2", res->fields[1].name);
    EXPECT_EQ(4, res->fields[1].type);
    EXPECT_EQ("col3", res->fields[2].name);
    EXPECT_EQ(2, res->fields[2].type);
    EXPECT_EQ(0, res->fields[2].constraints[0]);
}
TEST(MAIN_TEST, TEST6) {
    std::string str = "CREATE TABLE name5(col1 FLOAT UNIQUE PRIMARY KEY, col2 CHAR NOT NULL PRIMARY KEY UNIQUE, col3 "
                      "BOOLEAN);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name5", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(2, res->fields[0].type);
    EXPECT_EQ(2, res->fields[0].constraints[0]);
    EXPECT_EQ(1, res->fields[0].constraints[1]);
    EXPECT_EQ("col2", res->fields[1].name);
    EXPECT_EQ(4, res->fields[1].type);
    EXPECT_EQ(0, res->fields[1].constraints[0]);
    EXPECT_EQ(1, res->fields[1].constraints[1]);
    EXPECT_EQ(2, res->fields[1].constraints[1]);
    EXPECT_EQ("col3", res->fields[2].name);
    EXPECT_EQ(3, res->fields[2].type);
}
TEST(MAIN_TEST, TEST7) {
    std::string str = "CREATE TABLE name6(col1 BOOLEAN , col2 CHAR , col3 INT);";
    res = parse_request(str.c_str());
    EXPECT_EQ("name6", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(3, res->fields[0].type);
    EXPECT_EQ("col2", res->fields[1].name);
    EXPECT_EQ(4, res->fields[1].type);
    EXPECT_EQ("col3", res->fields[2].name);
    EXPECT_EQ(1, res->fields[2].type);
}
TEST(MAIN_TEST, TEST8) {
    std::string str = "CREATE TABLE name7(col1 CHAR UNIQUE, col2 INT NOT NULL, col3 CHAR PRIMARY KEY); ";
    res = parse_request(str.c_str());
    EXPECT_EQ("name7", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(4, res->fields[0].type);
    EXPECT_EQ(2, res->fields[0].constraints[0]);
    EXPECT_EQ("col2", res->fields[1].name);
    EXPECT_EQ(1, res->fields[1].type);
    EXPECT_EQ(0, res->fields[1].constraints[0]);
    EXPECT_EQ("col3", res->fields[2].name);
    EXPECT_EQ(4, res->fields[2].type);
    EXPECT_EQ(1, res->fields[2].constraints[0]);
}
TEST(MAIN_TEST, TEST9) {
    std::string str = "CREATE TABLE name8(col1 CHAR UNIQUE, col2 CHAR UNIQUE, col3 CHAR UNIQUE, col4 CHAR UNIQUE); ";
    res = parse_request(str.c_str());
    EXPECT_EQ("name8", res->name);
    EXPECT_EQ("col1", res->fields[0].name);
    EXPECT_EQ(4, res->fields[0].type);
    EXPECT_EQ(2, res->fields[0].constraints[0]);
    EXPECT_EQ("col2", res->fields[1].name);
    EXPECT_EQ(4, res->fields[1].type);
    EXPECT_EQ(2, res->fields[1].constraints[0]);
    EXPECT_EQ("col3", res->fields[2].name);
    EXPECT_EQ(4, res->fields[2].type);
    EXPECT_EQ(2, res->fields[2].constraints[0]);
    EXPECT_EQ("col4", res->fields[3].name);
    EXPECT_EQ(4, res->fields[3].type);
    EXPECT_EQ(2, res->fields[3].constraints[0]);
}