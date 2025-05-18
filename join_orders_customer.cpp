#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

struct Order {
    int orderKey;
    int customerKey;
    std::string orderStatus;
    double totalPrice;
    std::string orderDate;
};

struct Nation {
    int nationKey;
    std::string name;
    int regionKey;
    std::string comment;
};

struct Region {
    int regionKey;
    std::string name;
    std::string comment;
};
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

std::vector<Order> loadOrderTable(const std::string& filePath) {
    std::vector<Order> orders;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Order o;
        std::string token;

        std::getline(ss, token, '|');
        o.orderKey = std::stoi(token);

        std::getline(ss, token, '|');
        o.customerKey = std::stoi(token);

        std::getline(ss, o.orderStatus, '|');

        std::getline(ss, token, '|');
        o.totalPrice = std::stod(token);

        std::getline(ss, o.orderDate, '|');

        orders.push_back(o);
    }

    return orders;
}


int main() {
    std::string orderFile = "orders.tbl";
    std::string customerFile = "customer.tbl";
    std::string nationFile = "nation.tbl";
    std::string regionFile = "region.tbl";

    auto orders = loadOrderTable(orderFile);
    auto customers = loadCustomerTable(customerFile);
    auto nations = loadNationTable(nationFile);
    auto regions = loadRegionTable(regionFile);

    for (const auto& order : orders) {
        auto custIt = std::find_if(customers.begin(), customers.end(),
            [&](const Customer& c) { return c.customerKey == order.customerKey; });

        if (custIt != customers.end()) {
            const auto& customer = *custIt;

            auto nationIt = std::find_if(nations.begin(), nations.end(),
                [&](const Nation& n) { return n.nationKey == customer.nationKey; });

            if (nationIt != nations.end()) {
                const auto& nation = *nationIt;

                auto regionIt = std::find_if(regions.begin(), regions.end(),
                    [&](const Region& r) { return r.regionKey == nation.regionKey; });

                if (regionIt != regions.end()) {
                    const auto& region = *regionIt;

                    std::cout << "Order: " << order.orderKey
                              << " | Customer: " << customer.name
                              << " | Nation: " << nation.name
                              << " | Region: " << region.name
                              << " | Total Price: $" << std::fixed << std::setprecision(2)
                              << order.totalPrice
                              << " | Date: " << order.orderDate
                              << "\n";
                }
            }
        }
    }

    return 0;
}
