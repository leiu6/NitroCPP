#include <nitro/value.h>

#include <cassert>

NitroValue NitroValueNil() {
	NitroValue value;
	value.type = NITRO_TYPE_NIL;
	value.value.r = nullptr;
	return value;
}

NitroValue NitroValueChar(char c) {
	NitroValue value;
	value.type = NITRO_TYPE_CHAR;
	value.value.c = c;
	return value;
}

NitroValue NitroValueBool(bool b) {
	NitroValue value;
	value.type = NITRO_TYPE_BOOL;
	value.value.b = b;
	return value;
}

NitroValue NitroValueTrue(void) {
	return NitroValueBool(true);
}

NitroValue NitroValueFalse(void) {
	return NitroValueBool(false);
}

NitroValue NitroValueInt(int64_t i) {
	NitroValue value;
	value.type = NITRO_TYPE_INT;
	value.value.i = i;
	return value;
}

NitroValue NitroValueFloat(double f) {
	NitroValue value;
	value.type = NITRO_TYPE_FLOAT;
	value.value.f = f;
	return value;
}

NitroValue NitroValueArray(NitroType contains) {
	assert(false && "not implemented");
	return NitroValueNil();
}

NitroValue NitroValueString(const char* copy) {
	assert(false && "not implemented");
	return NitroValueNil();
}

NitroType NitroValueType(NitroValue v) {
	return v.type;
}

char NitroValueAsChar(NitroValue v) {
	assert(NitroValueIsChar(v));
	return v.value.c;
}


