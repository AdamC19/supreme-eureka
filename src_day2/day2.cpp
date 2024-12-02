
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


bool verify_report(std::vector<int>& report, int skip_ind) {
    std::vector<int>::iterator level = report.begin();
    int ind = 1;
    if (skip_ind == 0) {
        ++level;
        ind++;
    }
    int last_level = *level;
    int last_sign = 0;
    int last_delta = 0;
    int sign = 0;
    ++level;
    for( ; level != report.end(); ++level) {
        if (ind++ != skip_ind) {
            int delta = *level - last_level;
            
            sign = (delta >= 0 ? 1 : -1);
            if (last_sign == 0) {
                last_sign = sign;
            } else {
                if (sign != last_sign) {
                    return false;
                }
            }
            int delta_mag = abs(delta);
            if (delta_mag < 1 || delta_mag > 3) {
                return false;
            }

            last_sign = sign;
            last_level = *level;
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "@@@@@@@@@@@@@@@@@\r\n";
    std::cout << "@@@@@ DAY 2 @@@@@\r\n";
    std::cout << "@@@@@@@@@@@@@@@@@\r\n";


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

    int safe_reports = 0;
    int part2_sum = 0;
    
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
        std::vector<std::string> parts = split(*it, ' ');
        std::vector<int> report;
        for(std::vector<std::string>::iterator part = parts.begin(); part != parts.end(); ++part) {
            int n = atoi((*part).c_str());
            std::cout << " " << n;
            report.push_back(n);
        }
        if (verify_report(report, -1)) {
            safe_reports++;
            part2_sum++;
            std::cout << " Safe without removing any level";
        } else {
            int good_omit = 0;
            // attempt to omit 1 index and only 1 at a time
            for(int omit = 0; omit < report.size(); omit++) {
                if (verify_report(report, omit)) {
                    std::cout << " Safe by removing level " << omit << " ";
                    good_omit = 1;
                    part2_sum++;
                    break;
                }
            }
            if (good_omit != 1) {
                std::cout << " Unsafe";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "PART 1 ANSWER : " << safe_reports << std::endl;
    std::cout << "PART 2 ANSWER : " << part2_sum << std::endl;

    return 0;
}