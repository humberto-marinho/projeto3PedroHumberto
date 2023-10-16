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
  glm::vec2 m_P{0.0f, 0.0f};

  void setupModel();
  float m_R{0.9f};
  float m_r{0.3f};
  float m_d{0.3};
  float m_t{0.0f};
  float m_t_inc{0.01f};
  bool m_stop_draw{false};
};

#endif