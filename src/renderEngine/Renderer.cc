#define GLEW_STATIC
#include "Renderer.h"
#include "../common.h"
#include "../entities/Camera.h"
#include "../inputManager/MouseManager.h"
#include "../models/CylinderList.h"
#include "../models/TwoCircles.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <algorithm>
#include <iostream>
using std::cout;

typedef struct Sorter {
  float dist;
  int id;
  Sorter(int id, float dist): id(id), dist(dist) {}
} Sorter;

int sorter(const Sorter& a, const Sorter& b) {
  return a.dist > b.dist;
}

Renderer::Renderer() { }

Renderer::~Renderer() {
  entityShader.clean();
  colorPickShader.clean();
}

void Renderer::render(vector<Entity*> entities) {
  prepare();

  glDisable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  colorPickShader.start();
  colorPickShader.loadViewMatrix();
  colorPickShader.loadProjectionMatrix(glm::perspective(ZOOM, (float) WIDTH / (float) HEIGHT, NEAR_PLANE, FAR_PLANE));
  for (int i = 0; i < entities.size(); ++i) {
    prepareModel(entities[i]);
    renderEntity(entities[i], COLORPICK);
    // two circles
    if (TwoCircles::renderCircle) {
      colorPickShader.loadColor(entities[i]->getID());
      colorPickShader.loadTransformationMatrx(entities[i]->createTransformationMatrix());
      prepareModel(TwoCircles::twoCircles);
      glDrawArrays(GL_TRIANGLES, 0, TwoCircles::twoCircles->getModel().getVertexCount());
    }
  }
  colorPickShader.stop();
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);

  if (MouseManager::doubleClick()) {
    glFlush();
    glFinish();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char data[4];
    int x = (WIDTH / WINDOW_WIDTH) * (int)MouseManager::currentX;
    int y = HEIGHT - (HEIGHT / WINDOW_HEIGHT) * (int)MouseManager::currentY;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

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
  // render entity by order
  vector<Sorter> order;
  glm::vec3 eye = Camera::getPos();
  for (int i = 0; i < entities.size(); ++i) {
    glm::vec4 pos = entities[i]->getTransformtationMatrix() * glm::vec4(entities[i]->getPos(), 1.0f);
    float dx = pos.x - eye.x;
    float dy = pos.y - eye.y;
    float dz = pos.z - eye.z;
    order.push_back(Sorter(i, dx*dx+dy*dy+dz*dz));
  }
  std::sort(order.begin(), order.end(), sorter);

  for (int i = 0; i < entities.size(); ++i) {
    Entity* e = entities[order[i].id];
    prepareModel(e);
    renderEntity(e);
    // render two circles separately
    if (transparent) {
      // render axis
      glDisable(GL_DEPTH_TEST);
      Entity* axis = CylinderList::axis;
      axis->setTransform(e->getTransformtationMatrix());
      glm::vec3 scale = e->getScale();
      scale.x = 0.01; scale.z = 0.01;
      axis->setScale(scale);
      prepareModel(axis);
      renderEntity(axis);
      glEnable(GL_DEPTH_TEST);
    }
    if (TwoCircles::renderCircle) {
      entityShader.loadColor(e->getColor());
      entityShader.loadTransformationMatrx(e->createTransformationMatrix());
      prepareModel(TwoCircles::twoCircles);
      glDrawArrays(GL_TRIANGLES, 0, TwoCircles::twoCircles->getModel().getVertexCount());
    }
  }
  entityShader.stop();

  if (CylinderList::selected) {
    colorPickShader.start();
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
