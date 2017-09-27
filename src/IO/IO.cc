#include "IO.h"
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
using std::cout;
using std::ifstream;
using std::ofstream;

std::vector<std::string> split(std::string const& original, char separator);

int IO::fileCount = 0;

void IO::write(std::vector<Entity*>& entities) {
  ofstream outfile;
  outfile.open("../IO/" + std::to_string(++fileCount) + ".txt");
  cout << "Writing file " << fileCount << "...\n";
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

void IO::read(std::string name, std::vector<Entity*>& entities, RawModel& model) {
  glm::vec3 position, scale;
  glm::mat4 rotation;
  ifstream infile;
  cout << "Reading file " << name << "...\n";
  infile.open("../IO/" + name + ".txt");
  if (!infile) {
    cout << "ERROR::File Stream: Failed to open file '" << name << "'\n";
    return;
  }

  std::string line;
  int lineCount = 0;

  while (std::getline(infile, line)) {
    if (line == "" || line[0] == '#')
      continue;
    // cout << line << '\n';
    // position
    if (lineCount % 6 == 0) {
      std::vector<std::string> floats = split(line, ' ');
      position = glm::vec3(std::stof(floats[0]), std::stof(floats[1]), std::stof(floats[2]));
    }
    // scale
    else if (lineCount % 6 == 1) {
      std::vector<std::string> floats = split(line, ' ');
      scale = glm::vec3(std::stof(floats[0]), std::stof(floats[1]), std::stof(floats[2]));
    }
    // rotation
    else {
      int row = lineCount % 6 - 2;
      std::vector<std::string> floats = split(line, ' ');
      for (int i = 0; i < floats.size(); ++i)
      rotation[row] = glm::vec4(std::stof(floats[0]), std::stof(floats[1]), std::stof(floats[2]), std::stof(floats[3]));
    }

    if (lineCount % 6 == 5)
      entities.push_back(new Entity(model, glm::vec3(0.3f), position, scale, rotation));
    ++lineCount;
  }
}

std::vector<std::string> split(std::string const& original, char separator) {
  std::vector<std::string> results;
  std::string::const_iterator start = original.begin();
  std::string::const_iterator end = original.end();
  std::string::const_iterator next = std::find(start, end, separator);
  while (next != end) {
    results.push_back(std::string(start, next));
    start = next + 1;
    next = std::find(start, end, separator);
  }
  results.push_back(std::string(start, next));
  return results;
}
