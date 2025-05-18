#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Nation {
    int nationKey;
    std::string name;
    int regionKey;
    std::string comment;
};

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
    std::string filePath = "nation.tbl"; 
    std::vector<Nation> nations = loadNationTable(filePath);

    for (const auto& n : nations) {
        std::cout << n.nationKey << " | " << n.name << " | " << n.regionKey << " | " << n.comment << "\n";
    }

    return 0;
}
