#include "visualizer/ideal_gas_visualizer.h"

#include "core/histogram.h"
#include "core/particle.h"
#include "core/particle_container.h"

using ci::Color;
using ci::ColorT;
using ci::Font;
using ci::Rectf;
using ci::app::setFullScreen;
using ci::app::setWindowPos;
using ci::app::setWindowSize;
using ci::gl::clear;
using ci::gl::color;
using ci::gl::drawSolidCircle;
using ci::gl::drawStringCentered;
using ci::gl::drawStrokedRect;
using glm::vec2;
using idealgas::Histogram;
using idealgas::Particle;
using idealgas::ParticleContainer;
using std::stoi;
using std::stoull;
using std::uint32_t;
using std::unordered_map;

namespace idealgas {

IdealGasVisualizer::IdealGasVisualizer() {
  // Gets map of configuration variables read from JSON
  unordered_map<string, string> config = container_.Configure(config_path_);
  window_width_ = stoi(config["window width"]);
  window_height_ = stoi(config["window height"]);
  margin_ = stoi(config["margin"]);
  stroke_ = stoi(config["stroke"]);

  // Set box width and height to appropriate margins (not a magic number)
  container_width_ = stoi(config["container width"]);
  container_height_ = stoi(config["container height"]);
  histogram_bin_count_ = stoi(config["histogram bin count"]);

  // Set draw colors
  background_color_ =
      ColorT<float>().hex(uint32_t(stoull(config["background color"], 0, 16)));
  stroke_color_ =
      ColorT<float>().hex(uint32_t(stoull(config["stroke color"], 0, 16)));
  text_color_ =
      ColorT<float>().hex(uint32_t(stoull(config["text color"], 0, 16)));
  font_family_ = config["font"];
}

void IdealGasVisualizer::setup() {
  setWindowSize(window_width_, window_height_);
  setWindowPos(0, 0);
  setFullScreen(true);

  size_t num_particle_types = container_.GetParticleNames().size();
  for (size_t index = 0; index < num_particle_types; ++index) {
    string name = container_.GetParticleNames().at(index);

    // Create histogram object for each type of particle present
    Histogram hist(
        name, histogram_bin_count_, margin_ * 3,
        container_height_ / num_particle_types,
        vec2(container_width_ + 2 * margin_,
             margin_ + index * (container_height_ / num_particle_types)),
        stroke_, stroke_color_, text_color_, text_color_, font_family_);

    // Insert into list of histograms
    histograms_.insert({{name, hist}});
  }
}

void IdealGasVisualizer::mouseDown(MouseEvent event) {
}

void IdealGasVisualizer::update() {
  container_.Increment();
  UpdateHistograms();
}

void IdealGasVisualizer::draw() {
  DrawContainer();

  DrawMessage();

  DrawParticles();

  DrawHistograms();
}

void IdealGasVisualizer::keyDown(KeyEvent event) {
  // Increase or decrease velocity for left and right
  // Increase or decrease size for up and down
  switch (event.getCode()) {
    case KeyEvent::KEY_DOWN: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetRadius(particle.GetRadius() * 0.9);
      }
      break;
    }
    case KeyEvent::KEY_UP: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetRadius(particle.GetRadius() * 1.1);
      }
      break;
    }
    case KeyEvent::KEY_LEFT: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetVelocity(particle.GetVelocity() * vec2(0.90, 0.90));
      }

      break;
    }
    case KeyEvent::KEY_RIGHT: {
      for (auto& particle : container_.GetParticles()) {
        particle.SetVelocity(particle.GetVelocity() * vec2(1.1, 1.1));
      }
      break;
    }
  }
}

void IdealGasVisualizer::DrawContainer() {
  // Draw background color
  clear(background_color_);

  vec2 pixel_top_left = vec2(margin_, margin_);

  vec2 pixel_bottom_right =
      pixel_top_left + vec2(container_width_, container_height_);

  Rectf pixel_bounding_box(pixel_top_left, pixel_bottom_right);

  // Draw container frame
  color(stroke_color_);
  drawStrokedRect(pixel_bounding_box, stroke_);
}

void IdealGasVisualizer::DrawMessage() {
  drawStringCentered(
      "Press the arrow keys to change the particle sizes and speeds.",
      vec2(window_width_ / 2, window_height_ - margin_ / 2), text_color_,
      Font(font_family_, margin_ / 4));
}

void IdealGasVisualizer::DrawParticles() {
  // Draw an appropriately colored circle for each particle
  vector<Particle> particles = container_.GetParticles();
  for (auto particle : particles) {
    color(particle.GetColor());
    drawSolidCircle(vec2(margin_, margin_) + particle.GetPosition(),
                    particle.GetRadius());
  }
}

void IdealGasVisualizer::UpdateHistograms() {
  // Updates the appropriate histogram with the velocity from its particle of
  // matching type
  for (auto& particle : container_.GetParticles()) {
    string name = particle.GetName();
    float speed = length(particle.GetVelocity());
    histograms_.at(name).Update(speed);
  }
}

void IdealGasVisualizer::DrawHistograms() {
  // Draws the histograms to the screen
  for (auto& hist : histograms_) {
    hist.second.Draw();
  }
}

}  // namespace idealgas