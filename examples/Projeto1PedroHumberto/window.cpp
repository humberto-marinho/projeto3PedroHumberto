#include "window.hpp"
#include <string>

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es
    layout(location = 0) in vec2 inPosition;

    void main() { 
      gl_PointSize = 2.0;
      gl_Position = vec4(inPosition, 0, 1); 
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
    precision mediump float;

    out vec4 outColor;

    void main() { outColor = vec4(1); }
  )gl"};

  // Create shader program
  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  // Clear window
  abcg::glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b,
                     m_clearColor.a);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  std::array<GLfloat, 2> sizes{};
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  abcg::glGetFloatv(GL_POINT_SIZE_RANGE, sizes.data());
#else
  abcg::glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes.data());
#endif
  fmt::print("Point size: {:.2f} (min), {:.2f} (max)\n", sizes.at(0),
             sizes.at(1));
}

void Window::onPaint() {
  if (m_stop_draw) {
    return;
  }

  // Create OpenGL buffers for drawing the point at m_P
  setupModel();

  // Set the viewport
  abcg::glViewport(200, 0, 600, 600);

  // Start using the shader program
  abcg::glUseProgram(m_program);
  // Start using VAO
  abcg::glBindVertexArray(m_VAO);

  // Draw a single point
  abcg::glDrawArrays(GL_POINTS, 0, 1);

  // End using VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);

  m_P.x = (m_R - m_r) * cos(m_t) + m_d * cos((m_R - m_r) * m_t / m_r);
  m_P.y = (m_R - m_r) * sin(m_t) - m_d * sin((m_R - m_r) * m_t / m_r);
  m_t += m_t_inc;
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    ImGui::SetNextWindowPos(ImVec2(5, 5));
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    if (ImGui::Button("Clear window", ImVec2(150, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    // Edit background color
    if (ImGui::ColorEdit3("Background", &m_clearColor.r,
                          ImGuiColorEditFlags_NoSmallPreview)) {
      abcg::glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b,
                         m_clearColor.a);
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if (ImGui::DragFloat("velocidade do traçado", &m_t_inc, 0.001f, 0.001f,
                         1.0f)) {
      m_t = 0.0f;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }
    if (ImGui::DragFloat("R", &m_R, 0.001f, 0.01f, 0.95f)) {
      m_t = 0.0f;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }
    if (ImGui::DragFloat("r", &m_r, 0.001f, 0.01f, 0.95f)) {
      m_t = 0.0f;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }
    if (ImGui::DragFloat("d", &m_d, 0.001f, 0.01f, 0.95f)) {
      m_t = 0.0f;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if (ImGui::Button(
            ((!m_stop_draw ? std::string("Stop") : std::string("Continue")) +
             std::string(" drawing"))
                .c_str())) {
      m_stop_draw = !m_stop_draw;
    }

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  // Release shader program, VBO and VAO
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::setupModel() {
  // Release previous VBO and VAO
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_VBOVertices);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  // Upload data to VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_P), &m_P, GL_STATIC_DRAW);
  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}