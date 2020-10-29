#include "core/particle_container.h"
#include "core/particle.h"
#include "../build/_deps/json-src/include/nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include "cinder/gl/gl.h"
#include <any>


using idealgas::ParticleContainer;
using idealgas::Particle;
using nlohmann::json;
using std::string;
using std::ifstream;
using std::stoi;
using std::vector;
using std::map;
using glm::vec2;
using std::unordered_map;
using std::any;

namespace idealgas {

// ParticleContainer::ParticleContainer(size_t width, size_t height) {
//   width_ = width;
//   height_ = height;
//   Configure();
// }

unordered_map<string, string> ParticleContainer::ConfigureSizes() {
  ifstream input(kConfigPath);
  json config;

  input >> config;
  particle_count_ = stoi(string(config["container"]["particle count"]));

  unordered_map<string, string> visualizer_info;
  visualizer_info["window width"] = config["window"]["width"];
  visualizer_info["window height"] = config["window"]["height"];
  visualizer_info["margin"] = config["window"]["margin"];
  visualizer_info["stroke"] = config["window"]["stroke"];
  visualizer_info["background color"] = config["window"]["background color"];
  visualizer_info["stroke color"] = config["window"]["stroke color"];
  visualizer_info["text color"] = config["window"]["text color"];

  width_ = stoi(visualizer_info["window width"]) - 2 * stoi(visualizer_info["margin"]);
  height_ = stoi(visualizer_info["window height"]) - 2 * stoi(visualizer_info["margin"]);

  InitializeParticles();

  return visualizer_info;
}

// unordered_map<string, Color> ParticleContainer::ConfigureColors() {
//   // ifstream input(kConfigPath)
// }

void ParticleContainer::InitializeParticles() {
  for (size_t i = 0; i < particle_count_; ++i) {
    particles_.push_back(Particle(vec2(i * 10, i * 10), vec2(0, 0), 5, 5, Color(100, 100, 100)));
  }
}

}  // namespace idealgas