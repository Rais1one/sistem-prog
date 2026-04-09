#include <iostream>
#include <iomanip>

int main() {
    setlocale(LC_ALL, "Russian");
    float* q = new float; 

   
    *q = -7.5; 

    std::cout << "Значение переменной q: " << std::fixed << std::setprecision(2) << *q << std::endl;

 
    delete q; 

    q = nullptr; 

    return 0;
}
