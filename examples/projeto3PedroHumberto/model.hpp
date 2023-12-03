#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"
#include <string>

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  Model(std::string objName, std::string shadersName) {
    m_objName = objName;
    m_shadersName = shadersName;
  }

  void loadObj(std::string_view path);
  virtual void render(glm::mat4, glm::mat4, glm::vec4) = 0;
  void setupVAO(GLuint program);
  void create();
  void destroy();

  // deletar?
  GLuint const &getProgram() const { return m_program; };

  // deletar?
  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }

  glm::mat4 const &getModelMatrix() const { return m_modelMatrix; };

protected:
  std::string m_objName{};
  std::string m_shadersName{};

  GLuint m_program{};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::mat4 m_modelMatrix{1.0f};

  // mover para a especializacao?
  // Light and material properties initial values
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd{0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks{1.0f};
  float m_shininess{25.0f};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_hasNormals{false};

  void createBuffers();
  void computeNormals();
};

#endif