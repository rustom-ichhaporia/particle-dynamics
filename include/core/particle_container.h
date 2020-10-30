#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "../build/_deps/json-src/include/nlohmann/json.hpp"
#include "cinder/gl/gl.h"
#include "core/particle.h"

using glm::vec2;
using nlohmann::json;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;

namespace idealgas {

class ParticleContainer {
 public:
  // ParticleContainer(size_t width, size_t height);
  unordered_map<string, string> ConfigureSizes();
  void Increment();
  vector<Particle>& GetParticles();
  vector<string>& GetParticleNames();

  // Absolute path to config file (used because of relative path involves many
  // steps from debug directory)

 private:
  string kConfigPath =
      "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
      "ideal-gas-rustom-ichhaporia/config.json";

  void InitializeParticles(const string& name, size_t particle_count, size_t min_velocity, size_t max_velocity, size_t min_mass, size_t max_mass, size_t min_radius, size_t max_radius, const ColorT<float>& color);
  bool ExecuteWalls(size_t index);
  bool ExecuteCollision(size_t base, size_t neighbor);

  // The number of particles in the container at a given time
  size_t width_;
  size_t height_;
  // size_t min_velocity_;
  // size_t max_velocity_;
  // size_t min_mass_;
  // size_t max_mass_;
  // size_t min_radius_;
  size_t max_radius_ = 10;

  // size_t particle_count_ = 100;
  vector<string> particle_names_;
  vector<Particle> particles_;
};

}  // namespace idealgas