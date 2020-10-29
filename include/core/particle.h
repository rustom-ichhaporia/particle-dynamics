#pragma once
#include "cinder/gl/gl.h"
#include <string>

using std::string;
using ci::ColorT;
using glm::vec2;

namespace idealgas {

class Particle {
 public:
  Particle(const string& name, const vec2& position, const vec2& velocity, float mass, float radius, const ColorT<float>& color);
  string GetName() const;
  void SetName(const string& name);
  vec2 GetPosition() const;
  void SetPosition(const vec2& position);
  vec2 GetVelocity() const;
  void SetVelocity(const vec2& velocity);
  float GetMass() const;
  void SetMass(float mass);
  float GetRadius() const;
  void SetRadius(float radius);
  ColorT<float> GetColor() const;
  void SetColor(const ColorT<float>& color);

 private:
  string name_;
  vec2 position_;
  vec2 velocity_;
  float mass_;
  float radius_;
  ColorT<float> color_;
};

}  // namespace idealgas