#include <iostream>
#include <vector>

int calc(int n, int (*f)(int) ) {
  return f(n);
}

int twice(int n) {
  return n * 2;
}

int main () {

  std::cout << calc(2, twice) << std::endl;

  return 0;
}
