#include <unordered_map>

#include "model.hpp"
#include "planet.hpp"

void Planet::create() { Model::create(); } // usa metodo do pai

void Planet::paint() {

  abcg::glBindVertexArray(m_VAO);

  // Draw planet
  glm::mat4 model{1.0f};

  model = glm::mat4(1.0);

  // Translates upwards and doubles the scale
  model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
  model = glm::scale(model, glm::vec3(2.0f));

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  abcg::glBindVertexArray(0);
}
