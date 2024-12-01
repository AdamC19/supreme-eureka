#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdio>

#include "util.h"


int parseInput(std::vector<std::string> &lines, std::ifstream& fin) {
    while (!fin.eof()) {
        std::string s;
        std::getline(fin, s);
        lines.push_back(s);
    }
    return lines.size();
}


std::vector<std::string> split(std::string src, char delim) {
    std::vector<std::string> retval;
    std::stringstream ss(src);
    std::string tmp;

    while(std::getline(ss, tmp, delim)) {
        if (tmp.length() > 0)
            retval.push_back(tmp);
    }
    return retval;
}