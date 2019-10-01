//
// Created by sapiest on 01.10.2019.
//

#include <gtest/gtest.h>
#include "parser.cpp"
TEST(PARSER_TEST, TEST_1) {
    char request[] = "CREATE TABLE name(ID INT);";
    auto res = parse_request(request);
    EXPECT_EQ("name", res.name);
}

TEST(PARSER_TEST, TEST_2){
    char request[] = "CREATE TABLE name(ID INT PRIMARY KEY);";
    auto res = parse_request(request);
    EXPECT_EQ("ID", res.fields[0].name);
}