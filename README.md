# Espaçonave Projeto Final

### Humberto Turioni Marinho - RA: 11201722439 (Turma A)
### Pedro Moreno Silva - RA: 11201722110 (Turma B)
Segundo projeto ministrado para a disciplina  [MCTA008-17 Computação Gráfica](http://professor.ufabc.edu.br/~harlen.batagelo/cg/) na [UFABC](https://www.ufabc.edu.br/) (período 2023.3) usando a biblioteca [ABCg](https://github.com/hbatagelo/abcg) e [OpenGL](https://www.opengl.org).

[Link para o primeiro projeto](examples/hipocicloide/README.md)

## Instalação

1. Siga as instruções de instalação e montagem no [site da disciplina](https://hbatagelo.github.io/abcg/abcg/doc/html/).
2. Faça o build e execute o arquivo `hipocicloide` em `build/bin/hipocicloide`.

Alternativamente, o programa também está hospedado [nesta página web](https://humberto-marinho.github.io/Projeto1PedroHumberto/public/index).


## Sobre o projeto

**Espaçonave** é uma aplicação em que podemos simular a câmera de uma espaçonave ao redor de um planeta Terra. Além disso, há um coelho dentro do planeta, a fim de demonstrar o uso de shaders diferentes para objetos diferentes.

Esta aplicação se baseia fortemente nos exemplos fornecidos em aula, especialmente a aula de [matrizes e transformações](https://hbatagelo.github.io/cg/transformations.html). A construção do chão e do coelho é idêntica aos exemplos de aula, no que não entraremos um muitos detalhes sobre a implementação destes.

### Planeta

Quanto ao planeta, ele é um [modelo gratuito](https://www.cgtrader.com/free-3d-models/space/planet/low-poly-planet-earth). A escolha dele foi especial pois existe uma distinção marcante entre as porções de água e de terra, tornando-se fácil colorir cada região com uma cor sem muito trabalho no shader. O código de `earth.vert` é o seguinte:
```glsl
#version 300 es

layout(location = 0) in vec3 inPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

float dist_from_planet_center(vec3 pos) {
  float sqrsum = pow(pos.x, 2.0) + pow(pos.y, 2.0) + pow(pos.z, 2.0);
  return sqrt(sqrsum);
}

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0 - (-posEyeSpace.z / 5.0);
  if (dist_from_planet_center(inPosition) > 1.009) {
    fragColor = vec4(i, i, i, 1) * vec4(0.0f, 1.0f, 0.0f, 1.0f);
  } else {
    fragColor = vec4(i, i, i, 1) * vec4(0.0f, 0.0f, 1.0f, 1.0f);
  }

  gl_Position = projMatrix * posEyeSpace;
}
```
Ele é muito semelhante à `lookat.vert`, exceto pela função `dist_from_planet_center` e pelo condicional ao final de `main`. `dist_from_planet_center` retorna a distância entre um vetor de 3 dimensões `pos` e o ponto $(0, 0, 0)$. Essa função nos deixará decidir se devemos colorir um ponto de verde ou azul se sua distância ultrapassar o valor de $1.009$ (esse valor foi determinado empiricamente). Isso funciona pois o centro do planeta está em $(0, 0, 0)$ em coordenadas do objeto, e `inPosition` também chega no shader em coordenadas de objeto. Após decidir a cor, voltamos a utilizar as matrizes de modelo, visualização e projeção no cálculo de `gl_Position`.

Vamos ver a classe `Planet`:
```cpp
// planet.hpp
#ifndef PLANET_HPP_
#define PLANET_HPP_

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Planet {
public:
  void create();
  void paint();
  void destroy();

  GLuint const &getProgram() const { return m_program; };

  GLint const &getViewMatrix() const { return m_viewMatrixLocation; };
  GLint const &getProjMatrix() const { return m_projMatrixLocation; };
  GLint const &getModelMatrix() const { return m_modelMatrixLocation; };

private:
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
```
Aqui vemos a definição da classe. Incluímos `struct Vertex` pois ele será necessário para `loadModelFromFile`. Outro ponto a se notar é que incluímos o atributo privado `m_program` para que possamos separar o shader do planeta dos demais shaders usados. Também foi necessário guardar os _locations_ das matrizes MVP na classe para que pudessemos usar dois shaders diferentes. Todos esses atributos adicionados têm métodos acessores de leitura.

```cpp
#include <unordered_map>

#include "planet.hpp"

// Explicit specialization of std::hash for Vertex
// Omitido (identico as notas de aula)

void Planet::create() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
m_program =
  abcg::createOpenGLProgram({
    {"source", assetsPath + "earth.vert", "stage", abcg::ShaderStage::Vertex},
    {"source", assetsPath + "earth.frag", "stage", abcg::ShaderStage::Fragment}
  });

  // Load model
  loadModelFromFile(assetsPath + "earth.obj");

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

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

void Planet::destroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

// void Planet::loadModelFromFile
// Omitido (identico as notas de aula)
```
`create` segue a mesma lógica de `Window::onCreate`: geramos o programa em `m_program` com base em `earth.vert` e `earth.frag`, guardamos os _locations_ das variáveis uniformes dos shaders nos atributos correspondentes, lemos o modelo `earth.obj`, geramos o VBO e o EBO, e realizamos o _bind_ desses no VAO.

`paint` também segue uma lógica semelhante à `Window::onPaint`: realizamos o _bind_ do VAO, definimos uma matriz de identidade `model` para realizar as operações de translação vertical em 3 unidades, e o aumento da escala do planeta em x2.

`destroy`, além do VBO e VAO, também tem que liberar o programa e o EBO.

Antes da chamada de `paint` em `Window::onPaint`, temos que indicar os valores das variáveis uniformes manualmente, pois elas dependem dos valores que estão em `Camera`, e `Planet` não tem acesso a eles: 
```cpp
void Window::onPaint() {
  //...

  // Setup to draw planet
  abcg::glUseProgram(m_planet.getProgram());

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_planet.getViewMatrix(), 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_planet.getProjMatrix(), 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  // Draw planet
  m_planet.paint();

  abcg::glUseProgram(0);
}
```
### Camera

A classe `Camera` tem a seguinte forma:

```cpp
// camera.hpp
#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
  void computeViewMatrix();
  void computeProjectionMatrix(glm::vec2 const &size);

  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);
  void rotateAroundVectorZ(float speed);
  void rotateAroundVectorX(float speed);
  void moveY(float speed);
  void resetPosition();

  glm::mat4 const &getViewMatrix() const { return m_viewMatrix; }
  glm::mat4 const &getProjMatrix() const { return m_projMatrix; }
  glm::vec3 const &getEye() const { return m_eye; }

private:
  glm::vec3 m_eye{0.0f, 0.5f, 2.5f}; // Camera position
  glm::vec3 m_at{0.0f, 0.5f, 0.0f};  // Look-at point
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};  // "up" direction

  // Matrix to change from world space to camera space
  glm::mat4 m_viewMatrix;

  // Matrix to change from camera space to clip space
  glm::mat4 m_projMatrix;
};

#endif
``` 

Muito semelhante aos códigos de aula também, mas com algumas modificações. A primeira delas é a presença de _getter_ para `m_eye`. Ele é usado para a exibir a posição da câmera pela UI. A segunda mudança é a adição de `rotateAroundVectorZ`, `rotateAroundVectorX`, `moveY`, e `resetPosition`, cujas implementações discutiremos a seguir.

```cpp
void Camera::resetPosition() {
  m_eye = {0.0f, 0.5f, 2.5f}; // Camera initial position
  m_at = {0.0f, 0.5f, 0.0f};  // Look-at initial point
  m_up = {0.0f, 1.0f, 0.0f};  // Initial "up" direction
}

void Camera::moveY(float speed) {
    m_eye += m_up * speed;
    m_at += m_up * speed;
    computeViewMatrix();
}
```

`resetPosition` não surpreendentemente retorna os atributos `m_eye`, `m_at` e `m_up` a seus valores iniciais. Essa função é chamada pelo botão de reiniciar a posição da UI. `moveY` incrementa a posição de `m_at` e `m_eye` verticalmente, dando um efeito semelhante a um foguete decolando.

```cpp
void Camera::rotateAroundVectorZ(float speed) {
  // Compute a vector that points along the camera's local Z-axis
  auto const forward{glm::normalize(m_at - m_eye)};

  glm::mat4 transform{1.0f};
  // Rotate camera around its local z axis
  transform = glm::rotate(transform, -speed, forward);
  m_up = glm::vec3(transform * glm::vec4(m_up, 0.0f));

  // Ensure the up vector is normalized
  m_up = glm::normalize(m_up);

  computeViewMatrix();
}
```

`rotateAroundVectorZ` acha o vetor `forward` e cria uma matriz de rotação `transform` em torno desse eixo. Posteriormente `transform` é aplicada a `m_up`. Isso gera o efeito de rotação como uma aeronave rodando.

```cpp
void Camera::rotateAroundVectorX(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};
  glm::vec3 left{glm::cross(m_up, forward)};

  // Rotate the view vector around the left vector
  glm::mat4 transformAt{1.0f};
  transformAt = glm::rotate(transformAt, -speed, left);
  m_at = m_eye + glm::vec3(transformAt * glm::vec4(m_at - m_eye, 0.0f));

  // Rotate the up vector around the left vector as well
  glm::mat4 transformUp{1.0f};
  transformUp = glm::rotate(transformUp, -speed, left);
  m_up = glm::vec3(transformUp * glm::vec4(m_up, 0.0f));

  computeViewMatrix();
}
```

Analogamente, `rotateAroundVectorX` também gera uma matriz de rotação, mas o eixo de rotação é `left`, um vetor gerado pelo produto vetorial entre `m_up` e `forward`. Essa matriz de rotação é aplicada a `m_up` e `forward`. Somando o novo `forward` com `m_eye` achamos o novo `m_at`.

### UI

Adicionamos uma janela que indica a posição da câmera e que possui um botão de resetar a posição por conveniência:

```cpp
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
```
