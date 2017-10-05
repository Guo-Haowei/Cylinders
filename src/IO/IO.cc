#include "IO.h"
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::stringstream;

void IO::write(std::vector<Entity*>& entities) {
  ofstream outfile;
  outfile.open("../cylinders.txt");
  cout << "Writing file cylinders.txt...\n";
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

void IO::read(std::vector<Entity*>& entities, RawModel& model) {
  glm::vec3 position, scale;
  glm::mat4 rotation;
  ifstream infile;
  cout << "Reading file cylinders.txt...\n";
  infile.open("../cylinders.txt");
  if (!infile) {
    cout << "ERROR::File Stream: Failed to open file cylinders.txt\n";
    return;
  }

  std::string line;
  int lineCount = 0;

  while (std::getline(infile, line)) {
    if (line == "")
      continue;
    if (line[0] == '#')
      continue;

    stringstream ss(line);
    if (lineCount % 6 == 0) {
      ss >> position.x >> position.y >> position.z;
    }
    // scale
    else if (lineCount % 6 == 1) {
      ss >> scale.x >> scale.y >> scale.z;
    }
    // rotation
    else {
      int row = lineCount % 6 - 2;
      ss >> rotation[row].x >> rotation[row].y >> rotation[row].z >> rotation[row].w;
    }

    if (lineCount % 6 == 5)
      entities.push_back(new Entity(model, glm::vec3(0.3f), position, scale, rotation));
    ++lineCount;
  }
}
