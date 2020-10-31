#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "cinder/gl/gl.h"
#include "core/particle.h"

using ci::ColorT;
using ci::Font;
using glm::vec2;
using idealgas::Particle;
using std::string;
using std::vector;

namespace idealgas {

class Histogram {
 public:
  /**
   * @brief Constructs a new Histogram object.
   *
   * @param title the title of the plot
   * @param bin_count the number of bins in the histogram
   * @param width the width of the plot
   * @param height the height of the plot
   * @param offset the vec2 offset from the origin of the upper left of the
   * histogram
   * @param stroke the width of the stroke of the histogram's lines
   * @param stroke_color the color of the histogram's lines
   * @param bar_color the color of the histogram's bars
   * @param text_color the color of the histogram's text
   * @param font the font of the histogram's text
   */
  Histogram(const string& title, size_t bin_count, size_t width, size_t height,
            const vec2& offset, size_t stroke,
            const ColorT<float>& stroke_color, const ColorT<float>& bar_color,
            const ColorT<float>& text_color, const string& font);

  /**
   * @brief Calls the appropriate drawing functions in order to display the
   * histogram in the app.
   *
   */
  void Draw();

  /**
   * @brief Updates the histogram by adding a value to the distribution.
   *
   * @param value the value
   */
  void Update(float value);

 private:
  /**
   * @brief Draws the frame of the histogram.
   *
   */
  void DrawFrame();

  /**
   * @brief Draws the titles and labels of the histogram.
   *
   */
  void DrawText();

  /**
   * @brief Draws the bins/bars of the histogram.
   *
   */
  void DrawBins();

  /**
   * @brief Calculates the normalized frequencies of different speeds,
   * representing by bin heights.
   *
   */
  void CalculateFrequencies();

  /**
   * @brief Normalizes the size of each bin height for graphing. 
   * 
   */
  void NormalizeBins();

  // The values from which to construct the histogram
  vector<float> values_;
  // The evenly spaced upper cutoffs for each bin, ranging from minimum to
  // maximum value
  vector<size_t> bin_cutoffs_;
  // The relative bin heights to be graphed
  vector<float> bin_heights_;

  // The string title of the histogram
  string title_;
  // The number of bins to use to create the histogram
  size_t bin_count_;
  // The pixel width of the graph
  size_t width_;
  // The pixel height of the graph
  size_t height_;
  // The vec2 offset of the top left of the graph, relative to the top left of
  // the window
  vec2 offset_;
  // The thickness of the stroke with which to draw the frame lines
  size_t stroke_;
  // The color of the stroke
  ColorT<float> stroke_color_;
  // The color of the histogram bars
  ColorT<float> bar_color_;
  // The color of the label and title text
  ColorT<float> text_color_;
  // The font family of the text printed on the graph
  string font_family_;

  // The offset of the graph (separate from the offset of the frame)
  vec2 graph_offset_;
  // The pixel width of the graph (marginally smaller than the width of the
  // histogram)
  size_t graph_width_;
  // The pixel width of each bin in the graph
  size_t bin_width_;
  // Label font 
  Font label_font_;
  // Title font 
  Font title_font_;
};

}  // namespace idealgas