#include "IO.h"
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
using std::ofstream;
using std::cerr;

int IO::fileCount = 0;

void IO::write(std::vector<Entity*>& entities) {
  cerr << "Writing files...\n";
  ofstream outfile;
  outfile.open("../IO/" + std::to_string(++fileCount));
  for (int i = 0; i < entities.size(); ++i) {
    glm::vec3 position = entities[i]->getPos();
    glm::vec3 scale = entities[i]->getScale();
    glm::mat4 rotation = entities[i]->getRotationMatrix();
    outfile << "## Cylinder" << (i + 1) << '\n';
    outfile << "## position\n";
    outfile << position.x << ' ' << position.y << ' ' << position.z << '\n';
    outfile << "## scale\n";
    outfile << scale.x << ' ' << scale.y << ' ' << scale.z << '\n';
    outfile << "## rotation\n";
    for (int j = 0; j < 4; ++j) {
      outfile << rotation[j].x << ' ' << rotation[j].y << ' ' << rotation[j].z << ' ' << rotation[j].w << '\n';
    }
    outfile << '\n';
  }

  outfile.close();
}

void IO::read(const char* name) {

}
