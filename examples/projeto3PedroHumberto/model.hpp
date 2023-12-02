#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"
#include <string>

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  Model(std::string name) { m_name = name; }

  void create();
  void paint();
  void destroy();

  GLuint const &getProgram() const { return m_program; };

  GLint const &getViewMatrix() const { return m_viewMatrixLocation; };
  GLint const &getProjMatrix() const { return m_projMatrixLocation; };
  GLint const &getModelMatrix() const { return m_modelMatrixLocation; };

protected:
  std::string m_name{};

  GLuint m_program{};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  void loadModelFromFile(std::string_view path);
};

#endif