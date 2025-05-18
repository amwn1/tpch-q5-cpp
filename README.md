# TPCH Query 5 — C++ Implementation

This is a multithreaded C++ implementation of the TPCH Benchmark Query 5 (“Local Supplier Volume”) using in-memory joins, filters, and a POSIX-style CLI interface. The program is written entirely in modern C++ without a database backend.

---

## 🔧 Build

```bash
# inside the project folder
make           # compiles all .cpp files → produces ./tpch_q5
```

## 🚀 Run

```bash
./tpch_q5 \
  --region "ASIA" \
  --start-date 1994-01-01 \
  --end-date 1995-01-01 \
  --threads 4 \
  --data-dir .
```

## 🧹 Clean

```bash
make clean     # removes tpch_q5 and any *.o object files
```

Running `make` again will recreate `tpch_q5` in seconds.  


## 📂 Dataset

Due to GitHub size limits, `.tbl` files are **not included** in this repository.

You can either:

### ✅ Option 1 – Generate the data manually with `dbgen`

```bash
git clone https://github.com/electrum/tpch-dbgen.git
cd tpch-dbgen
make
./dbgen -s 1
```

This will generate files such as `region.tbl`, `nation.tbl`, `lineitem.tbl`, etc.  
Move them into your working directory or use the `--data-dir` flag to point to their location.

### ✅ Option 2 – Download a pre-generated dataset

[Link to Google Drive ZIP will be provided here soon]

---

## 📁 Folder Structure (after setup)

```
TPCH-Q5/
├── parse_region.cpp
├── parse_nation.cpp
├── join_supplier.cpp
├── tpch_q5.cpp
├── Makefile
├── README.md
└── *.tbl   (ignored in repo via .gitignore)
```

---

## 📝 Notes

- Compatible with **g++ (C++17)** on Linux / WSL  
- Uses **`getopt_long`** for CLI parsing  
- Multithreaded using **POSIX threads** (`-pthread`)  
- CLI exposes runtime config: region, date range, thread count, and dataset path  
