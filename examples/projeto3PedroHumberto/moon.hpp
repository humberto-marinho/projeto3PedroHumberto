#ifndef MOON_HPP_
#define MOON_HPP_

#include "planet.hpp"
#include <glm/fwd.hpp>

class Moon : public Planet {
public:
  Moon(std::string objName, std::string shadersName, std::string textureName)
      : Planet(objName, shadersName, textureName) {
  } // construtor filho chama construtor pai

  glm::vec3 const &getPos() const { return m_pos; }

  void transformModel() override;
  void update();

private:
  const float m_biggerAxis{3.0f};
  const float m_smallerAxis{2.0f};
  const float m_angularSpeed{10.0f};
  float translation_increment{0.0f};
  float rotation_increment{0.0f};
  glm::vec3 m_pos{
      m_biggerAxis * std::cos(m_angularSpeed * translation_increment), 0.0f,
      m_smallerAxis *std::sin(m_angularSpeed *translation_increment)};
};

#endif