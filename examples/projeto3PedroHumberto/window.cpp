#include "window.hpp"
#include "abcgOpenGLFunction.hpp"

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_r)
      m_zSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_t)
      m_zSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_f)
      m_xSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_g)
      m_xSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_x)
      m_YSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_z)
      m_YSpeed = -1.0f;
  } else if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_r && m_zSpeed != 0)
      m_zSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_t && m_zSpeed != 0)
      m_zSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_f && m_xSpeed > 0)
      m_xSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_g && m_xSpeed < 0)
      m_xSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_x && m_YSpeed > 0)
      m_YSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_z && m_YSpeed < 0)
      m_YSpeed = 0.0f;
  }
}

void Window::onCreate() { m_planet.create(); }

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Draw planet
  m_planet.render(m_camera.getViewMatrix(), m_camera.getProjMatrix(),
                  m_lightDir);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto flags{ImGuiWindowFlags_NoResize};

    glm::ivec2 winSize{160, 100};
    ImGui::SetNextWindowSize(ImVec2(winSize.x, winSize.y));
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - winSize.x, 0));

    ImGui::Begin("Helpers", nullptr, flags);

    ImGui::Text("Current position:\n(%.0f, %.0f, %.0f)", m_camera.getEye().x,
                m_camera.getEye().y, m_camera.getEye().z);

    if (ImGui::Button("Initial position")) {
      m_camera.resetPosition();
    }

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() { m_planet.destroy(); }

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.rotateAroundVectorZ(m_zSpeed * deltaTime);
  m_camera.rotateAroundVectorX(m_xSpeed * deltaTime);
  m_camera.moveY(m_YSpeed * deltaTime);
}