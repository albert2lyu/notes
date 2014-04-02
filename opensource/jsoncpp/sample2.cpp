#include <fstream>
#include <string>
#include "json/json.h"

int main(int argc, char* argv[])
{
    std::ifstream ifs;
    ifs.open("sample2.json");

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(ifs, root, false))
    { return -1; }

    for (int i = 0; i < root.size(); ++i)
    {
        Json::Value temp_value = root[i];
        std::string strID = temp_value["id"].asString();
        std::string strCityID = temp_value["city_id"].asString();
        std::string strEmpty = temp_value["empty"].asString();
        std::cout << "id: " << strID << " CityID: " << strCityID << ", empty: '"<< strEmpty << "'" << std::endl;
    }
    return 0;
}
