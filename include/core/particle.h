#pragma once
#include "cinder/gl/gl.h"

using ci::ColorT;
using glm::vec2;

namespace idealgas {

class Particle {
 public:
  Particle(const vec2& position, const vec2& velocity, double mass, double radius, const ColorT<float>& color);
  vec2 GetPosition() const;
  void SetPosition(const vec2& position);
  vec2 GetVelocity() const;
  void SetVelocity(const vec2& velocity);
  double GetMass() const;
  void SetMass(double mass);
  double GetRadius() const;
  void SetRadius(double radius);
  ColorT<float> GetColor() const;
  void SetColor(const ColorT<float>& color);

 private:
  vec2 position_;
  vec2 velocity_;
  double mass_;
  double radius_;
  ColorT<float> color_;
};

}  // namespace idealgas