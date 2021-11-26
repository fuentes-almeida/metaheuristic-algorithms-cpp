#include "NoOp.h"

extern "C" {
	LocalSearch *maker(){
		return new NoOp();
	}
}
