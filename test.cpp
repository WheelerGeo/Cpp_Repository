#include <iostream>
#include <memory>

class Test{
public:
    std::shared_ptr<Test> getSharedPtr(void) {
        return std::shared_ptr<Test>(this);
    }
    ~Test() {
        std::cout << "Test is distructed" << std::endl;
    }
};


int main(int argc, char **argv) {
    std::shared_ptr<Test> ptr1(new Test);
    std::shared_ptr<Test> ptr2 = ptr1->getSharedPtr();

    return 0;
}