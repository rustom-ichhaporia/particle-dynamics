#include <iostream>

#include "Cinder/gl/gl.h"

int main(int, char**) {
  std::cout << "Hello, world!\n";
  glm::vec2 position(5.5, 6.7);
  glm::vec2 velocity(0.4, -0.6);
  std::cout << position << velocity;
}
