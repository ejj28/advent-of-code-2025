#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct range {
    long long rangeStart, rangeEnd;
};

std::ifstream input;

std::vector<range> ranges;


void consolidateRanges() {

    bool erased;
    int r1 = 0;

    while (r1 < ranges.size()) {
        
        erased = false;

        // Check every other range for a collision
        // This only checks for instances where r1 starts before r2, but all the ranges are checked agaist each other in both directions at some point so it works
        for (int r2 = 0; r2 < ranges.size(); r2++) {
            
            if (r1 == r2) continue;

            // Check if r1 ends somewhere inside the r2 range
            if ((ranges[r1].rangeEnd >= ranges[r2].rangeStart) && (ranges[r1].rangeEnd <= ranges[r2].rangeEnd)) {
                
                if (ranges[r1].rangeStart < ranges[r2].rangeStart) {
                    // Consolidate into r2 if r1 starts before it does
                    ranges[r2].rangeStart = ranges[r1].rangeStart;   
                }

                // Delete r1
                ranges.erase(ranges.begin() + r1);

                erased = true;
                break;

            }

        }

        if (!erased) r1++;
        
    }
}

int main() {
    
    std::string line;

    int pt1Count = 0;
    long long pt2Count = 0;

    input.open("day5_input.txt");
    if (input.is_open()) {
        
        bool readingRanges = true;

        int delimiterPos;
        long long rStart, rEnd;

        while (getline(input, line)) {

            if (readingRanges) {

                // Load ranges

                delimiterPos = line.find('-');
                if (delimiterPos != -1) { 

                    rStart = stoll(line.substr(0, delimiterPos));
                    rEnd = stoll(line.substr(delimiterPos + 1));
                    ranges.push_back({rStart, rEnd});

                } else readingRanges = false; // Move to next stage once done with ranges

            } else {

                // Read through IDs
                
                for (range &r : ranges) {
                    if (stoll(line) >= r.rangeStart && stoll(line) <= r.rangeEnd) {
                        pt1Count ++;
                        break;
                    }
                }

            }

        }

        // Calculate # of possible fresh IDs
        consolidateRanges();
        for (range &r : ranges) {
            pt2Count += (r.rangeEnd - r.rangeStart) + 1;
        }

        std::cout << "Part 1 / # of fresh IDs in input: " << pt1Count << '\n';
        std::cout << "Part 2 / # of possible fresh IDs: " << pt2Count << '\n';

    }
    
    return 0;
}