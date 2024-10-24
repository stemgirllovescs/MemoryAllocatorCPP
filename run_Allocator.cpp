// ----------------
// RunAllocator.cpp
// ----------------

// --------
// includes
// --------

#include <iostream> // cin, cout
#include <vector>   // vector
#include <sstream>

#include "Allocator.hpp"

using namespace std;

// ----
// main
// ----
// Main program to use the Allocator
// #include <iostream>
// #include <vector>

int main() {
    int t; // Number of test cases
    std::cin >> t;
    std::cin.ignore(); // Clear the newline after reading t

    // Read the blank line after t
    std::string line;
    std::getline(std::cin, line);

    // Process each test case
    for (int i = 0; i < t; ++i) {
        My_Allocator<int, 1000> allocator; // Initialize the allocator
        std::vector<int> sentinels; // To store the sentinel values
        std::vector<My_Allocator<int, 1000>::pointer> busy_blocks;

        // Read requests until a blank line or EOF
        std::vector<std::string> test_case_lines;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                break; // End of current test case
            }
            test_case_lines.push_back(line);
        }

        // Process each request in the current test case
        for (const auto& request_line : test_case_lines) {
            int request = std::stoi(request_line);

            if (request > 0) {
                // Allocation request
                try {
                    auto ptr = allocator.allocate(request);
                    busy_blocks.push_back(ptr);
                    // sort by pointer value
                    std::sort(busy_blocks.begin(), busy_blocks.end());
                } catch (const std::bad_alloc& e) {
                    std::cerr << "Allocation failed: " << e.what() << std::endl;
                    // Do not add to busy_blocks; indices remain consistent
                }
            } else {
                // Deallocation request
                std::size_t blockIndex = static_cast<std::size_t>(-request - 1); // Convert to zero-based index
                if (blockIndex < busy_blocks.size()) {
                    try {
                        allocator.deallocate(busy_blocks[blockIndex], 0);
                        // Remove the block from busy_blocks to keep indices consistent
                        busy_blocks.erase(busy_blocks.begin() + blockIndex);
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Deallocation failed: " << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Invalid block index for deallocation: " << blockIndex + 1 << std::endl;
                }
            }
        }

        // Gather and store sentinel values after processing all requests
        for (auto it = allocator.begin(); it != allocator.end(); ++it) {
            sentinels.push_back(*it);
        }

        // Output the sentinels for the current test case
        for (size_t k = 0; k < sentinels.size(); ++k) {
            std::cout << sentinels[k];
            if (k < sentinels.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl; // New line after each test case output
    }

    return 0;
}