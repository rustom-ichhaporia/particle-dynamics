#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "cinder/gl/gl.h"
#include "core/particle.h"

using glm::vec2;
using nlohmann::json;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;

namespace idealgas {

/**
 * @brief The ParticleContainer class holds all the logic behind the particle
 * collisions with walls and other particles and manages the particles during
 * each time increment.
 *
 */
class ParticleContainer {
 public:
  /**
   * @brief The string map of parameters for the window read from the
   * configuration file.
   *
   * @return unordered_map<string, string> with key variable name and value of
   * corresponding variable
   */
  unordered_map<string, string> Configure(const string& config_path);

  /**
   * @brief Initializes a set of particles of a given type by creating them with
   * randomly dispersed masses, radii, and speeds within the constraints of the
   * input.
   *
   * @param name the string name of the particle type
   * @param particle_count the number of particles to initialize
   * @param min_velocity the minimum velocity of each particle
   * @param max_velocity the maximum velocity of each particle
   * @param min_mass the minimum mass of each particle
   * @param max_mass the maximum mass of each particle
   * @param min_radius the minimum radius of each particle
   * @param max_radius the maximum radius of each particle
   * @param color the color of the particle
   */
  void InitializeParticles(const string& name, size_t particle_count,
                           size_t min_velocity, size_t max_velocity,
                           size_t min_mass, size_t max_mass, size_t min_radius,
                           size_t max_radius, const ColorT<float>& color);

  /**
   * @brief Initializes a particle in the container.
   *
   * @param particle the reference particle to add
   */
  void InitializeParticle(const Particle& particle);

  /**
   * @brief Increments the positions and velocities all of the particles in the
   * container by one time "step".
   *
   */
  void Increment();

  /**
   * @brief Gets a reference to the vector of particles in the container.
   *
   * @return vector<Particle>& the vector of particles in the container
   */
  vector<Particle>& GetParticles(); 

  /**
   * @brief Set the particles by a vector. 
   * 
   * @param particles a vector of Particle objects to reset
   */
  void SetParticles(const vector<Particle>& particles);

  /**
   * @brief Gets a reference to a vector of the particle type names.
   *
   * @return the vector of particle type strings
   */
  vector<string> GetParticleNames() const;

  /**
   * @brief Gets the current time step of the object. 
   * 
   * @return float representing the time step
   */
  float GetTimeStep() const;

  /**
   * @brief Sets the time step of the container
   * 
   * @param time_step the float time step to set
   */
  void SetTimeStep(float time_step);

 private:
  // The maximum allowed radius of a particle
  const size_t kRadiusLimit = 100;
  // The nearby particle limit for checking for collisions
  const size_t kNearbyLimit = 10;

  /**
   * @brief Checks and executes all collisions between particles.
   *
   */
  void IncrementParticleCollisions();

  /**
   * @brief Checks and executes all collisions between particles and a wall.
   *
   */
  void IncrementWallCollisions();

  /**
   * @brief Alters two given particles' velocities as required if they are close
   * to or touching each other.
   *
   * @param base the index of the first particle
   * @param neighbor the index of the second particle
   * @return true when a collision has occurred
   * @return false when a collision has occurred
   */
  bool ExecuteParticleCollision(size_t base, size_t neighbor);
  
  /**
   * @brief Alters a given particle's velocity as required if they are close to
   * a wall.
   *
   * @param index the index of the particle to check for wall collisions
   * @return true when a collision has occurred
   * @return false when a collision has not occurred
   */
  bool ExecuteWallCollision(size_t index);

  // A vector of all the particles in the container
  vector<Particle> particles_;
  // A vector of the names of particle types
  vector<string> particle_names_;
  // The time step for particle incrementing
  float time_step_ = 1;
  // The pixel width of the container
  size_t width_;
  // The pixel height of the container
  size_t height_;
};

}  // namespace idealgas