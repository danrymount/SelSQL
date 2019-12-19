#include <gtest/gtest.h>
#include <thread>
#include "../../Client/Client.h"
#include "../Headers/TestUtils.h"

#define Request std::pair<std::string, std::string>

TEST(SERVER_TEST_THREAD, THREAD_TEST_INSERT_IN_DIFFERENT_THREADS_IN_DIFFERENT_TABLES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                              {"create table t1(id int, name char(255), city char(255), age float);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t;", ""},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"insert into t1 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"insert into t1 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t1;", ""},
                                  {"commit;", "Success"}};
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t;", ""}, {"select * from t1;", ""}});
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_NOT_YET_INSERTED_VALUES) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                                  {"begin;", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"update t set id = 2;", "Success"},
                                  {"select * from t;", "Success"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"}};
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_INSERT_IN_ONE_TABLE_IN_DIFFERENT_TRANSACTIONS) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                                  {"begin;", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"}};
    std::vector<Request> request2{{"select * from t;", "Success"},
                                  {"begin;", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n1 "
                                   "|'Vasya'|'Gorod'|7.500000|\n"}};
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_DELETE_NOT_YET_INSERTED_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                              {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"insert into t values(2, 'Vas', 'Gorodok',  4.5);", "Success"},
                                  {"select * from t;", ""},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"delete from t where id = 1;", "Success"},
                                  {"delete from t where id = 2;", "Success"},
                                  {"select * from t;", ""},
                                  {"commit;", "Success"}};
    // в этом тесте предполагается, что делит у второго клиента выполнится во время работы первого и соответственно не
    // удаит значение с айди равным 2
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t;", ""}});
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_IN_TWO_TRANSACTIONS) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"begin;", "Success"},
                                  {"update t set id = 3;", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n3 |'Vasya'|'Gorod'|7.500000|\n"}};
    std::vector<Request> request2{{"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"begin;", "Success"},
                                  {"update t set id = 3;", "Table is in use ERROR: 14"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n3 |'Vasya'|'Gorod'|7.500000|\n"}};
    // должна быть ошибка, апдейт сразу в двух транзакциях
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_DIFFERENT_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                              {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"insert into t values(2, 'Vas', 'Gorodok',  4.5);", "Success"},
                                  {"update t set id = 10 where id = 2;", "Success"},
                                  {"select * from t;", ""},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},    {"delete from t where id = 10;", "Success"},
                                  {"select * from t;", ""}, {"update t set id = 9 where id = 1;", "Success"},
                                  {"select * from t;", ""}, {"commit;", "Success"}};
    // в этом тесте предполагается, что делит у второго клиента выполнится во время работы первого и соответственно не
    // удаит значение с айди равным 10 а также выполнятся апдейты в обоих транзакциях, т.к. они апдейтят разные поля
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t;", ""}});
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_DELETE_INTO_TWO_TRANSACTIONS) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                                  {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"begin;", "Success"},
                                  {"delete from t where id = 1;", "Success"},
                                  {"select * from t;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"delete from t where id = 1;", "Table is in use ERROR: 14"},
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"}, {"select * from t;", "Success"}};
    // должна быть ошибка, делит сразу в двух транзакциях
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_INTO_TWO_TRANSACTIONS_PARALLEL_TREADS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t(id int, name char(255), city char(255), age float);", "Success"},
                              {"insert into t values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"update t set id = 10 where id = 1;", "Success"},
                                  {"select * from t;", ""},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"update t set id = 10 where id = 1;", "Success"},
                                  {"select * from t;", ""},
                                  {"commit;", "Success"}};
    // в этом тесте должна быть ошибка, апдейт сразу в двух транзакциях
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t;", ""}});
}
