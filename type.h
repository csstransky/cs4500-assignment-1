#pragma once

#include <string>
#include <sstream>

using namespace std;

enum enum_type {INTEGER, FLOAT, BOOL, STRING, EMPTY};

string get_enum_string(enum_type enum_type) {
    switch (enum_type) {
        case INTEGER:
            return "INTEGER";
        case FLOAT:
            return "FLOAT";
        case BOOL:
            return "BOOL";
        case STRING:
            return "STRING";
        case EMPTY:
            return "EMPTY";
        default: {
            cout << "ERROR: UNKNOWN ENUM - " << enum_type << '\n';
            return "ERROR";
        }
    }
}

class Type {
    public:

    virtual ~Type() {}
    
    virtual string to_string() { return ""; }

    virtual bool empty() { return false; }
};

class Integer : virtual public Type {
    private:
    int val;

    public:

    Integer() {

    }

    Integer(int val) {
        this->val = val;
    }
    
    string to_string() {
        return std::to_string(val);
    }
};

class Boolean : virtual public Type {
    private:
    bool val;

    public:

    Boolean() {

    }

    Boolean(bool val) {
        this->val = val;
    }
    
    string to_string() {
        return std::to_string(val);
    }
};

class Float : virtual public Type {
    private:
    float val;

    public:

    Float() {

    }

    Float(float val) {
        this->val = val;
    }
    
    string to_string() {
        return std::to_string(val);
    }
};

class String : virtual public Type {
    private:
    string val;

    public:

    String() {

    }

    String(string val) {
        this->val = val;
    }
    
    string to_string() {
        return val;
    }
};

class Empty : public Float, public Integer, public Boolean, public String {
    public:

    Empty() {

    }

    string to_string() {
        return "";
    }

    bool empty() {
        return true;
    }
};