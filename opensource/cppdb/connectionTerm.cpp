//                                                                             
//  Copyright (C) 2010-2011  Artyom Beilis (Tonkikh) <artyomtnk@yahoo.com>     
//                                                                             
//  Distributed under:
//
//                   the Boost Software License, Version 1.0.
//              (See accompanying file LICENSE_1_0.txt or copy at 
//                     http://www.boost.org/LICENSE_1_0.txt)
//
//  or (at your opinion) under:
//
//                               The MIT License
//                 (See accompanying file MIT.txt or a copy at
//              http://www.opensource.org/licenses/mit-license.php)
//

#include <cppdb/frontend.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <thread>

const static int MAX = 10 * 10;

const char* CONNECTION = "postgresql:@pool_size=10;@blob=bytea;@pool_max_idle=500;host=cq02-lbs-ssd2-map02.cq02.baidu.com;port=8999;dbname=stdb;user=stadmin;password=stpostgis0807";

const char* SQL_STR = "select id, name, type, refid, checkid from sa.node_info where checkid is not null order by random() LIMIT 10";

std::string NumToStr(long long num)
{           
    std::ostringstream ss;
    ss << num;  
    return ss.str();
}               

void insertTransactionTest2(cppdb::session sql) 
{
    cppdb::statement stat;
    std::time_t start_time = std::time(0);
    std::time_t now_time = std::time(0);
    std::tm now = *std::localtime(&now_time);

    cppdb::transaction guard(sql);

    int i = 0;
    for (i=0; i < MAX; i++) {
        now_time = std::time(0);
        now = *std::localtime(&now_time);
        stat = sql << 
            "INSERT INTO public.test(n,f,t,name) "
            "VALUES(?,?,?,?)"
            << i << i << now <<"CPPDB Test'";
        stat.exec();
    }
    guard.commit();
    std::time_t end_time = std::time(0);
    printf("Insert Transaction Test2 time: %lds\n", end_time - start_time);
}

void select(cppdb::session &sql)
{
    cppdb::result res = sql << SQL_STR;
    std::cout << SQL_STR << std::endl;
    while(res.next()) {
        std::string id, name, type, refid, checkid;
        res >> id >> name >> type >> refid >> checkid;
        std::cout << id << ", " << name << ',' << type << "," << refid << "," << checkid << std::endl;
    }
}

void select()
{
    cppdb::session sql(CONNECTION);
    cppdb::result res = sql << SQL_STR;
    std::cout << SQL_STR << std::endl;
    while(res.next()) {
        std::string id, name, type, refid, checkid;
        res >> id >> name >> type >> refid >> checkid;
        std::cout << id << ", " << name << ',' << type << "," << refid << "," << checkid << std::endl;
    }
}

int main(int argc, char **argv)
{
    cppdb::session sql(CONNECTION);
    int count = 1;
    if (argc == 2) {
        count = atoi(argv[1]);
    }
    try {
        printf("Connection postgresql DB\n");

        std::cout << "CONNECTION: " << CONNECTION << std::endl;

#if 0
        //select();
        sleep(60 * 15);
        //select();
#endif

#if 1
        cppdb::transaction guard(sql);
        select(sql);
        sleep(60 * 15);
        select(sql);
        guard.commit();
#endif

        //insertTransactionTest2(sql);
    } catch(std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    //sql.close();
    return 0;
}


