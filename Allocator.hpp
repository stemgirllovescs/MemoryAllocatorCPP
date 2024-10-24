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
        return false;
    }

    // -----------
    // operator !=
    // -----------

    friend bool operator != (const My_Allocator& lhs, const My_Allocator& rhs) { // this is correct
        return !(lhs == rhs);
    }

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

        friend bool operator == (const iterator& lhs, const iterator& rhs) { // fix!
            return (&lhs._r == &rhs._r) && (lhs._i == rhs._i);
        }

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const iterator& lhs, const iterator& rhs) { // this is correct
            return !(lhs == rhs);
        }

    public:
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

        int& operator * () const { // fix!
            return _r[_i];
        }

        // -----------
        // operator ++
        // -----------

        iterator& operator ++ () { // fix!
            int block_size = std::abs(_r[_i]);
            _i += block_size + 8;
            return *this;
        }

        // -----------
        // operator ++
        // -----------

        iterator operator ++ (int) { // this is correct
            iterator x = *this;
            ++*this;
            return x;
        }

        // -----------
        // operator --
        // -----------

        iterator& operator -- () { // fix!
            int prevEndIndex = _i - 4;
            int prevSize = std::abs(_r[prevEndIndex]);
            _i -= prevSize + 8;
            return *this;
        }

        // -----------
        // operator --
        // -----------

        iterator operator -- (int) { // this is correct
            iterator x = *this;
            --*this;
            return x;
        }
    };

    // ---------------
    // const_iterator
    // over the blocks
    // ---------------

    class const_iterator {
        // -----------
        // operator ==
        // -----------

        friend bool operator == (const const_iterator& lhs, const const_iterator& rhs) { // fix!
            return (&lhs._r == &rhs._r) && (lhs._i == rhs._i);
        }

        // -----------
        // operator !=
        // -----------

        friend bool operator != (const const_iterator& lhs, const const_iterator& rhs) { // this is correct
            return !(lhs == rhs);
        }

    public:
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
            return _r[_i];
        }

        // -----------
        // operator ++
        // -----------

        const_iterator& operator ++ () { // fix!
            int block_size = std::abs(_r[_i]);
            _i += block_size + 8;
            return *this;
        }

        // -----------
        // operator ++
        // -----------

        const_iterator operator ++ (int) { // this is correct
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        // -----------
        // operator --
        // -----------

        const_iterator& operator -- () { // fix!
            int prevEndIndex = _i - 4;
            int prevSize = std::abs(_r[prevEndIndex]);
            _i -= prevSize + 8;
            return *this;
        }

        // -----------
        // operator --
        // -----------

        const_iterator operator -- (int) { // this is correct
            const_iterator tmp = *this;
            --*this;
            return tmp;
        }
    };

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
     * Check if the allocator's sentinels are consistent
     */
    bool valid () const {
        int i = 0;
        while (i < static_cast<int>(N)) {
            int block_size = (*this)[i];
            int block_end = i + 4 + std::abs(block_size);
            if (block_end + 4 > static_cast<int>(N)) {
                return false;
            }
            int end_sentinel = (*this)[block_end];
            if (block_size != end_sentinel) {
                return false;
            }
            i = block_end + 4;
        }
        return true;
    }

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
        if (N < (8 + (2 * sizeof(int))))
            throw std::bad_alloc();
        (*this)[0]   = N-8;
        (*this)[N-4] = N-8;
        assert(valid());
    }

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
    pointer allocate (size_type s) {
        int size_in_bytes = s * 8; // Object size is 8 bytes

        for (iterator it = begin(); it != end(); ++it) {
            int& block_size = *it;
            if (block_size > 0 && block_size >= size_in_bytes) {
                int original_size = block_size;
                int remaining = original_size - size_in_bytes - 8; // Remaining data size after allocating and adding end sentinel

                if (remaining >= static_cast<int>(8)) {
                    // Split the block
                    block_size = -size_in_bytes;
                    (*this)[it._i + 4 + size_in_bytes] = -size_in_bytes; // End sentinel for allocated block
                    // Create new free block
                    int new_block_index = it._i + 8 + size_in_bytes;
                    (*this)[new_block_index] = remaining;
                    (*this)[new_block_index + 4 + remaining] = remaining;
                } else {
                    // Do not split, allocate entire block
                    block_size = -original_size;
                    (*this)[it._i + 4 + original_size] = -original_size;
                }
                assert(valid());
                return reinterpret_cast<pointer>(&a[it._i +4]);
            }
        }
        throw std::bad_alloc();
    }


    // ---------
    // construct
    // ---------

    /**
     * O(1) in space
     * O(1) in time
     */
    void construct (pointer p, const_reference v) { // this is correct and exempt
        new (p) T(v);                               // from the prohibition of new
        assert(valid());
    }
    // ----------
    // deallocate
    // ----------

    /**
     * O(1) in space
     * O(1) in time
     * after deallocation adjacent free blocks must be coalesced
     * throw an invalid_argument exception, if p is invalid
     */
    // ----------
