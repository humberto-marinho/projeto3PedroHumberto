#include "window.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({.samples = 2, .doubleBuffering = false});
    window.setWindowSettings({.width = 1000,
                              .height = 1000,
                              .showFPS = false,
                              .showFullscreenButton = false,
                              .title = "Projeto 1 Pedro e Humberto"});

    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}
