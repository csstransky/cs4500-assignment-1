#pragma once

#include <vector>
#include <algorithm>

#include "column.h"
#include "type.h"

using namespace std;

class SoR {
        private:
    vector<Column<Type>*> cols;

    Type* to_type(enum_type t, string s) {
        if (t == INTEGER) {
            return new Integer(stoi(s));
        } else if (t == BOOLEAN) {
            bool b;
            istringstream("1") >> b;
            return new Boolean(b);
        } else if (t == FLOAT) {
            return new Float(stof(s));
        } else if (s.length > 0) {
            return new String(s);
        } else {
            return new Empty();
        }
    }

    public:

    SoR() {

    }

    ~SoR() {

    }

    void add_col(Column<Type>* col) {
        cols.push_back(col);
    }

    void add_line(vector<string> line) {
        for (int i = 0; i < line.size; i++) {
            Column<Type>* c = cols.at(i);
            string element = line.at(i);
            if (c->getType() == BOOLEAN
                && (element.compare("0") || element.compare("1"))) {
                c->add(to_type(BOOLEAN, element));
            } else if (c->getType == FLOAT && element.find(".")) {
                c->add(to_type(FLOAT, element));
            } else if (c->getType == INTEGER && all_of(element.begin(), element.end(), ::isdigit)) {
                c->add(to_type(INTEGER, element));
            } else if (c->getType == STRING && all_of(element.begin(), element.end(), ::isalpha)) { // isalpha is not right
                c->add(to_type(STRING, element));
            } else {
                // bad element, delete row
                delete_row(i - 1);
            }
        }
    }

    void delete_row(int start_col_index) {
        for (int i = start_col_index; i >= 0; i--) {
            cols.at(i)->remove_last();
        }
    }

    string get(size_t x, size_t y) {
        return cols.at(y)->get(x).to_string();
    }

    enum_type get_col_type(size_t y) {
        return cols.at(y)->getType();
    }

    bool is_missing(size_t x, size_t y) {
        return cols.at(y)->is_missing(x);
    }

};