// deallocate
// ----------

    /**
     * O(1) in space
     * O(1) in time
     * after deallocation adjacent free blocks must be coalesced
     * throw an invalid_argument exception, if p is invalid
     */
// deallocate
// ----------

    /**
     * O(1) in space
     * O(1) in time
     * After deallocation adjacent free blocks must be coalesced.
     * Throw an invalid_argument exception, if p is invalid.
     */
    void deallocate(pointer p, size_type) {
        int index = reinterpret_cast<char*>(p) - a - 4;
        if (index < 0 || index >= static_cast<int>(N)) {
            throw std::invalid_argument("Invalid pointer");
        }

        int& block_size = (*this)[index];
        if (block_size >= 0) {
            throw std::invalid_argument("Block is already free");
        }

        // Mark block as free
        int size = -block_size;
        block_size = size;
        (*this)[index + 4 + size] = size;

        // Coalesce with the next block if it is free
        int next_index = index + size + 8;
        if (next_index < static_cast<int>(N)) {
            int& next_block_size = (*this)[next_index];
            if (next_block_size > 0) {
                // Update size to include the next free block and the header/footer
                size += next_block_size + 8;
                (*this)[index] = size;  // Update start sentinel
                (*this)[index + 4 + size] = size;  // Update end sentinel
                block_size = size; // Update block_size after coalescing
            }
        }

        // Coalesce with the previous block if it is free
        if (index > 0) {
            int prev_end_index = index - 4;
            int prev_size = (*this)[prev_end_index];
            if (prev_size > 0) {
                int prev_index = index - prev_size - 8;
                // Update size to include the previous free block and the header/footer
                size += prev_size + 8;
                (*this)[prev_index] = size;  // Update start sentinel
                (*this)[prev_index + 4 + size] = size;  // Update end sentinel
                index = prev_index;      // Update index to new start
                block_size = size;       // Update block_size to new size
            }
        }

        assert(valid());
    }



    // -------
    // destroy
    // -------

    /**
     * O(1) in space
     * O(1) in time
     */
    void destroy (pointer p) { // this is correct
        p->~T();
        assert(valid());
    }

    // -----------
    // operator []
    // -----------

    /**
     * O(1) in space
     * O(1) in time
     */
    int& operator [] (int i) { // this is correct
        return *reinterpret_cast<int*>(&a[i]);
    }

    /**
     * O(1) in space
     * O(1) in time
     */
    const int& operator [] (int i) const { // this is correct
        return *reinterpret_cast<const int*>(&a[i]);
    }

    // -----
    // begin
    // -----

    iterator begin () { // this is correct
        return iterator(*this, 0);
    }

    const_iterator begin () const { // this is correct
        return const_iterator(*this, 0);
    }

    // ---
    // end
    // ---

    iterator end () { // this is correct
        return iterator(*this, N);
    }

    const_iterator end () const { // this is correct
        return const_iterator(*this, N);
    }
};



#endif // Allocator_hpp
