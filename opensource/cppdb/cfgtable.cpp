#include <sstream>
#include <ctime>
#include <unistd.h>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("%s sqlname\n", argv[0]);
        return 0;
    }
    std::string tables[] = {
        "beijing_beijing",
        "guangdong_dongguan",
        "guangdong_foshan",
        "guangdong_guangzhou",
        "guangdong_huizhou",
        "guangdong_shenzhen",
        "hainan_haikou",
        "hainan_hainanshenzhixiaxian",
        "hainan_sanya",
        "hebei_zhangjiakou",
        "hubei_xiangfan",
        "jiangsu_nantong",
        "jiangsu_suzhou",
        "jiangsu_wuxi",
        "jiangsu_xuzhou",
        "liaoning_dalian",
        "liaoning_shenyang",
        "shanghai_shanghai",
        "shanxi_jinzhong",
        "sichuan_chengdu",
        "tianjin_tianjin",
        "xizang_lasa",
        "xizang_linzhi",
        "xizang_naqu",
        "xizang_rikaze",
        "xizang_shannan",
        "yunnan_kunming",
        "zhejiang_jiaxing",
        "zhejiang_ningbo"
    };
    std::string infilename = argv[1];
    std::string outfilename = infilename + ".new.sql";
    std::ifstream file (infilename);
    if (! file.is_open()){
    	std::cout << "Error opening file";
    	exit(1);
    }
    std::string sqlstr;
    std::string buffer;
    while (!file.eof() ) {
    	std::getline(file, buffer);
        sqlstr.append(buffer + "\n");
    }
    //printf("%s\n", sqlstr.c_str());
    int size_table = sizeof( tables ) / sizeof( tables[ 0 ] );

    std::ofstream ofs;
    ofs.open (outfilename, std::ofstream::out | std::ofstream::app);
    for (int i=0; i<size_table; i++) {
        std::string sq = sqlstr;
        printf("%s\n", tables[i].c_str());
        replaceAll(sq, "{{CITY_NAME}}", tables[i].c_str());
        printf("%s\n", sq.c_str());
        //sql << sq.c_str() << cppdb::exec;
        printf("%s Done\n", tables[i].c_str());
        ofs << sq;
    }
    ofs.close();
    return 0;
}

