#pragma once

#include <stdio.h>
#include <string.h>
#include "helper.h"

class String : public Object { 
public:
    char* val_; // owned
    size_t size_;

    String() {
        size_ = 0;
        val_ = nullptr;
    }

    String(char c) {
        size_ = 1;
        val_ = new char[size_];
        val_[0] = c;
        val_[size_] = '\0';
    }

    String(char* c) {
        size_ = strlen(c);
        val_ = new char[size_];
        for (size_t i=0; i < size_; i++) {
            val_[i] = c[i];
        }
        val_[size_] = '\0';
    }
    
    String(const char* c) {
        size_ = strlen(c);
        val_ = new char[size_];
        for (size_t i=0; i < size_; i++) {
            val_[i] = c[i];
        }
        val_[size_] = '\0';
    }

    ~String() {
        delete[] val_;
    }

    size_t hash() {
        size_t res = 0;
        for(size_t ii = 0; ii < size_; ii++) {
            res += val_[ii];
        }
        return res;
    }

    bool equals(Object* other) {
        if (other == nullptr) return false;
        String* s = dynamic_cast<String*>(other);
        if (s == nullptr) return false;

        return strcmp(val_, s->val_) == 0;
    }

    String* concat(String* s) {
        char* temp_string = new char[size_ + s->size_];
        strcpy(temp_string, val_);
        strcat(temp_string, s->val_);
        return new String(temp_string);
    }
    
    void concat_char(char c) {
        char* temp_string = new char[size_ + 1];
        for (size_t ii = 0; ii < size_; ii++) {
            temp_string[ii] = val_[ii];
        }
        temp_string[size_] = c;
        temp_string[size_ + 1] = '\0';
        delete[] val_;
        val_ = temp_string;
        size_++;
    }

    size_t size() {
        return size_;
    }

    void print() {
        Cout * c = new Cout();
        c->pln(val_);
        delete c;
    }
};
