#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"

class Planet : public Model {
public:
  Planet(std::string objName, std::string shadersName, std::string textureName)
      : Model(objName, shadersName, textureName) {
  } // construtor filho chama construtor pai
  virtual ~Planet() = default;

  // void create();
  void render(glm::mat4 viewMat, glm::mat4 projMat,
              glm::vec4 lightDir) override;
  virtual void transformModel();
};

#endif