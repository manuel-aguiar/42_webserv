
#include <cstdlib>

template<typename T>
class UniquePtr
{
public:
    // Constructor: takes ownership of the raw pointer
    explicit UniquePtr(T* newPtr = NULL) : _ptr(newPtr) {}

    // Destructor: deletes the managed pointer
    ~UniquePtr() {
        _safeDeletePtr();
    }

    // Move constructor: transfer ownership from another UniquePtr
    UniquePtr(UniquePtr& moveCopy) : _ptr(moveCopy._ptr) {
        moveCopy._ptr = NULL;  // Set the moved-from object’s pointer to NULL
    }

    // Move assignment operator: transfer ownership from another UniquePtr
    UniquePtr& operator=(UniquePtr& moveAssign) {
        if (this != &moveAssign) {    // Avoid self-assignment
            _safeDeletePtr();         // Delete the current resource
            _ptr = moveAssign._ptr;   // Transfer ownership
            moveAssign._ptr = NULL;   // Set the moved-from object’s pointer to NULL
        }
        return *this;
    }

    // Disable copy constructor and copy assignment (private)
private:
    UniquePtr(const UniquePtr& copy);            // Prevent copying
    UniquePtr& operator=(const UniquePtr& copy); // Prevent copy assignment

public:
    // Access the underlying pointer
    T* get() const {
        return _ptr;
    }

    // Dereference operators
    T& operator*() const {
        return *_ptr;
    }

    T* operator->() const {
        return _ptr;
    }

    // Release ownership and return the raw pointer
    T* release() {
        T* oldPtr = _ptr;
        _ptr = NULL;
        return oldPtr;
    }

    // Reset the pointer, optionally to a new one
    void reset(T* newPtr = NULL) {
        _safeDeletePtr();
        _ptr = newPtr;
    }

private:
    T* _ptr;  // Raw pointer

    // Helper function to safely delete the managed pointer
    void _safeDeletePtr() {
        if (_ptr) {
            delete _ptr;
            _ptr = NULL;
        }
    }
};

#include <iostream>

// Usage example
struct Test {
    void sayHello() {
        std::cout << "Hello from Test!" << std::endl;
    }
};

// A function that allocates and returns a pointer
Test* create() {
    return new Test();
}

UniquePtr<Test> createTest() {
    return UniquePtr<Test>(new Test());
}

int main() {
    // Create a new Test object and pass it to the unique pointer
    UniquePtr<Test> p(create());
    UniquePtr<Test> p2 = createTest();
    // Use the smart pointer
    p->sayHello();

    // No need to manually delete the object; the destructor of UniquePtr will take care of it
    return 0;
}