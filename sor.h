#pragma once

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "column.h"
#include "type.h"
#include "helper.h"

using namespace std;

class SoR {
        private:
    vector<Column<Type>*> cols;


    Type* to_type(enum_type t, string s) {
        if (t == INTEGER) {
            return new Integer(stoi(s));
        } 
        else if (t == BOOL) {
            bool b;
            istringstream("1") >> b;
            return new Boolean(b);
        } 
        else if (t == FLOAT) {
            return new Float(stof(s));
        } 
        else if (s.length() > 0) {
            return new String(s);
        } 
        else {
            return new Empty();
        }
    }

    public:

    SoR() {

    }

    SoR(char* file_path) {
        // 1. Build the data structure using "from" and "len"
        ifstream in_file;
        in_file.open(file_path);
        if (in_file.is_open()) {
            // Worst case scenario, our buffer needs to hold the entire size of the file
            // char* file_line_string = new char[file_size(file_path)];
            string file_line_string;
            char file_char;
            bool is_record = false;
            bool is_quotes = false;
            while(!in_file.eof()) {
                // in_file >> file_line_string; // buffer magic assigns file_line_string to next file line
                // pln(file_line_string);
                in_file >> noskipws >> file_char;
                switch (file_char) {
                    case ' ':
                        if (is_quotes && is_record) {
                            file_line_string += file_char;
                        }
                        break;
                    case '\n':
                        cout << "I AM A NEW LINE, HERE I AM.\n";
                        // TODO: This is how we'll know when to move to the next row in col
                        break;
                    case '\"':
                        if (is_record) {
                            is_quotes = !is_quotes;
                            file_line_string += file_char;
                        }
                        break;
                    case '<':
                        is_record = true;
                        break;
                    case '>':
                        // I put this check here to make sure we have a pair of <>, to avoid the 
                        // case of "> dude >", which in our case will completely ignore it
                        if (is_record) {
                            is_record = false;
                            cout << file_line_string << '\n';
                            file_line_string.clear();
                            // TODO: input the string into the thing
                        }
                        is_quotes = false;
                        break;
                    default:
                        if (is_record) {
                            file_line_string += file_char;
                        }
                }
            }
            in_file.close(); 
        }
        else {
            cout << "~ERROR: FILE NOT FOUND~\n";
        }
    }

    enum_type get_column_enum_type(string line_string) {
        if (is_file_boolean(line_string)) {
            return BOOL;
        } 
        else if (is_file_float(line_string)) {
            return FLOAT;
        }
        else if (is_file_int(line_string)) {
            return INTEGER;
        }
        else if (is_file_string(line_string)) {
            return STRING;
        }
        else {
            return EMPTY;
        }
    }

    vector<enum_type> convert_strings_to_column_types(vector<string> column_strings) {
        vector<enum_type> column_types;
        for (size_t ii = 0; ii < column_strings.size(); ii++) {
            enum_type col_enum = get_column_enum_type(column_strings[ii]);
            column_types.push_back(col_enum);
        }
        return column_types;
    }

    vector<enum_type> get_column_types(char* file_path) {
        vector<enum_type> column_types;
        ifstream in_file;
        in_file.open(file_path);
        if (in_file.is_open()) {
            string file_line_string;
            char file_char;
            bool is_record = false;
            bool is_quotes = false;
            size_t max_column_size = 0;
            size_t current_column_size = 0;
            vector<string> max_column_strings;
            vector<string> current_column_strings;
            // TODO, put len and from in here correctly
            while(!in_file.eof()) {
                in_file >> noskipws >> file_char;
                switch (file_char) {
                    case ' ':
                        if (is_quotes && is_record) {
                            file_line_string += file_char;
                        }
                        break;
                    case '\n':
                        cout << "=========----------==========\n";
                        if (max_column_size < current_column_size) {
                            cout << "FOUND LARGER: " << current_column_size << '\n';
                            max_column_size = current_column_size;
                            max_column_strings = current_column_strings;
                        }
                        current_column_strings.clear();
                        current_column_size = 0;
                        break;
                    case '\"':
                        if (is_record) {
                            is_quotes = !is_quotes;
                            file_line_string += file_char;
                        }
                        break;
                    case '<':
                        is_record = true;
                        break;
                    case '>':
                        // I put this check here to make sure we have a pair of <>, to avoid the 
                        // case of "> dude >", which in our case will completely ignore it
                        if (is_record) {
                            current_column_size++;
                            current_column_strings.push_back(file_line_string);

                            is_record = false;
                            cout << file_line_string << '\n';
                            file_line_string.clear(); 
                        }
                        is_quotes = false;
                        break;
                    default:
                        if (is_record) {
                            file_line_string += file_char;
                        }
                }
            }
            in_file.close(); 
            cout << "------------LARGEST COLUMN ELEMENTS------------\n";
            for(size_t ii = 0; ii < max_column_strings.size(); ii++) {
                cout << '\"' << max_column_strings.at(ii) << "\"\n";
            }
            cout << "-----------------------------------------------\n";
            column_types = convert_strings_to_column_types(max_column_strings);
            
        }
        else {
            cout << "~ERROR: FILE NOT FOUND~\n";
        }
        // TODO return thisng 
        cout << "------------LARGEST ENUM TYPES------------\n";
        for(size_t ii = 0; ii < column_types.size(); ii++) {
            print_enum(column_types.at(ii)); 
        }
        cout << "------------------------------------------\n";
        return column_types;
    }

    ~SoR() {

    }

    void add_col(Column<Type>* col) {
        cols.push_back(col);
    }

    void add_line(vector<string> line) {
        for (int i = 0; i < line.size(); i++) {
            Column<Type>* c = cols.at(i);
            string element = line.at(i);
            if (c->getType() == BOOL
                && (element.compare("0") || element.compare("1"))) {
                c->add(to_type(BOOL, element));
            } 
            else if (c->getType() == FLOAT && element.find(".")) {
                c->add(to_type(FLOAT, element));
            } 
            else if (c->getType() == INTEGER && all_of(element.begin(), element.end(), ::isdigit)) {
                c->add(to_type(INTEGER, element));
            // TODO: Fix this if bad things happen
            } 
            else if (c->getType() == STRING) {
                c->add(to_type(STRING, element));
            } 
            else {
                // We will treat elements that do not follow their types as Empty elements
                // Example: Column = <INT>, Element = <"hello">, this will be recorded as <>
                c->add(new Empty());
            }
        }
    }

    void delete_row(int start_col_index) {
        // NOTE: deletes last row
        for (int i = start_col_index; i >= 0; i--) {
            cols.at(i)->remove_last();
        }
    }

    string get(size_t x, size_t y) {
        return cols.at(x)->get(y)->to_string();
    }

    enum_type get_col_type(size_t x) {
        return cols.at(x)->getType();
    }

    bool is_missing(size_t x, size_t y) {
        return cols.at(x)->is_missing(y);
    }

};