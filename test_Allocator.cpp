// -----------------
// TestAllocator.cpp
// -----------------

// https://github.com/google/googletest
// https://github.com/google/googletest/blob/master/docs/reference/assertions.md

// --------
// includes
// --------

#include <algorithm> // count
#include <cstddef>   // ptrdiff_t
#include <string>    // string

#include "gtest/gtest.h"

#include "Allocator.hpp"

using namespace std;

struct A {
    friend bool operator == (const A&, const A&) {
        A::log += "==(A, A) ";
        return true;}

    static string log;

    A  ()         = default;
    A  (int)      {log += "A(int) ";}
    A  (const A&) {log += "A(A) ";}
    ~A ()         {log += "~A() ";}};

string A::log;

TEST(AllocatorFixture, test0) {
    using allocator_type = My_Allocator<A, 1000>;
    using value_type     = typename allocator_type::value_type;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s = 2;
    const pointer   b = x.allocate(s);
    const pointer   e = b + s;

    const value_type v = 0;
    ASSERT_EQ(A::log, "A(int) ");

    pointer p = b;
    while (p != e) {
        x.construct(p, v);
        ++p;}
    ASSERT_EQ(A::log, "A(int) A(A) A(A) ");

    ASSERT_EQ(count(b, e, v), ptrdiff_t(s));
    ASSERT_EQ(A::log, "A(int) A(A) A(A) ==(A, A) ==(A, A) ");

    p = e;
    while (b != p) {
        --p;
        x.destroy(p);}
    ASSERT_EQ(A::log, "A(int) A(A) A(A) ==(A, A) ==(A, A) ~A() ~A() ");
    x.deallocate(b, s);}

TEST(AllocatorFixture, test1) {
    using allocator_type = My_Allocator<A, 1000>;

    allocator_type x;        // read/write
    ASSERT_EQ(x[  0], 992);
    ASSERT_EQ(x[996], 992);}

TEST(AllocatorFixture, test2) {
    using allocator_type = My_Allocator<A, 1000>;

    const allocator_type x;  // read-only
    ASSERT_EQ(x[  0], 992);
    ASSERT_EQ(x[996], 992);}

TEST(AllocatorFixture, test3) {
    A::log.clear(); // Clear the log before running the test

    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s = 4;
    const pointer   b = x.allocate(s);
    const pointer   e = b + s;

    const A v = 0;
    pointer p = b;
    while (p != e) {
        x.construct(p, v);
        ++p;}
    ASSERT_EQ(A::log, "A(int) A(A) A(A) A(A) A(A) ");

    p = e;
    while (b != p) {
        --p;
        x.destroy(p);}
    ASSERT_EQ(A::log, "A(int) A(A) A(A) A(A) A(A) ~A() ~A() ~A() ~A() ");
    x.deallocate(b, s);}

TEST(AllocatorFixture, test4) {
    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s1 = 2;
    const pointer   b1 = x.allocate(s1);
    const size_type s2 = 3;
    const pointer   b2 = x.allocate(s2);

    ASSERT_NE(b1, b2);

    x.deallocate(b1, s1);
    x.deallocate(b2, s2);}

TEST(AllocatorFixture, test5) {
    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s1 = 2;
    const pointer   b1 = x.allocate(s1);
    const size_type s2 = 3;
    const pointer   b2 = x.allocate(s2);

    x.deallocate(b1, s1);
    const pointer b3 = x.allocate(s1);
    ASSERT_EQ(b1, b3);

    x.deallocate(b2, s2);
    x.deallocate(b3, s1);}

TEST(AllocatorFixture, test6) {
    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s1 = 2;
    const pointer   b1 = x.allocate(s1);
    const size_type s2 = 3;
    const pointer   b2 = x.allocate(s2);

    x.deallocate(b2, s2);
    const pointer b3 = x.allocate(s2);
    ASSERT_EQ(b2, b3);

    x.deallocate(b1, s1);
    x.deallocate(b3, s2);}

TEST(AllocatorFixture, test7) {
    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s1 = 2;
    const pointer   b1 = x.allocate(s1);
    const size_type s2 = 3;
    const pointer   b2 = x.allocate(s2);

    x.deallocate(b1, s1);
    x.deallocate(b2, s2);

    const pointer b3 = x.allocate(s1);
    const pointer b4 = x.allocate(s2);
    ASSERT_EQ(b1, b3);
    ASSERT_EQ(b2, b4);

    x.deallocate(b3, s1);
    x.deallocate(b4, s2);}

TEST(AllocatorFixture, test8) {
    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s1 = 2;
    const pointer   b1 = x.allocate(s1);
    const size_type s2 = 4;
    const pointer   b2 = x.allocate(s2);

    x.deallocate(b2, s2);

    const size_type s3 = 3;
    const pointer   b3 = x.allocate(s3);
    ASSERT_EQ(b2, b3); // b3 should reuse the block of b2

    x.deallocate(b3, s3);
    x.deallocate(b1, s1);}

TEST(AllocatorFixture, test9) {
    using allocator_type = My_Allocator<A, 1000>;
    using size_type      = typename allocator_type::size_type;
    using pointer        = typename allocator_type::pointer;

    allocator_type  x;
    const size_type s1 = 2;
    const pointer   b1 = x.allocate(s1);
    const size_type s2 = 3;
    const pointer   b2 = x.allocate(s2);

    x.deallocate(b1, s1);
    x.deallocate(b2, s2);

    const pointer b3 = x.allocate(s1);
    const pointer b4 = x.allocate(s1);
    ASSERT_EQ(b1, b3);
    ASSERT_NE(b1, b4);

    x.deallocate(b3, s1);
    x.deallocate(b4, s1);}