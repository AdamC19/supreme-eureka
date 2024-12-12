
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

class Stone {
private:
    int64_t number;
    Stone* next;
public:
    Stone() : Stone(0) {}

    Stone(int64_t num) {
        number = num;
        next = nullptr;
    }
    Stone(int64_t num, Stone* next_node) {
        number = num;
        next = next_node;
    }
    void setNumber(int64_t n) {
        number = n;
    }
    int64_t getNumber() {
        return number;
    }
    Stone* getNext() {
        return next;
    }
    void setNext(Stone* newNext) {
        this->next = newNext;
    }
};


class StoneList {
private:
    Stone* head;
    Stone* position; // current position in the list
public:
    StoneList() : head(nullptr), position(nullptr) { }

    // Append value to end of list
    void append(int64_t value) {
        if (head == nullptr) {
            head = new Stone(value);
            position = head;
            return;
        }
        Stone* tail = head;
        while (tail->getNext() != nullptr) {
            tail = tail->getNext();
        }
        tail->setNext(new Stone(value));
    }

    // Pre-pend value to beginning of list
    void prepend(int64_t value) {
        Stone* newHead = new Stone(value);
        newHead->setNext(head);
        head = newHead;
    }

    // insert at a position in the list
    void insertAt(int64_t value, int ind) {
        if (ind < 0) {
            return;
        }
        if (ind == 0) {
            this->prepend(value);
            return;
        }
        Stone* tail = head;
        int counter;
        for (counter = 0; counter < ind ; counter++) {
            if(tail->getNext() == nullptr) {
                break;
            } else {
                tail = tail->getNext();
            }
        }
        tail->setNext(new Stone(value, tail->getNext()));
    }

    /// @brief Returns the value stored at a zero-indexed positon in the list
    /// @param ind index in the list
    /// @return value of the stone at specified index, or the last element if ind exceeded the length of the list.
    int getValueAt(int ind) {
        if (ind < 0) {
            return 0;
        }
        int i = 0;
        Stone* tail = head;
        while (i < ind && tail->getNext() != nullptr) {
            tail = tail->getNext();
            i++;
        }
        return tail->getNumber();
    }

    void setValueAt(int64_t value, int ind) {
        if (ind < 0) {
            return;
        }
        int i = 0;
        Stone* tail = head;
        while (i < ind && tail->getNext() != nullptr) {
            tail = tail->getNext();
            i++;
        }
        
        tail->setNumber(value);
    }

    int size() {
        int retval = 1;
        Stone* tail = head;
        while(tail->getNext() != nullptr) {
            tail = tail->getNext();
            retval++;
        }
        return retval;
    }

    void printStones() {
        Stone* tail = head;
        while (tail != nullptr) {
            std::cout << tail->getNumber() << " ";
            tail = tail->getNext();
        }
        std::cout << std::endl;
    }

    int getValAtPosition() {
        return position->getNumber();
    }

    Stone* getPosition() {
        return position;
    }

    Stone* nextPosition() {
        position = position->getNext();
        return position;
    }

    void resetPosition() {
        position = head;
    }

    /// @brief Split the stone at the current postion. Current position gets left_val.
    /// right_val stone inserted after current postion, then postion is pointed to the 
    /// Stone that was next at the time before insertion of the right_val stone.
    /// @param left_val 
    /// @param right_val 
    void splitPosition(int left_val, int right_val) {
        Stone* currPos = position;
        currPos->setNumber(left_val);
        Stone* jumpToNode = position->getNext();
        currPos->setNext(new Stone(right_val, jumpToNode));
        position = jumpToNode;
    }
};


void processBlink(StoneList& stones) {
    int len = stones.size();
    std::cout << "Stone list is apparently " << len << " stones long\r\n";

    // std::string unused;
    // std::getline(std::cin, unused);

    stones.resetPosition();
    for (int i = 0; i < len; i++) {
        Stone* stone = stones.getPosition();

        if (stone == nullptr) {
            std::cout << "ERROR: Encountered null pointer before the end. Stone list may be corrupted...\r\n";
            break;
        }
        // =============
        // run the rules
        // =============
        // IF 0, make it 1
        if (stone->getNumber() == 0) {
            stone->setNumber(1);
            stones.nextPosition();
            continue;
        }
        // IF even number of digits, split
        static char scratch[16];
        static char left_buf[8];
        static char right_buf[8];
        snprintf(scratch, 16, "%ld", stone->getNumber());
        int digits = strlen(scratch);
        if ((digits % 2) == 0) {
            int new_len = digits / 2;
            memcpy(left_buf, scratch, new_len);
            memcpy(right_buf, scratch + new_len, new_len);
            left_buf[new_len] = '\0';
            right_buf[new_len] = '\0';
            int left_val = atoi(left_buf);
            int right_val = atoi(right_buf);
            stones.splitPosition(left_val, right_val); // this also jumps ahead to next position we need
            continue;
        }

        // When no above rules apply
        // multiply value by 2024
        stone->setNumber(stone->getNumber() * 2024);
        stones.nextPosition();
    }
}


void printIndent(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
}


