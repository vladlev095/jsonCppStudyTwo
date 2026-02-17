#include <iostream>

int main() {
    int numbers1[] = { 125, 312, 56, 666 , 32, 42, 44};
    int numbers2[] = { 111, 252, 43, 12, 33, 41 };
    size_t size1 = sizeof(numbers1)/sizeof(numbers1[0]);
    size_t size2 = sizeof(numbers2)/sizeof(numbers2[0]);
    size_t sizeMax = (size1 > size2) ? size1 : size2;
    for(int i = 0; i < sizeMax; i++) {
        if(i >= size1) {
            std::cout << numbers2[i];
        }
        else if(i >= size2) {
            std::cout << numbers1[i];
        }
        else {
            std::cout << numbers1[i] + numbers2[i];
        }
        if(i != sizeMax - 1) {
            std::cout << ", ";
        }
        else {
            std::cout << "\n";
        }
    }
//                                236, 564, 99, 678, 33, 41
}
