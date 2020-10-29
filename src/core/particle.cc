#include "core/particle.h"
// #include "cinder/gl/gl.h"

using ci::ColorT;

namespace idealgas {

Particle::Particle(const vec2& position, const vec2& velocity, double mass, double radius, const ColorT<float>& color)
    : position_(position), velocity_(velocity), mass_(mass), radius_(radius), color_(color) {
}

vec2 Particle::GetPosition() const {
  return position_;
}

void Particle::SetPosition(const vec2& position) {
  position_ = position;
}

vec2 Particle::GetVelocity() const {
  return velocity_;
}

void Particle::SetVelocity(const vec2& velocity) {
  velocity_ = velocity;
}

double Particle::GetMass() const {
  return mass_;
}

void Particle::SetMass(double mass) {
  mass_ = mass;
}

double Particle::GetRadius() const {
  return radius_;
}

void Particle::SetRadius(double radius) {
  radius_ = radius;
}

ColorT<float> Particle::GetColor() const {
  return color_;
}

void Particle::SetColor(const ColorT<float>& color) {
  color_ = color;
}

}  // namespace idealgas