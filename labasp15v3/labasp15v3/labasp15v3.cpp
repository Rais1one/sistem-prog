#include <iostream>
#include <cmath>
#include <stdio.h>
#include <clocale>

extern "C" double compute_function(double x);

int main()
{
	setlocale(LC_ALL, "Russian");

	double x;
	std::cout << "Введите значение х: ";
	std::cin >> x;
	double result = compute_function(x);

	std::cout << "f(" << x << ") = " << result << std::endl;
	return 0;
}