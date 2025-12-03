#include <iostream>
#include <fstream>
#include <string>

long long part1_total = 0;
long long part2_total = 0;

void isInvalid(long long id) {

    std::string id_str = std::to_string(id);
    std::string pattern;

    int maxPatternSize;

    bool matched;
        
    maxPatternSize = id_str.length() / 2;

    // Try increasingly smaller pattern sizes, down to a single repeated digit
    for (int size = maxPatternSize; size > 0; size--) {
        
        if (id_str.length() % size != 0) {
            // Advance immediately to smaller pattern size, since this size can't fit multiple times evenly into the number
            continue;
        }

        matched = true;
        
        pattern = id_str.substr(0,size);
        
        // Step through the ID, checking if the pattern continues
        for (int offset = size; offset + size <= id_str.length(); offset += size) {
            if (id_str.substr(offset, size) != pattern) {
                // If the pattern doesn't continue, break
                matched = false;
                break;
            } 
        }

        if (matched == true) {
            if ((size == maxPatternSize) && maxPatternSize * 2 == id_str.length()) {
                // Extra check necessary for 3-digit values since they're the only ones where their initial pattern length (of one) fits more than twice
                part1_total += id;
                part2_total += id;
            } else {
                part2_total += id;
            }
            break;
        }

    }

}

int main() {
    
    std::ifstream input;
    std::string range;

    int delimiter;
    std::string rangeLow;
    std::string rangeHigh;

    input.open("day2_input.txt");
    if (input.is_open()) {

        // Iterate through the ranges
        while (getline(input, range, ',')) {
            
            delimiter = range.find('-');

            rangeLow = range.substr(0, delimiter);
            rangeHigh = range.substr(delimiter + 1);

            // Iterate through each ID in the range
            for (long long i = stoll(rangeLow); i < stoll(rangeHigh); i++) {

                isInvalid(i);

            }
            
        }

        std::cout << "Part 1 total: " << part1_total << '\n';
        std::cout << "Part 2 total: " << part2_total << '\n';
        
    }

}