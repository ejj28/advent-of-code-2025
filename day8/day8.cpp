#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

struct Junction {

    int id;

    int x, y, z;

    Junction(std::string coordinates, int id) {

        x = stoi(coordinates.substr(0, coordinates.find(',')));
        coordinates.erase(0, coordinates.find(',') + 1);
        y = stoi(coordinates.substr(0, coordinates.find(',')));
        coordinates.erase(0, coordinates.find(',') + 1);
        z = stoi(coordinates.substr(0, coordinates.length()));
    
        this->id = id;

    }

};

struct JunctionPair {
    int j1, j2;
    float distance;
};

struct Circuit {

    std::vector<int> junctionIDs;

    Circuit(int initialJunctionID) {
        junctionIDs.push_back(initialJunctionID);
    }

    // Add the junction IDs from another Circuit into this one
    void absorb(Circuit &c) {
        junctionIDs.insert(junctionIDs.end(), c.junctionIDs.begin(), c.junctionIDs.end());
    }

};

// Comparison function for std::sort()
bool junctionPairSort(JunctionPair const& jp1, JunctionPair const& jp2) {
    return jp1.distance < jp2.distance;
}

// Calculates 3D euclidean distance between junctions
float getDistance(Junction &j1, Junction &j2) {
    return sqrt(pow(j1.x - j2.x, 2) + pow(j1.y - j2.y, 2) + pow(j1.z - j2.z, 2));
}

// Searches all Circuits to find the one that the junction is a part of
int findJunctionCircuitIndex(int junctionID, std::vector<Circuit> &circuits, std::vector<Junction> &junctions) {

    for (int i = 0; i < circuits.size(); i++) {

        for (int c = 0; c < circuits[i].junctionIDs.size(); c++) {

            if (junctions[circuits[i].junctionIDs[c]].id == junctionID) return i;

        }
    }
    return -1;
}

// Performs connection between the Junctions in the JunctionPair specified by index
void matchJunctions(std::vector<Junction> &junctions, std::vector<JunctionPair> &junctionPairs, std::vector<Circuit> &circuits, int &index) {

    Junction &j1 = junctions[junctionPairs[index].j1];
    Junction &j2 = junctions[junctionPairs[index].j2];

    int j1CircuitID = findJunctionCircuitIndex(j1.id, circuits, junctions);
    int j2CircuitID = findJunctionCircuitIndex(j2.id, circuits, junctions);

    // Check if they're already in the same circuit (as a result of a previous connection)
    if (j1CircuitID != j2CircuitID) {
        
        // Merge into one circuit and delete the other
        circuits[j1CircuitID].absorb(circuits[j2CircuitID]);
        circuits.erase(circuits.begin() + j2CircuitID);

    }

}

int main() {

    std::vector<Junction> junctions;
    std::vector<Circuit> circuits;

    std::ifstream input;

    input.open("day8_input.txt");
    if (input.is_open()) {

        std::string line;

        int index = 0;
        while (getline(input, line)) {

            // Instantiate each Junction and a Circuit containing that Junction
            junctions.push_back(Junction(line, index));
            circuits.push_back(Circuit(index));

            index++;

        }

    } else {
        std::cout << "Unable to open input\n";
        return -1;
    }

    std::vector<JunctionPair> junctionPairs;

    // Calculate all possible pairings of Junctions, creating a JunctionPair for each
    for (int j1 = 0; j1 < junctions.size(); j1++) {

        for (int j2 = j1 + 1; j2 < junctions.size(); j2++) {

            if (j1 != j2) {
                junctionPairs.push_back({j1, j2, getDistance(junctions[j1], junctions[j2])});
            }
            
        }

    }

    // Sort the JunctionPairs by euclidean distance
    std::sort(junctionPairs.begin(), junctionPairs.end(), &junctionPairSort);

    int pt1_num_matches = 1000;

    // Perform connections between the first 1000 closest pairs for part 1
    for (int i = 0; i < pt1_num_matches; i++) {

        matchJunctions(junctions, junctionPairs, circuits, i);

    }


    // This whole bit could just be a sort but I don't care

    int topCountFirst = 0;
    int topCountSecond = 0;
    int topCountThird = 0;

    for (Circuit &c : circuits) {
        if (c.junctionIDs.size() > topCountFirst) {
            topCountThird = topCountSecond;
            topCountSecond = topCountFirst;
            topCountFirst = c.junctionIDs.size();
        } else if (c.junctionIDs.size() > topCountSecond) {
            topCountThird = topCountSecond;
            topCountSecond = c.junctionIDs.size();
        } else if (c.junctionIDs.size() > topCountThird) {
            topCountThird = c.junctionIDs.size();
        }
    }

    int pairIndex = pt1_num_matches;

    // Continue connecting the remaining closest junction pairs until we're at 1 circuit remaining
    while (circuits.size() > 1) {

        matchJunctions(junctions, junctionPairs, circuits, pairIndex);

        // Don't increment if we're down to one circuit, pairIndex will be used to get the final result
        if (circuits.size() > 1) {
            pairIndex++;
        }

    }

    // Get junctions in final pair
    Junction &finalPairJunction1 = junctions[junctionPairs[pairIndex].j1];
    Junction &finalPairJunction2 = junctions[junctionPairs[pairIndex].j2];

    std::cout << "Part 1 / Product of the junction counts of the biggest 3 circuits: " << (topCountFirst * topCountSecond * topCountThird) << '\n';
    std::cout << "Part 2 / Product of last connected junctions' x values: " << ((long long) finalPairJunction1.x * finalPairJunction2.x) << '\n';

    return 0;
}