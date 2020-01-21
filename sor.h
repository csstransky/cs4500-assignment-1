#pragma once

#include <vector>

#include "column.h"

using namespace std;

class SoR {
    public:

    SoR() {

    }

    ~SoR() {

    }

    void add_col(Column* col) {
        cols.push_back(col);
    }

    void delete_row(int start_col_index) {
        for (int i = start_col_index; i >= 0; i--) {
            cols.at(i)->remove_last();
        }
    }

    string get(size_t x, size_t y) {
        return cols.at(y)->get(x);
    }

    Type get_col_type(size_t y) {
        return cols.at(y)->getType();
    }

    bool is_missing(size_t x, size_t y) {
        return cols.at(y)->is_missing(x);
    }

    private:
    vector<Column*> cols;

};