#include <thread>
// #include <mutex>
#include <getopt.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <iomanip>

static struct option longOpts[] = {
  { "region",     required_argument, nullptr, 'r' },
  { "start-date", required_argument, nullptr, 's' },
  { "end-date",   required_argument, nullptr, 'e' },
  { "threads",    required_argument, nullptr, 't' },
  { "data-dir",   required_argument, nullptr, 'd' },
  { "help",       no_argument,       nullptr, 'h' },
  { nullptr,      0,                 nullptr,  0  }
};


struct Supplier {
    int          suppKey;
    std::string  name;
    std::string  address;
    int          nationKey;
    std::string  phone;
    double       acctBalance;
    std::string  comment;
};

struct LineItem {
    int orderKey;
    int suppKey;
    double extendedPrice;
    double discount;
    std::string shipDate;
};

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






std::vector<LineItem> loadLineItemTable(const std::string& filePath) {
    std::vector<LineItem> items;
    std::ifstream file(filePath);
    std::string line, token;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        LineItem l;

        std::getline(ss, token, '|');
        l.orderKey = std::stoi(token);

        std::getline(ss, token, '|');                // skip partKey
        std::getline(ss, token, '|'); l.suppKey      = std::stoi(token); // now read it
        std::getline(ss, token, '|');                // skip lineNumber
        std::getline(ss, token, '|');                // skip quantity

        std::getline(ss, token, '|'); l.extendedPrice = std::stod(token);
        std::getline(ss, token, '|'); l.discount      = std::stod(token);

        for (int i = 0; i < 3; ++i) std::getline(ss, token, '|'); // tax, returnflag, linestatus
        std::getline(ss, l.shipDate, '|');                      // shipDate

        items.push_back(l);
    }
    return items;
}




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

std::vector<Supplier> loadSupplierTable(const std::string& filePath) {
    std::vector<Supplier> suppliers;
    std::ifstream file(filePath);
    std::string line, token;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Supplier s;

        std::getline(ss, token, '|'); s.suppKey     = std::stoi(token);
        std::getline(ss, s.name,   '|');
        std::getline(ss, s.address,'|');
        std::getline(ss, token,   '|'); s.nationKey  = std::stoi(token);
        std::getline(ss, s.phone,  '|');
        std::getline(ss, token,   '|'); s.acctBalance = std::stod(token);
        std::getline(ss, s.comment,'|');

        suppliers.push_back(s);
    }
    return suppliers;
}


