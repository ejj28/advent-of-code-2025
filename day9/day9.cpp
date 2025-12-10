#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

enum Direction {
    North,
    South,
    East,
    West
};

struct Tile {
    int x, y;
};

struct Rectangle {

    Tile corner1, corner2;

    int h, w;

    long long area;

    Rectangle(Tile corner1, Tile corner2) {

        this->corner1 = corner1;
        this->corner2 = corner2;

        // Precomputes area

        this->h = corner1.y - corner2.y;
        this->w = corner1.x - corner2.x;

        if (this->h < 0) this->h *= -1;
        if (this->w < 0) this->w *= -1;

        this->area = (long long) (h+1) * (w+1);
    }

};

struct LineSegment {

    Tile start, end;

    Direction direction;

    LineSegment(Tile start, Tile end) {

        this->start = start;
        this->end = end;

        if      (start.x < end.x) this->direction = East;
        else if (start.x > end.x) this->direction = West;
        else if (start.y > end.y) this->direction = North;
        else if (start.y < end.y) this->direction = South;
        
    }

};


bool isClearPath(int &x1, int &y1, int &x2, int &y2, std::vector<LineSegment> &linesNS, std::vector<LineSegment> &linesEW) {

    // Checks if the path from x1,y1 to x2,y2 is not blocked by a line segment
    // Also checks that x2,y2 is within the boundary

    // Separate vectors are used for vertical and horizontal lines for better performance
    // Horizontal paths are only checked against vertical lines, and vice versa.

    Direction pathD;

    // Determine direction of path
    if (x1 == x2) {
        if (y1 > y2) pathD = North; else pathD = South;
    } else if (y1 == y2) {
        if (x1 > x2) pathD = West; else pathD = East;
    }

    bool hitsWall = true;

    if (pathD == North || pathD == South) { // Vertical path

        // Check all horizontal line segments
        for (LineSegment &line : linesEW) {
            
            // Check if the path is within the same horizontal range as this line segment
            if ((line.direction == East && x1 >= line.start.x && x1 <= line.end.x) || (line.direction == West && x1 <= line.start.x && x1 >= line.end.x)) {

                if ((pathD == North && line.start.y < y1 && line.start.y > y2) || (pathD == South && line.start.y > y1 && line.start.y < y2)) {
                    
                    return false; // This line segment intersects the path

                } else if ((pathD == North && line.start.y < y1 && line.start.y <= y2) || (pathD == South && line.start.y > y1 && line.start.y >= y2)) {
                    
                    hitsWall = false; // This line segment is past the end of the path

                }

            }

        }

    } else { // Horizontal path

        // Check all vertical line segments
        for (LineSegment &line : linesNS) {

            // Check if the path is within the same vertical range as this line segment
            if ((line.direction == North && y1 <= line.start.y && y1 >= line.end.y) || (line.direction == South && y1 >= line.start.y && y1 <= line.end.y)) {

                if ((pathD == East && line.start.x > x1 && line.start.x < x2) || (pathD == West && line.start.x < x1 && line.start.x > x2)) {
                    
                    return false; // This line segment intersects the path

                } else if ((pathD == East && line.start.x > x1 && line.start.x >= x2) || (pathD == West && line.start.x < x1 && line.start.x <= x2)) {
                    
                    hitsWall = false; // This line segment is past the end of the path

                }

            }

        }

    }

    if (hitsWall) {
        return false; // The path never hits a line segment, meaning it's starting at a boundary edge and going towards the wall of the grid
    } else {
        return true; // The path never hits a line segment until after it has reached the destination, meaning it's within the boundary
    }

    
}

// Comparison function for std::sort
bool compareRects(Rectangle const&r1, Rectangle const&r2) {
    return r1.area > r2.area;
}


int main() {

    std::ifstream input;

    std::vector<Tile> tiles;
    std::vector<Rectangle> rectangles;

    input.open("day9_input.txt");
    if (input.is_open()) {

        std::string line;

        while (getline(input, line)) {
            tiles.push_back({stoi(line.substr(0, line.find(','))), stoi(line.substr(line.find(',')+1, line.length()))});
        }

    } else {
        std::cout << "Unable to open input\n";
        return -1;
    }

    
    // Part 1, find biggest possible rectangle


    // Find all possible rectangles
    for (int i = 0; i < tiles.size(); i++) {
        for (int j = i + 1; j < tiles.size(); j++) {

            rectangles.push_back(Rectangle(tiles[i], tiles[j]));

        }
    }

    // Sort rectangles by area, descending
    std::sort(rectangles.begin(), rectangles.end(), &compareRects);

    std::cout << "Part 1 / Largest possible rectangle's area is " << rectangles[0].area << '\n';


    // Part 2, find biggest valid rectangle within the boundary


    // Sorting horizontal and vertical line segments into separate lists for better performance
    std::vector<LineSegment> linesNS;
    std::vector<LineSegment> linesEW;

    for (int tIndex = 0; tIndex < tiles.size() - 1; tIndex++) {

        if (tIndex == tiles.size() - 2) { 

            // Connect last point in the input to the first point

            LineSegment l = LineSegment(tiles[tIndex], tiles[0]);

            if (l.direction == North || l.direction == South) {
                linesNS.push_back(l);
            } else {
                linesEW.push_back(l);
            }
            
        } else {

            LineSegment l = LineSegment(tiles[tIndex], tiles[tIndex + 1]);

            if (l.direction == North || l.direction == South) {
                linesNS.push_back(l);
            } else {
                linesEW.push_back(l);
            }

        }

    }

    std::vector<Tile> corners;
    Tile opposite1;
    Tile opposite2;

    bool valid;

    for (Rectangle &r : rectangles) {

        // Get the other two corners of the rectangle
        opposite1 = {r.corner1.x,r.corner2.y};
        opposite2 = {r.corner2.x,r.corner1.y};

        // All corners
        corners = {r.corner1, r.corner2, opposite1, opposite2};

        valid = true;

        // Test every pair of corners
        for (int t1 = 0; t1 < corners.size() - 1; t1++) {
            for (int t2 = t1 + 1; t2 < corners.size(); t2++) {

                Tile &tile1 = corners[t1];
                Tile &tile2 = corners[t2];
                
                // Don't check diagonal corner pairs
                if (tile1.x == tile2.x || tile1.y == tile2.y) {
                    
                    // Check if path between pair is unobstructed and within bounds
                    if (!isClearPath(tile1.x, tile1.y, tile2.x, tile2.y, linesNS, linesEW)) {
                        valid = false;
                        break;
                    }
                
                }

            }

            if (!valid) break;

        }

        if (valid) {
            std::cout << "Part 2 / Largest possible rectangle's area is " << r.area << '\n';
            break;
        }

    }

    return 0;
}