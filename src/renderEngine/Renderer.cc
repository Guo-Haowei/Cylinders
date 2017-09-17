#define GLEW_STATIC
#include "Renderer.h"
#include <GL/glew.h>

Renderer::Renderer() { }

void Renderer::render(vector<Entity*> entities) {
  prepare();
  entityShader.start();
  for (auto& entity: entities) {
    renderEntity(entity);
  }
  entityShader.stop();
}

void Renderer::renderEntity(Entity* entity) {
  // pass its matrices
  glDrawElements(GL_TRIANGLES, entity->getModel().getVertexCount(), GL_UNSIGNED_INT, (void*) 0);
}

void Renderer::prepare() {
  glClearColor(0.3f, 0.4f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::prepareModel(Entity* entity) {
  glBindVertexArray(entity->getModel().getVaoID());
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void Renderer::unbindModel() {
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindVertexArray(0);
}
