#include "core/particle_container.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "cinder/gl/gl.h"
#include "core/particle.h"
#include "nlohmann/json.hpp"

using ci::ColorT;
using glm::dot;
using glm::vec2;
using idealgas::Particle;
using idealgas::ParticleContainer;
using std::stoi;
using std::string;
using std::vector;

namespace idealgas {

std::unordered_map<string, string> ParticleContainer::Configure(const string& config_path) {
  // Read in JSON configuration file information
  std::ifstream input(config_path);
  nlohmann::json config;
  input >> config;

  // Create mapping between variable names and values
  std::unordered_map<string, string> visualizer_info;

  visualizer_info["window width"] = config["window"]["width"];
  visualizer_info["window height"] = config["window"]["height"];
  visualizer_info["margin"] = config["window"]["margin"];
  visualizer_info["stroke"] = config["window"]["stroke"];
  visualizer_info["background color"] = config["window"]["background color"];
  visualizer_info["stroke color"] = config["window"]["stroke color"];
  visualizer_info["text color"] = config["window"]["text color"];
  visualizer_info["font"] = config["window"]["font"];

  // Cast width and height to strings, others are cast in visualizer class
  width_ = stoi(visualizer_info["window width"]) * 3 / 4 -
           stoi(visualizer_info["margin"]);
  visualizer_info["container width"] = std::to_string(width_);
  height_ = stoi(visualizer_info["window height"]) -
            2 * stoi(visualizer_info["margin"]);
  visualizer_info["container height"] = std::to_string(height_);

  visualizer_info["histogram bin count"] = config["histogram"]["bin count"];

  for (auto it = config["container"]["particles"].begin();
       it != config["container"]["particles"].end(); ++it) {
    // Iterating through, initialie particles of each type
    ColorT<float> color =
        ColorT<float>().hex(std::stoull(string(it.value()["color"]), 0, 16));

    particle_names_.push_back(it.key());

    InitializeParticles(it.key(), it.value()["particle count"],
                        it.value()["min velocity"], it.value()["max velocity"],
                        it.value()["min mass"], it.value()["max mass"],
                        it.value()["min radius"], it.value()["max radius"],
                        color);
  }

  return visualizer_info;
}

void ParticleContainer::InitializeParticles(
    const string& name, size_t particle_count, size_t min_velocity,
    size_t max_velocity, size_t min_mass, size_t max_mass, size_t min_radius,
    size_t max_radius, const ColorT<float>& color) {
  // Check if radius argument is too large for visualizer
  if (max_radius > kRadiusLimit) {
    throw std::invalid_argument(
        ("The maximum allowed radius is: " + std::to_string(kRadiusLimit) +
         ". Please edit your configuration file."));
  }

  // Used to obtain a seed for the random number engine
  std::random_device rd;   
  // Gets random position from distribution
  std::mt19937 gen(rd());  
  // Distribution of possible x values
  std::uniform_int_distribution<> width_dist(0, width_);
  // Distribution of y values
  std::uniform_int_distribution<> height_dist(0, height_);
  // Distribution of velocities
  std::uniform_real_distribution<> velocity_dist(min_velocity, max_velocity);
  // Distribution of masses
  std::uniform_real_distribution<> mass_dist(min_mass, max_mass);
  // Distribution of radii
  std::uniform_real_distribution<> radius_dist(min_radius, max_radius);

  for (size_t i = 0; i < particle_count; ++i) {
    // Create particle and add it to vector
    particles_.push_back(Particle(name, vec2(width_dist(gen), height_dist(gen)),
                                  vec2(velocity_dist(gen), velocity_dist(gen)),
                                  mass_dist(gen), radius_dist(gen), color));
  }
}

void ParticleContainer::InitializeParticle(const Particle& particle) {
  // Add singular particle
  particles_.push_back(particle);
}

void ParticleContainer::Increment() {
  IncrementParticleCollisions();

  IncrementWallCollisions();
}

vector<Particle>& ParticleContainer::GetParticles() {
  return particles_;
}

void ParticleContainer::SetParticles(const vector<Particle>& particles) {
  particles_ = particles;
}

vector<string> ParticleContainer::GetParticleNames() const {
  return particle_names_;
}

float ParticleContainer::GetTimeStep() const {
  return time_step_;
}

void ParticleContainer::SetTimeStep(float time_step) {
  time_step_ = time_step;
}

void ParticleContainer::IncrementParticleCollisions() {
  // First sort particles for more efficient searching
  std::sort(particles_.begin(), particles_.end(),
       [](const Particle& lhs, const Particle& rhs) {
         if (lhs.GetPosition().x != rhs.GetPosition().x) { return lhs.GetPosition().x < rhs.GetPosition().x; }
         else { return lhs.GetPosition().y < rhs.GetPosition().y; }
       });
    
  for (size_t base = 0; base < particles_.size(); ++base) {
    // Search only the first few particle pairings nearby, with no overlaps
    size_t cutoff = base + kNearbyLimit;
    // Prevent excess index
    if (cutoff > particles_.size() - 1) {
      cutoff = particles_.size();
    }
    // Check for collision between each pairing
    for (size_t neighbor = base + 1; neighbor < cutoff; ++neighbor) {
      // If collision occurs, make particle bluer (feature)
      if (ExecuteParticleCollision(base, neighbor)) {
        particles_.at(base).SetColor(particles_.at(base).GetColor() *
                                     ColorT<float>(0.99, 0.99, 1));
      }
    }
  }
}

void ParticleContainer::IncrementWallCollisions() {
  for (size_t index = 0; index < particles_.size(); ++index) {
    if (ExecuteWallCollision(index)) {
      // If collision with wall occurs, then make particle redder (feature)
      particles_.at(index).SetColor(particles_.at(index).GetColor() *
                                    ColorT<float>(1, 0.95, 0.95));
    }
    // Increment particle position based on velocity
    particles_.at(index).SetPosition(particles_.at(index).GetPosition() +
                                     time_step_ * particles_.at(index).GetVelocity());
  }
}

bool ParticleContainer::ExecuteParticleCollision(size_t base, size_t neighbor) {
  auto p1 = particles_.at(base);
  auto p2 = particles_.at(neighbor);
  float distance_cutoff = p1.GetRadius() + p2.GetRadius();
  vec2 x1 = p1.GetPosition();
  vec2 x2 = p2.GetPosition();
  vec2 v1 = p1.GetVelocity();
  vec2 v2 = p2.GetVelocity();
  float m1 = p1.GetMass();
  float m2 = p2.GetMass();
  float distance_between = glm::distance(x1, x2);
  float displacement_threshold = dot(v1 - v2, x1 - x2);

  // Use directional check to ensure that particles won't get stuck or frozen
  if (distance_between <= distance_cutoff && displacement_threshold < 0) {
    // Calculate new velocity for p1
    float mass_term_1 = 2 * m2 / (m1 + m2);
    vec2 interaction_term_1 =
        dot(v1 - v2, x1 - x2) / length(x1 - x2) / length(x1 - x2) * (x1 - x2);
    vec2 new_velocity_1 = v1 - mass_term_1 * interaction_term_1;
    particles_.at(base).SetVelocity(new_velocity_1);

    // Calculate new velocity for p2
    float mass_term_2 = 2 * m1 / (m1 + m2);
    vec2 interaction_term_2 =
        dot(v2 - v1, x2 - x1) / length(x2 - x1) / length(x2 - x1) * (x2 - x1);
    vec2 new_velocity_2 = v2 - mass_term_2 * interaction_term_2;
    particles_.at(neighbor).SetVelocity(new_velocity_2);
    return true;
  }
  return false;
}

bool ParticleContainer::ExecuteWallCollision(size_t index) {
  auto position = particles_.at(index).GetPosition();
  auto velocity = particles_.at(index).GetVelocity();
  auto radius = particles_.at(index).GetRadius();

  // Check each of the four walls for near or close collision and reverse velocity as necessary
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