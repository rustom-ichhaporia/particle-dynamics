#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "../build/_deps/json-src/include/nlohmann/json.hpp"
#include "cinder/gl/gl.h"
#include "core/particle.h"
#include <any>

using glm::vec2;
using nlohmann::json;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;
using std::any;

namespace idealgas {

class ParticleContainer {
 public:
  // ParticleContainer(size_t width, size_t height);
  unordered_map<string, string> ConfigureSizes();
  // unordered_map<string, Color> ConfigureColors();
  vector<Particle> GetParticles();

  // Absolute path to config file (used because of relative path involves many
  // steps from debug directory)

 private:
  string kConfigPath =
      "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
      "ideal-gas-rustom-ichhaporia/config.json";

  void InitializeParticles();

  // The number of particles in the container at a given time
  size_t width_;
  size_t height_;

  size_t particle_count_;
  vector<Particle> particles_;
};

}  // namespace idealgas