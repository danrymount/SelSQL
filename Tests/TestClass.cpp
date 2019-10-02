//
// Created by sapiest on 03.10.2019.
//
#include <gtest/gtest.h>
#include "parser.cpp"

TEST(MAIN_TEST, TEST1){
    std::string str = "SHOW CREATE TABLE NAME;";
    parse_request(str.c_str());
    EXPECT_EQ(1, 1);
}
