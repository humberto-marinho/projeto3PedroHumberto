#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};
  glm::vec4 m_clearColor{0.0f, 0.0f, 0.0f, 1.00f};

  GLuint m_VAO{};
  GLuint m_VBOVertices{};
  GLuint m_program{};
  glm::vec2 m_P{};

  void setupModel();
  float t{};
};

#endif