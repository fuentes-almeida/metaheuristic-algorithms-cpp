#include "MonoGA2.h"

extern "C" {
	EA *maker(){
		return new MonoGA2();
	}
}
