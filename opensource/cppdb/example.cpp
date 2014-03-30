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

void createTable(cppdb::session sql) 
{
    sql << "DROP TABLE IF EXISTS public.test" << cppdb::exec;
    sql << "DROP SEQUENCE IF EXISTS public.test_id_seq" << cppdb::exec;

    sql << "CREATE SEQUENCE public.test_id_seq" << cppdb::exec;

    sql<< "CREATE TABLE public.test ( "
        "   id   INTEGER NOT NULL DEFAULT nextval('public.test_id_seq'), "
        "   n    INTEGER,"
        "   f    REAL, "
        "   t    TIMESTAMP,"
        "   name TEXT "
        ")  " << cppdb::exec;

    printf("Created Table public.test\n");
}

void insertTest1(cppdb::session sql) 
{
    cppdb::statement st = sql <<
        "INSERT INTO public.test(n,f,t,name) "
        "VALUES(?,?,?,?)";
    std::time_t now_time = std::time(0);
    std::tm now = *std::localtime(&now_time);

    std::time_t start_time = std::time(0);
    int i = 0;
    for (i=0; i < MAX; i++) {
        st.bind(i);
        st.bind(i);
        now_time = std::time(0);
        now = *std::localtime(&now_time);
        st.bind(now);
        st.bind("CPPDB Test");
        st.exec();
        st.reset();
    }
    std::time_t end_time = std::time(0);
    printf("insert Test1 time: %lds\n", end_time - start_time);
}

void insertTest2(cppdb::session sql) 
{
    cppdb::statement stat;
    std::time_t start_time = std::time(0);
    std::time_t now_time = std::time(0);
    std::tm now = *std::localtime(&now_time);


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
    std::time_t end_time = std::time(0);
    printf("insert Test2 time: %lds\n", end_time - start_time);
}

std::string NumToStr(long long num)
{           
    std::ostringstream ss;
    ss << num;  
    return ss.str();
}               

void insertTestMultiRow() 
{
    cppdb::session sql("postgresql:host=127.0.0.1;dbname=stdb;user=border;password=border;@pool_size=20");
    cppdb::statement stat;
    std::time_t start_time = std::time(0);
    std::time_t now_time = std::time(0);
    std::tm now = *std::localtime(&now_time);

    std::string str = "INSERT INTO public.test(n,f,t,name) values ";

    int i = 0;
    for (i=0; i < MAX; i++) {
        now_time = std::time(0);
        now = *std::localtime(&now_time);
        if (i != MAX-1) {
            str += "(" + NumToStr(i) + ", " + NumToStr(i) + ", now()," + "'CPPPDB Test'" + "),";
        } else {
            str += "(" + NumToStr(i) + ", " + NumToStr(i) + ", now()," + "'CPPPDB Test'" + ")";
        }
    }
    stat = sql << str;
    stat.exec();
    std::time_t end_time = std::time(0);
    printf("insert insertTestMultiRow time: %lds\n", end_time - start_time);
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

int main()
{
    //cppdb::session sql("postgresql:host=127.0.0.1;dbname=stdb;user=border;password=border;@pool_size=10");
    try {
        printf("Connection postgresql DB\n");

        /*
        createTable(sql);

        std::time_t now_time = std::time(0);

        std::tm now = *std::localtime(&now_time);

        cppdb::statement stat;

        stat = sql << 
            "INSERT INTO public.test(n,f,t,name) "
            "VALUES(?,?,?,?)"
            << 10 << 3.1415926565 << now <<"Hello 'World'";

        stat.exec();

        std::cout<<"Affected rows "<<stat.affected()<<std::endl;

        stat.reset();

        stat.bind(20);
        stat.bind_null();
        stat.bind(now);
        stat.bind("Hello 'World'");
        stat.exec();

        cppdb::result res = sql << "SELECT id,n,f,t,name FROM public.test";

        while(res.next()) {
            double f=-1;
            int id,k;
            std::tm atime;
            std::string name;
            res >> id >> k >> f >> atime >> name;
            std::cout <<id << ' '<<k <<' '<<f<<' '<<name<<' '<<asctime(&atime)<< std::endl;
        }

        res = sql << "SELECT n,f FROM public.test WHERE id=?" << 1 << cppdb::row;
        if(!res.empty()) {
            int n = res.get<int>("n");
            double f=res.get<double>(1);
            std::cout << "The values are " << n <<" " << f << std::endl;
        }

        //insertTest1(sql);
        //insertTest2(sql);
        */

        for (int i=0; i<200; i++) {
            //std::thread th(insertTestMultiRow);
            insertTestMultiRow();
        }
        //insertTransactionTest2(sql);
    } catch(std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    for (int i=0; i<100; i++) {
        sleep(1000);
    }
    //sql.close();
    return 0;
}


