#include "TwoDPack.h"

extern "C" {
	Individual *maker(){
		return new TwoDPack();
	}
}
