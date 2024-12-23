#include <iostream>
#include <cstring>  // for memmove

class ToolkitBase {
public:
    virtual void someothere() {
        std::cout << "Base someother!" << std::endl;
    }
    virtual void speak() {
        std::cout << "Base says: Hello!" << std::endl;
    }

    virtual ~ToolkitBase() {}
};

class ToolkitDerived : public ToolkitBase {
public:
    virtual void special() {
        std::cout << "Derived's special function!" << std::endl;
    }
    void speak() {
        std::cout << "Derived says: Hello!" << std::endl;
    }


};

int main() {
    ToolkitDerived obj1;
    ToolkitBase obj2;

    std::cout << "Before memmove:" << std::endl;
    obj1.speak();  // Calls Derived::speak
    obj2.speak();  // Calls Base::speak

    // Incorrect use of memmove, treating obj2 and obj1 as raw memory.
    std::memmove((void* )&obj2, (void* )&obj1, sizeof(ToolkitDerived));

    std::cout << "\nAfter memmove:" << std::endl;
    obj2.speak();  // Undefined behavior: should call Base::speak, but calls Derived's function or crashes.

    ToolkitBase* ptr1 = new ToolkitDerived;
    ToolkitBase* ptr2 = new ToolkitBase;

    std::cout << "\nPointer version:" << std::endl;
    std::memmove((void* )&ptr2, (void* )&ptr1, sizeof(ToolkitDerived));
    ptr2->speak();  

    return 0;
}
