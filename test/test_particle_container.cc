#include <catch2/catch.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "cinder/gl/gl.h"
#include "core/histogram.h"
#include "core/particle.h"
#include "core/particle_container.h"

using ci::ColorT;
using idealgas::Histogram;
using idealgas::ParticleContainer;
using std::string;
using std::vector;

// I have not included a separate test suite for the Particle class because it
// consists exclusively of getters and setters, all of which are used in the
// code below.

TEST_CASE("Setup", "[configure][initialize]") {
  SECTION("JSON configuration particle initialization") {
    SECTION("Empty file") {
      // Catch2 is not able to recognize the json::parse_error error type that
      // is thrown by the json library, so this test is commented. However,
      // uncommenting it will demonstrate that the program properly throws an
      // error for an improperly formatted file.

      string test_config_path =
          "../../../../../../config/test/config_empty.json";
      ParticleContainer container;
      REQUIRE_THROWS_AS(container.Configure(test_config_path),
                        json::parse_error);
    }

    SECTION("Incomplete") {
      string test_config_path =
          "../../../../../../config/test/config_invalid.json";
      ParticleContainer container;
      REQUIRE_THROWS_AS(container.Configure(test_config_path),
                        json::type_error);
    }

    SECTION("Valid file") {
      string test_config_path =
          "../../../../../../config/test/config_test.json";
      ParticleContainer container;
      container.Configure(test_config_path);
      REQUIRE(container.GetParticleNames() ==
              vector<string>{"Solitary Particle"});
      REQUIRE(container.GetParticles().size() == 1);
    }
  }

  SECTION("Parameterized particle initialization") {
    string test_config_path = "../../../../../../config/test/config_test.json";
    ParticleContainer container;
    container.Configure(test_config_path);
    auto particle = container.GetParticles()[0];
    REQUIRE(particle.GetName() == "Solitary Particle");
    REQUIRE(length(particle.GetVelocity()) <= 2);
    REQUIRE(particle.GetMass() == 1);
    REQUIRE(particle.GetRadius() == 1);
    REQUIRE(particle.GetColor() == ColorT<float>().hex(0xFFFFFF));
  }

  SECTION("Individual particle initialization") {
    ParticleContainer container;
    auto input = Particle("Test", vec2(0, 0), vec2(1, 1), 1, 1,
                          ColorT<float>().hex(0xFFFFFF));
    container.InitializeParticle(input);
    auto particle = container.GetParticles()[0];
    REQUIRE(particle.GetName() == "Test");
    REQUIRE(particle.GetPosition() == vec2(0, 0));
    REQUIRE(particle.GetVelocity() == vec2(1, 1));
    REQUIRE(particle.GetMass() == 1);
    REQUIRE(particle.GetRadius() == 1);
    REQUIRE(particle.GetColor() == ColorT<float>().hex(0xFFFFFF));
  }
}

