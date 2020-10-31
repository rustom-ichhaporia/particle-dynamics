#pragma once

#include <vector>
#include <iostream>
#include <string>
#include "cinder/gl/gl.h"
#include "core/particle.h"


using std::string;
using glm::vec2;
using idealgas::Particle;
using std::vector;
using ci::ColorT;

namespace idealgas {

class Histogram {
 public:
  Histogram(const string& title, size_t bin_count, size_t width, size_t height, const vec2& offset, size_t stroke, const ColorT<float>& stroke_color, const ColorT<float>& bar_color, const ColorT<float>& text_color, const string& font);
  void Draw();
  void DrawFrame();
  void DrawText();
  void DrawBins();
  void Update(float speed);

 private:
  void CalculateNormalizedBins();

  vector<float> speeds_;

  vector<size_t> bin_cutoffs_;

  vector<float> bin_heights_;

  string title_;
  size_t bin_count_;
  size_t width_;
  size_t height_;
  vec2 offset_;
  size_t stroke_;
  ColorT<float> stroke_color_;
  ColorT<float> bar_color_;
  ColorT<float> text_color_;
  string font_;

  vec2 graph_offset_;
  size_t graph_width_;
  size_t bin_width_;
};

}