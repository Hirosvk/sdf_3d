#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main () {
  glm::vec3 v1(30.0, 30.0, 30.0);
  glm::vec3 v2(20.0, 20.0, 20.0);

  std::cout << (v1 - v2).r << std::endl;

  return 0;
}
