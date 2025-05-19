#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Region {
    int regionKey;
    std::string name;
    std::string comment;
};

// Parses region.tbl and stores data in a vector of Region
std::vector<Region> loadRegionTable(const std::string& filePath) {
    std::vector<Region> regions;
    std::ifstream file(filePath); // open the file for reading
    std::string line; //will hold each raw line

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Region r;
        std::string token;

        std::getline(ss, token, '|');
        r.regionKey = std::stoi(token);
 
        std::getline(ss, r.name, '|');
        std::getline(ss, r.comment, '|');

        regions.push_back(r);
    }

    return regions;
}

int main() {
    std::string filePath = "region.tbl"; 
    std::vector<Region> regions = loadRegionTable(filePath);

   
    for (const auto& r : regions) {
        std::cout << r.regionKey << " | " << r.name << " | " << r.comment << "\n";
    }

    return 0;
}
