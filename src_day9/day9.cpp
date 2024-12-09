
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


void printDisk(std::vector<int>& disk) {
    int col = 0;
    for (auto it = disk.begin(); it != disk.end(); it++) {
        if (*it >= 0)
            std::cout << *it;
        else
            std::cout << ".";

        col++;
        if (col >= 80) {
            std::cout << std::endl;
            col = 0;
        }
    } 
    std::cout << std::endl;
}

/**
 * Returns index of first free space in disk.
 * If no free space, returns -1
 */
int findFreeSpace(std::vector<int>& disk) {
    auto it = std::find(disk.begin(), disk.end(), -1);
    if (it == disk.end()) {
        return -1; // no more free space
    }
    return std::distance(disk.begin(), it);
}


/**
 * Returns a pair of the starting index of the right-most file, 
 * and the size of the file in blocks.
 */
std::pair<int, int> findRightmostFile(std::vector<int>& disk, int start) {
    // find first non-free block starting from the right
    std::pair<int, int> retval(0, 0);

    int j = start;
    while (j >= 0 && disk[j] < 0) {
        j--;
    }

    if (j < 0) {
        return retval;
    }
    int file_id = disk[j];
    int i = j;
    while (i >= 0 && disk[i] == file_id) {
        i--;
    }
    retval.second = j - i;
    retval.first = i + 1;
    return retval;
}

/**
 * Finds a sufficiently sized free block of space. Returns the start
 * index of said block.
 */
int findFreeBlock(std::vector<int>& disk, int len, int max) {
    int retval = max;
    if (len <= 0) {
        return retval;
    }
    int i = 0;
    int found_len = 0;
    while(i < max && found_len < len) {
        if (disk[i] < 0) {
            found_len ++;
        } else {
            found_len = 0;
        }
        i++;
    }
    if (found_len == len && i <= max) {
        retval = i - found_len;
    }
    return retval;
}


void moveFile(std::vector<int>& disk, std::pair<int, int> file, int dest) {
    int dest_ind = dest;
    int src_ind = file.first;
    for (int i = 0; i < file.second; i++) {
        disk[dest_ind] = disk[src_ind];
        disk[src_ind] = -1;
        dest_ind++;
        src_ind++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "(((((((((((((((((((((((((\r\n";
    std::cout << "((((((((( DAY 9 (((((((((\r\n";
    std::cout << "(((((((((((((((((((((((((\r\n";


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

    std::vector<int8_t> disk_map;
    char c;
    while (fin.get(c)) {
        disk_map.push_back((int8_t)((c - 48) & 0x0F));
    }

    fin.close();

    std::cout << "Unravelling the disk map...\r\n";
    std::vector<int> disk_og;

    int id = 0;
    for (auto it = disk_map.begin(); it != disk_map.end(); it++) {
        int file_blocks = *it;

        // add file blocks of current ID
        for (int i = 0; i < file_blocks; i++) {
            disk_og.push_back(id);
        }

        it++;
        if (it == disk_map.end()) {
            break;
        }
        int free_space = *it;
        // add empty space blocks of -1
        for (int i = 0; i < free_space; i++) {
            disk_og.push_back(-1);
        }

        id++;
    }
    std::cout << "  Disk is " << disk_og.size() << "blocks in size.\r\n";
    // printDisk(disk_og);

    // ===== PART 1 =====
    std::cout << "\r\n==== Starting PART 1 ====\r\n";
    std::cout << "Compacting the disk...\r\n";
    std::vector<int> disk(disk_og);
    int i = disk.size() - 1; // start at the end

    int free_space_i = findFreeSpace(disk);
    while (free_space_i < i && i >= 0) {
        
        if (disk[i] >= 0) {
            disk[free_space_i] = disk[i];
            disk[i] = -1;
        }
        
        i--;

        free_space_i = findFreeSpace(disk); // find the first free space

    }
    std::cout << "  Done.\r\n";
    // printDisk(disk);

    // compute checksum
    uint64_t part1 = 0;
    for (int i = 0; i < disk.size() && disk[i] >= 0; i++) {
        part1 += (uint64_t)disk[i] * (uint64_t)i;
    }


    std::cout << "\r\n((((((((( PART 1 ANSWER (((((((((\r\n";
    std::cout << "  CHECKSUM: " << part1 << std::endl;


    // ===== PART 2 =====
    std::cout << "\r\n==== Starting PART 2 ====\r\n";
    std::vector<int> disk_pt2(disk_og);
    
    i = disk_pt2.size() - 1; // start at the end
    while (i >= 0) {
        // Searching for file with max index of i
        std::pair<int, int> file_loc = findRightmostFile(disk_pt2, i);
        if (file_loc.second == 0 && file_loc.first == i) {
            break;
        }
        //std::cout << "Found file at " << file_loc.first << " of size " << file_loc.second << " blocks\r\n";
        int j = findFreeBlock(disk_pt2, file_loc.second, file_loc.first);
        if (j < file_loc.first) {
            //std::cout << "  Moving file into free block at " << j << "\r\n";
            // move the file into free block
            moveFile(disk_pt2, file_loc, j);
        }
        i = file_loc.first - 1;
        
        //printDisk(disk_pt2);
    }
    //printDisk(disk_pt2);
    
    // compute checksum
    uint64_t part2 = 0;
    for (int i = 0; i < disk_pt2.size(); i++) {
        if (disk_pt2[i] >= 0) {
            part2 += (uint64_t)disk_pt2[i] * (uint64_t)i;
        }
    }

    std::cout << "\r\n((((((((( PART 2 ANSWER (((((((((\r\n";
    std::cout << "  CHECKSUM: " << part2 << std::endl;

    return 0;

}