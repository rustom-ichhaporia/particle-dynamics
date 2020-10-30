#include "core/histogram.h"
#include <vector>
#include "core/particle.h"
#include "cinder/gl/gl.h"
#include <numeric>

using std::vector;
using idealgas::Particle;
using glm::vec2;
using ci::Rectf;
using ci::gl::drawStrokedRect;
using ci::ColorT;
using ci::gl::color;
using ci::Font;
using idealgas::Histogram;
using ci::gl::drawStringCentered;
using std::begin;
using std::accumulate;
using std::sort;
using std::end;
using std::max_element;
using std::min_element;
using ci::gl::drawSolidRoundedRect;

namespace idealgas {

Histogram::Histogram(const string& title, size_t bin_count, size_t width, size_t height, const vec2& offset, size_t stroke, const ColorT<float>& stroke_color, const ColorT<float>& bar_color, const ColorT<float>& text_color, const string& font) : title_(title), bin_count_(bin_count), width_(width), height_(height), offset_(offset), stroke_(stroke), stroke_color_(stroke_color), bar_color_(bar_color), text_color_(text_color), font_(font) {
}

void Histogram::Draw() {
  DrawFrame();

  DrawText();

  DrawBins(CalculateNormalizedBins());
}

void Histogram::DrawFrame() {
  vec2 top_left = offset_;

  vec2 bottom_right = offset_ + vec2(width_, height_);

  Rectf bounding_box(top_left, bottom_right);

  color(stroke_color_);
  drawStrokedRect(bounding_box, stroke_);
}

void Histogram::DrawText() {
  drawStringCentered(title_, offset_ + 
                     vec2(width_ / 2, height_ * 9 / 10),
                     text_color_, Font(font_, height_ / 20));
}

void Histogram::DrawBins(vector<float> bin_heights) {
  size_t max_bar_height = height_ * 8 / 10;
  color(bar_color_);
  size_t bin_width = width_ / bin_count_;

  for (size_t bin = 0; bin < bin_count_; ++bin) {
    // size_t average_speed = accumulate(speeds_.begin(), speeds_.end(), 0);
    size_t proportional_height = bin_heights.at(bin) * max_bar_height;
    // size_t bar_height = average_speed / max_speed / max_bar_height;
    vec2 top_left = offset_ + vec2(bin * bin_width, max_bar_height - proportional_height);
    vec2 bottom_right = offset_ + vec2((bin + 1) * bin_width, max_bar_height);
    Rectf bounding_box(top_left, bottom_right);
    drawSolidRoundedRect(bounding_box, 1);
  }
}

void Histogram::Update(float speed) {
  speeds_.push_back(speed);
}

vector<float> Histogram::CalculateNormalizedBins() {
  sort(speeds_.begin(), speeds_.end());

  float smallest_speed = speeds_.front();//*min_element(bin_heights.begin(), bin_heights.end());
  float highest_speed = speeds_.back();



  // float bin_width = speeds_.size() / bin_count_;

  vector<float> bin_heights;

  size_t bin_increment = (highest_speed - smallest_speed) / bin_count_;
  size_t bin_cutoff = bin_increment;

  float sum = 0;
  for (size_t begin = 0; begin < speeds_.size(); ++begin) {
    if (speeds_.at(begin) < bin_cutoff) {
      sum += 1;
    } else {
      bin_heights.push_back(sum);
      sum = 0;
      bin_cutoff += bin_increment;
    }
  }
  // bin_heights.push_back(sum);



  // for (size_t begin = 0; begin < speeds_.size(); begin += bin_width) {
  //   float sum = 0;
  //   float cutoff = begin + bin_width;
  //   if (cutoff > speeds_.size()) {
  //     cutoff = speeds_.size();
  //   }
  //   for (size_t index = begin; index < cutoff; ++index) {
  //     sum += speeds_.at(index);
  //   }
  //   bin_heights.push_back(sum / bin_width);
  // }

  float max_bin = *max_element(bin_heights.begin(), bin_heights.end());

  for (size_t index = 0; index < bin_heights.size(); ++index) {
    bin_heights.at(index) /= max_bin;
  }

  return bin_heights;
}

}