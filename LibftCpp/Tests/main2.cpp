




template<typename T, T nullvalue, int (*delete_)(T*)>
class UniqueObj
{
private:
    T t;

public:
    // Constructor: take ownership of a resource
    UniqueObj(T t = nullvalue) : t(t) {}

    // Destructor: release the resource
    ~UniqueObj() {
        if (t != nullvalue) {
            delete_(t);
        }
    }

    // Disable copying
private:
    UniqueObj(const UniqueObj&);
    UniqueObj& operator=(const UniqueObj&);

public:
    // Enable move semantics via transfer of ownership
    void transfer(UniqueObj& other) {
        if (this != &other) {
            if (t != nullvalue) {
                delete_(t);
            }
            t = other.t;
            other.t = nullvalue;
        }
    }

    // Get the underlying resource (like file descriptor)
    T get() const {
        return t;
    }

    // Explicit bool operator for checking validity
    operator bool() const {
        return t != nullvalue;
    }
};

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
// Function to create a unique_fd
//typedef UniqueObj<int, -1, ::close> unique_fd;
typedef UniqueObj<pthread_mutex_t, 0, ::pthread_mutex_destroy> unique_mutex;

int main() {
    // Open a file and create a unique file descriptor
    //unique_fd fd1(open("fd.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644));
//
    //// Check if the file descriptor is valid
    //if (fd1.get() != -1) {
    //    // Write to the file
    //    write(fd1.get(), "hello\n", 6);
    //}

    pthread_mutex_t pthread_mutex;

    pthread_mutex_init(&pthread_mutex, NULL);

    unique_mutex mutex(pthread_mutex);

    pthread_mutex_lock(&mutex.get());

    std::cout << "writing while locked" << std::endl;

    pthread_mutex_unlock(&mutex.get());
    return 0;
}