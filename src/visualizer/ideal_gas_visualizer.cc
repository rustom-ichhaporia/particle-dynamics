#include "visualizer/ideal_gas_visualizer.h"

#include "core/particle_container.h"

using ci::Color8u;
using ci::Color;
using ci::Rectf;
using ci::app::setWindowPos;
using ci::app::setWindowSize;
using ci::gl::clear;
using ci::gl::color;
using ci::gl::drawStringCentered;
using ci::gl::drawStrokedRect;
using std::stoull;
using ci::ColorT;
using std::uint32_t;
using glm::vec2;
using idealgas::ParticleContainer;
using std::unordered_map;
using ci::Font;
using std::any_cast;
using std::stoi;

namespace idealgas {

IdealGasVisualizer::IdealGasVisualizer() {
  unordered_map<string, string> size_config = container_.ConfigureSizes();
  window_width_ = stoi(size_config["window width"]);//any_cast<size_t>(size_config["window width"]);
  window_height_ =  stoi(size_config["window height"]);
  margin_ =  stoi(size_config["margin"]);
  stroke_ =  stoi(size_config["stroke"]);
  box_width_ =  window_width_ - 2 * margin_;
  box_height_ = window_height_ - 2 * margin_;

  // auto color_config = container_.ConfigureColors();
  background_color_ = ColorT<float>().hex(uint32_t(stoull(size_config["background color"], 0, 16)));//static_cast<uint32_t>(stoul(size_config["background color"])));
  stroke_color_ = ColorT<float>().hex(uint32_t(stoull(size_config["stroke color"], 0, 16)));
  text_color_ = ColorT<float>().hex(uint32_t(stoull(size_config["text color"], 0, 16)));
}

void IdealGasVisualizer::setup() {
  // window_width_ = 1500;
  setWindowSize(window_width_, window_height_);
  setWindowPos(0, 0);
}

void IdealGasVisualizer::mouseDown(MouseEvent event) {
}

void IdealGasVisualizer::update() {
}

void IdealGasVisualizer::draw() {
  DrawContainer();

  drawStringCentered("Press Delete to restart the simulation. ",
                     vec2(window_width_ / 2, window_height_ - margin_ / 2),
                     text_color_,
                     Font("IBM Plex Mono", margin_ / 4));
}

void IdealGasVisualizer::DrawContainer() {
  clear(background_color_);

  vec2 pixel_top_left = vec2(margin_, margin_);

  vec2 pixel_bottom_right = vec2(window_width_ - margin_, window_height_ - margin_);
  
  Rectf pixel_bounding_box(pixel_top_left, pixel_bottom_right);

  color(stroke_color_);
  drawStrokedRect(pixel_bounding_box, stroke_);
}

}  // namespace idealgas