
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


class Robot {
private:
    int m_init_x;
    int m_init_y;
    int m_x;
    int m_y;
    int m_vel_x;
    int m_vel_y;
public:
    Robot(int x, int y, int vel_x, int vel_y) {
        m_x = x;
        m_y = y;
        m_init_x = x;
        m_init_y = y;
        m_vel_x = vel_x;
        m_vel_y = vel_y;
    }
    void takeStep(int width, int height) {
        m_x += m_vel_x;
        m_y += m_vel_y;
        if (m_x < 0) {
            m_x = width + m_x;
        } else if (m_x >= width) {
            m_x = m_x - width;
        }

        if (m_y < 0) {
            m_y = height + m_y;
        } else if (m_y >= height) {
            m_y = m_y - height;
        }
    }

    std::pair<int, int> getCoord() {
        return std::make_pair(m_x, m_y);
    }
};


void takeStep(std::vector<std::vector<int>>& tiles, std::vector<Robot>& robots) {
    for (auto robot = robots.begin(); robot != robots.end(); ++robot) {
        auto coord = robot->getCoord();
        if (tiles[coord.first][coord.second] > 0)
            tiles[coord.first][coord.second] -= 1;

        robot->takeStep(tiles.size(), tiles[0].size());
        coord = robot->getCoord();
        tiles[coord.first][coord.second] += 1;
    }
}


void printRobots(std::vector<std::vector<int>>& tiles) {
    for (int y = 0; y < tiles[0].size(); y++) {
        for (int x = 0; x < tiles.size(); x++) {
            if (tiles[x][y] == 0) {
                std::cout << '.';
            } else {
                printf("%d", tiles[x][y]);
            }
        }
        std::cout << "\r\n";
    }
}


int checkForXmas(std::vector<std::vector<int>>& tiles) {
    int retval = 0;
    int left_count = 0;
    for (int y = 0; y < tiles[0].size(); y++) {
        int row_len = tiles.size();
        for(int x = 0; x < row_len/2; x++) {
            int left = tiles[x][y];
            int right = tiles[(row_len - 1) - x][y];
            if ((left != 0 && right == 0) || (left == 0 && right != 0)) {
                retval += 1;
            }
            left_count += left;
        }
    }
    return retval;
    //return left_count;
}


///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 14 =========\r\n";
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

    int width = 11;
    int height = 7;
    int seconds = 100;
    if (argc > 2) {
        width = atoi(argv[2]);
    }

    if (argc > 3) {
        height = atoi(argv[3]);
    }

    if (argc > 4) {
        seconds = atoi(argv[4]);
    }


    /* Generate tile field */
    std::vector<std::vector<int>> tiles;
    std::vector<int> col;
    for (int x_ind = 0; x_ind < width; x_ind++){
        for (int y_ind = 0; y_ind < height; y_ind++) {
            col.push_back(0);
        }
        tiles.push_back(col);
        col.clear();
    }

    std::cout << "Reading-in data...\r\n";
    fin.open(argv[1], std::ifstream::in);

    std::vector<std::string> lines;
    std::vector<Robot> robots;

    parseInput(lines, fin);

    for (auto line = lines.begin(); line != lines.end(); ++line) {
        auto parts = split(*line, ' ');
        auto pos_parts = split(parts[0], '=');
        auto pos_x_y = split(pos_parts[1], ',');
        int x = atoi(pos_x_y[0].c_str());
        int y = atoi(pos_x_y[1].c_str());

        auto vel_parts = split(parts[1], '=');
        auto vel_x_y = split(vel_parts[1], ',');
        int x_vel = atoi(vel_x_y[0].c_str());
        int y_vel = atoi(vel_x_y[1].c_str());
        
        Robot robot(x, y, x_vel, y_vel);
        auto coord = robot.getCoord();
        tiles[coord.first][coord.second] += 1;
        robots.push_back(robot);
    }

    fin.close();

    std::cout << "0 Seconds (initial state): " << std::endl;
    printRobots(tiles);
    
    for (int sec = 1; sec <= seconds; sec++) {
        takeStep(tiles, robots);
        std::cout << "After " << sec << " seconds: score = ";
        int xmas = checkForXmas(tiles);
        std::cout << xmas << std::endl;
        if (xmas < 250) {
            printRobots(tiles);
            std::string unused;
            std::getline(std::cin, unused);
        }
    }

    int quad1 = 0;
    int quad2 = 0;
    int quad3 = 0;
    int quad4 = 0;
    int x_midline = width / 2;
    int y_midline = height / 2;

    for (int x = 0; x < tiles.size(); x++) {
        for (int y = 0; y < tiles[x].size(); y++) {
            if (tiles[x][y] > 0) {
                if (x < x_midline && y < y_midline) {
                    quad1 += tiles[x][y];
                } else if (x < x_midline && y > y_midline) {
                    quad3 += tiles[x][y];
                } else if (x > x_midline && y < y_midline) {
                    quad2 += tiles[x][y];
                } else if (x > x_midline && y > y_midline) {
                    quad4 += tiles[x][y];
                }
            }
        }
    }

    int part1 = quad1 * quad2 * quad3 * quad4;

    std::cout << "PART 1 ANSWER: " << part1 << std::endl;

    return 0;

}