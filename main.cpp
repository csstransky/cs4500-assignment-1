 /*
Cristian Stransky  
CS4500 - Software Dev
Spring 2020
 */

#include <fstream>
#include "helper.h"  // Your file, with any C++ code that you need
#include "object.h"  // Your file with the CwC declaration of Object
#include "string.h"  // Your file with the String class
#include "list.h"    // Your file with the two list classes

// ===================================================================================================
// TODO: I use this for cheap debugging crap, make sure to actually get rid of this later
void p(char s) { std::cout << s ; }
void p(char* s) { std::cout << s ; }
void p(const char* s) { std::cout << s ; }
void p(size_t s) { std::cout << s ; }
void pln() { std::cout << '\n' ; }
void pln(char s) { std::cout << s << "\n" ; }
void pln(char* s) { std::cout << s << "\n" ; }
void pln(const char* s) { std::cout << s << "\n" ; }
void pln(size_t s) { std::cout << s << "\n" ; }
void pln(bool s) { std::cout << s << "\n" ; }
void pln(std::string s) { std::cout << s << "\n" ; }
// ====================================================================================================

size_t file_size(char* path) {
    FILE* file = fopen(path, "rb");
    fseek(file, 0L, SEEK_END); // goes to the end of the file
    size_t file_size = ftell(file); // gets the size
    fclose(file); 
    return file_size;
}

bool is_same_string(const char* string1, const char* string2) {
    return strcmp(string1, string2) == 0;
}

bool is_valid_flag(const char* flag, char* flag_string, int argh, char** argv, size_t ii, 
                    size_t args_next) {
    return !flag_string && is_same_string(argv[ii], flag) && ii + args_next < argh;
}

bool is_valid_flag(const char* flag, size_t flag_size_t, int argh, char** argv, size_t ii, 
                    size_t args_next) {
    return flag_size_t == SIZE_MAX && is_same_string(argv[ii], flag) && ii + args_next < argh;
}

bool is_valid_flag(const char* flag, size_t flag_size_t, size_t default_value, int argh, 
                    char** argv, size_t ii, size_t args_next) {
    return flag_size_t == default_value && is_same_string(argv[ii], flag) && ii + args_next < argh;
}

size_t get_size_t_from_next_arg(const char* next_arg) {
    // Since our arguments are unsigned ints, negative numbers aren't allowed, but if we don't 
    // handle them, it underflows into a super large number, so I handle it correctly here
    char first_letter_of_next_arg = next_arg[0];
    if (first_letter_of_next_arg == '-') {
        next_arg = "0";
    }
    // Since there's no real C++ way to convert string to size_t, I'm hoping converting to a 
    // long, long int will do the job as it's much larger than a size_t (this is probably overkill)
    return atoll(next_arg);
}

