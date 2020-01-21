#pragma once

#include <string>
#include <vector>

enum Type {BOOL, INTEGER, FLOAT, STRING};

using namespace std;

class Column {
    public:

    Column(Type type) {
        col_type = type;
    }

    ~Column() {

    }

    void add(string s) {
        elements.push_back(s);
    }

    string get(size_t i) {
        return elements.at(i);
    }

    Type getType() {
        return col_type;
    }

    bool is_missing(size_t i) {
        return elements.at(i).empty();
    }

    void remove_last() {
        elements.pop_back();
    }

    private:
    
    Type col_type;
    vector<string> elements;

};