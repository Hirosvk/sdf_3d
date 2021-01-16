#include <iostream>
#include <set>

int main () {
  int i1[] = {0, 2, 3, 4};
  std::set<int> s1(i1, i1+4);

  int i2[] = {2, 0, 3, 4, 5};
  std::set<int> s2(i2, i2+5);

  std::cout << (s1 == s2) << std::endl;

  return 0;
}
