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

// The assignment wants the default behavior to grab only words of length greater than 4, so 5 it is
size_t DEFAULT_PRINT_LENGTH = 5;

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


bool is_same_string(const char* string1, const char* string2) {
    return strcmp(string1, string2) == 0;
}

size_t file_size(char* path) {
    FILE* file = fopen(path, "rb");
    fseek(file, 0L, SEEK_END); // goes to the end of the file
    size_t file_size = ftell(file); // gets the size
    fclose(file); 
    return file_size;
}

void add_string_to_word_list(size_t print_length, StrList* word_list, String* string) {
    // NOTE: This is mainly used as a helper function
    // We will either use the string for the word list, or delete it
    if (string->size() >= print_length) {
        word_list->push_back(string);
    }
    else {
        delete string;
    }
}

void add_file_line_to_word_list(size_t print_length, StrList* word_list, char* file_line_string) {
    // NOTE: This is mainly used as a helper function
    String* temp_string = new String();
    for(size_t ii = 0; ii < strlen(file_line_string); ii++) {
        if (isalpha(file_line_string[ii])) {
            char lower_case_file_char = tolower(file_line_string[ii]);
            temp_string->concat_char(lower_case_file_char);
        } 
        // It's possible to get an output with words seperated by symbols, so this 'if' will
        // correctly seperate them, ex. "'tis!--whose": We want that to be "tis", "whose".
        else if (temp_string->size() > 0) {
            add_string_to_word_list(print_length, word_list, temp_string);
            temp_string = new String();
        }
    }
    add_string_to_word_list(print_length, word_list, temp_string);
}

void print_file(size_t print_length, char* file_path, StrList* word_list) {
    // We will only print from a file if the user actually specifies a file with the "-f" flag.
    if (file_path) {
        std::ifstream in_file;
        in_file.open(file_path);
        if (in_file.is_open()) {
            // Worst case scenario, our buffer needs to hold the entire size of the file
            char* file_line_string = new char[file_size(file_path)];
            while(!in_file.eof()) {
                in_file >> file_line_string; // buffer magic assigns file_line_string to next file line
                add_file_line_to_word_list(print_length, word_list, file_line_string);
            }
            in_file.close(); 
        }
        else {
            pln("~ERROR: FILE NOT FOUND~");
        }
    } 
    word_list->print_occurences();
}

bool is_valid_flag(const char* flag, char* flag_string, int argh, char** argv, size_t ii, size_t args_next) {
    return !flag_string && is_same_string(argv[ii], flag) && ii + args_next < argh;
}

bool is_valid_flag(const char* flag, size_t flag_size_t, int argh, char** argv, size_t ii, size_t args_next) {
    return flag_size_t == SIZE_MAX && is_same_string(argv[ii], flag) && ii + args_next < argh;
}

bool is_valid_flag(const char* flag, size_t flag_size_t, size_t default_value, int argh, char** argv, size_t ii, size_t args_next) {
    return flag_size_t == default_value && is_same_string(argv[ii], flag) && ii + args_next < argh;
}

size_t get_size_t_from_next_arg(const char* next_arg) {
    // Since checks for print_length rely that they're greater than 0 (we need to read at least
    // 1 character in a word), anything input that's 0 or below will be set to 1 to avoid seg faults
    char first_letter_of_next_arg = next_arg[0];
    if (first_letter_of_next_arg == '-') {
        next_arg = "0";
    }
    return atoll(next_arg);
}

void remove_print_length_strings(size_t print_length, StrList* word_list) {
    for(size_t ii = 0; ii < word_list->size(); ii++) {
        if(word_list->get(ii)->size_ < print_length) {
            String* removed_string = word_list->remove(ii);
            delete removed_string;
            // I decrement ii to keep the index with the new array after having an element removed 
            // This mainly stops "skips" from happening. I could go reverse as well.
            ii--;
        }
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
    // I want to ignore first argument ("./a.out") as it's the command itself, so I start at 1
    for (int ii = 1; ii < argh; ii++) {
        // TODO: I can't find a way to use switch cases here with char*, try again in the future

        // The argument right next to a flag is considered its value, so it's skipped with ii++
        // This means that "-i -f" and "-f -i" will be considered valid, but give strange output
        // NOTE: Will only use the first instance of a flag. "-i 5 -i 6" will make print_length = 5.
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
            // TODO: Do something, I don't know
        }
    }
    Cout* c = new Cout();
    if (file_path) { c->p("-f: \"")->p(file_path)->pln("\""); }
    if (from != 0) { c->p("-from: \"")->p(from)->pln("\""); }
    if (len != SIZE_MAX) { c->p("-len: \"")->p(len)->pln("\""); }
    if (print_col_type_index != SIZE_MAX) { c->p("-print_col_type: \"")->p(print_col_type_index)->pln("\""); }
    if (print_col_index_x != SIZE_MAX && print_col_index_y != SIZE_MAX) 
        c->p("-print_col_idx: \"")->p(print_col_index_x)->p("\", \"")->p(print_col_index_y)->pln("\"");
    if (is_missing_index_x != SIZE_MAX && is_missing_index_y != SIZE_MAX) 
        c->p("-is_missing_idx: \"")->p(is_missing_index_x)->p("\", \"")->p(is_missing_index_y)->pln("\"");

    // // Now that we have the print_length for certain, we have to go and remove anything that isn't long enough
    // // This is mainly for stuff like "./a.out -f doc.txt here is some texty", which should only include "texty"
    // if (word_list->size() > 0) {
    //     remove_print_length_strings(print_length, word_list);
    // }
    // print_file(print_length, file_path, word_list);
}