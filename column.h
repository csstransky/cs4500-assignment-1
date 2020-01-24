#pragma once

#include <string>
#include <vector>

#include "type.h"

using namespace std;

//template <class T>
class Column {
    private:
    enum_type col_type;
    vector<Type*> elements;

    public:

    Column(enum_type type) {
        col_type = type;
    }

    ~Column() {

    }

    void add(Type* s) {
        elements.push_back(s);
    }

    size_t size() {
        return elements.size();
    }

    Type* get(size_t i) {
        return elements.at(i);
    }

    enum_type get_type() {
        return col_type;
    }

    void set_type(enum_type enum_type) {
        col_type = enum_type;
    }

    bool is_missing(size_t i) {
        return elements.at(i)->empty();
    }

    void remove_last() {
        elements.pop_back();
    }



};