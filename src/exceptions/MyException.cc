#include "MyException.h"

MyException::MyException(const char* error): error(error) { }

const char* MyException::what() const throw() {
  return this->error;
}
