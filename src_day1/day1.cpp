
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "util.h"



///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "===============\r\n";
    std::cout << "==== Day 1 ====\r\n";
    std::cout << "===============\r\n";



    std::ifstream fin;

    if (argc < 2) {
        std::cout << "Need to supply a file path. Exiting...\r\n";
        return -1;
    }

    if (!file_exists(argv[1])) {
        std::cout << "File " << argv[1] << " does not exist. Exiting...\r\n";
        return -2;
    }

    fin.open(argv[1], std::ifstream::in);

    std::vector<std::string> lines;

    int line_count = parseInput(lines, fin);

    std::cout << "Line count: " << line_count << std::endl;

    std::vector<int> left;
    std::vector<int> right;

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
        std::vector<std::string> parts = split(*it, ' ');
        // std::cout << "Left : " << parts[0] << " | Right : " << parts[1] << std::endl;
        left.push_back(std::atoi(parts[0].c_str()));
        right.push_back(std::atoi(parts[1].c_str()));
    }

    // sort ascending
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    uint64_t part1_sum = 0;
    for (int i = 0; i < left.size(); i++) {
        part1_sum += abs(right[i] - left[i]);
    }

    std::cout << "PART 1 ANSWER : " << part1_sum << std::endl;

    // PART 2
    // foreach number in left list:
    //      - multiply with number of occurences in right list
    //      - sum

    std::map<int, int> finds;
    uint64_t part2_sum = 0;
    for (std::vector<int>::iterator it = left.begin(); it != left.end(); ++it) {
        
        int count = 0; // num of occurences in right list

        std::map<int, int>::iterator map_it = finds.find(*it); // see if we know the num of occurences already
        if (map_it != finds.end()) {
            // we've already looked this one up, so just refer to it
            count = finds.at(*it);
        } else {
            for (std::vector<int>::iterator right_it = right.begin(); right_it != right.end(); ++right_it) {
                if (*it == *right_it) {
                    count++;
                }
            }
            finds[*it] = count;
        }

        part2_sum += (*it) * count;
    }


    std::cout << "PART 2 ANSWER : " << part2_sum << std::endl;

    return 0;
}