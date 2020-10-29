#include "core/particle.h"
// #include "cinder/gl/gl.h"

using ci::Color;

namespace idealgas {

Particle::Particle(vec2 position, vec2 velocity, double mass, double radius, Color color)
    : position_(position), velocity_(velocity), mass_(mass), radius_(radius), color_(color) {
}

double Particle::GetMass() {
  return mass_;
}

double Particle::GetRadius() {
  return radius_;
}

Color Particle::GetColor() {
  return color_;
}

}  // namespace idealgas