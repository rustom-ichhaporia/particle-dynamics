#pragma once
#include "cinder/gl/gl.h"

using ci::Color;
using glm::vec2;

namespace idealgas {

class Particle {
 public:
  Particle(vec2 position, vec2 velocity, double mass, double radius, Color color);
  vec2 GetPosition();
  vec2 GetVelocity();
  double GetMass();
  double GetRadius();
  Color GetColor();

 private:
  vec2 position_;
  vec2 velocity_;
  double mass_;
  double radius_;
  Color color_;
};

}  // namespace idealgas