#include <catch2/catch.hpp>
#include <iostream>
#include "core/histogram.h"
#include "core/particle_container.h"
#include <string>

using std::string;
using idealgas::ParticleContainer;
using idealgas::Histogram;

TEST_CASE("Bin heights") {
  SECTION("Empty histogram") {
    Histogram hist("", 10);
    hist.CalculateFrequencies();
    auto heights = hist.GetBinHeights();
    REQUIRE(heights.size() == 0);
  }

  SECTION("Negative values") {
    Histogram hist("", 2);
    for (int i = -10; i < 0; ++i) {
      hist.Update(i);
    }

    hist.CalculateFrequencies();
    auto heights = hist.GetBinHeights();

    REQUIRE(heights.size() == 2);
    REQUIRE(heights[0] == 5);
    REQUIRE(heights[1] == 4);

    hist.NormalizeBins();
    heights = hist.GetBinHeights();

    REQUIRE(heights.size() == 2);
    REQUIRE(heights[0] == Approx(1).epsilon(0.01));
    REQUIRE(heights[1] == Approx(0.8).epsilon(0.01));
  }

  SECTION("Uniform values") {
    Histogram hist("", 2);
    for (int i = 0; i < 11; ++i) {
      hist.Update(3);
    }

    hist.CalculateFrequencies();
    auto heights = hist.GetBinHeights();

    hist.NormalizeBins();
    heights = hist.GetBinHeights();

    REQUIRE(heights.size() == 2);
    REQUIRE(heights[0] == 1);
    REQUIRE(heights[1] == 1);
  }
}