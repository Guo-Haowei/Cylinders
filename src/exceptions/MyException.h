#include <exception>

class MyException: public std::exception {
private:
  const char* error;
public:
  MyException(const char* error);
  const char* what() const throw();
};
