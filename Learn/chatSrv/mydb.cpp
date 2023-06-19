//
// Created by larry on 23-6-18.
//
#include <cstdio>
#include<cstdlib>
#include <mysql/mysql.h>
#include <cstring>

const char *host = "192.168.122.213";
const char *user = "tom";
const char *passwd = "Confidence*1234";
const char *db = "test";

int conn_to_mysql(MYSQL *mysql) {
    mysql_init(mysql);
    if (!mysql_real_connect(mysql, host, user, passwd, db, 0, NULL, 0)) {
        printf("Failed to connect to mysql!\n");
        return -1;
    } else {
        printf("Connect to Mysql successfully~\n");
    }

    return 0;
}

int insert(const char szName[]) {
    MYSQL mysql;
    auto ret = conn_to_mysql(&mysql);
    if (ret == -1) {
        perror("fail to connect");
        // return -1;
    }

    char buf[512] = {0};
    sprintf(buf, "insert into qqnum(name) values(\'%s\')", szName);
    auto r = mysql_query(&mysql, buf);
    if (r) {
        printf("insert data failure~\n");
        return 0;
    } else {
        printf("insert data success~\n");
    }
    mysql_close(&mysql);
    return 0;
}

int isExist(const char szName[]) {
    MYSQL mysql;
    conn_to_mysql(&mysql);

    char buf[512] = {0};
    sprintf(buf, "select name from qqnum where name = \'%s\'", szName);
    if (mysql_query(&mysql, buf)) {
        mysql_close(&mysql);
        return -1;
    }

    auto result = mysql_store_result(&mysql);
    if (result == nullptr) {
        mysql_close(&mysql);
        return -1;
    }

    auto res = -1;
    MYSQL_ROW row;
    do {
        row = mysql_fetch_row(result);
        if (row == 0) break;
        for (int t = 0; t < mysql_num_fields(result); ++t) {
            printf("%s\t", row[t]);
            res = 1;
        }
        printf("\n");

    } while (1);
    mysql_free_result(result);
    mysql_close(&mysql);
    return res;
}


int showTable() {
    MYSQL mysql;
    conn_to_mysql(&mysql);

    const char *buf = "select * from qqnum";
    auto flag = mysql_real_query(&mysql, buf, strlen(buf));
    if (flag) {
        printf("Query failed!\n");
        return 0;
    } else {
        printf("[%s] made...\n", buf);
    }

    auto res = mysql_store_result(&mysql);
    do {
        auto row = mysql_fetch_row(res);
        if (row == 0) break;
        for (int t = 0; t < mysql_num_fields(res); ++t) {
            printf("%s\t", row[t]);
        }
        printf("\n");
    } while (1);

    mysql_free_result(res);
    mysql_close(&mysql);
    return 0;
}

//int main() {
//
////    insert("tom");
////
////    const char *name = "tom";
////    printf("is Existed %s %d\n", name, isExist(name));
////
////    const char *jack = "jack";
////    printf("is Existed %s %d\n", jack, isExist(jack));
//
//
//    showTable();
//    return 0;
//}