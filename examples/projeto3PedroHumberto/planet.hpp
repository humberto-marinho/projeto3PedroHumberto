#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"

class Planet : public Model {
public:
  Planet(std::string name)
      : Model(name) {} // construtor filho chama construtor pai

  void create();
  void paint();
};

#endif