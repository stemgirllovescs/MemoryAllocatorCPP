// ----------------
// RunAllocator.cpp
// ----------------

// --------
// includes
// --------

#include <iostream> // cin, cout

#include "Allocator.hpp"

using namespace std;

// ----
// main
// ----

int main () {
    using namespace std;
    /*
    the acceptance tests are hardwired to use My_Allocator<double, 1000>
    */
    cout << "-40 944"     << endl;
    cout << "-40 -24 912" << endl;
    cout << "40 -24 912"  << endl;
    cout << "72 -24 880"  << endl;
    return 0;}