// each number yields a fanout
// compute each digit's fanout for remaining number of blinks.
// obviously if we have computed it already, just look it up.
// digit == 0:
//      at 0 blinks, it yields 1 (    0     )
//      at 1 blinks, it yields 1 (    1     )
//      at 2 blinks, it yields 1 (   2024   )
//      at 3 blinks, it yields 2 ( 20    24 )
//      at 4 blinks, it yields 4 (2  0  2  4)

int64_t computeStones(std::map<int64_t, std::vector<int64_t>>& fanouts, int blinks_left, int64_t digit) {
    // see if we have knowledge of this many blinks for this digit
    std::cout << "computeStones(...); " << blinks_left << " blinks remaining; digit = " << digit << "\r\n";
    // if (blinks_left == 0) {
    //     return 1;
    // }
    auto found = fanouts.find(digit);
    if (found == fanouts.end()) {
        // no knowledge of this digit, need to compute and add it
        std::cout << "Digit " << digit << " not in fanout map yet. Creating vector for it...\r\n";
        std::vector<int64_t> tmp {1}; // at 0 blinks, the number of stones must be 1
        fanouts[digit] = tmp;
    }
    found = fanouts.find(digit);

    if (found != fanouts.end()) {
        if (blinks_left < found->second.size() && blinks_left >= 0 && found->second[blinks_left] > 0) {
            std::cout << "Lookup success! Returning pre-computed value (" << found->second[blinks_left] << ")...\r\n";
            return found->second[blinks_left];
        } else {
            int64_t left_val = -1;  // -1 is invalid
            int64_t right_val = -1; // so this will just stop 

            if (digit == 0) {
                std::cout << "0 --> 1\r\n";
                left_val = 1;
            } else {
                char scratch[32];
                char left_buf[16];
                char right_buf[16];
                snprintf(scratch, 32, "%lu", digit);
                int digits = strlen(scratch);
                if ((digits % 2) == 0) {
                    int new_len = digits / 2;
                    memcpy(left_buf, scratch, new_len);
                    memcpy(right_buf, scratch + new_len, new_len);
                    left_buf[new_len] = '\0';
                    right_buf[new_len] = '\0';
                    left_val = atol(left_buf);
                    right_val = atol(right_buf);
                } else {
                    left_val = digit * (int64_t)2024;
                }
            }

            int64_t retval = 0;
            if (left_val >= 0) {
                retval += computeStones(fanouts, blinks_left - 1, left_val);
            }
            if (right_val >= 0) {
                retval += computeStones(fanouts, blinks_left - 1, right_val);
            }
            //found->second[blinks_left] = retval;
            while (blinks_left >= found->second.size()) {
                found->second.push_back(0);
            }
            // if (blinks_left < found->second.size()) {
            //     std::cout << "Digit " << digit << ": Inserting " << retval << " at index = " << blinks_left << "....\r\n";
            //     found->second[blinks_left] = retval;
            // } 
            // else {
            //std::cout << "Digit " << digit << ": Inserting " << retval << " at index = " << blinks_left << "....\r\n";
            // std::cout << "Digit " << digit << ": Pushing " << retval << " to back of array....\r\n";
            found->second[blinks_left] = retval;
            // found->second.push_back(retval);
            //}
            return retval;
        }
        
    }

    // if (found == fanouts.end()) {
    return 0; // I don't think we can get here but I'll add this just in case 
} 



///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){

    std::cout << "==========================\r\n";
    std::cout << "========= DAY 11 =========\r\n";
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

    int blinks = 6;
    if (argc > 2) {
        blinks = atoi(argv[2]);
    }
    std::cout << "Blinks = " << blinks << std::endl;

    std::cout << "Reading-in data...\r\n";
    fin.open(argv[1], std::ifstream::in);

    //StoneList stones;
    std::vector<int64_t> init_stones;

    std::vector<std::string> lines;

    int line_count = parseInput(lines, fin);

    fin.close();

    std::cout << "---------- BEGIN -----------\r\n";

    std::vector<std::string> parts = split(lines[0], ' ');

    for (auto it = parts.begin(); it != parts.end(); it++) {
        int val = atoi((*it).c_str());
        //stones.append(val);
        init_stones.push_back((int64_t)val);
    }
    //stones.printStones();
    
    std::string unused;
    // std::getline(std::cin, unused);

    // for (int i = 0; i < blinks; i++) {
    //     processBlink(stones);
    //     if (((i + 1) % 5) == 0) {
    //         std::cout << "After " << i + 1 << " blinks we have " << stones.size() << " stones\r\n";
    //     }
    //     // stones.printStones();
    //     // std::cout << std::endl;
    //     // std::getline(std::cin, unused);
    // }
    int64_t ans = 0;
    std::map<int64_t, std::vector<int64_t>> fanouts;
    for (auto it = init_stones.begin(); it != init_stones.end(); it++) {
        std::cout << "Computing fanout for digit " << *it << " with " << blinks << "-many blinks...\r\n";
        int64_t stone_count = computeStones(fanouts, blinks, (int64_t)*it);
        std::cout << stone_count << std::endl;
        ans += stone_count;
    }

    std::cout << "  STONES AFTER BLINKING " << blinks << " TIMES: " << ans << "\r\n";

    // ===== PART 1 =====

    return 0;

}