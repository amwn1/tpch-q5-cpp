#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
struct Customer {
    int customerKey;                // Customer ID
    std::string name;               // Customer name
    std::string address;            // Customer address
    int nationKey;                  // Foreign key to nation
    std::string phone;              // Phone number
    double accountBalance;          // Account balance
    std::string mktSegment;         // Market segment
    std::string comment;            // Comment text
};


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


// Nation structure to map nation.tbl rows
struct Nation {
    int nationKey;
    std::string name;
    int regionKey;
    std::string comment;
};

// Function to parse nation.tbl into vector<Nation>
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
    std::string customerFile = "customer.tbl";
    std::string nationFile = "nation.tbl";

    std::vector<Customer> customers = loadCustomerTable(customerFile);
    std::vector<Nation> nations = loadNationTable(nationFile);

    // For every customer, find the corresponding nation by nationKey
    for (const auto& customer : customers) {
        auto it = std::find_if(nations.begin(), nations.end(), [&](const Nation& n) {
            return n.nationKey == customer.nationKey;
        });

        if (it != nations.end()) {
            std::cout << "Customer: " << customer.name
                      << " | Nation: " << it->name
                      << " | Balance: $" << customer.accountBalance
                      << std::endl;
        }
    }

    return 0;
}
