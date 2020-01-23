#pragma once
// lang::Cpp

#include <iostream>
#include <string.h>


class Cout {
  public:
    Cout* p(char* s) { std::cout << s ; return this; }
    Cout* p(size_t i) { std::cout << i ; return this; }
    Cout* p(int i) { std::cout << i ; return this; }
    Cout* p(const char* s) { std::cout << s ; return this; }

    Cout* pln(char* s) { std::cout << s << "\n" ;  return this; }
    Cout* pln(size_t i) { std::cout << i << "\n" ; return this; }
    Cout* pln(int i) { std::cout << i << "\n" ; return this; }
    Cout* pln(const char* s) { std::cout << s << "\n" ; return this; } 
};

bool is_boolean(std::string line_string) {
  return line_string.compare("0") || line_string.compare("1");
}

bool is_first_char_numeric(std::string line_string) {
  // Ints and floats can start with a + or - character, like "-2" or "+30"
  return line_string[0] == '+' || line_string[0] == '-' || isdigit(line_string[0]);
}

bool is_float(std::string line_string) {
  if (is_first_char_numeric(line_string)) {
    bool has_dot = false;

    for (size_t ii = 1; ii < line_string.length(); ii++) {
      if (line_string[ii] == '.' && !has_dot) {
        has_dot = true;
      }
      else if (!isdigit(line_string[ii]) || (line_string[ii] == '.' && has_dot)) {
        return false;
      }
    }
    return true;
  }
  else {
    return false;
  }


}


