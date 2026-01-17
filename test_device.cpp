#include <iostream>
#include <string>

int main() {
    std::string deviceName = "______";
    std::cout << "Length: " << deviceName.length() << std::endl;
    std::cout << "Hex: ";
    for (unsigned char c : deviceName) {
        printf("%02x ", c);
    }
    std::cout << std::endl;
    return 0;
}
