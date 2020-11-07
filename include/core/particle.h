#pragma once
#include <string>

#include "cinder/gl/gl.h"

using ci::ColorT;
using glm::vec2;
using std::string;

namespace idealgas {

/**
 * @brief The Particle class contains getters and setters for all the properties of a particle in the simulation. 
 * 
 */
class Particle {
 public:
  Particle();

  /**
   * @brief Constructs a particle with the given parameters.
   *
   * @param name the string name of the particle
   * @param position the vec2 position of the particle
   * @param velocity the vec2 velocity of the particle
   * @param mass the float mass of the particle
   * @param radius the float radius of the particle
   * @param color the Color of the particle
   */
  Particle(const string& name, const vec2& position, const vec2& velocity,
           float mass, float radius, const ColorT<float>& color);

  /**
   * @brief Gets the name of the particle.
   *
   * @return the string name of the particle
   */
  string GetName() const;

  /**
   * @brief Sets the name of the particle.
   *
   * @param name the string name to be set
   */
  void SetName(const string& name);

  /**
   * @brief Gets the position of the particle.
   *
   * @return the vec2 position of the particle
   */
  vec2 GetPosition() const;

  /**
   * @brief Sets the position of the particle.
   *
   * @param position the vec2 position of the particle to be set
   */
  void SetPosition(const vec2& position);

  /**
   * @brief Gets the velocity of the particle.
   *
   * @return the vec2 velocity of the particle
   */
  vec2 GetVelocity() const;

  /**
   * @brief Sets the velocity of the particle.
   *
   * @param velocity the vec2 velocity of the particle to be set
   */
  void SetVelocity(const vec2& velocity);

  /**
   * @brief Gets the mass of the particle. 
   *
   * @return the float mass of the particle
   */
  float GetMass() const;

  /**
   * @brief Sets the mass of the particle. 
   * 
   * @param mass the float mass of the particle to be set
   */
  void SetMass(float mass);

  /**
   * @brief Gets the radius of the particle. 
   * 
   * @return the float radius of the particle
   */
  float GetRadius() const;

  /**
   * @brief Sets the radius of the particle. 
   * 
   * @param radius the float radius of the particle to be set
   */
  void SetRadius(float radius);

  /**
   * @brief Gets the color of the particle. 
   * 
   * @return the ColorT<float> of the particle
   */
  ColorT<float> GetColor() const;

  /**
   * @brief Sets the color of the particle. 
   * 
   * @param color the ColorT<float> of the particle to be set
   */
  void SetColor(const ColorT<float>& color);

 private:
  // The name of the particle
  string name_;
  // The position of the particle
  vec2 position_;
  // The velocity of the particle
  vec2 velocity_;
  // The mass of the particle
  float mass_;
  // The radius of the particle
  float radius_;
  // The color of the particle
  ColorT<float> color_;
};

}  // namespace idealgas