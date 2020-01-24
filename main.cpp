 /*
Cristian Stransky & Kaylin Devchand
CS4500 - Software Dev
Spring 2020
 */

#include <fstream>
#include <string.h>
#include "helper.h"  // Your file, with any C++ code that you need
#include "sor.h"

size_t get_file_size(char* path) {
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
        size_t file_size = get_file_size(file_path);
        if (from > file_size) {
            from = file_size;
        }

        // TODO: Make this
        SoR* sor_struct = new SoR(file_path, from, len);
        sor_struct->print();
        // sor_struct->get_column_types(file_path, from, len);

        // 2. print column type option
        // 3. print column index element
        // 4. print if element is missing
        if (print_col_type_index != SIZE_MAX) {
            if (print_col_type_index >= sor_struct->get_column_size())  {
                print_col_type_index = sor_struct->get_column_size() - 1;
            }
            enum_type column_type = sor_struct->get_col_type(print_col_type_index);
            cout << "Column Type at " << print_col_type_index << ": " << get_enum_string(column_type) << '\n';
        }
        if (print_col_index_x != SIZE_MAX && print_col_index_y != SIZE_MAX) {
            // TODO: use the column function to print this
            string element = sor_struct->get(print_col_index_x, print_col_index_y);
            cout << "Element at " << print_col_index_x << ", " << print_col_index_y << ": " << element << '\n';
        }
        if (is_missing_index_x != SIZE_MAX && is_missing_index_y != SIZE_MAX) {
            // TODO: use the column function to print this
            bool is_missing = sor_struct->is_missing(is_missing_index_x, is_missing_index_y);
            cout << "Is element missing at " << is_missing_index_x << ", " << is_missing_index_y << ": " << is_missing << '\n';
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


    cout << "TESTING!!!\n";
    cout << is_file_boolean("0") << '\n';
    cout << is_file_boolean("1") << '\n';
    cout << is_file_boolean("2") << '\n';
    cout << '\n';
    cout << is_file_float("hi") << '\n';
    cout << is_file_float("123") << '\n';
    cout << is_file_float("12.2") << '\n';
    cout << is_file_float("1.1.") << '\n';
    cout << '\n';
    cout << is_file_int("123") << '\n';
    cout << is_file_int("1.2") << '\n';
    cout << is_file_int("1 ") << '\n';
    cout << '\n';
    cout << is_file_string(" ") << '\n';
    cout << is_file_string("") << '\n';
    cout << is_file_string(" dude") << '\n';
    cout << is_file_string("dude\" de lol \"") << '\n';


    print_console(file_path, from, len, print_col_type_index, print_col_index_x, print_col_index_y, is_missing_index_x, is_missing_index_y);
}