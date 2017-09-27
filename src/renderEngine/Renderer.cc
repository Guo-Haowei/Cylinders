#define GLEW_STATIC
#include "Renderer.h"
#include "../common.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

Renderer::Renderer() { }

void Renderer::render(vector<Entity*> entities) {
  prepare();
  entityShader.start();
  entityShader.loadViewMatrix();
  entityShader.loadProjectionMatrix(glm::perspective(ZOOM, (float) WIDTH / (float) HEIGHT, NEAR_PLANE, FAR_PLANE));
  for (auto& entity: entities) {
    prepareModel(entity);
    renderEntity(entity);
  }
  entityShader.stop();
}

void Renderer::renderEntity(Entity* entity) {
  entityShader.loadColor(entity->getColor());
  entityShader.loadTransformationMatrx(entity->createTransformationMatrix());
  // glDrawElements(GL_TRIANGLES, entity->getModel().getVertexCount(), GL_UNSIGNED_INT, (void*) 0);
  glDrawArrays(GL_TRIANGLES, 0, entity->getModel().getVertexCount());
}

void Renderer::prepare() {
  glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
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
