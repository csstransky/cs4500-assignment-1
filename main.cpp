 /*
Cristian Stransky & Kaylin Devchand
CS4500 - Software Dev
Spring 2020
 */

#include <fstream>
#include <string.h>
#include "helper.h"  // Your file, with any C++ code that you need
#include "sor.h"

using namespace std;

size_t get_file_size(char* path) {
    FILE* file = fopen(path, "rb");
    size_t file_size = 0;
    if (file) {
        fseek(file, 0L, SEEK_END); // goes to the end of the file
        file_size = ftell(file); // gets the size
        fclose(file); 
    }
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

bool is_file_exists(char* file_path) {
    ifstream in_file;
    in_file.open(file_path);
    bool is_file_exists = in_file.is_open();
    in_file.close();
    return is_file_exists;
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

void print_column_type(SoR* sor_struct, size_t print_col_type_index) {
    print_col_type_index = min(print_col_type_index, sor_struct->get_column_size() - 1);
    enum_type column_type = sor_struct->get_col_type(print_col_type_index);
    cout << "Column Type at " << print_col_type_index << ": " << get_enum_string(column_type) << '\n';
}

void print_column_element(SoR* sor_struct, size_t print_col_index_x, size_t print_col_index_y) {
    print_col_index_x = min(print_col_index_x, sor_struct->get_column_size() - 1);
    print_col_index_y = min(print_col_index_y, sor_struct->get_row_size() - 1);
    string element = sor_struct->get(print_col_index_x, print_col_index_y);
    cout << "Element at " << print_col_index_x << ", " << print_col_index_y << ": " << element << '\n';
}

void print_is_missing_element(SoR* sor_struct, size_t is_missing_index_x, size_t is_missing_index_y) {
    is_missing_index_x = min(is_missing_index_x, sor_struct->get_column_size() - 1);
    is_missing_index_y = min(is_missing_index_y, sor_struct->get_row_size() - 1);
    bool is_missing = sor_struct->is_missing(is_missing_index_x, is_missing_index_y);
    cout << "Is element missing at " << is_missing_index_x << ", " << is_missing_index_y << ": " << is_missing << '\n';
}

void print_console(char* file_path, size_t from, size_t len, size_t print_col_type_index, 
                    size_t print_col_index_x, size_t print_col_index_y,
                    size_t is_missing_index_x, size_t is_missing_index_y) {
    // I first check if there even is a valid "-f" field
    if (file_path) {
        if (!is_file_exists(file_path)) {
            cout << "~ERROR: FILE NOT FOUND~\n";
            return;
        }

        size_t file_size = get_file_size(file_path);
        from = min(from, file_size);
        len = min(len, file_size);

        SoR* sor_struct = new SoR(file_path, from, len);

        if (print_col_type_index != SIZE_MAX) {
            print_column_type(sor_struct, print_col_type_index);
        }
        if (print_col_index_x != SIZE_MAX && print_col_index_y != SIZE_MAX) {
            print_column_element(sor_struct, print_col_index_x, print_col_index_y);
        }
        if (is_missing_index_x != SIZE_MAX && is_missing_index_y != SIZE_MAX) {
            print_is_missing_element(sor_struct, is_missing_index_x, is_missing_index_y);
        }
        delete sor_struct;
    }
    else {
        cout << "No File Entered. Enter a file with: \n"
            << "./a.out -f <file_name>\n";
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
    print_console(file_path, from, len, print_col_type_index, print_col_index_x, print_col_index_y, 
                    is_missing_index_x, is_missing_index_y);
}