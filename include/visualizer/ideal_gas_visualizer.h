#include <string>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/histogram.h"
#include "core/particle_container.h"

using ci::Color;
using ci::app::App;
using ci::app::KeyEvent;
using ci::app::MouseEvent;
using idealgas::Histogram;
using idealgas::ParticleContainer;
using std::string;
using std::unordered_map;

namespace idealgas {

class IdealGasVisualizer : public App {
 public:
  /**
   * @brief Constructs a visualizer object and initializes its container.
   *
   */
  IdealGasVisualizer();

  /**
   * @brief Sets up the beginning variables of the visualization for Cinder.
   *
   */
  void setup() override;

  /**
   * @brief Executes an event if the mouse is moved down.
   *
   * @param event the MouseEvent to parse
   */
  void mouseDown(MouseEvent event) override;

  /**
   * @brief Updates all the variables of the visualization before each new draw
   * heartbeat.
   *
   */
  void update() override;

  /**
   * @brief The heartbeat "draw" function of the visualization that is called
   * every time the screen is refreshed.
   *
   */
  void draw() override;

  /**
   * @brief Executes a change in particle size or speed when the arrow keys are
   * pressed.
   *
   * @param event the KeyEvent to parse
   */
  void keyDown(KeyEvent event) override;

 private:
  /**
   * @brief Draws the frame of the container that bounds the particles.
   *
   */
  void DrawContainer();

  /**
   * @brief Draws the string messages to be displayed on the screen.
   *
   */
  void DrawMessage();

  /**
   * @brief Draws each of the particles in the simulation.
   *
   */
  void DrawParticles();

  /**
   * @brief Updates all of the histograms with the relevant particle speeds
   * before refreshing them.
   *
   */
  void UpdateHistograms();

  /**
   * @brief Draws the histograms of particle speeds on the screen.
   *
   */
  void DrawHistograms();

  // The pixel wixth of the window
  size_t window_width_;
  // The pixel height of the window
  size_t window_height_;
  // The pixel margin around the container and other objects
  size_t margin_;
  // The pixel width of the stroke used to draw boundaries
  size_t stroke_;
  // The pixel width of the particle container
  size_t container_width_;
  // The pixel height of the particle container
  size_t container_height_;
  // The color of the visualization background
  Color background_color_;
  // The color of the stroke used to draw the boundaries
  Color stroke_color_;
  // The color of the text printed on the visualization
  Color text_color_;
  // The string font name of the text printed in the visualization
  string font_family_;
  // The map of histograms plotting each string particle type's velocities
  unordered_map<string, Histogram> histograms_;
  // The number of bins in each histogram
  size_t histogram_bin_count_;
  // The particle container object contianing all of the particles and their
  // data
  ParticleContainer container_;
};

}  // namespace idealgas