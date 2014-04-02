#include <fstream>
#include <string>
#include "json/json.h"

int main(int argc, char* argv[])
{
    std::ifstream ifs;
    ifs.open("test.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    { return -1; }

    Json::Value& add_value = root["address"];
    Json::Value append_value;
    append_value["name"] = "append";
    append_value["email"] = "append@163.com";

    if (append_value["kvbian"].empty()) {
        std::cout << "[kvbian] is empty" << std::endl;
    }

    add_value.append(append_value);

    for (int i = 0; i < add_value.size(); ++i)
    {
        Json::Value temp_value = add_value[i];
        std::string strName = temp_value["name"].asString();
        std::string strMail = temp_value["email"].asString();
        std::cout << "name: " << strName << " email: " << strMail << std::endl;
    }

    Json::FastWriter writer;
    std::string json_append_file = writer.write(root);

    std::ofstream ofs;
    ofs.open("test_append.json");
    ofs << json_append_file;
    return 0;
}
