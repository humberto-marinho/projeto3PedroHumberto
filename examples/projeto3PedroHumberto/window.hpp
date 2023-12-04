#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

#include "camera.hpp"
#include "planet.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};
  float y_panSpeed{};
  float m_zSpeed{};
  float m_xSpeed{};
  float m_YSpeed{};

  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};

  Planet m_planet{"roman_lamp", "earth"};
};

#endif