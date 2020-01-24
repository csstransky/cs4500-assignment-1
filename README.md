# cs4500-assignment-1

## Data Structures

Fields are represented by objects. Classes were created for each type of field (integer, float, string, and boolean) and they all inherit from the same class, Type. There is also an empty class to represent missing fields.

Column is represented with a class. It contains a list of Types which represents all values in the column. The column also has an enum to represent if the column consists of strings, integers, floats, or booleans.

The final class SoR represents a list of columns. Its constructor takes in a file path and then parses the file to fill in of values in the columns.

## Program Flow

The program first parses the command line arguments and determines if the input is valid. If the file does not exist or cannot be opened, an error message is printed to stdout. If no file is given, a usage message is given. If there is garbage in the command line arguments, it is ignored.

Next the program will open the file and parse the first 500 lines of the file looking for the row with the largest number of columns. Columns are constructed in our data structure corresponding to the type of each column of the max row. If there are any missings in the max row, the column is labeled empty. The program will later try to infer the type of the column using other rows. If a column is found to contain all missings, the column is labeled boolean as it is the default type outlined in the assignment.

The program will then add values to the columns. The file is opened again and the file pointer is moved to the from byte. Next the file pointer is moved until it encounters a newline if the from command line argument is greater than 0. The program will then parse the file character by characters, converting each value into its appropiate type and adding it to the data structure. Invalid fields are considered the same as missing fields and converted to empty values. 

Example:
```
Column type: INTEGER STRING FLOAT BOOL STRING
input line :  <7 9>  <"hi"> <2.3> <hi>
conversion :   <>    <"hi>  <2.3>  <>    <>
```

Parsing will continue until the program reaches the from + len byte in which the final line is not added. (We are assuming that the last line in a file ends with a newline). 

Finally, if the command line arguments, print_col_type, print_col_idx, or is_missing_idx are given, the information is taken from the data structures and printed to stdout for the user.

