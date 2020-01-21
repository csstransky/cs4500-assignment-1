#pragma once

#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "object.h"
#include "string.h"
#include <algorithm>

class StrList : public Object { // This means it inherits from the object class
public:
    size_t size_;
    String** list_;

    StrList() {
        size_ = 0;
        list_ = nullptr;
    }

    StrList(char** s) {
        size_ = size(s);
        list_ = new String*[size_];
        for (size_t ii=0; ii < size_; ii++) {
            list_[ii] = new String(s[ii]);
        }
    }
    
    StrList(const char* c) {
        size_ = strlen(c);
        list_ = new String*[size_];
        for (size_t ii=0; ii < size_; ii++) {
            list_[ii] = new String(c[ii]);
        }
    }

    ~StrList() {
        delete[] list_;
    }

    void push_back(String* e) {
		String** temp_list = new String*[size_ + 1];
        for (size_t ii = 0; ii < size_; ii++) {
            temp_list[ii] = list_[ii];
        }
        temp_list[size_] = e;
        delete[] list_;
        list_ = temp_list;
        size_++;
    }

    void add(size_t i, String* e) {
        size_t index = i; // I need this or I'm going to get confused
        if (index > size_) { 
            index = size_; 
        }
        String** temp_list = new String*[size_ + 1];
		// I do it this way to avoid having to do an if statement for every iteration
        size_t ii;
	    for (ii = 0; ii < index; ii++) {
            temp_list[ii] = list_[ii];
        }
        temp_list[index] = e;
        for (; ii < size_; ii++) {
            temp_list[ii + 1] = list_[ii];
        }
        delete[] list_;
        list_ = temp_list;
        size_++;
    }

    void add_all(size_t i, StrList* c) {
        size_t index = i; // I need this or I'm going to get confused
        size_t combined_size = size_ + c->size();
        if (index > size_) { 
            index = size_; 
        }
        String** temp_list = new String*[combined_size];
        size_t ii = 0; // temp index
        size_t jj = 0; // base index
        size_t kk = 0; // added index
	    for (; jj < index; jj++) {
            temp_list[ii] = list_[jj];
            ii++;
        }
        for (; kk < c->size(); kk++) {
            temp_list[ii] = c->list_[kk];
            ii++;
        } 
        for (; jj < size_; jj++) {
            temp_list[ii] = list_[jj];
            ii++;
        }
        delete[] list_;
        list_ = temp_list;
        size_ = combined_size;
    }

    void clear() {
        delete this;
        size_ = 0;
        list_ = nullptr;
    }

    bool equals(Object* other) {
        if (other == nullptr) return false;
        StrList* s = dynamic_cast<StrList*>(other);
        if (s == nullptr) return false;

        for(size_t ii = 0; ii < size_; ii++) { 
            if (!list_[ii]->equals(s->list_[ii])) {
                return false;
            }
        }
        return true;
    }

    String* get(size_t index) {
        if (size_ <= 0) {
            return nullptr;
        }
        if (index >= size_) {
            index = size_ - 1;
        }
        return list_[index];
    }

    size_t hash() {
        size_t res = 0;
        for(size_t ii = 0; ii < size_; ii++) {
            // I want position of elements to also matter in the hash, that's why I added * (ii + 1)
            res += list_[ii]->hash() * (ii + 1);
        }
        return res;
    }

    size_t index_of(Object* o) {
        size_t no_string_found_value = 1 + size(); // as per the doc: ">size()"
        if (o == nullptr) return no_string_found_value;
        String* s = dynamic_cast<String*>(o);
        if (s == nullptr) return no_string_found_value;

        for(size_t ii = 0; ii < size_; ii++) {
            if (list_[ii]->equals(s)) {
                return ii;
            }
        }
        return no_string_found_value;
    }

    String* remove(size_t i) {
        size_t index = i; // I need this or I'm going to get confused
        if (index > size_) { 
            index = size_ - 1; 
        }
        String* old_string = list_[index];
        String** temp_list = new String*[size_ - 1];
		// I do it this way to avoid having to do an if statement for every iteration
        size_t ii;
	    for (ii = 0; ii < index; ii++) {
            temp_list[ii] = list_[ii];
        }
        ii++; // This ignores the item being removed
        for (; ii < size_; ii++) {
            temp_list[ii - 1] = list_[ii];
        }
        delete[] list_;
        list_ = temp_list;
        size_--;

        // I'm assuming you want the old string being removed
        return old_string;
    }

    String* set(size_t i, String* e) {
        if (i >= size_) {
            i = size_ - 1;
        }
        String* old_string = list_[i];
        list_[i] = e;
        // I'm assuming you want the old element being set to
        return old_string;
    }

    size_t size() {
        return size_;
    }

    size_t size(char** s) {
        return sizeof(s)/sizeof(s[0]);
    }

    size_t occurences(String* string) {
        size_t occurences = 0;
        for (size_t ii = 0; ii < size_; ii++) {
            if (list_[ii]->equals(string)) {
                occurences++;
            }
        }
        return occurences;
    }

    void print() {
        Cout* c = new Cout();
        c->pln("-----------");
        for (size_t ii = 0; ii < size_; ii++) {
            list_[ii]->print();
        }
        c->pln("-----------");
        delete c;
    }
    
    void print_occurences() {
        Cout* c = new Cout();
        StrList* temp_list = new StrList();
        temp_list->add_all(0, this);
        while(temp_list->size_ > 0) {
            size_t top_occurence = 0;
            String* top_string = nullptr;
            // I really, REALLY wish I could use a hashmap here, but like the assignment wants,
            // I decided to strictly use lists. It's O(n^3) now, but with a map I could get O(n).

            // Maybe I could've also created a subclass of SortedStrList to correctly sort the words
            // in order of occurence (instead of alphabetical), but I relented as I thought that 
            // would change the W2 class requirements, which I can't for this assignment.
        
            // I first find the top occurence
            for (size_t ii = 0; ii < temp_list->size(); ii++) {
                size_t string_occurences = occurences(temp_list->get(ii));
                if (string_occurences > top_occurence) {
                    top_occurence = string_occurences;
                    top_string = temp_list->get(ii);
                }
            }
            // Print it
            c->p(top_string->val_)->p(" ")->pln(top_occurence);
            // Then I remove all occurences of that word from the list so it won't show up next time
            while(top_occurence > 0) {
                size_t string_index = temp_list->index_of(top_string);
                temp_list->remove(string_index);
                top_occurence--;
            }
        }
        delete c;
        delete temp_list;
    }
};

class SortedStrList : public StrList {
public:
    SortedStrList() {}

    SortedStrList(char* s) {
        std::sort(list_, list_ + size_);
    }

    SortedStrList(StrList* s) {
        size_ = s->size_;
        list_ = s->list_;
        // Got lazy and decided to rely on the C gods for this one
        std::sort(list_, list_ + size_);
    }

    void sorted_add(String* element) {
        push_back(element);
        // This could be much better with a proper O(logn) insertion, but I'm in a panic
        // and O(nlogn) isn't so bad
        std::sort(list_, list_ + size_);
    }
};
