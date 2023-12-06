# Espaçonave Projeto Final

### Humberto Turioni Marinho - RA: 11201722439 (Turma A)
### Pedro Moreno Silva - RA: 11201722110 (Turma B)
Terceiro projeto ministrado para a disciplina  [MCTA008-17 Computação Gráfica](http://professor.ufabc.edu.br/~harlen.batagelo/cg/) na [UFABC](https://www.ufabc.edu.br/) (período 2023.3) usando a biblioteca [ABCg](https://github.com/hbatagelo/abcg) e [OpenGL](https://www.opengl.org).

[Link para o primeiro projeto](examples/hipocicloide/README.md)

## Instalação

1. Siga as instruções de instalação e montagem no [site da disciplina](https://hbatagelo.github.io/abcg/abcg/doc/html/).
2. Faça o build e execute o arquivo `projeto3PedroHumberto` em `build/bin/projeto3PedroHumberto`.

Alternativamente, o programa também está hospedado [nesta página web]().


## Sobre o projeto

Essa segunda versão do projeto **Espaçonave** é uma extensão do projeto anterior, agora com texturas, iluminação e movimentos.

Baseamo-nos fortemente nas aulas de [iluminação](https://hbatagelo.github.io/cg/lighting.html) e [texturização](https://hbatagelo.github.io/cg/texturing.html).

### Abstrações

Algumas refatorações foram feitas no código a fim de aumentar a legibilidade e isolar responsabilidades. A primeira foi a introdução da classe abstrata `Model`, que contem todas as operações relevantes para a renderização de um objeto 3D com iluminação e textura. Vejamos sua definição:

```cpp
#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"
#include <string>

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  Model(std::string objName, std::string shadersName, std::string textureName) {
    m_objName = objName;
    m_shadersName = shadersName;
    m_textureName = textureName;
  }

  void loadObj(std::string_view path, bool standart = true);
  void loadDiffuseTexture(std::string_view path);
  virtual void render(glm::mat4, glm::mat4, glm::vec4) = 0;
  void setupVAO(GLuint program);
  void create();
  void destroy();

  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }

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
  std::string m_textureName{};

  GLuint m_program{};
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::mat4 m_modelMatrix{1.0f};

  // Light and material properties initial values
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd{0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks{1.0f};
  float m_shininess{25.0f};
  GLuint m_diffuseTexture{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void computeNormals();
  void createBuffers();
  void standardize();
};

#endif
```

Vemos aqui a semelhança com a classe `Model` desenvolvida em aula por conta das funções `loadObj`, `loadDiffuseTexture`, `computeNormals`, `setupVAO`, `createBuffers` e `standardize`. Elas têm as mesmas implementações daquelas vistas em aula. O que a nossa classe `Model` tem de diferente é que ela seus atributos com acesso `protected`, permitindo que suas filhas usem esses atributos e métodos. Também temos de diferença o método `create` e o método virtual `render`. Vejamos primeiro `create`:

```cpp
void Model::create() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);

  // Create program
  m_program = abcg::createOpenGLProgram(
      {{.source = assetsPath + "shaders/" + m_shadersName + ".vert", .stage = abcg::ShaderStage::Vertex}
        .stage = abcg::ShaderStage::Vertex},
       {.source = assetsPath + "shaders/" + m_shadersName + ".frag",
        .stage = abcg::ShaderStage::Fragment}});

  // Load model
  loadDiffuseTexture(assetsPath + "maps/" + m_textureName);
  loadObj(assetsPath + m_objName + ".obj");
  setupVAO(m_program);
}
```

Esse método implementa a compilação do programa de _shaders_, bem como a leitura das texturas e do objeto, e faz o _setup_ do VAO. Os nomes do objeto, dos _shaders_ e das texturas são fornecidos pelo construtor da classe.

`Model` também possui o método virtual `render`, que deve ser implementado por todas as classes que a extendem. Vejamos a implementação feita por `Planet`.

```cpp
void Planet::render(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 lightDir) {
  abcg::glUseProgram(m_program);

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
  transformModel();

  /*
   * Setting uniforms
   */
  // Set uniform variable for modelMatrix
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

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
   * Binding resources
   */
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
   * Draws
   */
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  /*
   * Freeing resources
   */
  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}
```

Ele é extenso, mas simples. Consiste de fases bem definidas para que possamos fazer a renderização bem sucedida do modelo. As fases são as seguintes:
1. Realizar o _bind_ do programa de _shaders_ do modelo em questão.
2. Pegar as localizações das variáveis uniformes usadas nos shaders
3. Chamar `transformModel()`.
4. Passar os valores corretos para as variáveis uniformes.
5. Fazer o _bind_ do VAO e das texturas
6. Desenhar.
7. Liberar os recursos.

`transformModel` implementa as transformações a serem aplicadas na matrix de modelo e serão vistas mais a frente caso a caso. Esse método é virtual mas possui implementação em `Planet`. Como ela não faz nada, não é muito interessante de se mostrar. Entretanto, vamos ver `Moon` e sua implementação de `transformModel`.

### Terra e Lua

```cpp
void Moon::transformModel() {
  /*
   * Model matrix operations
   */
  m_modelMatrix = glm::mat4(1.0);

  // Translates upwards and doubles the scale
  m_modelMatrix = glm::rotate(m_modelMatrix, translation_increment,
                              glm::vec3{0.0f, 1.0f, 0.0f});
  m_modelMatrix = glm::translate(m_modelMatrix, m_pos);
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.5f));
}
```

Essas transformações envolvem rotacionar a Lua `translation_increment` radianos ao redor de $y$ e transladá-la para `m_pos`, além de diminuir o seu tamanho pela metade.

`m_pos` e `translation_increment` são implementados conforme `update`:

```cpp
void Moon::update() {
  translation_increment += 0.00005;
  rotation_increment += 2 * M_PI / 0.00005;
  m_pos.x = m_biggerAxis * std::cos(m_angularSpeed * translation_increment);
  m_pos.z = m_smallerAxis * std::sin(m_angularSpeed * translation_increment);
}
```

"Ao final de cada quadro" esse método é chamado e atualiza esse atributos. `m_pos` é atualizado de forma a desenhar uma elipse no plano $xz$.

As implementações de `Earth` são as seguintes:

```cpp
void Earth::transformModel() {
  /*
   * Model matrix operations
   */
  m_modelMatrix = glm::mat4(1.0);

  // Translates upwards and doubles the scale
  m_modelMatrix = glm::rotate(m_modelMatrix, rotation_increment,
                              glm::vec3{0.0f, 1.0f, 0.0f});
  m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(2.0f));
}

void Earth::update() { rotation_increment += M_PI * 0.00005; }
```

Muito semelhante ao caso de `Moon`, porém em `Earth` só há rotação e sua escala é dobrada.
Ambos usam os mesmos _shaders_, mas nada impediria de serem shaders diferentes. Veremo-os a seguir.

### Shaders

```glsl
#earth.vert
#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat3 normalMatrix;

// Light properties
uniform vec4 lightDirWorldSpace;

out vec3 fragV;
out vec3 fragL;
out vec3 fragN;
out vec2 fragTexCoord;

void main() {
  vec3 P = (viewMatrix * modelMatrix * vec4(inPosition, 1.0)).xyz;
  vec3 N = normalMatrix * inNormal;
  vec3 L = -(viewMatrix * lightDirWorldSpace).xyz;

  fragL = L;
  fragV = -P;
  fragN = N;
  fragTexCoord = inTexCoord;

  gl_Position = projMatrix * vec4(P, 1.0);
}
```

```glsl
#earth.frag
#version 300 es

precision mediump float;

in vec3 fragN;
in vec3 fragL;
in vec3 fragV;
in vec2 fragTexCoord;

// Light properties
uniform vec4 Ia, Id, Is;

// Material properties
uniform vec4 Ka, Kd, Ks;
uniform float shininess;

// Diffuse texture sampler
uniform sampler2D diffuseTex;

out vec4 outColor;

// Blinn-Phong reflection model
vec4 BlinnPhong(vec3 N, vec3 L, vec3 V, vec2 texCoord) {
  N = normalize(N);
  L = normalize(L);

  // Compute lambertian term
  float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    vec3 H = normalize(L + V);
    float angle = max(dot(H, N), 0.0);
    specular = pow(angle, shininess);
  }

  vec4 map_Kd = texture(diffuseTex, texCoord);
  vec4 map_Ka = map_Kd;

  vec4 diffuseColor = map_Kd * Kd * Id * lambertian;
  vec4 specularColor = Ks * Is * specular;
  vec4 ambientColor = map_Ka * Ka * Ia;

  return ambientColor + diffuseColor + specularColor;
}

void main() {
  vec4 color = BlinnPhong(fragN, fragL, fragV, fragTexCoord);

  if (gl_FrontFacing) {
    outColor = color;
  } else {
    float i = (color.r + color.g + color.b) / 3.0;
    outColor = vec4(i, 0, 0, 1.0);
  }
}
```
`earth.vert` é quase identico às notas de aula, menos pelo fato de que não passamos mais adiante os valores para os mapeamentos de texturas manuais, pois essas opções foram excluídas do nosso projeto. O mesmo pode ser dito para `earth.frag`: retiramos toda a lógica de escolha de mapeamento para só usarmos o _UV mapping_.

Isso conclui a análise dos objetos.

### Window

Com a refatoração feita anteriormente, `Window` passa somente a gerenciar o ciclo de vida da aplicação e guardar atributos e métodos que são globais à cena, como a direção de iluminação:

```cpp
void Window::onCreate() {
  m_earth.create();
  m_moon.create();
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Draw planet
  m_earth.render(m_camera.getViewMatrix(), m_camera.getProjMatrix(),
                 m_lightDir);

  m_moon.render(m_camera.getViewMatrix(), m_camera.getProjMatrix(), m_lightDir);
}

void Window::onDestroy() {
  m_earth.destroy();
  m_moon.destroy();
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  m_camera.rotateAroundVectorZ(m_zSpeed * deltaTime);
  m_camera.rotateAroundVectorX(m_xSpeed * deltaTime);
  m_camera.moveY(m_YSpeed * deltaTime);

  // Update objects
  m_moon.update();
  m_earth.update();
}
```

Fica claro que agora basta chamar os métodos adequados de cada objeto da cena que todo o _heavy lifting_ de renderizar o objeto é feito por eles mesmos.

### Câmera e UI

Não houve mudanças nessas classe, há não ser a mudança da posição inicial da câmera, no que não há o que comentar.
