#pragma once
// lang::CwC

#include <stdlib.h>
#include "helper.h"

class Object {
public:
	size_t hash_;

	Object() { 
		hash_ = 0; 
	}

	virtual ~Object() {}

	size_t get_hash() {
		if (hash_ == 0)
			hash_ = hash(); ///Compute the hash
		return hash_;
	}

	virtual size_t hash() {
		return reinterpret_cast<size_t>(this);
	}

	virtual bool equals(Object* other) {
		return this == other;
	}

	virtual void print() {
		Cout* c = new Cout();
        c->pln(hash_);
        delete c;
	}
};
