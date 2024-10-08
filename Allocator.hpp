// -------------
// Allocator.hpp
// -------------

#ifndef Allocator_hpp
#define Allocator_hpp

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstddef>   // ptrdiff_t, size_t
#include <new>       // bad_alloc, new
#include <stdexcept> // invalid_argument

// ------------
// My_Allocator
// ------------

template <typename T, std::size_t N>
class My_Allocator {
    // -----------
    // operator ==
    // -----------

    friend bool operator == (const My_Allocator&, const My_Allocator&) { // this is correct
        return false;}

    // -----------
    // operator !=
    // -----------

    friend bool operator != (const My_Allocator& lhs, const My_Allocator& rhs) { // this is correct
        return !(lhs == rhs);}

    public:
        // --------
        // typedefs
        // --------

        using value_type      = T;

        using size_type       = std::size_t;
        using difference_type = std::ptrdiff_t;

        using pointer         =       value_type*;
        using const_pointer   = const value_type*;

        using reference       =       value_type&;
        using const_reference = const value_type&;

    public:
        // ---------------
        // iterator
        // over the blocks
        // ---------------

        class iterator {
            // -----------
            // operator ==
            // -----------

            friend bool operator == (const iterator&, const iterator&) { // fix!
                // <your code>
                return false;}

            // -----------
            // operator !=
            // -----------

            friend bool operator != (const iterator& lhs, const iterator& rhs) { // this is correct
                return !(lhs == rhs);}

            private:
                // ----
                // data
                // ----

                My_Allocator& _r;
                std::size_t   _i;

            public:
                // -----------
                // constructor
                // -----------

                iterator (My_Allocator& r, size_type i) :
                		_r (r),
                		_i (i)
                	{}

                // ----------
                // operator *
                // ----------

				/**
				 * beginning sentinel of the block
				 */
                int& operator * () const { // fix!
                    // <your code>
                    static int tmp = 0;
                    return tmp;}

                // -----------
                // operator ++
                // -----------

                iterator& operator ++ () { // fix!
                    // <your code>
                    return *this;}

                // -----------
                // operator ++
                // -----------

                iterator operator ++ (int) { // this is correct
                    iterator x = *this;
                    ++*this;
                    return x;}

                // -----------
                // operator --
                // -----------

                iterator& operator -- () { // fix!
                    // <your code>
                    return *this;}

                // -----------
                // operator --
                // -----------

                iterator operator -- (int) { // this is correct
                    iterator x = *this;
                    --*this;
                    return x;}};

        // ---------------
        // const_iterator
        // over the blocks
        // ---------------

        class const_iterator {
            // -----------
            // operator ==
            // -----------

            friend bool operator == (const const_iterator&, const const_iterator&) { // fix!
                // <your code>
                return false;}

            // -----------
            // operator !=
            // -----------

            friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) { // this is correct
                return !(lhs == rhs);}

            private:
                // ----
                // data
                // ----

                const My_Allocator& _r;
                std::size_t         _i;

            public:
                // -----------
                // constructor
                // -----------

                const_iterator (const My_Allocator& r, size_type i) :
                		_r (r),
                		_i (i)
                	{}

                // ----------
                // operator *
                // ----------

				// beginning sentinel of the block
                const int& operator * () const { // fix!
                    // <your code>
                    static int tmp = 0;
                    return tmp;}

                // -----------
                // operator ++
                // -----------

                const_iterator& operator ++ () { // fix!
                    // <your code>
                    return *this;}

                // -----------
                // operator ++
                // -----------

                const_iterator operator ++ (int) { // this is correct
                    const_iterator tmp = *this;
                    ++*this;
                    return tmp;}

                // -----------
                // operator --
                // -----------

                const_iterator& operator -- () { // fix!
                    // <your code>
                    return *this;}

                // -----------
                // operator --
                // -----------

                const_iterator operator -- (int) { // this is correct
                    const_iterator tmp = *this;
                    --*this;
                    return tmp;}};

    private:
        // ----
        // data
        // ----

        char a[N]; // array of bytes

        // -----
        // valid
        // -----

        /**
         * O(1) in space
         * O(n) in time
         * <your documentation>
         */
        bool valid () const {
            // <your code>
            // <you must use allocator's iterators>
            return true;}

    public:
        // -----------
        // constructor
        // -----------

        /**
         * O(1) in space
         * O(1) in time
         * throw a std::bad_alloc exception, if N is less than sizeof(T) + (2 * sizeof(int))
         */
        My_Allocator () {
            if (N < (sizeof(T) + (2 * sizeof(int))))
            	throw std::bad_alloc();
            (*this)[0]   = N-8;
            (*this)[N-4] = N-8;
            assert(valid());}

        My_Allocator             (const My_Allocator&) = default;
        ~My_Allocator            ()                    = default;
        My_Allocator& operator = (const My_Allocator&) = default;

        // --------
        // allocate
        // --------

        /**
         * O(1) in space
         * O(n) in time
         * after allocation there must be enough space left for a valid block
         * the smallest allowable block is sizeof(T) + (2 * sizeof(int))
         * choose the first block that fits
         * throw a std::bad_alloc exception, if there isn't an acceptable free block
         */
        pointer allocate (size_type s) { // fix!
            // <your code>
            // <you must use allocator's iterators>
            assert(valid());
            T* a = new T[s];
            return a;}

        // ---------
        // construct
        // ---------

        /**
         * O(1) in space
         * O(1) in time
         */
        void construct (pointer p, const_reference v) { // this is correct and exempt
            new (p) T(v);                               // from the prohibition of new
            assert(valid());}

        // ----------
        // deallocate
        // ----------

        /**
         * O(1) in space
         * O(1) in time
         * after deallocation adjacent free blocks must be coalesced
         * throw an invalid_argument exception, if p is invalid
         * <your documentation>
         */
        void deallocate (pointer, size_type) {
            // <your code>
            // <you must use allocator's iterators>
            assert(valid());}

        // -------
        // destroy
        // -------

        /**
         * O(1) in space
         * O(1) in time
         */
        void destroy (pointer p) { // this is correct
            p->~T();
            assert(valid());}

        // -----------
        // operator []
        // -----------

        /**
         * O(1) in space
         * O(1) in time
         */
        int& operator [] (int i) { // this is correct
            return *reinterpret_cast<int*>(&a[i]);}

        /**
         * O(1) in space
         * O(1) in time
         */
        const int& operator [] (int i) const { // this is correct
            return *reinterpret_cast<const int*>(&a[i]);}

        // -----
        // begin
        // -----

        /**
         * O(1) in space
         * O(1) in time
         */
        iterator begin () { // this is correct
            return iterator(*this, 0);}

        /**
         * O(1) in space
         * O(1) in time
         */
        const_iterator begin () const { // this is correct
            return const_iterator(*this, 0);}

        // ---
        // end
        // ---

        /**
         * O(1) in space
         * O(1) in time
         */
        iterator end () { // this is correct
            return iterator(*this, N);}

        /**
         * O(1) in space
         * O(1) in time
         */
        const_iterator end () const { // this is correct
            return const_iterator(*this, N);}};

#endif // Allocator_hpp
