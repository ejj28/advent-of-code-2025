#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    
    std::ifstream input;

    input.open("day7_input.txt");
    if (input.is_open()) {

        std::vector<long long> particles;

        int splitCount = 0;

        std::string line;

        bool firstLine = true;

        while (getline(input, line)) {
            
            if (firstLine) {

                // Set width of particle vector to match line width, and set first particle
                particles.resize(line.length());
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == 'S') {
                        particles[i]++;
                    }
                }
                firstLine = false;

            } else {

                // Iterate through characters in line
                for (int i = 0; i < line.length(); i++) {
                    
                    // Check for splitter and at least one particle affected
                    if (line[i] == '^' && particles[i] > 0) {
                        
                        splitCount++;
                        
                        // All particles that hit the splitter get added to each side if possible (effectively duplicating them)
                        if (i > 0) {
                            particles[i - 1] += particles[i];
                        }
                        if (i < line.length() - 1) {
                            particles[i + 1] += particles[i];
                        }

                        // Splitter split all particles off this index, so clear it
                        particles[i] = 0;
                    }

                }

            }
            
        }

        long long totalTimelines = 0;

        for (long long &i : particles) {
            totalTimelines += i;
        }

        std::cout << "Part 1 / Total splits: " << splitCount << '\n';
        std::cout << "Part 2 / Total timelines: " << totalTimelines << '\n';

    }

    return 0;
}