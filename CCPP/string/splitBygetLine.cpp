#include <sstream>
#include <iostream>
#include <vector>

int main() {
    std::vector<std::string> strings;
    std::istringstream f("denmark;sweden;india;us");
    std::string s;    
    while (std::getline(f, s, ';')) {
        std::cout << s << std::endl;
        strings.push_back(s);
    }
}