int main(int argc, char** argv) {
    // 1) Defaults
    std::string region    = "ASIA";
    std::string startDate = "1994-01-01";
    std::string endDate   = "1995-01-01";
    int         nThreads  = 1;
    std::string dataDir   = ".";

    const char *optstring = "r:s:e:t:d:h";

    // 2) Parse CLI flags
    int c, idx;
    while ((c = getopt_long(argc, argv, optstring, longOpts, &idx)) != -1) {
      switch (c) {
        case 'r': region    = optarg; break;
        case 's': startDate = optarg; break;
        case 'e': endDate   = optarg; break;
        case 't': nThreads  = std::stoi(optarg); break;
        case 'd': dataDir   = optarg; break;
        case 'h':
          std::cout << "Usage: " << argv[0] << " [OPTIONS]\n"
                    << "  -r, --region     Region name (default " << region << ")\n"
                    << "  -s, --start-date YYYY-MM-DD (default " << startDate << ")\n"
                    << "  -e, --end-date   YYYY-MM-DD (default " << endDate   << ")\n"
                    << "  -t, --threads    # of worker threads (default " << nThreads << ")\n"
                    << "  -d, --data-dir   Path to .tbl files (default " << dataDir << ")\n"
                    << "  -h, --help       Show this help\n";
          return 0;
        case '?': return 1;
      }
    }

        if (nThreads < 1) {
        std::cerr << "Error: number of threads must be >= 1 (you passed "
                  << nThreads << ")\n";
        return 1;
    }

    // 3b) Date range must make sense
    if (startDate >= endDate) {
        std::cerr << "Error: start-date (" << startDate
                  << ") must be before end-date (" << endDate << ")\n";
        return 1;
    }

    // 3c) Make sure each .tbl file actually exists & is readable
    auto check_file = [&](const std::string &name){
        std::ifstream f(dataDir + "/" + name);
        if (!f.is_open()) {
            std::cerr << "Error: unable to open " << dataDir << "/" << name << "\n";
            std::exit(1);
        }
    };
    check_file("orders.tbl");
    check_file("customer.tbl");
    check_file("supplier.tbl");
    check_file("nation.tbl");
    check_file("region.tbl");
    check_file("lineitem.tbl");

    // 3) Load tables from dataDir
    auto orders    = loadOrderTable   (dataDir + "/orders.tbl");
    auto customers = loadCustomerTable(dataDir + "/customer.tbl");
    auto suppliers = loadSupplierTable(dataDir + "/supplier.tbl");
    auto nations   = loadNationTable  (dataDir + "/nation.tbl");
    auto regions   = loadRegionTable  (dataDir + "/region.tbl");
    auto lineitems = loadLineItemTable(dataDir + "/lineitem.tbl");

    // 4) Build O(1) lookup maps
    std::unordered_map<int,Order>    orderByKey;
    for (auto& o : orders)    orderByKey[o.orderKey]     = o;

    std::unordered_map<int,Customer> customerByKey;
    for (auto& c : customers) customerByKey[c.customerKey] = c;

    std::unordered_map<int,Supplier> supplierByKey;
    for (auto& s : suppliers) supplierByKey[s.suppKey]    = s;

    std::unordered_map<int,Nation>    nationByKey;
    for (auto& n : nations)   nationByKey[n.nationKey]    = n;

    std::unordered_map<int,Region>    regionByKey;
    for (auto& r : regions)   regionByKey[r.regionKey]    = r;

    // 5) Accumulate revenue
    // number of workers requested on the command line:
int n = nThreads;  

// one map per thread, to avoid any locking
std::vector<std::map<std::string,double>> partial(n);
size_t total = lineitems.size();
size_t chunk = (total + n - 1) / n;  // ceil division
std::vector<std::thread> workers;
workers.reserve(n);

for(int t = 0; t < n; ++t) {
  size_t begin = t * chunk;
  size_t end   = std::min(begin + chunk, total);

  workers.emplace_back(
    [&, t, begin, end]() {
      auto& local = partial[t];
      for(size_t i = begin; i < end; ++i) {
        auto const& li = lineitems[i];

        // — join to order —
        auto oIt = orderByKey.find(li.orderKey);
        if (oIt == orderByKey.end()) continue;
        auto const& ord = oIt->second;

        // — date filter on order date —
        if (ord.orderDate < startDate || ord.orderDate >= endDate)
          continue;

        // — join to customer —
        auto cIt = customerByKey.find(ord.customerKey);
        if (cIt == customerByKey.end()) continue;

        // — join to supplier —
        auto sIt = supplierByKey.find(li.suppKey);
        if (sIt == supplierByKey.end()) continue;

        // — ensure same nation —
        if (cIt->second.nationKey != sIt->second.nationKey)
          continue;

        // — join to nation —
        auto nIt = nationByKey.find(cIt->second.nationKey);
        if (nIt == nationByKey.end()) continue;

        // — join to region and filter region name —
        auto rIt = regionByKey.find(nIt->second.regionKey);
        if (rIt == regionByKey.end() || rIt->second.name != region)
          continue;

        // — accumulate into this thread’s local map —
        double rev = li.extendedPrice * (1.0 - li.discount);
        local[nIt->second.name] += rev;
      }
    }
  );
}
for(auto &w : workers) 
  w.join();

std::map<std::string,double> revenueByNation;
for (auto &local : partial) {
  for(auto & [nat, sum] : local)
    revenueByNation[nat] += sum;
}


    // 6) Print or warn
    if (revenueByNation.empty()) {
        std::cout << "⚠️  No revenue data found for " << region
                  << " between " << startDate
                  << " and " << endDate << ".\n";
        return 0;
    }

    std::vector<std::pair<std::string,double>> sorted(
        revenueByNation.begin(), revenueByNation.end());
    std::sort(sorted.begin(), sorted.end(),
              [](auto const& a, auto const& b){ return a.second > b.second; });

    std::cout << "\n=== ✅ TPCH Q5: Local Supplier Volume ("
              << region << ", " << startDate << ") ===\n";
    for (auto const& [nation,total] : sorted) {
        std::cout << nation << ": $"
                  << std::fixed << std::setprecision(4)
                  << total << "\n";
    }

    return 0;
}
