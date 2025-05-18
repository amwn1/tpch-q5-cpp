#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

struct Region {
    int regionKey;
    std::string name;
    std::string comment;
};

struct Nation {
    int nationKey;
    std::string name;
    int regionKey;
    std::string comment;
};

// Reads region.tbl into a vector of Region structs
std::vector<Region> loadRegionTable(const std::string& filePath) {
    std::vector<Region> regions;
    std::ifstream file(filePath);
    std::string line;

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

// Reads nation.tbl into a vector of Nation structs
std::vector<Nation> loadNationTable(const std::string& filePath) {
    std::vector<Nation> nations;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Nation n;
        std::string token;

        std::getline(ss, token, '|');
        n.nationKey = std::stoi(token);

        std::getline(ss, n.name, '|');

        std::getline(ss, token, '|');
        n.regionKey = std::stoi(token);

        std::getline(ss, n.comment, '|');

        nations.push_back(n);
    }

    return nations;
}

int main() {
    std::string regionFile = "region.tbl";
    std::string nationFile = "nation.tbl";

    std::vector<Region> regions = loadRegionTable(regionFile);
    std::vector<Nation> nations = loadNationTable(nationFile);

    // Map to quickly find region name by regionKey
    std::unordered_map<int, std::string> regionMap;
    for (const auto& r : regions) {
        regionMap[r.regionKey] = r.name;
    }

    // Print nations joined with their region name
    for (const auto& n : nations) {
        std::string regionName = regionMap[n.regionKey];
        std::cout << "Nation: " << n.name
                  << " | Region: " << regionName
                  << " | Comment: " << n.comment << std::endl;
    }

    return 0;
}
