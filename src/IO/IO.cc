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
  // write file 1
  ofstream outfile;
  cout << "Writing files...\n";
  outfile.open("../cylinders.txt");
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

  // write file 2
  outfile.open("../cylinders_standard.txt");
  for (int i = 0; i < entities.size(); ++i) {
    float h = entities[i]->getScale().y * 2.0f;
    glm::vec4 y4d = entities[i]->getRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    glm::vec3 y = h / 2.0f * glm::vec3(y4d.x, y4d.y, y4d.z);
    glm::vec3 A = entities[i]->getPos() - y;
    glm::vec3 B = entities[i]->getPos() + y;
    glm::vec3 v = glm::normalize(B - A);

    outfile << "## Cylinder" << (i + 1) << '\n';
    outfile << "## P\n";
    outfile << A.x << ' ' << A.y << ' ' << A.z << '\n';
    outfile << "## v\n";
    outfile << v.x << ' ' << v.y << ' ' << v.z << '\n';
    outfile << "## h\n" << h << '\n';
    outfile << "## r\n" << entities[i]->getScale().x << "\n\n";
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
