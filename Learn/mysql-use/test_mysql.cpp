//
// Created by larry on 23-6-17.
//
#include <mysql/mysql.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <ctime>


int insert(MYSQL *mysql) {
    // char buf[512], cur_time[55], szName[100];

    time_t the_time;
    time(&the_time);
    auto tm_ptr = gmtime(&the_time);

    printf("date: %02d-%02d-%02d\n",
           1900 + tm_ptr->tm_year, tm_ptr->tm_mon + 1, tm_ptr->tm_mday);
    printf("time: %02d:%02d:%02d\n",
           tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);

    char cur_time[55];
    sprintf(cur_time, "%02d-%02d-%02d %02d:%02d:%02d",
            1900 + tm_ptr->tm_year, tm_ptr->tm_mon + 1, tm_ptr->tm_mday,
            tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
    printf("cur_time: %s\n", cur_time);

    char buf[512];
    char szName[100] = "Tom";
    sprintf(buf, "insert into student(name,age,SETTIME) values(\'%s\', %d, \'%s\')",
            szName, 27, cur_time);
    auto r = mysql_query(mysql, buf);

    if (r) {
        printf("Insert data failure!\n");
        return 0;
    } else {
        printf("Insert data success!\n");
    }
    return 0;
}

int showTable(MYSQL *mysql) {
    // query
    const char *query = "select * from student";
    auto flag = mysql_real_query(mysql, query, (unsigned long) strlen(query));
    if (flag) {
        printf("Query failed!\n");
        return 0;
    } else {
        printf("[%s] made...\n", query);
    }

    auto res = mysql_store_result(mysql);
    MYSQL_ROW row;
    do {
        row = mysql_fetch_row(res);
        if (row == 0) break;
        for (int t = 0; t < mysql_num_fields(res); ++t) {
            printf("%s\t", row[t]);
        }
        printf("\n");
    } while (1);

    mysql_free_result(res);
    return 0;
}

int main() {

    MYSQL mysql;
    //MYSQL_RES *res;
    //MYSQL_ROW row;
    std::string query;

    mysql_init(&mysql);
    std::string passwd = "";
    // std::cin >> passwd;
    if (!mysql_real_connect(&mysql, "192.168.122.213", "tom", passwd.c_str(), "test", 0, NULL, 0)) {
        printf("Error connecting to mysql!\n");
    } else {
        printf("Connected Mysql successful!\n");
    }

    // insert
    insert(&mysql);
    std::cout << std::endl;
    showTable(&mysql);
    mysql_close(&mysql);

    return 0;
}