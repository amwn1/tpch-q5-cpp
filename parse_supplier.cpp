#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Define a struct to represent each supplier record
struct Supplier {
    int suppKey;
    std::string name;
    std::string address;
    int nationKey;
    std::string phone;
    double acctBal;
    std::string comment;
};

// Load and parse the supplier.tbl file
std::vector<Supplier> loadSupplierTable(const std::string& filePath) {
    std::vector<Supplier> suppliers;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Supplier s;
        std::string token;

        std::getline(ss, token, '|');
        s.suppKey = std::stoi(token);

        std::getline(ss, s.name, '|');
        std::getline(ss, s.address, '|');

        std::getline(ss, token, '|');
        s.nationKey = std::stoi(token);

        std::getline(ss, s.phone, '|');

        std::getline(ss, token, '|');
        s.acctBal = std::stod(token);

        std::getline(ss, s.comment, '|');

        suppliers.push_back(s);
    }

    return suppliers;
}

int main() {
    std::string filePath = "supplier.tbl";
    std::vector<Supplier> suppliers = loadSupplierTable(filePath);

    // Print sample output
    for (const auto& s : suppliers) {
        std::cout << s.suppKey << " | " << s.name << " | " << s.nationKey << " | " << s.acctBal << " | " << s.comment << "\n";
    }

    return 0;
}
