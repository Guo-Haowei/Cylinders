#define GLEW_STATIC
#include "Renderer.h"
#include "../common.h"
#include "../inputManager/MouseManager.h"
#include "../models/CylinderList.h"
#include "../models/TwoCircles.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>
using std::cout;

Renderer::Renderer() { }

Renderer::~Renderer() {
  entityShader.clean();
  colorPickShader.clean();
}

void Renderer::render(vector<Entity*> entities) {
  prepare();

  colorPickShader.start();
  colorPickShader.loadViewMatrix();
  colorPickShader.loadProjectionMatrix(glm::perspective(ZOOM, (float) WIDTH / (float) HEIGHT, NEAR_PLANE, FAR_PLANE));
  for (int i = 0; i < entities.size(); ++i) {
    prepareModel(entities[i]);
    renderEntity(entities[i], COLORPICK);
  }
  colorPickShader.stop();

  if (MouseManager::doubleClick()) {
    glFlush();
    glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4];
    int x = (WIDTH / WINDOW_WIDTH) * (int)MouseManager::currentX;
    int y = HEIGHT - (HEIGHT / WINDOW_HEIGHT) * (int)MouseManager::currentY;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    int id = data[0] + data[1] * 256 + data[2] * 256 * 256;
    if (id == 0x00ffffff) {
      CylinderList::selected = nullptr;
      MouseManager::setMode(SCENE);
    } else {
      for (int i = 0; i < CylinderList::cylinders.size(); ++i) {
        if (CylinderList::cylinders[i]->getID() == id) {
          CylinderList::selected = CylinderList::cylinders[i];
          MouseManager::setMode(OBJECT);
          break;
        }
      }
    }
  }

  prepare();

  entityShader.start();
  entityShader.loadViewMatrix();
  entityShader.loadProjectionMatrix(glm::perspective(ZOOM, (float) WIDTH / (float) HEIGHT, NEAR_PLANE, FAR_PLANE));
  for (int i = 0; i < entities.size(); ++i) {
    prepareModel(entities[i]);
    renderEntity(entities[i]);
  }
  entityShader.stop();

  if (CylinderList::selected) {
    colorPickShader.start();
    glm::vec3 scale = CylinderList::selected->getScale();
    scale.y += 0.004f;
    // upper circle
    prepareModel(TwoCircles::upper);
    colorPickShader.loadColor(TwoCircles::upper->getColor());
    colorPickShader.loadTransformationMatrx(CylinderList::selected->createTransformationMatrix());
    glDrawArrays(GL_LINES, 0, TwoCircles::upper->getModel().getVertexCount());

    // lower circle
    prepareModel(TwoCircles::lower);
    colorPickShader.loadColor(TwoCircles::lower->getColor());
    colorPickShader.loadTransformationMatrx(CylinderList::selected->createTransformationMatrix());
    glDrawArrays(GL_LINES, 0, TwoCircles::lower->getModel().getVertexCount());
    colorPickShader.stop();
  }
}

void Renderer::renderEntity(Entity* entity, SHADER type) {
  if (type == ENTITY) {
    entityShader.loadColor(entity->getColor());
    entityShader.loadTransformationMatrx(entity->createTransformationMatrix());
  } else if (type == COLORPICK) {
    colorPickShader.loadColor(entity->getID());
    colorPickShader.loadTransformationMatrx(entity->createTransformationMatrix());
  }
  glDrawArrays(GL_TRIANGLES, 0, entity->getModel().getVertexCount());
}

void Renderer::prepare() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
