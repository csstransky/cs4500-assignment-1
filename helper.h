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

bool is_file_boolean(std::string line_string) {
  return line_string.compare("0") == 0 || line_string.compare("1") == 0;
}

bool is_first_char_numeric(std::string line_string) {
  // Ints and floats can start with a + or - character, like "-2" or "+30"
  return line_string.length() > 0 && (line_string[0] == '+' || line_string[0] == '-' || isdigit(line_string[0]));
}

bool is_file_float(std::string line_string) {
  if (!is_first_char_numeric(line_string)) 
    return false;

  // We only consider a float to have ONE dot, ex: "12.2" = True, "1.1.1" = False, "13" = False
  bool has_dot = false;
  for (size_t ii = 1; ii < line_string.length(); ii++) {
    if (line_string[ii] == '.' && !has_dot) {
      has_dot = true;
    }
    else if (!isdigit(line_string[ii]) || (line_string[ii] == '.' && has_dot)) {
      return false;
    }
  }
  return has_dot;
}

bool is_file_int(std::string line_string) {
  if (!is_first_char_numeric(line_string)) 
    return false;

  for (size_t ii = 1; ii < line_string.length(); ii++) {
    if (!isdigit(line_string[ii])) {
      return false;
    }
  }
  return true;
}

bool is_file_string(std::string line_string) {
  // A file_string can NOT have spaces, UNLESS they are spaces within quotes, ex: '  hu ' = False, '"hi there"' = True
  // A string with nothing is not considered a string either, ex: '' = False, '""' = True
  if (line_string.length() <= 0) 
    return false;

  bool is_quotes = false;
  for (size_t ii = 0; ii < line_string.length(); ii++) {
    if (line_string[ii] == ' ' && !is_quotes) {
      return false;
    }
    else if (line_string[ii] == '\"') {
      is_quotes = !is_quotes;
    }
  }
  return true;
}


