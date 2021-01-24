#pragma once

#include "debug/assert.h"
#include "debug/output.h"
#include "syscall/guarded_bell.h"
#include "utils/random.h"
#include "utils/size.h"
#include "def.h"

#define USHRT_MAX 65535

static Random rand(42);

inline unsigned number() {
	return rand.number();
}
