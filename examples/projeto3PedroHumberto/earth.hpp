#ifndef EARTH_HPP_
#define EARTH_HPP_

#include "planet.hpp"

class Earth : public Planet {
public:
  // construtor filho chama construtor pai
  Earth(std::string objName, std::string shadersName, std::string textureName)
      : Planet(objName, shadersName, textureName) {}

  void transformModel() override;
  void update();

private:
  float rotation_increment{0.0};
  glm::vec3 m_pos{0.0f, 0.0f, 0.0f};
};

#endif