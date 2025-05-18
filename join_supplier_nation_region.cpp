#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

struct Region {
    int regionKey;
    std::string name;
};

struct Nation {
    int nationKey;
    std::string name;
    int regionKey;
};

struct Supplier {
    int suppKey;
    std::string name;
    int nationKey;
    double acctBal;
};

// Utility function to split a line by delimiter
std::vector<std::string> splitLine(const std::string& line, char delimiter = '|') {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Load region.tbl
std::unordered_map<int, Region> loadRegions(const std::string& filePath) {
    std::unordered_map<int, Region> regions;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        auto tokens = splitLine(line);
        if (tokens.size() >= 2) {
            Region r;
            r.regionKey = std::stoi(tokens[0]);
            r.name = tokens[1];
            regions[r.regionKey] = r;
        }
    }

    return regions;
}

// Load nation.tbl
std::unordered_map<int, Nation> loadNations(const std::string& filePath) {
    std::unordered_map<int, Nation> nations;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        auto tokens = splitLine(line);
        if (tokens.size() >= 3) {
            Nation n;
            n.nationKey = std::stoi(tokens[0]);
            n.name = tokens[1];
            n.regionKey = std::stoi(tokens[2]);
            nations[n.nationKey] = n;
        }
    }

    return nations;
}

// Load supplier.tbl
// Load supplier.tbl with correct field positions
std::vector<Supplier> loadSuppliers(const std::string& filePath) {
    std::vector<Supplier> suppliers;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        auto tokens = splitLine(line);
        if (tokens.size() >= 6) {
            Supplier s;
            s.suppKey = std::stoi(tokens[0]);      // S_SUPPKEY
            s.name = tokens[1];                     // S_NAME
            s.nationKey = std::stoi(tokens[3]);     // S_NATIONKEY
            s.acctBal = std::stod(tokens[5]);       // S_ACCTBAL
            suppliers.push_back(s);
        }
    }

    return suppliers;
}


int main() {
    auto regions = loadRegions("region.tbl");
    auto nations = loadNations("nation.tbl");
    auto suppliers = loadSuppliers("supplier.tbl");

    for (const auto& s : suppliers) {
        if (nations.find(s.nationKey) != nations.end()) {
            Nation n = nations[s.nationKey];
            if (regions.find(n.regionKey) != regions.end()) {
                Region r = regions[n.regionKey];

                // Print result
                std::cout << s.name << " | " << n.name << " | " << r.name
                          << " | $" << s.acctBal << '\n';
            }
        }
    }

    return 0;
}