void print_console(char* file_path, size_t from, size_t len, size_t print_col_type_index, 
                    size_t print_col_index_x, size_t print_col_index_y,
                    size_t is_missing_index_x, size_t is_missing_index_y) {
    // Correctly set up all values
    if (file_path) {
        // 1. Build the data structure using "from" and "len"
        std::ifstream in_file;
        in_file.open(file_path);
        if (in_file.is_open()) {
            // Worst case scenario, our buffer needs to hold the entire size of the file
            // char* file_line_string = new char[file_size(file_path)];
            std::string file_line_string;
            char file_char;
            bool is_record = false;
            bool is_quotes = false;
            while(!in_file.eof()) {
                // in_file >> file_line_string; // buffer magic assigns file_line_string to next file line
                // pln(file_line_string);
                in_file >> std::noskipws >> file_char;
                switch (file_char) {
                    case ' ':
                        if (is_quotes && is_record) {
                            file_line_string += file_char;
                        }
                        break;
                    case '\n':
                        pln("I AM A NEW LINE, HERE I AM.");
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
                            pln(file_line_string);
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
            Cout* c = new Cout();
            c->pln("~ERROR: FILE NOT FOUND~");
            delete c;
        }

        // 2. print column type option
        // 3. print column index element
        // 4. print if element is missing
        if (print_col_type_index != SIZE_MAX) {
            // TODO: use the column function to print this
        }
        else if (print_col_index_x != SIZE_MAX && print_col_index_y != SIZE_MAX) {
            // TODO: use the column function to print this
        }
        else if (is_missing_index_x != SIZE_MAX && is_missing_index_y != SIZE_MAX) {
            // TODO: use the column function to print this
        }
    }
    else {
        // TODO: maybe a message here that you didn't input anything?
    }
}

int main(int argh, char** argv) {
    char* file_path = nullptr;
    size_t from = 0;
    size_t len = SIZE_MAX;
    // I am making a very, VERY large assumption that a file will not have more than or equal to the
    // number of elements of SIZE_MAX. SIZE_MAX is my -1, or a way to check if the user used a flag.
    size_t print_col_type_index = SIZE_MAX;
    size_t print_col_index_x = SIZE_MAX;
    size_t print_col_index_y = SIZE_MAX;
    size_t is_missing_index_x = SIZE_MAX;
    size_t is_missing_index_y = SIZE_MAX;
    // TODO: Turn the entire thing into a tuple? It might be cool, it's how I usually do it
    // I want to ignore first argument ("./a.out") as it's the command itself, so I start at 1
    for (int ii = 1; ii < argh; ii++) {
        // The argument right next to a flag is considered its value, so it's skipped with ii++
        // This means that "-len -f" and "-f -len" will be considered valid, but give strange output
        // NOTE: Will only use the first instance of a flag. "-from 5 -from 6" will make from = 5.
        if (is_valid_flag("-f", file_path, argh, argv, ii, 1)) {
            file_path = argv[ii + 1];
            ii++;
        } 
        else if (is_valid_flag("-from", from, 0, argh, argv, ii, 1)) {
            from = get_size_t_from_next_arg(argv[ii + 1]);
            ii++;
        }
        else if (is_valid_flag("-len", len, argh, argv, ii, 1)) {
            len = get_size_t_from_next_arg(argv[ii + 1]);
            ii++;
        }
        else if (is_valid_flag("-print_col_type", print_col_type_index, argh, argv, ii, 1)) {
            print_col_type_index = get_size_t_from_next_arg(argv[ii + 1]);
            ii++;
        }
        else if (is_valid_flag("-print_col_idx", print_col_index_x, argh, argv, ii, 2)) {
            print_col_index_x = get_size_t_from_next_arg(argv[ii + 1]);
            print_col_index_y = get_size_t_from_next_arg(argv[ii + 2]);
            ii += 2;
        } 
        else if (is_valid_flag("-is_missing_idx", is_missing_index_x, argh, argv, ii, 2)) {
            is_missing_index_x = get_size_t_from_next_arg(argv[ii + 1]);
            is_missing_index_y = get_size_t_from_next_arg(argv[ii + 2]);
            ii += 2;
        }
        else {
            // Currently, we're deciding to ignore everything that isn't one of our argument flags.
        }
    }
    // TODO: Make sure to get rid of all this later =====================================================
    Cout* c = new Cout();
    if (file_path) { c->p("-f: \"")->p(file_path)->pln("\""); }
    if (from != 0) { c->p("-from: \"")->p(from)->pln("\""); }
    if (len != SIZE_MAX) { c->p("-len: \"")->p(len)->pln("\""); }
    if (print_col_type_index != SIZE_MAX) { c->p("-print_col_type: \"")->p(print_col_type_index)->pln("\""); }
    if (print_col_index_x != SIZE_MAX && print_col_index_y != SIZE_MAX) 
        c->p("-print_col_idx: \"")->p(print_col_index_x)->p("\", \"")->p(print_col_index_y)->pln("\"");
    if (is_missing_index_x != SIZE_MAX && is_missing_index_y != SIZE_MAX) 
        c->p("-is_missing_idx: \"")->p(is_missing_index_x)->p("\", \"")->p(is_missing_index_y)->pln("\"");
    // ==================================================================================================

    print_console(file_path, from, len, print_col_type_index, print_col_index_x, print_col_index_y, is_missing_index_x, is_missing_index_y);
}