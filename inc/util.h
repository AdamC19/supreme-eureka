#pragma once


#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <sys/stat.h>

inline bool file_exists(const std::string& name){
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

int parseInput(std::vector<std::string> &lines, std::ifstream& fin);

std::vector<std::string> split(std::string src, char delim);