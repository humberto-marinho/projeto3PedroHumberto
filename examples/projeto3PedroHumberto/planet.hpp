#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"

class Planet : public Model {
public:
  Planet(std::string objName, std::string shadersName)
      : Model(objName, shadersName) {} // construtor filho chama construtor pai

  // void create();
  void render(glm::mat4 viewMat, glm::mat4 projMat,
              glm::vec4 lightDir) override;
};

#endif