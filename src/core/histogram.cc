#include "core/histogram.h"

#include <numeric>
#include <vector>

#include "cinder/gl/gl.h"
#include "core/particle.h"

using ci::ColorT;
using ci::Font;
using ci::Rectf;
using ci::gl::color;
using ci::gl::drawSolidRoundedRect;
using ci::gl::drawString;
using ci::gl::drawStringCentered;
using ci::gl::drawStringRight;
using ci::gl::drawStrokedRect;
using glm::vec2;
using idealgas::Histogram;
using idealgas::Particle;
using std::accumulate;
using std::begin;
using std::end;
using std::max_element;
using std::min_element;
using std::sort;
using std::to_string;
using std::vector;

namespace idealgas {

Histogram::Histogram(const string& title, size_t bin_count, size_t width,
                     size_t height, const vec2& offset, size_t stroke,
                     const ColorT<float>& stroke_color,
                     const ColorT<float>& bar_color,
                     const ColorT<float>& text_color, const string& font)
    // Uses an initializer list to set all the private variables
    : title_(title),
      bin_count_(bin_count),
      width_(width),
      height_(height),
      offset_(offset),
      stroke_(stroke),
      stroke_color_(stroke_color),
      bar_color_(bar_color),
      text_color_(text_color),
      font_family_(font) {

  graph_offset_ = offset_ + vec2(stroke_ / 2, 0) + vec2(width_ / 10, 0);
  graph_width_ = width_ * 8.5 / 10;
  bin_width_ = graph_width_ / bin_count_;
  title_font_ = Font(font_family_, height_ / 20);
  label_font_ = Font(font_family_, height_ / 25);
}

void Histogram::Draw() {
  // Calls each heartbeat function in order to draw the graph
  CalculateFrequencies();

  NormalizeBins();

  DrawFrame();

  DrawBins();

  DrawText();
}

void Histogram::DrawFrame() {
  vec2 top_left = offset_;

  vec2 bottom_right = offset_ + vec2(width_, height_);

  Rectf bounding_box(top_left, bottom_right);

  color(stroke_color_);

  // Draw the box frame of each histogram
  drawStrokedRect(bounding_box, stroke_);
}

void Histogram::DrawText() {
  // The "magic numbers" in the following function are necessary, as setting
  // private variables for all of them would be poor design.

  // Draws the title on the bottom middle
  drawStringCentered(title_, offset_ + vec2(width_ / 2, height_ * 9.3 / 10),
                     text_color_, title_font_);

  // Sorts bins by heights to label axes
  sort(bin_heights_.begin(), bin_heights_.end());


  // Draws the y axis labels
  drawString(to_string((int)bin_heights_.front()),
             graph_offset_ + vec2(-(int)stroke_ * 2, height_ * 0.8),
             text_color_, label_font_);
  drawString(to_string((int)bin_heights_.back()),
             graph_offset_ + vec2(-(int)stroke_ * 2, height_ * 0.05),
             text_color_, label_font_);
  drawString(
      to_string(((int)bin_heights_.front() + (int)bin_heights_.back()) / 2),
      graph_offset_ + vec2(-(int)stroke_ * 2, height_ * 0.45), text_color_,
      label_font_);

  // Draws the x axis labels
  drawString(to_string(bin_cutoffs_.front()),
             graph_offset_ + vec2(stroke_, height_ * 0.85), text_color_,
             label_font_);

  drawString(to_string((bin_cutoffs_.front() + bin_cutoffs_.back()) / 2),
             graph_offset_ + vec2(graph_width_ / 2, height_ * 8.5 / 10),
             text_color_, label_font_);

  drawStringRight(to_string(bin_cutoffs_.back()),
                  graph_offset_ + vec2(graph_width_, height_ * 0.85),
                  text_color_, label_font_);

  // Draw the axis label
  drawStringCentered("x = Speed, y = Frequency",
                     graph_offset_ + vec2(graph_width_ / 2, height_ * 0.025),
                     text_color_, label_font_);
}

void Histogram::DrawBins() {
  // The "magic numbers" in the following function are necessary, as setting
  // private variables for all of them would be poor design.
  size_t max_bar_height = height_ * 8 / 10;
  color(bar_color_);

  for (size_t bin = 0; bin < bin_heights_.size(); ++bin) {
    size_t proportional_height = bin_heights_.at(bin) * max_bar_height;

    vec2 top_left;

    // If statement ensures that low-level bar does not go "negative"
    if (max_bar_height + stroke_ <
        max_bar_height - proportional_height + stroke_ * 3) {
      top_left = graph_offset_ +
                 vec2(bin * bin_width_, max_bar_height - proportional_height);
    } else {
      top_left = graph_offset_ +
                 vec2(bin * bin_width_,
                      max_bar_height - proportional_height + stroke_ * 3);
    }

    vec2 bottom_right =
        graph_offset_ + vec2((bin + 1) * bin_width_, max_bar_height + stroke_);

    Rectf bounding_box(top_left, bottom_right);
    drawSolidRoundedRect(bounding_box, 1);
  }
}

void Histogram::Update(float value) {
  values_.push_back(value);
}

void Histogram::CalculateFrequencies() {
  // Clear bin vectors before each calculation
  bin_heights_.clear();
  bin_cutoffs_.clear();

  // Order all values for sequential binning
  sort(values_.begin(), values_.end());

  float min_speed = values_.front();
  float max_speed = values_.back();

  float bin_increment = (max_speed - min_speed) / bin_count_;
  float bin_cutoff = bin_increment;

  float sum = 0;

  for (size_t value = 0; value < values_.size(); ++value) {
    // If value is within bin, increment
    if (values_.at(value) < min_speed + bin_cutoff) {
      sum += 1;
    } 
    // Otherwise, shift to the next bin and redo value
    else {
      bin_cutoffs_.push_back(bin_cutoff);
      bin_heights_.push_back(sum);
      sum = 0;
      bin_cutoff += bin_increment;
      --value;
    }
  }
}

void Histogram::NormalizeBins() {
  float max_bin = *max_element(bin_heights_.begin(), bin_heights_.end());

  // Divide each by max element
  
  for (size_t index = 0; index < bin_heights_.size(); ++index) {
    bin_heights_.at(index) /= max_bin;
  }
}

}  // namespace idealgas