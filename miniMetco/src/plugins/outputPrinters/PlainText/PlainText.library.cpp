#include "PlainText.h"

extern "C" {
	OutputPrinter *maker(){
		return new PlainText();
	}
}
