#include "earth.hpp"
#include <cmath>
#include <math.h>

void Earth::transformModel() {
  /*
   * Model matrix operations
   */
  m_modelMatrix = glm::mat4(1.0); // isso vai quebrar no onUpdate?

  // Translates upwards and doubles the scale
  m_modelMatrix = glm::rotate(m_modelMatrix, rotation_increment,
                              glm::vec3{0.0f, 1.0f, 0.0f});
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(2.0f));
}

void Earth::update() { rotation_increment += M_PI * 0.00005; }