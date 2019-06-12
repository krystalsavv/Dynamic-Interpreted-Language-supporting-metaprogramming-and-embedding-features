#pragma once
#include "Object.h"

class Environment : Object {
	Environment() {
		symbols["$outer"] = nullptr;
	}

	Environment(Object * outer) {
		symbols["$outer"] = outer;
	}
};