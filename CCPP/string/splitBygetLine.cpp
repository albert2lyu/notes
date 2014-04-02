#include <sstream>
#include <iostream>
#include <vector>

void split(const std::string& src, const std::string separate_character, vector<string> &strs)  
{
    int separate_characterLen = separate_character.size();
    int lastPosition = 0,index = -1;  
    while (-1 != (index = src.find(separate_character,lastPosition)))  
    {  
        strs.push_back(src.substr(lastPosition,index - lastPosition));  
        lastPosition = index + separate_characterLen;  
    }  
    std::string lastString = src.substr(lastPosition);
    if (!lastString.empty()) {
        strs.push_back(lastString);
    }
}


int main() {
    std::vector<std::string> strings;
    std::istringstream f("denmark;sweden;india;us");
    std::string s;    
    while (std::getline(f, s, ';')) {
        std::cout << s << std::endl;
        strings.push_back(s);
    }
}
