#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

// Define Customer structure
struct Customer {
    int customerKey;
    std::string name;
    std::string address;
    int nationKey;
    std::string phone;
    double accountBalance;
    std::string mktSegment;
    std::string comment;
};

// Define Nation structure
struct Nation {
    int nationKey;
    std::string name;
    int regionKey;
    std::string comment;
};

// Define Region structure
struct Region {
    int regionKey;
    std::string name;
    std::string comment;
};

// Load region.tbl file
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

// Load nation.tbl file
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

// Load customer.tbl file
std::vector<Customer> loadCustomerTable(const std::string& filePath) {
    std::vector<Customer> customers;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Customer c;
        std::string token;

        std::getline(ss, token, '|');
        c.customerKey = std::stoi(token);
        std::getline(ss, c.name, '|');
        std::getline(ss, c.address, '|');
        std::getline(ss, token, '|');
        c.nationKey = std::stoi(token);
        std::getline(ss, c.phone, '|');
        std::getline(ss, token, '|');
        c.accountBalance = std::stod(token);
        std::getline(ss, c.mktSegment, '|');
        std::getline(ss, c.comment, '|');

        customers.push_back(c);
    }

    return customers;
}

int main() {
    std::string customerFile = "customer.tbl";
    std::string nationFile = "nation.tbl";
    std::string regionFile = "region.tbl";

    std::vector<Customer> customers = loadCustomerTable(customerFile);
    std::vector<Nation> nations = loadNationTable(nationFile);
    std::vector<Region> regions = loadRegionTable(regionFile);

    // Join all 3 tables
    for (const auto& customer : customers) {
        auto nationIt = std::find_if(nations.begin(), nations.end(), [&](const Nation& n) {
            return n.nationKey == customer.nationKey;
        });

        if (nationIt != nations.end()) {
            auto regionIt = std::find_if(regions.begin(), regions.end(), [&](const Region& r) {
                return r.regionKey == nationIt->regionKey;
            });

            if (regionIt != regions.end()) {
                std::cout << "Customer: " << customer.name
                          << " | Nation: " << nationIt->name
                          << " | Region: " << regionIt->name
                          << " | Balance: $" << customer.accountBalance
                          << std::endl;
            }
        }
    }

    return 0;
}
