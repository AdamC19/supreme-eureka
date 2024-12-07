
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

uint64_t evaluateEquation(std::vector<uint64_t> nums, uint64_t ops) {
    
    uint64_t result = nums[0];

    for (uint64_t i = 0; i < nums.size() - 1; i++) {
        uint64_t op = (ops >> i) & 1;

        // 0 means +, 1 means *
        if (!op) {
            result = result + ((uint64_t)nums[i + 1]);
        } else {
            result = result * ((uint64_t)nums[i + 1]);
        }
    }
    return result;
}


int findSolution(uint64_t ans, std::vector<uint64_t> nums) {
    int retval = 0;

    uint64_t ops = 0;

    while (ops < ((uint64_t)(1 << (nums.size() - 1)))) {
        // evaulate with current state
        // 0 means +, 1 means *
        uint64_t eval = evaluateEquation(nums, ops);
        if (eval == ans) {
            retval++;
        }
        ops++;
    }
    return retval;
}


uint64_t concat(uint64_t upper, uint64_t lower) {
    std::stringstream ss;
    ss << upper << lower;
    return atol(ss.str().c_str());
}


uint64_t evaluateEquationPart2(std::vector<uint64_t> nums, uint64_t ops) {
    uint64_t result = nums[0];

    for (uint64_t i = 0; i < nums.size() - 1; i++) {
        int op = (ops >> (2 * i)) & 0x03;
        switch (op){
            default:
            case 3:
                // invalid operation, do nothing, return
                return 0;
            case 0:
                // + op
                result = result + nums[i + 1];
                break;
            case 1:
                // * operator
                result = result * nums[i + 1];
                break;
            case 2:
                // || operator
                result = concat(result, nums[i + 1]);
                break;
        }
    }
    return result;
}

int findSolutionPart2(uint64_t ans, std::vector<uint64_t> nums) {
    int retval = 0;

    uint64_t ops = 0;
    while (retval == 0 && ops < (1 << (2*(nums.size() - 1)) ) ) {
        uint64_t eval = evaluateEquationPart2(nums, ops);
        if (eval == ans) {
            retval++;
        }
        ops++;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "&&&&&&&&&&&&&&&&&&&&&&\r\n";
    std::cout << "&&&&&&& DAY 7 &&&&&&&&\r\n";
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&\r\n";


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

    fin.close();


    // std::map<int, std::vector<int>> equations;
    int valid_count = 0;
    uint64_t part1 = 0;

    for (auto line = lines.begin(); line != lines.end(); ++line) {
        std::vector<std::string> parts = split(*line, ':');
        uint64_t result = atol(parts[0].c_str());
        std::vector<std::string> operand_strs = split(parts[1], ' ');

        std::vector<uint64_t> operands;

        for (auto part = operand_strs.begin(); part != operand_strs.end(); ++part) {
            if (part->length() > 0) {
                operands.push_back(atol(part->c_str()));
            }
        }
        int num_solutions = findSolution(result, operands);

        if (num_solutions > 0) {
            valid_count++;
            uint64_t last_sum = part1;
            part1 += result;
            if (part1 < last_sum) {
                std::cout << "Wrapped around! Part 1 sum was " << last_sum;
                std::cout << ", then we added " << result << " and now part1 = " << part1 << ".\r\n";
            }
        }

    }


    std::cout << "============= PART 1 =============\r\n";
    std::cout << "  VALID EQUATIONS         : " << valid_count << "\r\n";
    std::cout << "  TOTAL CALIBRATION RESULT: " << part1 << "\r\n";
    std::cout << "==================================\r\n";


    /* ====== PART 2 ===== */
    uint64_t part2 = 0;
    uint64_t valid_count_pt2 = 0;
    int max_prog = lines.size();
    int prog = 0;
    for (auto line = lines.begin(); line != lines.end(); ++line) {
        std::vector<std::string> parts = split(*line, ':');
        uint64_t result = atol(parts[0].c_str());
        std::vector<std::string> operand_strs = split(parts[1], ' ');

        std::vector<uint64_t> operands;

        for (auto part = operand_strs.begin(); part != operand_strs.end(); ++part) {
            if (part->length() > 0) {
                operands.push_back(atol(part->c_str()));
            }
        }
        std::cout << (int)((100.0*prog/max_prog) + 0.5) << "% complete. Evaluating \"" << *line << "\"...\r\n";
        int num_solutions = findSolutionPart2(result, operands);

        if (num_solutions > 0) {
            valid_count_pt2++;
            uint64_t last_sum = part2;
            part2 += result;
            if (part2 < last_sum) {
                std::cout << "Wrapped around! Part 2 sum was " << last_sum;
                std::cout << ", then we added " << result << " and now part1 = " << part2 << ".\r\n";
            }
        }
        prog++;
    }

    std::cout << "&&&&&&&&&&&&& PART 2 &&&&&&&&&&&&&\r\n";
    std::cout << "  VALID EQUATIONS         : " << valid_count_pt2 << "\r\n";
    std::cout << "  TOTAL CALIBRATION RESULT: " << part2 << "\r\n";
    std::cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\r\n";


    return 0;
}