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
                                  {"select * from t;",
                                   "\nid|name   |city   |age     |\n"
                                   "1 |'Vasya'|'Gorod'|7.500000|\n"
                                   "1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"insert into t1 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"insert into t1 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t1;",
                                   "\nid|name   |city   |age     |\n"
                                   "1 |'Vasya'|'Gorod'|7.500000|\n"
                                   "1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t;",
                               "\nid|name   |city   |age     |\n"
                               "1 |'Vasya'|'Gorod'|7.500000|\n"
                               "1 |'Vasya'|'Gorod'|7.500000|\n"},
                              {"select * from t1;",
                               "\nid|name   |city   |age     |\n"
                               "1 |'Vasya'|'Gorod'|7.500000|\n"
                               "1 |'Vasya'|'Gorod'|7.500000|\n"}});
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_NOT_YET_INSERTED_VALUES) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t2(id int, name char(255), city char(255), age float);", "Success"},
                                  {"begin;", "Success"},
                                  {"insert into t2 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t2;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"update t2 set id = 2;", "Success"},
                                  {"select * from t2;", "Success"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"},
                                  {"select * from t2;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"}};
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_INSERT_IN_ONE_TABLE_IN_DIFFERENT_TRANSACTIONS) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t3(id int, name char(255), city char(255), age float);", "Success"},
                                  {"begin;", "Success"},
                                  {"insert into t3 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t3;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"}};
    std::vector<Request> request2{{"select * from t3;", "Success"},
                                  {"begin;", "Success"},
                                  {"insert into t3 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"select * from t3;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"},
                                  {"select * from t3;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n1 "
                                   "|'Vasya'|'Gorod'|7.500000|\n"}};
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_DELETE_NOT_YET_INSERTED_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t4(id int, name char(255), city char(255), age float);", "Success"},
                              {"insert into t4 values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"insert into t4 values(2, 'Vas', 'Gorodok',  4.5);", "Success"},
                                  {"select * from t4;",
                                   "\nid|name   |city     |age     |\n"
                                   "1 |'Vasya'|'Gorod'  |7.500000|\n"
                                   "2 |'Vas'  |'Gorodok'|4.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"delete from t4 where id = 1;", "Success"},
                                  {"delete from t4 where id = 2;", "Success"},
                                  {"select * from t4;", "Success"},
                                  {"commit;", "Success"}};
    // в этом тесте предполагается, что делит у второго клиента выполнится во время работы первого и соответственно не
    // удаит значение с айди равным 2
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t4;",
                               "\nid|name |city     |age     |\n"
                               "2 |'Vas'|'Gorodok'|4.500000|\n"}});
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_IN_TWO_TRANSACTIONS) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t5(id int, name char(255), city char(255), age float);", "Success"},
                                  {"insert into t5 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"begin;", "Success"},
                                  {"update t5 set id = 3;", "Success"},
                                  {"select * from t5;",
                                   "\nid|name   |city   |age     |\n3 |'Vasya'|'Gorod'|7.500000|\n"}};
    std::vector<Request> request2{{"select * from t5;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"begin;", "Success"},
                                  {"update t5 set id = 3;", "Table is in use ERROR: 14"},
                                  {"select * from t5;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"},
                                  {"select * from t5;",
                                   "\nid|name   |city   |age     |\n3 |'Vasya'|'Gorod'|7.500000|\n"}};
    // должна быть ошибка, апдейт сразу в двух транзакциях
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_DIFFERENT_VALUES) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t6(id int, name char(255), city char(255), age float);", "Success"},
                              {"insert into t6 values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"insert into t6 values(2, 'Vas', 'Gorodok',  4.5);", "Success"},
                                  {"update t6 set id = 10 where id = 2;", "Success"},
                                  {"select * from t6;",
                                   "\nid|name   |city     |age     |\n"
                                   "1 |'Vasya'|'Gorod'  |7.500000|\n"
                                   "10|'Vas'  |'Gorodok'|4.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"delete from t6 where id = 10;", "Success"},
                                  {"select * from t6;",
                                   "\nid|name   |city   |age     |\n"
                                   "1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"update t6 set id = 9 where id = 1;", "Success"},
                                  {"select * from t6;", "\nid|name   |city   |age     |\n"
                                                        "9 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    // в этом тесте предполагается, что делит у второго клиента выполнится во время работы первого и соответственно не
    // удаит значение с айди равным 10 а также выполнятся апдейты в обоих транзакциях, т.к. они апдейтят разные поля
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t6;", "\nid|name   |city     |age     |\n"
                                                    "10|'Vas'  |'Gorodok'|4.500000|\n"
                                                    "9 |'Vasya'|'Gorod'  |7.500000|\n"}});
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_DELETE_INTO_TWO_TRANSACTIONS) {
    TestUtils::clear();
    Client client1;
    std::vector<Request> request1{{"create table t7(id int, name char(255), city char(255), age float);", "Success"},
                                  {"insert into t7 values(1, 'Vasya', 'Gorod',  7.5);", "Success"},
                                  {"begin;", "Success"},
                                  {"delete from t7 where id = 1;", "Success"},
                                  {"select * from t7;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"delete from t7 where id = 1;", "Table is in use ERROR: 14"},
                                  {"select * from t7;",
                                   "\nid|name   |city   |age     |\n1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request3{{"commit;", "Success"}, {"select * from t7;", "Success"}};
    // должна быть ошибка, делит сразу в двух транзакциях
    TestUtils::checkRequestsClient(request1, client1);
    Client client2;
    TestUtils::checkRequestsClient(request2, client2);
    TestUtils::checkRequestsClient(request3, client1);
}

TEST(SERVER_TEST_THREAD, THREAD_TEST_UPDATE_INTO_TWO_TRANSACTIONS_PARALLEL_TREADS) {
    TestUtils::clear();
    TestUtils::checkRequests({{"create table t8(id int, name char(255), city char(255), age float);", "Success"},
                              {"insert into t8 values(1, 'Vasya', 'Gorod',  7.5);", "Success"}});
    std::vector<Request> request1{{"begin;", "Success"},
                                  {"update t8 set id = 10 where id = 1;", "Success"},
                                  {"select * from t8;",
                                   "\nid|name   |city   |age     |\n"
                                   "10|'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    std::vector<Request> request2{{"begin;", "Success"},
                                  {"update t8 set id = 10 where id = 1;", "Table is in use ERROR: 14"},
                                  {"select * from t8;",
                                   "\nid|name   |city   |age     |\n"
                                   "1 |'Vasya'|'Gorod'|7.500000|\n"},
                                  {"commit;", "Success"}};
    // в этом тесте должна быть ошибка, апдейт сразу в двух транзакциях
    std::thread client1(TestUtils::checkRequests, request1);
    std::thread client2(TestUtils::checkRequests, request2);
    client1.join();
    client2.join();
    TestUtils::checkRequests({{"select * from t8;",
                               "\nid|name   |city   |age     |\n"
                               "10|'Vasya'|'Gorod'|7.500000|\n"}});
}
