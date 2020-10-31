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
using ci::gl::drawString;
using ci::gl::drawStringRight;
using std::begin;
using std::accumulate;
using std::sort;
using std::end;
using std::max_element;
using std::min_element;
using ci::gl::drawSolidRoundedRect;
using std::to_string;

namespace idealgas {

Histogram::Histogram(const string& title, size_t bin_count, size_t width, size_t height, const vec2& offset, size_t stroke, const ColorT<float>& stroke_color, const ColorT<float>& bar_color, const ColorT<float>& text_color, const string& font) : title_(title), bin_count_(bin_count), width_(width), height_(height), offset_(offset), stroke_(stroke), stroke_color_(stroke_color), bar_color_(bar_color), text_color_(text_color), font_(font) {
}

void Histogram::Draw() {
  CalculateNormalizedBins();

  DrawFrame();


  DrawBins();
    DrawText();

}

void Histogram::DrawFrame() {
  vec2 top_left = offset_;

  vec2 bottom_right = offset_ + vec2(width_, height_);

  Rectf bounding_box(top_left, bottom_right);

  color(stroke_color_);
  drawStrokedRect(bounding_box, stroke_);
}

void Histogram::DrawText() {
  graph_offset_ = offset_ + vec2(stroke_ / 2, 0) + vec2(width_ / 10, 0);
  graph_width_ = width_ * 8.5 / 10; 
  bin_width_ = graph_width_ / bin_count_;

  drawStringCentered(title_, offset_ + 
                     vec2(width_ / 2, height_ * 9.3 / 10),
                     text_color_, Font(font_, height_ / 20));

  // for (size_t cutoff = 0; cutoff < bin_cutoffs_.size(); ++cutoff) {
  //   drawString(to_string(bin_cutoffs_.at(cutoff)), graph_offset_ + vec2(bin_width_ * cutoff, height_ * 8.5 / 10), text_color_, Font(font_, height_ / 20));
  // }
  sort(bin_heights_.begin(), bin_heights_.end());

  drawString(to_string((int)bin_heights_.front()), graph_offset_ + vec2(-(int)stroke_ * 2, height_ * 0.8), text_color_, Font(font_, height_ / 25));
  
  drawString(to_string((int)bin_heights_.back()), graph_offset_ + vec2(-(int)stroke_ * 2, height_ * 0.05), text_color_, Font(font_, height_ / 25));
  drawString(to_string(((int)bin_heights_.front() + (int)bin_heights_.back()) / 2), graph_offset_ + vec2(-(int)stroke_ * 2, height_ * 0.45), text_color_, Font(font_, height_ / 25));


  drawString(to_string(bin_cutoffs_.front()), graph_offset_ + vec2(stroke_, height_ * 0.85), text_color_, Font(font_, height_ / 25));

  drawString(to_string((bin_cutoffs_.front() + bin_cutoffs_.back()) / 2), graph_offset_ + vec2(graph_width_ / 2,  height_ * 8.5 / 10), text_color_, Font(font_, height_ / 25));

  drawStringRight(to_string(bin_cutoffs_.back()), graph_offset_ + vec2(graph_width_, height_ * 0.85), text_color_, Font(font_, height_ / 25));

  drawStringCentered("x = Speed, y = Frequency", graph_offset_ + vec2(graph_width_ / 2, height_ * 0.025), text_color_, Font(font_, height_ / 25));
}

void Histogram::DrawBins() {
  size_t max_bar_height = height_ * 8 / 10;
  color(bar_color_);

  for (size_t bin = 0; bin < bin_heights_.size(); ++bin) {
    size_t proportional_height = bin_heights_.at(bin) * max_bar_height;
    
    vec2 top_left;
    
    if (max_bar_height + stroke_ < max_bar_height - proportional_height + stroke_ * 3) {
      top_left = graph_offset_ + vec2(bin * bin_width_, max_bar_height - proportional_height);
    } else {
      top_left = graph_offset_ + vec2(bin * bin_width_, max_bar_height - proportional_height + stroke_ * 3);
    }

    vec2 bottom_right = graph_offset_ + vec2((bin + 1) * bin_width_, max_bar_height + stroke_);

    Rectf bounding_box(top_left, bottom_right);
    drawSolidRoundedRect(bounding_box, 1);
  }
}

void Histogram::Update(float speed) {
  speeds_.push_back(speed);
}

void Histogram::CalculateNormalizedBins() {
  bin_heights_.clear();
  bin_cutoffs_.clear();

  sort(speeds_.begin(), speeds_.end());

  float min_speed = speeds_.front();//*min_element(bin_heights.begin(), bin_heights.end());
  float max_speed = speeds_.back();

  float bin_increment = (max_speed - min_speed) / bin_count_;
  float bin_cutoff = bin_increment;

  float sum = 0;
  for (size_t begin = 0; begin < speeds_.size(); ++begin) {
    if (speeds_.at(begin) < min_speed + bin_cutoff) {
      sum += 1;
    } else {
      bin_cutoffs_.push_back(bin_cutoff);
      bin_heights_.push_back(sum);
      sum = 0;
      bin_cutoff += bin_increment;
      --begin;
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

  float max_bin = *max_element(bin_heights_.begin(), bin_heights_.end());

  for (size_t index = 0; index < bin_heights_.size(); ++index) {
    bin_heights_.at(index) /= max_bin;
  }

}

}