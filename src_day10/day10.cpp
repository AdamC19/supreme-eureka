
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


bool check_for_trail(std::vector<std::vector<int8_t>>& map, int x, int y, int dir) {
    if (dir < 0) {
        return false;
    }
    int next_x = x;
    int next_y = y;

    if (dir == 0) {
        // check up
        if (y > 0) {
            // ok
            next_y = y - 1;
        } 
    }
    else if (dir == 1) {
        // check right
        if (x < (map[y].size() - 1)) {
            // ok
            next_x = x + 1;
        }
    } 
    else if (dir == 2) {
        // check down
        if (y < (map.size() - 1)) {
            // ok
            next_y = y + 1;
        }
    }
    else {
        // check left
        if (x > 0) {
            // ok
            next_x = x - 1;
        }
    }

    if (map[next_y][next_x] == (map[y][x] + 1)) {
        return true;
    }
    return false;
}

int walk_trail(std::vector<std::vector<int8_t>>& map, int x, int y, std::vector<std::vector<char>>& trails, int* rating) {
    trails[y][x] = map[y][x] + 48; // place marker on trail map

    if (map[y][x] == 9) {
        (*rating)++;
        return 0;
    }
    
    //std::cout << "\r\n(" << x << ", " << y << "): \r\n";
    if (check_for_trail(map, x, y, 0)) {
        //std::cout << "  UP...\r\n";
        walk_trail(map, x, y - 1, trails, rating);
    }
    if (check_for_trail(map, x, y, 1)) {
        //std::cout << "  RIGHT... \r\n";
        walk_trail(map, x + 1, y, trails, rating);
    }
    if (check_for_trail(map, x, y, 2)) {
        //std::cout << "  DOWN...\r\n";
        walk_trail(map, x, y + 1, trails, rating);
    }
    if (check_for_trail(map, x, y, 3)) {
        //std::cout << "  LEFT...\r\n";
        walk_trail(map, x - 1, y, trails, rating);
    }
    //std::cout << "\r\n";
    return 0;
}


class Trail {
private:
    int w;
    int h;
    std::vector<std::vector<char>> trailmap;
public:
    int x;
    int y;
    int score;
    int rating;
    Trail(int trailhead_x, int trailhead_y, std::vector<std::vector<int8_t>>& map) {
        x = trailhead_x;
        y = trailhead_y;
        std::vector<char> row;
        for (int i = 0; i < map.size(); i++) {
            for (int ii = 0; ii < map[i].size(); ii++) {
                row.push_back('.');
            }
            trailmap.push_back(row);
            row.clear();
        }
        int score_tmp = 0;
        rating = 0;
        walk_trail(map, x, y, trailmap, &rating);
        //score = score_tmp;
        score = 0;
        for (auto row = trailmap.begin(); row != trailmap.end(); row++) {
            for (auto c = row->begin(); c != row->end(); c++) {
                if ((*c) == '9' || (*c) == '^') {
                    score++;
                }
            }
        }
    }
    void printTrailmap() {
        for (auto row = trailmap.begin(); row != trailmap.end(); row++) {
            for (auto it = row->begin(); it != row->end(); it++) {
                std::cout << *it;
            }
            std::cout << "\r\n";
        }
        
    }
};




///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 10 =========\r\n";
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

    std::vector<std::vector<int8_t>> topo;
    std::vector<std::vector<char>> trailmap;

    std::vector<int8_t> row;
    std::vector<char> trail_row;

    char c;
    while (fin.get(c)) {
        if (c != '\n') {
            row.push_back(c - 48);
            trail_row.push_back('.');
        } else {
            topo.push_back(row);
            trailmap.push_back(trail_row);
            row.clear();
            trail_row.clear();
        }
    }
    topo.push_back(row);
    trailmap.push_back(trail_row);

    fin.close();

    for (auto line = topo.begin(); line !=  topo.end(); line++) {
        for (auto gubbin = line->begin(); gubbin != line->end(); gubbin++) {
            std::cout << (char)((*gubbin) + 48);
        }
        std::cout << "\r\n";
    }

    std::vector<Trail> trails;

    std::cout << "---------- PART 1 -----------\r\n";
    int x = 0;
    int y = 0;
    int part1 = 0;
    int part2 = 0;
    for (auto row = topo.begin(); row != topo.end(); row++) {
        x = 0;
        for (auto it = row->begin(); it != row->end(); it++) {
            if (topo[y][x] == 0) {
                std::cout << "Found trail at (" << x << ", " << y << ")\r\n";
                trails.push_back(Trail(x, y, topo));
                auto trail = trails.end() - 1;
                std::cout << "  Has score of " << trail->score << "\r\n";
                std::cout << "  Has RATING of " << trail->rating << "\r\n";
                trail->printTrailmap();
                part1 += trail->score;
                part2 += trail->rating;
            }
            x++;
        }
        y++;
    }

    std::cout << "Found " << trails.size() << " trails\r\n";
    std::cout << "PART 1 ANSWER: " << part1 << "\r\n";
    std::cout << "PART 2 ANSWER: " << part2 << "\r\n";

    return 0;

}