#include "core/particle_container.h"
#include "core/particle.h"
#include "../build/_deps/json-src/include/nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include "cinder/gl/gl.h"
#include <any>
#include <random>
#include <algorithm>

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
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::any;
using std::mt19937;
using ci::ColorT;
using std::random_device;
using std::sort;  

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
  max_velocity_ = stoi(string(config["container"]["max velocity"]));
  max_radius_ = stoi(string(config["container"]["max radius"]));

  unordered_map<string, string> visualizer_info;
  visualizer_info["window width"] = config["window"]["width"];
  visualizer_info["window height"] = config["window"]["height"];
  visualizer_info["margin"] = config["window"]["margin"];
  visualizer_info["stroke"] = config["window"]["stroke"];
  visualizer_info["background color"] = config["window"]["background color"];
  visualizer_info["stroke color"] = config["window"]["stroke color"];
  visualizer_info["text color"] = config["window"]["text color"];
  visualizer_info["font"] = config["window"]["font"];

  width_ = stoi(visualizer_info["window width"]) - 2 * stoi(visualizer_info["margin"]);
  height_ = stoi(visualizer_info["window height"]) - 2 * stoi(visualizer_info["margin"]);

  InitializeParticles();

  return visualizer_info;
}

void ParticleContainer::Increment() {
  sort(particles_.begin(), particles_.end(),
       [](const Particle& lhs, const Particle& rhs) {
         return lhs.GetPosition().x < rhs.GetPosition().x;
       });

  for (size_t index = 0; index < particles_.size(); ++index) {// - max_radius_; ++index) {
    if (CheckWalls(index)) {
      particles_.at(index).SetColor(particles_.at(index).GetColor() * ColorT<float>(1, 0.8, 0.8));
    }
    // for (size_t neighbor = index; neighbor < max_radius_ * 2; ++neighbor) {
      particles_[index].SetPosition(particles_[index].GetPosition() + particles_[index].GetVelocity());
    // }
  }
}

vector<Particle> ParticleContainer::GetParticles() {
  return particles_;
}

void ParticleContainer::InitializeParticles() {
  random_device rd;  //Used to obtain a seed for the random number engine
  mt19937 gen(rd()); //Gets random position from distribution
  uniform_int_distribution<> width_distribution(0, width_); // Distribution of possible x values
  uniform_int_distribution<> height_distribution(0, height_); // Distribution of y values
  uniform_real_distribution<> velocity_distribution(0, max_velocity_);
  
  for (size_t i = 0; i < particle_count_; ++i) {
    particles_.push_back(Particle(vec2(width_distribution(gen), height_distribution(gen)), 
        vec2(velocity_distribution(gen), velocity_distribution(gen)), 5, 5, ColorT<float>(1, 1, 1)));
  }
}

bool ParticleContainer::CheckWalls(size_t index) {
  auto position = particles_.at(index).GetPosition();
  auto velocity = particles_.at(index).GetVelocity();
  auto radius = particles_.at(index).GetRadius();
  if (position.x <= radius && velocity.x < 0) {
    particles_.at(index).SetVelocity(velocity * vec2(-1, 1));
  } 
  if (position.x >= width_ - radius && velocity.x > 0) {
    particles_.at(index).SetVelocity(velocity * vec2(-1, 1));
  }
  if (position.y <= radius && velocity.y < 0) {
    particles_.at(index).SetVelocity(velocity * vec2(1, -1));
  }
  if (position.y >= height_ - radius && velocity.y > 0) {
    particles_.at(index).SetVelocity(velocity * vec2(1, -1));
  }
  return particles_.at(index).GetVelocity() != velocity;
}

}  // namespace idealgas