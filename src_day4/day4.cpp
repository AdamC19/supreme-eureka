
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


bool find_str_in_dir(const char* find, std::vector<std::vector<char>>& letters, std::vector<std::vector<int>>& keep, int start_x, int start_y, int dir_x, int dir_y) {

    int len = std::strlen(find);
    int x_len = letters[0].size();
    int y_len = letters.size();
    
    int x_extent = start_x + (dir_x * (len - 1));
    int y_extent = start_y + (dir_y * (len - 1));

    if (y_extent < 0 || y_extent >= y_len || x_extent < 0 || x_extent >= x_len) {
        // no possibility of the string existing in this direction
        return false;
    }

    int i = 0;
    int x = start_x;
    int y = start_y;
    bool found = true;
    while  (found && i < len) {
        found = (letters[y][x] == find[i]);
        i++;
        x += dir_x;
        y += dir_y;
    }

    if (found) {
        while (i > 0) {
            x -= dir_x;
            y -= dir_y;
            i--;
            keep[y][x] += 1; // increment up
        }
    }
    return found;
}


bool find_xmas(std::vector<std::vector<char>>& letters, std::vector<std::vector<int>>& keep, int start_x, int start_y) {
    int x_len = letters[0].size();
    int y_len = letters.size();

    if (start_x < 1 || start_x > x_len - 2 || start_y < 1 || start_y > y_len - 2) {
        return false;
    }

    std::string findstr = "MAS";
    const char* find = findstr.c_str();

    int x = start_x;
    int y = start_y;
    
    bool found_xmas = false;
    int corners_checked = 0;

    while (corners_checked < 4 && !found_xmas) {
        
        int x_dir = 0;
        int y_dir = 0;
        switch (corners_checked) {
            default:
                corners_checked = 0;
            case 0:
                x_dir = 1;
                y_dir = 1;
                break;
            case 1:
                x_dir = -1;
                y_dir = 1;
                break;
            case 2:
                x_dir = -1;
                y_dir = -1;
                break;
            case 3:
                x_dir = 1;
                y_dir = -1;
                break;
        }
        corners_checked++;
        
        std::pair<int, int> corner;
        corner.first = x - x_dir;
        corner.second = y - y_dir;
        if (find_str_in_dir(find, letters, keep, corner.first, corner.second, x_dir, y_dir)) {
            // there are only two valid options for the other "MAS" string

            // flip across X
            int opt1_x = corner.first + (x_dir * (std::strlen(find) - 1));
            int opt1_y = corner.second; // no change in y
            bool opt1 = find_str_in_dir(find, letters, keep, opt1_x, opt1_y, 0 - x_dir, y_dir);
            
            bool opt2 = false;
            if (!opt1) {
                // flip across Y
                int opt2_x = corner.first;
                int opt2_y = corner.second + (y_dir * (std::strlen(find) - 1));
                opt2 = find_str_in_dir(find, letters, keep, opt2_x, opt2_y, x_dir, 0 - y_dir);
            }
            if (opt1 || opt2) {
                found_xmas = true;
                corners_checked = 4; // we're done
            }
        }
    }

    if (found_xmas) {
        // increment keep array points
        keep[y][x] += 1;
        keep[y - 1][x - 1] += 1;
        keep[y - 1][x + 1] += 1;
        keep[y + 1][x - 1] += 1;
        keep[y + 1][x + 1] += 1;
    }
    return found_xmas;
}


void print_wordsearch(std::vector<std::vector<char>>& letters, std::vector<std::vector<int>>& keep) {
    std::cout << std::endl;
    int y = 0;
    int x = 0;
    for (auto row = letters.begin(); row != letters.end(); ++row) {
        x = 0;
        for (auto letter = (*row).begin(); letter != (*row).end(); ++letter) {
            if (keep[y][x] > 0)
                std::cout << *letter;
            else
                std::cout << ".";
            x++;
        }
        y++;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "$$$$$$$$$$$$$$$$$\r\n";
    std::cout << "$$$$$ DAY 4 $$$$$\r\n";
    std::cout << "$$$$$$$$$$$$$$$$$\r\n";


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

    std::vector<std::vector<char>> words;
    std::vector<std::vector<int>> keep;
    std::vector<std::vector<int>> keep_pt2;
    std::vector<char> row;
    std::vector<int> keep_row;
    std::vector<int> keep_row_pt2;

    char c;
    while (fin.get(c)) {
        if (c != '\n') {
            row.push_back(c);
            keep_row.push_back(0);
            keep_row_pt2.push_back(0);
        } else {
            words.push_back(row);
            keep.push_back(keep_row);
            keep_pt2.push_back(keep_row_pt2);
            row.clear();
            keep_row.clear();
            keep_row_pt2.clear();
        }
    }
    words.push_back(row);
    keep.push_back(keep_row);
    keep_pt2.push_back(keep_row_pt2);

    fin.close();

    // 
    // print the word search
    // 
    // std::cout << std::endl;
    // for (auto row = words.begin(); row != words.end(); ++row) {
    //     for (auto letter = (*row).begin(); letter != (*row).end(); ++letter) {
    //         std::cout << *letter;
    //     }
    //     std::cout << std::endl;
    // }

    std::string findstr = "XMAS";
    const char* find = findstr.c_str();

    
    int part1_result = 0;
    int part2_result = 0;

    int y;
    int x;
    int dirs_max = 8; // 8 directions to check
    for (y = 0; y < words.size(); y++) {
        std::vector<char> row_y = words[y];
        for (x = 0; x < row_y.size(); x++) {
            if (words[y][x] == 'X') {
                // PART 1 search
                int dirs_checked = 0;
                do {
                    bool found = false;
                    int mas_count = 0;
                    switch (dirs_checked)
                    {
                        case 0: // check N
                            found = find_str_in_dir(find, words, keep, x, y, 0, -1);
                            break;
                        
                        case 1: // check NE
                            found = find_str_in_dir(find, words, keep, x, y, 1, -1);
                            break;
                        
                        case 2: // check E
                            found = find_str_in_dir(find, words, keep, x, y, 1, 0);
                            break;
                        
                        case 3: // check SE
                            found = find_str_in_dir(find, words, keep, x, y, 1, 1);
                            break;
                        
                        case 4: // check S
                            found = find_str_in_dir(find, words, keep, x, y, 0, 1);
                            break;
                        
                        case 5: // check SW
                            found = find_str_in_dir(find, words, keep, x, y, -1, 1);
                            break;
                        
                        case 6: // check W
                            found = find_str_in_dir(find, words, keep, x, y, -1, 0);
                            break;
                        
                        case 7: // check NW
                            found = find_str_in_dir(find, words, keep, x, y, -1, -1);
                            break;
                        default:
                            dirs_checked = dirs_max; // get out!
                            break;
                    }

                    if (found) {
                        part1_result += 1;
                    }
                    dirs_checked++;
                } while(dirs_checked < dirs_max);

                dirs_checked = 0;
            }

            if (words[y][x] == 'A') {
                // PART 2 search
                if (find_xmas(words, keep_pt2, x, y)) {
                    part2_result += 1;
                }
            }
        }
    }

    print_wordsearch(words, keep);
    print_wordsearch(words, keep_pt2);
    
    std::cout << "\r\nPART 1 ANSWER : " << part1_result << std::endl;
    std::cout << "\r\nPART 2 ANSWER : " << part2_result << std::endl;

    // PART 2

    return 0;
}