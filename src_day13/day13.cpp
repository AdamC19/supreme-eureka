
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
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

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 13 =========\r\n";
    std::cout << "==========================\r\n";


    std::ifstream fin;

    if (argc < 2) {
        std::cout << "Need to supply a file path. Exiting...\r\n";
        return -1;
    }

    if (!file_exists(argv[1])) {
        std::cout << "File " << argv[1] << " does not exist. Exiting...\r\n";
        return -2;
    }

    std::cout << "Reading-in data...\r\n";
    fin.open(argv[1], std::ifstream::in);

    std::vector<std::string> lines;

    parseInput(lines, fin);

    double x, y, a0, a1, b0, b1;
    std::vector<std::pair<double, double>> presses;
    uint64_t part1 = 0;

    for (auto line = lines.begin(); line != lines.end(); line++) {
        if ((*line).size() < 1) {
            y += 10000000000000.0;
            x += 10000000000000.0;
            double big_b = (y - (a1*x/a0))/(b1 - (a1*b0/a0));
            double big_a = (x - b0*big_b)/a0;
            uint64_t a = (uint64_t)(big_a + 0.5);
            uint64_t b = (uint64_t)(big_b + 0.5);
            uint64_t x_result = (uint64_t)(((a0 * a) + (b0 * b)) + 0.5);
            uint64_t y_result = (uint64_t)(((a1 * a) + (b1 * b)) + 0.5);
            printf("A button presses: %.3f... (%ld)\r\n", big_a, a);
            printf("B button presses: %.3f... (%ld)\r\n", big_b, b);
            
            bool success = true;
            printf("X result = %ld ", x_result);
            if (x_result == ((uint64_t)(x + 0.5))) {
                printf("which == %.3f...\r\n", x);
            } else {
                printf("which =/= %.3f...\r\n", x);
                success = false;
            }

            printf("Y result = %ld ", y_result);
            if (y_result == ((uint64_t)(y + 0.5))) {
                printf("which == %.3f...\r\n", y);
            } else {
                printf("which =/= %.3f...\r\n", y);
                success = false;
            }

            if (success) {
                uint64_t cost = 3*a + b;
                std::cout << "Prize won at a cost of " << cost << " tokens\r\n\r\n";
                part1 += cost;
            } else {
                std::cout << "No result wins a prize.\r\n\r\n";
            }
            continue;
        }

        std::vector<std::string> parts = split(*line, ':');
        std::vector<std::string> eqn_parts = split(parts[1], ',');
        if (parts[0].compare("Button A") == 0) {
            // button A actions
            auto a0_eqn = split(eqn_parts[0], '+');
            a0 = atof(a0_eqn[1].c_str());
            auto a1_eqn = split(eqn_parts[1], '+');
            a1 = atof(a1_eqn[1].c_str());
        } else if (parts[0].compare("Button B") == 0) {
            // button B actions
            auto b0_eqn = split(eqn_parts[0], '+');
            b0 = atof(b0_eqn[1].c_str());
            auto b1_eqn = split(eqn_parts[1], '+');
            b1 = atof(b1_eqn[1].c_str());
        } else if (parts[0].compare("Prize") == 0) {
            auto x_loc_eqn = split(eqn_parts[0], '=');
            x = atof(x_loc_eqn[1].c_str());
            auto y_loc_eqn = split(eqn_parts[1], '=');
            y = atof(y_loc_eqn[1].c_str());
        }
    }

    fin.close();

    std::cout << "PART 1 ANSWER: " << part1 << std::endl;

    return 0;

}