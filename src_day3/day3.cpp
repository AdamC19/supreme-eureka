
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


enum class States:int {
    FIND_INSTRUCT,
    FIND_FIRST_NUM,
    FIND_SECOND_NUM
};

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "#################\r\n";
    std::cout << "##### DAY 3 #####\r\n";
    std::cout << "#################\r\n";


    std::ifstream fin;

    if (argc < 2) {
        std::cout << "Need to supply a file path. Exiting...\r\n";
        return -1;
    }

    if (!file_exists(argv[1])) {
        std::cout << "File " << argv[1] << " does not exist. Exiting...\r\n";
        return -2;
    }

    std::cout << "Reading-in jumbled data...\r\n";
    fin.open(argv[1], std::ifstream::in);

    std::vector<char> mem;
    char c;
    while (fin.get(c)) {
        mem.push_back(c);
    }
    
    fin.close();
    std::cout << "Read-in complete.\r\n";
    
    std::string instruct = "mul(";
    std::string do_inst = "do()";
    std::string dont_inst = "don't()";

    std::vector<int> results;
    std::vector<int> pt2_results;
    const char* instruct_cstr = instruct.c_str();
    char* memdata = mem.data();
    int num_max_len = 3;
    char first_num[4];
    char second_num[4];
    States state = States::FIND_INSTRUCT;
    int i;
    bool enab = true;
    for(i = 0; i < mem.size(); ) {
       // std::cout << memdata[i] << std::endl; 

        if (std::memcmp(memdata + i, do_inst.c_str(), do_inst.length()) == 0) {
            enab = true;
            std::cout << "Detected do()..." << std::endl; 
            i += std::strlen(do_inst.c_str());
        } else if (std::memcmp(memdata + i, dont_inst.c_str(), std::strlen(dont_inst.c_str())) == 0) {
            std::cout << "Detected don't()..." << std::endl; 
            enab = false;
            i += std::strlen(dont_inst.c_str());
        } 

        switch (state) {
            default:
                state = States::FIND_INSTRUCT;
            case States::FIND_INSTRUCT: {
                if (std::memcmp(memdata + i, instruct_cstr, std::strlen(instruct_cstr)) == 0) {
                    std::memset(first_num, '\0', std::strlen(first_num));
                    state = States::FIND_FIRST_NUM;
                    i += std::strlen(instruct_cstr);
                } else {
                    i++;
                }
                break;
            }
            case States::FIND_FIRST_NUM: {
                if (memdata[i] < '0' || memdata[i] > '9') {
                    if (memdata[i] == ',') {
                        std::memset(second_num, '\0', std::strlen(second_num));
                        state = States::FIND_SECOND_NUM;
                        i++;
                    } else {
                        state = States::FIND_INSTRUCT;
                    }
                } else {
                    // else this is a number so just stay in this state
                    // but only if current number is length 3 or less.
                    int insert_at = std::strlen(first_num);
                    // std::cout << "\r\n insert_at = " << insert_at << std::endl;
                    if (insert_at >= num_max_len) {
                        state = States::FIND_INSTRUCT;
                    } else {
                        first_num[insert_at] = memdata[i];
                        // std::cout << "Inserted...\r\b";
                    }
                    i++;
                }
                break;
            }
            case States::FIND_SECOND_NUM: {
                if (memdata[i] < '0' || memdata[i] > '9') {
                    if (memdata[i] == ')') {
                        // PROCESS NUMBERS
                        int first = atoi(first_num);
                        int sec = atoi(second_num);
                        std::cout << "\r\nFound real command. " << first << "*" << sec << std::endl; 
                        results.push_back(first * sec);
                        if (enab) {
                            std::cout << "\r\nEnabled." << std::endl; 
                            pt2_results.push_back(first * sec);
                        }
                        i++;
                    }
                    state = States::FIND_INSTRUCT;
                } else {
                    // else this is a number so just stay in this state
                    // but only if current number is length 3 or less.
                    int insert_at = std::strlen(second_num);
                    if (insert_at >= num_max_len) {
                        state = States::FIND_INSTRUCT;
                    } else {
                        second_num[insert_at] = memdata[i];
                    }
                    i++;
                }
                break;
            }
        }
        //std::cout << "\r\n i = " << i << std::endl;
    }
    
    int part1 = 0;
    for (auto it = results.begin(); it != results.end(); ++it) {
        part1 += *it;
    }
    int part2 = 0;
    for (auto it = pt2_results.begin(); it != pt2_results.end(); ++it) {
        part2 += *it;
    }
    std::cout << "\r\nPART 1 ANSWER : " << part1 << std::endl;
    std::cout << "\r\nPART 2 ANSWER : " << part2 << std::endl;

    return 0;
}