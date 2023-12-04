#include <unordered_map>

#include "model.hpp"
#include "planet.hpp"

// void Planet::create() { Model::create(); }

void Planet::render(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 lightDir) {
  /*
   * Binding resources
   */
  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);
  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  /*
   * Getting locations...
   */
  // Visualizations matrices locations
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  auto const normalMatrixLoc{
      abcg::glGetUniformLocation(m_program, "normalMatrix")};
  auto const modelMatrixLoc{
      abcg::glGetUniformLocation(m_program, "modelMatrix")};

  // Light properties locations
  auto const lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  auto const IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program, "Is")};

  // Material properties locations
  auto const shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};

  // Textures locations
  auto const diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};

  /*
   * Model matrix operations
   */
  m_modelMatrix = glm::mat4(1.0); // isso vai quebrar no onUpdate?

  // Translates upwards and doubles the scale
  m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0f, 3.0f, 0.0f));
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(2.0f));

  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  /*
   * Setting uniforms
   */
  // Set uniform variables for viewMatrix and projMatrix
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMat[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &projMat[0][0]);

  // Set uniform variables for normalMatrix
  auto const modelViewMatrix{glm::mat3(viewMat * m_modelMatrix)};
  auto const normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  // Set uniform variables for diffuse texture
  abcg::glUniform1i(diffuseTexLoc, 0);

  // Sets uniform varialble for lightDirection
  abcg::glUniform4fv(lightDirLoc, 1, &lightDir.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables for the current model
  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  /*
   * Freeing resources
   */
  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}
