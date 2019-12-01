#include <gtest/gtest.h>
#include "../../Client/Client.h"
#include "../../src/Utils/Structures/Data/Variable.h"
#include "../Headers/TestUtils.h"

#define Request std::pair<std::string, std::string>

TEST(SERVER_TEST_SYN_STRESS, STRESS_TEST_UPDATE) {
    TestUtils::kill();
    TestUtils::clear();
    TestUtils::run();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150), col1 int, col2 int, col3 "
                               "int);",
                               "Success"}});
    std::string answerFirst = "\nid|age     |name  |col1|col2|col3|\n";
    std::string answerSecond = "\nid|age     |name    |col1|col2|col3|\n";
    std::vector<Request> allRequest;
    for (int i = 0; i < 71; i++) {
        allRequest.emplace_back(std::make_pair("INSERT INTO jj values(1, 2.9, 'sfsf', 1, 1, 1);", "Success"));
        answerFirst += "1 |2.900000|'sfsf'|1   |1   |1   |\n";
        answerSecond += "8 |3.780000|'sdfsdf'|5   |9   |6   |\n";
    };
    TestUtils::checkRequests(allRequest);
    TestUtils::checkDrop({{"UPDATE jj SET id = (8+3-3)*2/2, age = 3.78, name = 'sdfsdf', col1 = 5, "
                           "col2 = 9, col3 = 6 where id = 5 or not(id = 7 or id = 9);",
                           "SELECT * FROM jj;"},
                          {answerFirst, answerSecond}});
}

TEST(SERVER_TEST_SYN_STRESS, STRESS_TEST_DELETE) {
    TestUtils::kill();
    TestUtils::clear();
    TestUtils::run();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(150), col1 int, col2 int, col3 "
                               "int);",
                               "Success"}});
    std::string answerFirst = "\nid|age     |name  |col1|col2|col3|\n";
    std::vector<Request> allRequest;
    // TODO FIX ON LINUX
    for (int i = 0; i < 3700; i++) {
        allRequest.emplace_back(std::make_pair("INSERT INTO jj values(1, 2.9, 'sfsf', 1, 1, 1);", "Success"));
        answerFirst += "1 |2.900000|'sfsf'|1   |1   |1   |\n";
    };
    TestUtils::checkRequests(allRequest);
    TestUtils::checkDrop({{"delete from jj where (id = 5 or not(id = 7-7*1 or id = 9) or "
                           "(id = 2+3*(id-75+63)+47 and age >= 5.15) or name = 'sdfsdfsdfsdf' or "
                           "not age = id + age - 5*8 or name >= 'aaaaadddd') and id = 8 or id = 1 or age = 3.7 or "
                           "col1 = 1 or col2 = 1 or col3 = 1 or name = 'qwerty';",
                           "SELECT * FROM jj;"},
                          {answerFirst, "Success"}});
}

TEST(SERVER_TEST_SYN_STRESS, STRESS_TEST_INSERT) {
    TestUtils::kill();
    TestUtils::clear();
    TestUtils::run();
    TestUtils::checkRequests({{"CREATE TABLE jj(id INT NOT NULL , age float, name char(300), "
                               "col1 int NOT NULL, col2 int NOT NULL, col3 int NOT NULL, "
                               "col4 int NOT NULL, col5 int NOT NULL, col6 int NOT NULL, "
                               "col7 int NOT NULL, col8 int NOT NULL, col9 int NOT NULL, "
                               "col10 int NOT NULL, col11 int NOT NULL, col12 int NOT NULL);",
                               "Success"}});
    std::string answerFirst = "\nid |age     |name  |col1|col2|col3|col4|col5|col6|col7|col8|col9|col10|col11|col12|\n";
    std::vector<Request> allRequest;
    for (int i = 1; i < 999; i++) {
        std::string request = "INSERT INTO jj values(" + std::to_string(i) + ", 2.9, 'sfsf', ";
        answerFirst += std::to_string(i);
        for (int j = std::to_string(i).length(); j < 3; j++) {
            answerFirst += " ";
        }
        answerFirst += "|2.900000|'sfsf'|";
        for (int j = 0; j < 12; j++) {
            request += std::to_string(i);
            if (j != 11)
                request += ", ";
            answerFirst += std::to_string(i);
            for (int k = std::to_string(i).length(); k < 4; k++) {
                answerFirst += " ";
            }
            if (j > 8)
                answerFirst += " ";
            answerFirst += "|";
        }
        answerFirst += "\n";
        request += ");";
        allRequest.emplace_back(std::make_pair(request, "Success"));
    };
    TestUtils::checkRequests(allRequest);
    std::string answerSecond = answerFirst +
                               "999|7.500000|'dfdf'|999 |999 |999 |999 |999 |999 |999 |999 |999 |999  |999  |999  |\n";
    TestUtils::checkDrop({{"insert into jj values(999, 7.5, 'dfdf', 999, 999, 999, 999, 999, 999, "
                           "999, 999, 999, 999, 999, 999);",
                           "SELECT * FROM jj;"},
                          {answerFirst, answerSecond}});
}
