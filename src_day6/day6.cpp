
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


void printMap(std::vector<std::vector<char>>& map){
    std::cout << "\r\n======== BEGIN MAP ========" << std::endl;
    for(auto row = map.begin(); row != map.end(); ++row) {
        for(auto c = (*row).begin(); c != (*row).end(); ++c){
            std::cout << *c;
        }
        std::cout << std::endl;
    }
    std::cout << "===========================" << std::endl;
}

static std::map<char, std::pair<int, int>> dirs_glob = {
    std::make_pair('^', std::make_pair(0, -1)),
    std::make_pair('>', std::make_pair(1, 0)),
    std::make_pair('v', std::make_pair(0, 1)),
    std::make_pair('<', std::make_pair(-1, 0))
    // ,
    // std::make_pair('.', std::make_pair(0, 0)),
    // std::make_pair('#', std::make_pair(0, 0))
};

char rotate(char c) {
    switch (c) {
        case '^':
            return '>';
        case '>':
            return 'v';
        case 'v':
            return '<';
        case '<':
            return '^';
    }
    return c;
}

/**
 * Walk one step from the current map state. Returns true as long as we're still
 * on the map.
 */
std::pair<int, int> walkMap(std::vector<std::vector<char>>& map, std::pair<int, int> pos, std::map<char, std::pair<int, int>>& dirs) {
    std::pair<int, int> retval {-1, -1}; // just to indicate off board
    int row = pos.second;
    int col = pos.first;
    int max_row = map.size() - 1;
    int max_col = map[0].size() - 1;
    char c = map[row][col];

    std::map<char, std::pair<int, int>>::iterator found = dirs_glob.find(c);

    if (found != dirs_glob.end()) {
        int assess_row = row + (*found).second.second;
        int assess_col = col + (*found).second.first;
        if (assess_row <= max_row && assess_col <= max_col && assess_row >= 0 && assess_col >= 0) {
            if (map[assess_row][assess_col] == '.' || map[assess_row][assess_col] == 'X') {
                // able to move to this location
                map[row][col] = 'X';
                map[assess_row][assess_col] = c; // same direction as before
                retval.first = assess_col;
                retval.second = assess_row;
            } else {
                // just need to rotate ourselves 90 degrees.
                map[row][col] = rotate(c);

                // we didn't move, so just set retval coord to input coord.
                retval.first = col;
                retval.second = row;
            }
            
        } else {
            // this move would put us off the map. Replace marker with an X
            map[row][col] = 'X';
        }

    }
    return retval;
}


