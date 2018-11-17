#include "observable.h"
#include "observer.h"

void observable::subscribe(observer& o) {
	sig.connect(std::bind(&observer::update, &o, std::placeholders::_1));
}

void observable::notify(double s) { sig(s); }
