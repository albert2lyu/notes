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

const char* CONNECTION = "postgresql:host=db-rd-2013lbs10.db01.baidu.com;port=8999;dbname=test;user=stadmin;password=stadmin;@pool_size=20";

void select_query(cppdb::session sql)
{
    cppdb::result res = sql << "SELECT name, signup_date FROM user_tbl limit 1";

    while(res.next()) {
        std::tm atime;
        std::string name;
        res >> name >> atime;
        std::cout << name << ' ' << asctime(&atime) << std::endl;
    }
}

void select_query()
{
    cppdb::session sql(CONNECTION);
    select_query(sql);
    sql.close();
}

void updateTest(cppdb::session sql) 
{
    cppdb::statement stat;
    stat = sql << 
        "update user_tbl set name = 'border' ";
    stat.exec();
}

void updateTest() 
{
    cppdb::session sql(CONNECTION);
    updateTest(sql);
}

void insertTest(cppdb::session sql) 
{
    cppdb::statement stat;
    stat = sql << 
        "INSERT INTO user_tbl(name, signup_date) "
        "VALUES(?,?)"
        << "test" << "now()";
    stat.exec();
}

void insertTest() 
{
    cppdb::session sql(CONNECTION);
    insertTest(sql);
}

void deleteTest(cppdb::session sql) 
{
    cppdb::statement stat;
    stat = sql << 
        "delete from user_tbl";
    stat.exec();
}

void deleteTest() 
{
    cppdb::session sql(CONNECTION);
    deleteTest(sql);
}



int main(int argc, char **argv)
{
    int count = 1;
    if (argc == 2) {
        count = atoi(argv[1]);
    }
    try {
        std::cout << "CONNECTION: " << CONNECTION << std::endl;

        cppdb::session sql(CONNECTION);
        for (int i=0; i<count; i++) {
            //deleteTest();
            //insertTest(sql);
            //updateTest(sql);
            select_query();
            sleep(1);
            std::cout << "Count: " << i << std::endl;
        }
    } catch(std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}


