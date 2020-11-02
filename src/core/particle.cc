#include "core/particle.h"

using ci::ColorT;

namespace idealgas {

Particle::Particle() {
  
}

Particle::Particle(const string& name, const vec2& position,
                   const vec2& velocity, float mass, float radius,
                   const ColorT<float>& color)
    // Use initializer list to fill private variables
    : name_(name),
      position_(position),
      velocity_(velocity),
      mass_(mass),
      radius_(radius),
      color_(color) {
}

string Particle::GetName() const {
  return name_;
}

void Particle::SetName(const string& name) {
  name_ = name;
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

float Particle::GetMass() const {
  return mass_;
}

void Particle::SetMass(float mass) {
  mass_ = mass;
}

float Particle::GetRadius() const {
  return radius_;
}

void Particle::SetRadius(float radius) {
  radius_ = radius;
}

ColorT<float> Particle::GetColor() const {
  return color_;
}

void Particle::SetColor(const ColorT<float>& color) {
  color_ = color;
}

}  // namespace idealgas