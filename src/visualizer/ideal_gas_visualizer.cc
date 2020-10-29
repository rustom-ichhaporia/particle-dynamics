#include "visualizer/ideal_gas_visualizer.h"
#include "core/particle.h"
#include "core/particle_container.h"

using ci::Color;
using ci::ColorT;
using ci::Font;
using ci::Rectf;
using ci::app::setWindowPos;
using ci::app::setWindowSize;
using ci::gl::clear;
using ci::gl::color;
using ci::gl::drawStringCentered;
using ci::gl::drawStrokedRect;
using glm::vec2;
using idealgas::Particle;
using idealgas::ParticleContainer;
using std::any_cast;
using std::stoi;
using std::stoull;
using std::uint32_t;
using std::unordered_map;
using ci::gl::drawSolidCircle;

namespace idealgas {

IdealGasVisualizer::IdealGasVisualizer() {
  // Gets map of configuration variables read from JSON
  unordered_map<string, string> size_config = container_.ConfigureSizes();
  window_width_ = stoi(size_config["window width"]);  
  window_height_ = stoi(size_config["window height"]);
  margin_ = stoi(size_config["margin"]);
  stroke_ = stoi(size_config["stroke"]);
  
  // Set box width and height to appropriate margins (not a magic number)
  box_width_ = window_width_ - 2 * margin_;
  box_height_ = window_height_ - 2 * margin_;

  // Set draw colors
  background_color_ = ColorT<float>().hex(uint32_t(stoull(size_config["background color"], 0, 16))); 
  stroke_color_ = ColorT<float>().hex(uint32_t(stoull(size_config["stroke color"], 0, 16)));
  text_color_ = ColorT<float>().hex(uint32_t(stoull(size_config["text color"], 0, 16)));
  font_ = size_config["font"];
}

void IdealGasVisualizer::setup() {
  // window_width_ = 1500;
  setWindowSize(window_width_, window_height_);
  setWindowPos(0, 0);
}

void IdealGasVisualizer::mouseDown(MouseEvent event) {
}

void IdealGasVisualizer::update() {
  container_.Increment();
}

void IdealGasVisualizer::draw() {
  DrawContainer();

  DrawMessage();

  DrawParticles();
}

void IdealGasVisualizer::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_DOWN: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetVelocity(particle.GetVelocity() * vec2(0.90, 0.90));
      }
      break;
    }
    case KeyEvent::KEY_UP: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetVelocity(particle.GetVelocity() * vec2(1.1, 1.1));
      }
      break;
    }
    case KeyEvent::KEY_LEFT: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetRadius(particle.GetRadius() * 0.9);
      }
      break;
    }
    case KeyEvent::KEY_RIGHT: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetRadius(particle.GetRadius() * 1.1);
      }
    }
  }
}

void IdealGasVisualizer::DrawContainer() {
  clear(background_color_);

  vec2 pixel_top_left = vec2(margin_, margin_);

  vec2 pixel_bottom_right =
      vec2(window_width_ - margin_, window_height_ - margin_);

  Rectf pixel_bounding_box(pixel_top_left, pixel_bottom_right);

  color(stroke_color_);
  drawStrokedRect(pixel_bounding_box, stroke_);
}

void IdealGasVisualizer::DrawMessage() {
  drawStringCentered("Press the up arrow to speed up the particles. Press the down arrow to slow them down.",
                     vec2(window_width_ / 2, window_height_ - margin_ / 2),
                     text_color_, Font(font_, margin_ / 4));
}

void IdealGasVisualizer::DrawParticles() {
  vector<Particle> particles = container_.GetParticles();
  for (auto particle : particles) { 
    color(particle.GetColor());
    drawSolidCircle(vec2(margin_, margin_) + particle.GetPosition(), particle.GetRadius());
  }
}

}  // namespace idealgas