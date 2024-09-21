#include <iostream>
#include <map>
#include <cstddef> // For std::ptrdiff_t and std::size_t
#include <cstring>

// MemoryBlock definition for C++98 compatibility
template <typename T>
class MemoryBlock {
public:
    MemoryBlock(size_t blockSize)
        : m_next(NULL), m_data(new T[blockSize]), m_blockSize(blockSize), m_usedCount(0) {}

    ~MemoryBlock() {
        delete[] m_data;
    }

    bool isFull() const { return m_usedCount == m_blockSize; }
    bool isEmpty() const { return m_usedCount == 0; }

    T* allocate() {
        if (m_usedCount < m_blockSize) {
            return &m_data[m_usedCount++];
        }
        return NULL; // Block is full
    }

    void deallocate(T* ptr) {
        // Calculate the index of the pointer and decrement used count
        if (ptr >= m_data && ptr < m_data + m_blockSize) {
            size_t index = ptr - m_data;
            if (index < m_usedCount) {
                m_usedCount--;
                // Move the last used element to the freed position
                //std::memcpy(&m_data[index], &m_data[m_usedCount], sizeof(T));
                m_data[index] = m_data[m_usedCount]; // Overwrite with the last used
            }
        }
    }

    MemoryBlock* m_next;
    T* m_data;
    size_t m_blockSize;
    size_t m_usedCount;
};

// MemoryPool definition for C++98 compatibility
template <typename T>
class MemoryPool {
public:
    MemoryPool(size_t blockSize)
        : m_blockSize(blockSize), m_head(NULL), m_numBlocks(0) {}

    ~MemoryPool() {
        while (m_head) {
            MemoryBlock<T>* temp = m_head;
            m_head = m_head->m_next;
            delete temp;
        }
    }

    T* allocate() {
        if (!m_head || m_head->isFull()) {
            allocateBlock();
        }
        return m_head->allocate();
    }

    void deallocate(T* ptr) {
        if (!m_head || !ptr) return;

        // Find the block that contains ptr
        MemoryBlock<T>* current = m_head;
        while (current) {
            if (ptr >= current->m_data && ptr < current->m_data + current->m_blockSize) {
                current->deallocate(ptr);
                if (current->isEmpty()) {
                    removeEmptyBlock();
                }
                return;
            }
            current = current->m_next;
        }
    }

private:
    void allocateBlock() {
        MemoryBlock<T>* newBlock = new MemoryBlock<T>(m_blockSize);
        newBlock->m_next = m_head;
        m_head = newBlock;
        ++m_numBlocks;
    }

    void removeEmptyBlock() {
        if (!m_head) return; // No blocks to remove

        MemoryBlock<T>* temp = m_head;
        m_head = m_head->m_next;
        delete temp;
    }

    size_t m_blockSize;
    MemoryBlock<T>* m_head;
    size_t m_numBlocks;
};

// PoolAllocator definition for C++98 compatibility
#include <cstddef>  // For std::size_t and std::ptrdiff_t
#include <new>      // For placement new
#include <limits>   // For std::numeric_limits

// MemoryBlock and MemoryPool are assumed to be implemented here, as previously provided.

template <typename T>
class PoolAllocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    // Constructor accepting a reference to an external memory pool
    explicit PoolAllocator(MemoryPool<T>& sharedPool) : m_pool(&sharedPool) {}

    // Copy constructor for the same type (shares the same pool)
    PoolAllocator(const PoolAllocator& other) : m_pool(other.m_pool) {}

    // Copy constructor for a different type allocator (shares the same pool)
    template <typename U>
    PoolAllocator(const PoolAllocator<U>& other) : m_pool(reinterpret_cast<MemoryPool<T>*>(other.m_pool)) {}

    // Allocate memory for n objects
    T* allocate(size_type n, const void* hint = 0) {
        (void)hint;
        if (n == 1) {
            return m_pool->allocate();
        } else {
            throw std::bad_alloc();  // Only allocating one object at a time
        }
    }

    // Deallocate memory for n objects
    void deallocate(T* p, size_type n) {
        if (n == 1) {
            m_pool->deallocate(p);
        }
    }

    // Construct an object in allocated memory
    void construct(pointer p, const_reference val) {
        new(static_cast<void*>(p)) T(val);
    }

    // Destroy an object in allocated memory
    void destroy(pointer p) {
        p->~T();
    }

    // Return the maximum number of elements that can be allocated
    size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    // Rebind allocator to another type
    template <typename U>
    struct rebind {
        typedef PoolAllocator<U> other;
    };

private:
    MemoryPool<T>* m_pool;  // Pointer to the shared MemoryPool

    template <typename U>
    friend class PoolAllocator;
};

// Comparison operators for PoolAllocator
template <typename T1, typename T2>
bool operator==(const PoolAllocator<T1>&, const PoolAllocator<T2>&) {
    return true;
}

template <typename T1, typename T2>
bool operator!=(const PoolAllocator<T1>&, const PoolAllocator<T2>&) {
    return false;
}

// Example usage

int main() {
    // Shared memory pool for std::pair<const int, std::string>
    MemoryPool<std::pair<int, std::string> > pool(1024);

    // Create PoolAllocator for std::pair<const int, std::string>
    PoolAllocator<std::pair<int, std::string> > alloc(pool);

    std::less<int> comparator;
    // Create two std::maps using the same allocator
    std::map<int, std::string, std::less<int>, PoolAllocator<std::pair<int, std::string> > > map1(comparator, alloc);
    std::map<int, std::string, std::less<int>, PoolAllocator<std::pair<int, std::string> > > map2(comparator, alloc);

    // Insert values into the first map
    map1[1] = "One";
    map1[2] = "Two";

    // Insert values into the second map
    map2[3] = "Three";
    map2[4] = "Four";

    // Display contents of the first map
    std::cout << "Map 1:" << std::endl;
    for (std::map<int, std::string, std::less<int>, PoolAllocator<std::pair<int, std::string> > >::iterator it = map1.begin(); it != map1.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    // Display contents of the second map
    std::cout << "Map 2:" << std::endl;
    for (std::map<int, std::string, std::less<int>, PoolAllocator<std::pair<int, std::string> > >::iterator it = map2.begin(); it != map2.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    return 0;
}
