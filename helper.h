#pragma once
// lang::Cpp

#include <iostream>

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


