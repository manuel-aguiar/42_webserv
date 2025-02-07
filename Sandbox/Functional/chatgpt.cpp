#include <iostream>

// Define a type for the onEvents function pointer
typedef void (*OnEventsFunc)(void*);

// Template class for Socket
template <typename InterpreterFunctor>
class Socket {
private:
    OnEventsFunc onEventsFunc;  // Function pointer for onEvents
    InterpreterFunctor& interpreter;  // Reference to a stateful functor

public:
    // Constructor that accepts the onEvents function pointer and an interpreter functor
    Socket(OnEventsFunc onEvents, InterpreterFunctor& interp)
        : onEventsFunc(onEvents), interpreter(interp) {
        // Initialize other members...
    }

    // Method that triggers the event handler
    void onEvents() {
        onEventsFunc(this);  // Call the event handling function
    }

    // Method that triggers the interpreter logic
    void interpret() {
        interpreter(this);  // Call the functor (interpreter) assigned to this socket
    }

    // Static functions for specific event handling behavior
    static void serverOnEvents(void* obj) {
        Socket<InterpreterFunctor>* self = static_cast<Socket<InterpreterFunctor>*>(obj);
        std::cout << "ServerSocket handling events" << std::endl;
        // Server-specific event handling logic
    }

    static void connectionOnEvents(void* obj) {
        Socket<InterpreterFunctor>* self = static_cast<Socket<InterpreterFunctor>*>(obj);
        std::cout << "ConnectionSocket handling events" << std::endl;
        // Connection-specific event handling logic
    }
};

// HTTPInterpreter class with state (no inheritance, just a functor)
class HTTPInterpreter {
private:
    int requestCount;  // Example of state: number of requests handled

public:
    HTTPInterpreter() : requestCount(0) {}

    // Callable operator that interprets data and updates state
    void operator()(void* obj) {
        Socket<HTTPInterpreter>* self = static_cast<Socket<HTTPInterpreter>*>(obj);
        std::cout << "Interpreting as HTTP. Request #" << ++requestCount << std::endl;
        // Logic for interpreting HTTP data and updating internal state
    }
};

// FTPInterpreter class with state (no inheritance, just a functor)
class FTPInterpreter {
private:
    int fileTransferCount;  // Example of state: number of files transferred

public:
    FTPInterpreter() : fileTransferCount(0) {}

    // Callable operator that interprets data and updates state
    void operator()(void* obj) {
        Socket<FTPInterpreter>* self = static_cast<Socket<FTPInterpreter>*>(obj);
        std::cout << "Interpreting as FTP. File transfer #" << ++fileTransferCount << std::endl;
        // Logic for interpreting FTP data and updating internal state
    }
};

#include <vector>

int main() {

    std::vector <ConnInfo >v;

    // Create specific interpreters with state
    HTTPInterpreter httpInterpreter;
    FTPInterpreter ftpInterpreter;

    // Create a "server" socket with server-specific onEvents and the HTTP interpreter
    Socket<HTTPInterpreter> server(&Socket<HTTPInterpreter>::serverOnEvents, httpInterpreter);

    // Create a "connection" socket with connection-specific onEvents and the FTP interpreter
    Socket<FTPInterpreter> connection(&Socket<FTPInterpreter>::connectionOnEvents, ftpInterpreter);

    // Call onEvents for each type
    server.onEvents();     // Outputs: "ServerSocket handling events"
    connection.onEvents(); // Outputs: "ConnectionSocket handling events"

    // Call interpret for each type (which uses interpreters with state)
    server.interpret();     // Outputs: "Interpreting as HTTP. Request #1"
    server.interpret();     // Outputs: "Interpreting as HTTP. Request #2"
    
    connection.interpret(); // Outputs: "Interpreting as FTP. File transfer #1"
    connection.interpret(); // Outputs: "Interpreting as FTP. File transfer #2"

    return 0;
}
