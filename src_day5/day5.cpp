
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


int sort_based_on_rules(std::vector<std::pair<int, int>>& rules, std::vector<int>& list, int start, int len) {

    if (len < 2) {
        return 0;
    }
    else if (len == 2) {
        // attempt to find current permutation
        int first = list[start];
        int second = list[start + 1];
        std::pair<int, int> perm(first, second);
        std::vector<std::pair<int, int>>::iterator found = std::find(rules.begin(), rules.end(), perm);

        if (found == rules.end()) {
            // not found, so it must go the other way (I really hope otherwise something weird is goin on)
            list[start] = second;
            list[start + 1] = first;
        }
    } 
    else {
        // find which should go first
        for (int test_first = start; test_first < start + len; test_first++) {
            int i = start;
            bool ok = true;
            while (ok && i < start + len) {
                if (i != test_first) {
                    std::pair<int, int> test(list[test_first], list[i]);
                    std::vector<std::pair<int, int>>::iterator found = std::find(rules.begin(), rules.end(), test);
                    if (found == rules.end()) {
                        // no rule for this sequence, so this number is not first
                        ok = false;
                    }
                }
                i++;
            }
            if (ok && i <= start + len) {
                int tmp = list[start];
                list[start] = list[test_first];
                list[test_first] = tmp;
                break;
            }
        }
        sort_based_on_rules(rules, list, start + 1, len - 1);
    }

    return 0;
}


void print_seq(std::vector<int> vect){
    for (auto it = vect.begin(); it != vect.end(); ++it) {
        if (it + 1 != vect.end())
            std::cout << *it << ",";
        else
            std::cout << *it << std::endl;
    }
}

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "%%%%%%%%%%%%%%%%%%%%\r\n";
    std::cout << "%%%%%% DAY 5 %%%%%%%\r\n";
    std::cout << "%%%%%%%%%%%%%%%%%%%%\r\n";


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
    int line_count = parseInput(lines, fin);
    fin.close();


    std::cout << "Parsing page order...\r\n";
    std::vector<std::pair<int, int>> page_rules;
    std::vector<int> page_order;
    std::vector<std::vector<int>> valid_page_lists;
    std::vector<std::vector<int>> bad_page_lists;
    int section = 0;

    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it) {
        if ((*it).length() < 1) {
            section++;
            continue;
        }

        if (section == 0) {
            // building page ordering
            std::vector<std::string> parts = split(*it, '|');
            if (parts.size() < 2) {
                continue;
            }

            int first = atoi(parts[0].c_str());
            int second = atoi(parts[1].c_str());
            std::pair<int, int> rule(first, second);
            page_rules.push_back(rule);

            // std::vector<int>::iterator found_first = std::find(page_order.begin(), page_order.end(), first);
            // if (found_first != page_order.end()) {
            //     page_order.insert(found_first + 1, second);
            // } else {
            //     std::vector<int>::iterator found_second = std::find(page_order.begin(), page_order.end(), second);
            //     if (found_second != page_order.end()) {
            //         page_order.insert(found_second, first);
            //     } else {
            //         // need to add both in specified order
            //     }
            // }

        } else {
            // analyze this list of pages
            std::vector<std::string> parts = split(*it, ',');
            std::vector<int> page_list;
            for(auto part = parts.begin(); part != parts.end(); ++part) {
                page_list.push_back(atoi((*part).c_str()));
            }

            std::vector<int>::iterator last_page = page_list.begin();
            bool good = true;
            for (auto page = page_list.begin() + 1; page != page_list.end(); ++page) {
                // check that this number is after last number
                std::pair<int, int> find_pair(*last_page, *page);
                std::vector<std::pair<int, int>>::iterator found_rule = std::find(page_rules.begin(), page_rules.end(), find_pair);
                if (found_rule == page_rules.end()) {
                    good = false;
                    break;
                }
                ++last_page;
            }
            if (good) {
                valid_page_lists.push_back(page_list);
            } else {
                bad_page_lists.push_back(page_list);
            }
        }
    }

    int part1_ans = 0;

    for (auto valid = valid_page_lists.begin(); valid != valid_page_lists.end(); ++valid) {
        if ( (*valid).size() % 2 != 0 ) {
            int i = (*valid).size() / 2;
            int mid_page_num = (*valid)[i];
            std::cout << "Middle page num of this valid seq is " << mid_page_num << std::endl;
            part1_ans += mid_page_num;
        }
    }

    std::cout << "\r\nPART 1 ANSWER : " << part1_ans << std::endl;


    // PART 2 ANALYSIS
    int part2_ans = 0;
    for (auto list = bad_page_lists.begin(); list != bad_page_lists.end(); ++list) {
        std::vector<int> ordered = *list;
        print_seq(ordered);
        sort_based_on_rules(page_rules, ordered, 0, ordered.size());

        if ( ordered.size() % 2 != 0 ) {
            int i = ordered.size() / 2;
            int mid_page_num = ordered[i];
            print_seq(ordered);
            std::cout << "Newly ordered seq middle page num = " << mid_page_num << std::endl;
            part2_ans += mid_page_num;
        }
    }

    std::cout << "\r\nPART 2 ANSWER : " << part2_ans << std::endl;


    return 0;

}