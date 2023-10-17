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

  // Point to be draw
  glm::vec2 m_P{0.0f, 0.0f};

  void setupModel();

  // Bigger circle radius
  float m_R{0.9f};

  // Smaller circle radius
  float m_r{0.3f};

  // Offset between the center of the smaller circle and
  // the reference point m_P
  float m_d{0.3};

  // Parametric variable
  float m_t{0.0f};

  // Parametric value increment
  float m_t_inc{0.01f};

  // Skip render flag
  bool m_stop_draw{false};
};

#endif