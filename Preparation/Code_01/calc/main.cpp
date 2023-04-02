#include "calc.h"
#include <iostream>

int main() {

  int a = 10, b = 2;
  std::cout << "a = " << a << " ,b = " << b << std::endl;
  std::cout << "add (a,b) = " << calc::add(a, b) << std::endl;
  std::cout << "sub (a,b) = " << calc::sub(a, b) << std::endl;
  std::cout << "div (a,b) = " << calc::div(a, b) << std::endl;
  std::cout << "mult(a,b) = " << calc::mult(a, b) << std::endl;
 
  return 0;
}
