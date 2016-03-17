#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


int main(int argc, char const *argv[]) {
    std::string path = argv[1];

    std::ifstream file(path.c_str(),std::ios::in);
    if (!file.is_open())
        std::cerr << "Could not open "+path;
    else {
        std::string line;
        std::set<unsigned int> tmp;
        while (getline(file,line)) {
            std::stringstream lineStream(line);
            unsigned int buffer;
            while(lineStream >> buffer) 
                tmp.insert(buffer);
            for (std::set<unsigned int>::iterator it = tmp.begin(); it != tmp.end(); ++it) 
                std::cout << *it << " ";
            std::cout << std::endl;
        }
        file.close();
    }
    return 0;
}
