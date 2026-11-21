# Community Donation System

A console-based C++ inventory and distribution management application built to track incoming community donations, manage item categories, and process recipient requests.

## Features
- **Donation Intake & Logging:** Record donor contributions with item names, categories, quantities, and condition status.
- **Inventory Management:** Search, filter, and update available stock levels in real time.
- **Distribution Tracking:** Allocate items to recipients while automatically validating available quantities and preventing negative stock.
- **Persistent Storage:** Saves and loads inventory records across sessions using structured file I/O.

## Tech Stack
- **Language:** C++ (C++17)
- **IDE / Compiler:** Visual Studio / MSVC (compatible with `g++`)
- **Core Concepts:** Object-Oriented Programming (OOP), file stream serialization, input validation, and STL containers (`std::vector`, `std::string`).

## How to Build & Run

### Using Visual Studio
1. Clone the repository:
   ```bash
   git clone https://github.com/Alejandro-M-Astello/Community-Donation-System.git
    ```
Open the project in Visual Studio (or your C++ IDE), then build and run (Ctrl + F5)

### Using g++ (Command Line)
```bash
g++ -std=c++17 CommunityDonationSystem.cpp -o donation_system
./donation_system
```