std::pair<int, int> walkMapPt2(std::vector<std::vector<char>>& map, std::pair<int, int> pos, char* guard) {
    
    int row = pos.second;
    int col = pos.first;
    std::pair<int, int> retval {col, row};
    int max_row = map.size() - 1;
    int max_col = map[row].size() - 1;
    char guard_c = *guard; //map[row][col];
    int next_row = row;
    int next_col = col;
    bool rotate_cmd = false;
    char dir_char = 'X'; // will be either '|', '-', or '+'
    char enter_dir = *guard;
    bool off_map = false;

    do {
        std::map<char, std::pair<int, int>>::iterator dir = dirs_glob.find(guard_c); // figure out 

        if (dir != dirs_glob.end()) {
            next_row = row + dir->second.second;
            next_col = col + dir->second.first;
            if (next_row <= max_row && next_col <= max_col && next_row >= 0 && next_col >= 0) {
                if (map[next_row][next_col] == '#' || map[next_row][next_col] == 'O') {
                    rotate_cmd = true;
                    dir_char = enter_dir;
                    guard_c = rotate(guard_c); // process rotation, updating guard_c
                } 
                else {
                    if (!rotate_cmd) {
                        if (guard_c == '^' || guard_c == 'v') {
                            dir_char = '|';
                        } else {
                            dir_char = '-';
                        }
                    } 
                    rotate_cmd = false;
                }
                
            } else {
                // this move would put us off the map
                off_map = true;
                rotate_cmd = false;
            }
        } else {
            rotate_cmd = false;
            off_map = true;
        }
    } while (rotate_cmd);

    if (!off_map) {
        map[row][col] = dir_char;
        //map[next_row][next_col] = guard_c; // same direction as before, or direction we rotated to
        *guard = guard_c; // desired next position's guard state
        retval.first = next_col;
        retval.second = next_row;
    } else {
        retval.first = -1;
        retval.second = -1;
    }
    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "^^^^^^^^^^^^^^^^^^^^\r\n";
    std::cout << "^^^^^^ DAY 6 ^^^^^^^\r\n";
    std::cout << "^^^^^^^^^^^^^^^^^^^^\r\n";


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

    std::vector<std::vector<char>> og_map;
    std::vector<char> row_tmp;
    char c;
    while (fin.get(c)) {
        if (c != '\n') {
            row_tmp.push_back(c);
        } else {
            og_map.push_back(row_tmp);
            row_tmp.clear();
        }
    }
    og_map.push_back(row_tmp);

    fin.close();


    std::vector<std::vector<char>> map(og_map);
    printMap(map);

    int init_col = 0;
    int init_row = 0;
    while (init_col < map[0].size() && init_row < map.size()) {
        auto it = std::find(map[init_row].begin(), map[init_row].end(), '^');
        if (it != map[init_row].end()) {
            init_col = std::distance(map[init_row].begin(), it);
            break;
        }
        it = std::find(map[init_row].begin(), map[init_row].end(), '>');
        if (it != map[init_row].end()) {
            init_col = std::distance(map[init_row].begin(), it);
            break;
        }
        it = std::find(map[init_row].begin(), map[init_row].end(), 'v');
        if (it != map[init_row].end()) {
            init_col = std::distance(map[init_row].begin(), it);
            break;
        }
        it = std::find(map[init_row].begin(), map[init_row].end(), '<');
        if (it != map[init_row].end()) {
            init_col = std::distance(map[init_row].begin(), it);
            break;
        }
        init_row++;
    }

    std::cout << "Walking the map...\r\n";
    std::pair<int, int> coord = std::make_pair(init_col, init_row);
    do {
        coord = walkMap(map, coord, dirs_glob);
    } while (coord.first >= 0 && coord.second >= 0);

    printMap(map);

    int part1 = 0;
    for (auto row = map.begin(); row != map.end(); row++) {
        for (auto c = row->begin(); c != row->end(); c++) {
            if (*c == 'X') {
                part1++;
            }
        }
    }


    std::cout << "PART 1 ANSWER: " << part1 << "\r\n";

    // PART 2

    std::cout << "===== BEGIN PART 2 =====\r\n";
    int part2 = 0;
    int skip_x = 0, skip_y = 0;
    char guard_init = og_map[init_row][init_col];
    switch(guard_init){
        case '^':
            skip_x = init_col;
            skip_y = init_row - 1;
            break;
        case '>':
            skip_x = init_col + 1;
            skip_y = init_row;
            break;
        case 'v':
            skip_x = init_col;
            skip_y = init_row + 1;
            break;
        case '<':
            skip_x = init_col - 1;
            skip_y = init_row;
            break;
    }

    std::cout << "Begin search for loops...\r\n";
    for (int y = 0; y < og_map.size(); y++) {
        for (int x = 0; x < og_map[y].size(); x++) {
            bool at_origin = (x == init_col && y == init_row);
            bool at_skip = (x == skip_x && y == skip_y);
            if (og_map[y][x] == '.' && map[y][x] == 'X' && !at_origin && !at_skip) {
                std::vector<std::vector<char>> map2(og_map);
                map2[y][x] = 'O'; // emplace obstacle
                char guard = map2[init_row][init_col];
                std::pair<int, int> coord = walkMapPt2(map2, std::make_pair(init_col, init_row), &guard); 
                bool loop = false;
                do {
                    coord = walkMapPt2(map2, coord, &guard);
                    
                    if (coord.first >= 0 && coord.second >= 0) {
                        // compare with value of the next guard position, which we don't yet occupy
                        if (guard == map2[coord.second][coord.first]) {
                            loop = true;
                        }
                    } else if (coord.first < -1 && coord.second < -1) {
                        loop = true;
                    }
                    
                } while (!loop && coord.first >= 0 && coord.second >= 0);

                if (loop) {
                    std::cout << "Loop detected with the obstacle at (" << x << ", " << y << ")\r\n";
                    //printMap(map2);
                    part2++;
                } 
                
            }
        }
    }
    
    std::cout << "PART 2 ANSWER: " << part2 << "\r\n";

    return 0;
}