TEST_CASE("Incrementation", "[initialize][increment]") {
  SECTION("Particle collisions") {
    SECTION("Valid collision") {
      ParticleContainer container;
      Particle p1("Test 1", vec2(19.9, 20), vec2(0.1, 0), 2, 1,
                  ColorT<float>().hex(0xFFFFFF));
      Particle p2("Test 2", vec2(21.5, 21.4), vec2(-0.1, 0), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));
      Particle p3("Test 3", vec2(40, 40), vec2(0, 0), 2, 2,
                  ColorT<float>().hex(0xFFFFFF));
      container.InitializeParticle(p1);
      container.InitializeParticle(p2);
      container.InitializeParticle(p3);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new;
      Particle p2_new;
      if (particles[0].GetName() == "Test 1") {
        p1_new = particles[0];
        p2_new = particles[1];
      } else {
        p1_new = particles[1];
        p2_new = particles[0];
      }

      // This test uses the example collision math that was written in the
      // assignment example.

      REQUIRE(p1_new.GetPosition().x == Approx(20).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(20).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0.1).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(0).epsilon(0.01));
      REQUIRE(p2_new.GetPosition().x == Approx(21.4).epsilon(0.01));
      REQUIRE(p2_new.GetPosition().y == Approx(21.4).epsilon(0.01));
      REQUIRE(p2_new.GetVelocity().x == Approx(-0.1).epsilon(0.01));
      REQUIRE(p2_new.GetVelocity().y == Approx(0).epsilon(0.01));

      container.Increment();
      particles = container.GetParticles();
      if (particles[0].GetName() == "Test 1") {
        p1_new = particles[0];
        p2_new = particles[1];
      } else {
        p1_new = particles[1];
        p2_new = particles[0];
      }

      // Second check for second increment

      REQUIRE(p1_new.GetPosition().x == Approx(20).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(19.9).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0.0333).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(-0.06667).epsilon(0.01));
      REQUIRE(p2_new.GetPosition().x == Approx(21.4).epsilon(0.01));
      REQUIRE(p2_new.GetPosition().y == Approx(21.5).epsilon(0.01));
      REQUIRE(p2_new.GetVelocity().x == Approx(0.0333).epsilon(0.01));
      REQUIRE(p2_new.GetVelocity().y == Approx(0.1333).epsilon(0.01));
      REQUIRE(container.GetParticles()[2].GetPosition().x == Approx(40).epsilon(0.0001));
      REQUIRE(container.GetParticles()[2].GetPosition().y == Approx(40).epsilon(0.0001));
      REQUIRE(container.GetParticles()[2].GetVelocity().y == Approx(0).epsilon(0.0001));
      REQUIRE(container.GetParticles()[2].GetVelocity().y == Approx(0).epsilon(0.0001));    
    }

    SECTION("Prevention of sticky particles / displacement threshold") {
      ParticleContainer container;

      Particle p1("Test 1", vec2(5, 5), vec2(-0.1, 0), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      Particle p2("Test 2", vec2(5.01, 5), vec2(0.1, 0), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      container.InitializeParticle(p1);
      container.InitializeParticle(p2);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new;
      Particle p2_new;

      if (particles[0].GetName() == "Test 1") {
        p1_new = particles[0];
        p2_new = particles[1];
      } else {
        p1_new = particles[1];
        p2_new = particles[0];
      }

      // This test ensures that close particles that are traveling apart don't collide. 
      // The original particles are overlapping, but velocities indicate they are traveling apart. 
      // Thus, if the collision works properly, we should not see any change in velocities. 

      REQUIRE(p1_new.GetPosition().x == Approx(4.9).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(5).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(-0.1).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(0).epsilon(0.01));
      REQUIRE(p2_new.GetPosition().x == Approx(5.11).epsilon(0.01));
      REQUIRE(p2_new.GetPosition().y == Approx(5).epsilon(0.01));
      REQUIRE(p2_new.GetVelocity().x == Approx(0.1).epsilon(0.01));
      REQUIRE(p2_new.GetVelocity().y == Approx(0).epsilon(0.01));
    }
  }

  SECTION("Wall collisions") {
    SECTION("Valid wall top") {
      ParticleContainer container;

      Particle p1("Test 1", vec2(2, 1), vec2(0, -1), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      container.InitializeParticle(p1);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new = particles[0];

      REQUIRE(p1_new.GetPosition().x == Approx(2).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(2).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(1).epsilon(0.01));

      container.Increment();

      particles = container.GetParticles();
      p1_new = particles[0];

      // Increment again for second check
      
      REQUIRE(p1_new.GetPosition().x == Approx(2).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(1).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(-1).epsilon(0.01));
    }

    SECTION("Valid wall bottom") {
      ParticleContainer container;
      container.Configure("../../../../../../config/test/config_test.json");
      container.SetParticles(vector<Particle>());

      Particle p1("Test 1", vec2(2, 99), vec2(0, 1), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      container.InitializeParticle(p1);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new = particles[0];

      REQUIRE(p1_new.GetPosition().x == Approx(2).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(98).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(-1).epsilon(0.01));

      container.Increment();

      particles = container.GetParticles();
      p1_new = particles[0];

      // Increment again for second check
      
      REQUIRE(p1_new.GetPosition().x == Approx(2).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(97).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(-1).epsilon(0.01));
    }

    SECTION("Valid wall left") {
      ParticleContainer container;
      container.Configure("../../../../../../config/test/config_test.json");
      container.SetParticles(vector<Particle>());

      Particle p1("Test 1", vec2(1, 30), vec2(-1, 0), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      container.InitializeParticle(p1);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new = particles[0];

      REQUIRE(p1_new.GetPosition().x == Approx(2).epsilon(0.001));
      REQUIRE(p1_new.GetPosition().y == Approx(30).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().x == Approx(1).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().y == Approx(0).epsilon(0.001));

      container.Increment();

      particles = container.GetParticles();
      p1_new = particles[0];

      // Increment again for second check
      
      REQUIRE(p1_new.GetPosition().x == Approx(3).epsilon(0.001));
      REQUIRE(p1_new.GetPosition().y == Approx(30).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().x == Approx(1).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().y == Approx(0).epsilon(0.001));
    }
    
    SECTION("Valid wall right") {
      ParticleContainer container;
      container.Configure("../../../../../../config/test/config_test.json");
      container.SetParticles(vector<Particle>());

      Particle p1("Test 1", vec2(200, 30), vec2(1, 0), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      container.InitializeParticle(p1);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new = particles[0];

      REQUIRE(p1_new.GetPosition().x == Approx(199).epsilon(0.001));
      REQUIRE(p1_new.GetPosition().y == Approx(30).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().x == Approx(-1).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().y == Approx(0).epsilon(0.001));

      container.Increment();

      particles = container.GetParticles();
      p1_new = particles[0];

      // Increment again for second check
      
      REQUIRE(p1_new.GetPosition().x == Approx(198).epsilon(0.001));
      REQUIRE(p1_new.GetPosition().y == Approx(30).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().x == Approx(-1).epsilon(0.001));
      REQUIRE(p1_new.GetVelocity().y == Approx(0).epsilon(0.001));
    }

    SECTION("Prevention of particles sticking to wall") {
      ParticleContainer container;

      Particle p1("Test 1", vec2(2, 0.9), vec2(0, -1), 1, 1,
                  ColorT<float>().hex(0xFFFFFF));

      container.InitializeParticle(p1);
      container.Increment();

      auto particles = container.GetParticles();
      Particle p1_new = particles[0];

      // This test ensures that a particle moving away from the wall does not
      // stick to it, as designated by the displacement and velocity threshold.

      REQUIRE(p1_new.GetPosition().x == Approx(2).epsilon(0.01));
      REQUIRE(p1_new.GetPosition().y == Approx(1.9).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().x == Approx(0).epsilon(0.01));
      REQUIRE(p1_new.GetVelocity().y == Approx(1).epsilon(0.01));
    }
  }
}