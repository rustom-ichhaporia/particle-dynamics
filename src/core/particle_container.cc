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
// using nlohmann::json::iterator;
using std::string;
using std::stoull;
using std::ifstream;
using std::stoi;
using std::vector;
using std::map;
using glm::vec2;
using glm::distance;
using glm::dot;
using std::to_string;
using std::unordered_map;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::any;
using std::mt19937;
using ci::ColorT;
using std::random_device;
using std::pow;
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
  // particle_count_ = stoi(string(config["container"]["particle count"]));
  // min_velocity_ = stoi(string(config["container"]["min velocity"]));
  // max_velocity_ = stoi(string(config["container"]["max velocity"]));
  // min_radius_ = stoi(string(config["container"]["min radius"]));
  // max_radius_ = stoi(string(config["container"]["max radius"]));

  unordered_map<string, string> visualizer_info;
  visualizer_info["window width"] = config["window"]["width"];
  visualizer_info["window height"] = config["window"]["height"];
  visualizer_info["margin"] = config["window"]["margin"];
  visualizer_info["stroke"] = config["window"]["stroke"];
  visualizer_info["background color"] = config["window"]["background color"];
  visualizer_info["stroke color"] = config["window"]["stroke color"];
  visualizer_info["text color"] = config["window"]["text color"];
  visualizer_info["font"] = config["window"]["font"];

  width_ = stoi(visualizer_info["window width"]) * 3 / 4 - stoi(visualizer_info["margin"]);
  visualizer_info["container width"] = to_string(width_);
  height_ = stoi(visualizer_info["window height"]) - 2 * stoi(visualizer_info["margin"]);
  visualizer_info["container height"] = to_string(height_);

  visualizer_info["histogram bin count"] = config["histogram"]["bin count"];

  for (auto it = config["container"]["particles"].begin(); it != config["container"]["particles"].end(); ++it) {
    ColorT<float> color = ColorT<float>().hex(stoull(string(it.value()["color"]), 0, 16));
    
    particle_names_.push_back(it.key());

    InitializeParticles(it.key(), it.value()["particle count"],
                        it.value()["min velocity"], it.value()["max velocity"],
                        it.value()["min mass"], it.value()["max mass"],
                        it.value()["min radius"], it.value()["max radius"], color);
    
  }

  return visualizer_info;
}

void ParticleContainer::Increment() {
  sort(particles_.begin(), particles_.end(),
       [](const Particle& lhs, const Particle& rhs) {
         return lhs.GetPosition().x < rhs.GetPosition().x;
       });

  for (size_t base = 0; base < particles_.size(); ++base) {
    size_t cutoff = base + particles_.size() / max_radius_;
    if (cutoff > particles_.size() - 1) {
      cutoff = particles_.size();
    }
    for (size_t neighbor = base + 1; neighbor < cutoff; ++neighbor) {
      if (ExecuteCollision(base, neighbor)) {
        particles_.at(base).SetColor(particles_.at(base).GetColor() * ColorT<float>(0.99, 0.99, 1));
      }
    }
  }

  sort(particles_.begin(), particles_.end(),
       [](const Particle& lhs, const Particle& rhs) {
         return lhs.GetPosition().x < rhs.GetPosition().x;
       });

  for (size_t index = 0; index < particles_.size(); ++index) {
    // particles_[index].SetColor(particles_.at(index).GetColor() * ColorT<float>());
    if (ExecuteWalls(index)) {
      particles_.at(index).SetColor(particles_.at(index).GetColor() * ColorT<float>(1, 0.95, 0.95));
    }      
    particles_.at(index).SetPosition(particles_.at(index).GetPosition() + particles_.at(index).GetVelocity());
  }
}

vector<Particle>& ParticleContainer::GetParticles() {
  return particles_;
}

vector<string>& ParticleContainer::GetParticleNames() {
  return particle_names_;
}

void ParticleContainer::InitializeParticles(
    const string& name, size_t particle_count, size_t min_velocity,
    size_t max_velocity, size_t min_mass, size_t max_mass, size_t min_radius,
    size_t max_radius, const ColorT<float>& color) {
  random_device rd;  //Used to obtain a seed for the random number engine
  mt19937 gen(rd()); //Gets random position from distribution
  uniform_int_distribution<> width_dist(0, width_); // Distribution of possible x values
  uniform_int_distribution<> height_dist(0, height_); // Distribution of y values
  uniform_real_distribution<> velocity_dist(min_velocity, max_velocity);
  uniform_real_distribution<> mass_dist(min_mass, max_mass);
  uniform_real_distribution<> radius_dist(min_radius, max_radius);
  
  for (size_t i = 0; i < particle_count; ++i) {
    particles_.push_back(Particle(name, vec2(width_dist(gen), height_dist(gen)), 
        vec2(velocity_dist(gen), velocity_dist(gen)), mass_dist(gen), radius_dist(gen), color));
  }
}

bool ParticleContainer::ExecuteWalls(size_t index) {
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

bool ParticleContainer::ExecuteCollision(size_t base, size_t neighbor) {
  auto p1 = particles_.at(base);
  auto p2 = particles_.at(neighbor);
  float distance_cutoff = p1.GetRadius() + p2.GetRadius();
  vec2 x1 = p1.GetPosition();
  vec2 x2 = p2.GetPosition();
  vec2 v1 = p1.GetVelocity();
  vec2 v2 = p2.GetVelocity();
  float m1 = p1.GetMass();
  float m2 = p2.GetMass();
  float distance_between = distance(x1, x2);
  float displacement_threshold = dot(v1 - v2, x1 - x2);

  if (distance_between <= distance_cutoff && displacement_threshold < 0) {
    float mass_term_1 = 2 * m2 / (m1 + m2);
    vec2 interaction_term_1 = dot(v1 - v2, x1 - x2) / length(x1 - x2) / length(x1 - x2) * (x1 - x2);
    vec2 new_velocity_1 = v1 - mass_term_1 * interaction_term_1;
    particles_.at(base).SetVelocity(new_velocity_1);

    float mass_term_2 = 2 * m1 / (m1 + m2);
    vec2 interaction_term_2 = dot(v2 - v1, x2 - x1) / length(x2 - x1) / length(x2 - x1) * (x2 - x1);
    vec2 new_velocity_2 = v2 - mass_term_2 * interaction_term_2;
    particles_.at(neighbor).SetVelocity(new_velocity_2);
    return true;
  }
  return false;
}

}  // namespace idealgas