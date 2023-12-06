#include "moon.hpp"

void Moon::transformModel() {
  /*
   * Model matrix operations
   */
  m_modelMatrix = glm::mat4(1.0); // isso vai quebrar no onUpdate?

  // Translates upwards and doubles the scale
  m_modelMatrix = glm::rotate(m_modelMatrix, translation_increment,
                              glm::vec3{0.0f, 1.0f, 0.0f});
  m_modelMatrix = glm::translate(m_modelMatrix, m_pos);
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.5f));
}

void Moon::update() {
  translation_increment += 0.00005; // rapido
  rotation_increment += 2 * M_PI / 0.00005;
  // translation_increment = translation_increment + 0.000005; // devagar
  m_pos.x = m_biggerAxis * std::cos(m_angularSpeed * translation_increment);
  m_pos.z = m_smallerAxis * std::sin(m_angularSpeed * translation_increment);
}