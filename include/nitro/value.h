#ifndef _NITRO_VALUE_H
#define _NITRO_VALUE_H

#include "macros.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* Defines the C type for the values of variables in Nitro. This header is 
* made to be compatible with both C and C++
*/

typedef enum {
	NITRO_TYPE_NIL		= 0,
	NITRO_TYPE_CHAR		= 1,
	NITRO_TYPE_BOOL		= 2,
	NITRO_TYPE_INT		= 3,
	NITRO_TYPE_FLOAT	= 4,
	NITRO_TYPE_ARRAY	= 5,
	NITRO_TYPE_STRING	= 6,
	NITRO_TYPE_FUNCTION	= 7
} NitroType;

typedef struct {
	NitroType type;

	union {
		char c;
		bool b;
		int64_t i;
		double f;
		void* r;
	} value;
} NitroValue;

NitroValue NitroValueNil(void);
NitroValue NitroValueChar(char c);
NitroValue NitroValueBool(bool b);
NitroValue NitroValueTrue(void);
NitroValue NitroValueFalse(void);
NitroValue NitroValueInt(int64_t i);
NitroValue NitroValueFloat(double f);
NitroValue NitroValueArray(NitroType contains);
NitroValue NitroValueString(const char* copy);
// TODO: NitroValueFunction

NitroType NitroValueType(NitroValue v);

static inline bool
NitroValueIsNil(NitroValue v) {
	return v.type == NITRO_TYPE_NIL;
}

static inline bool
NitroValueIsChar(NitroValue v) {
	return v.type == NITRO_TYPE_CHAR;
}

static inline bool
NitroValueIsBool(NitroValue v) {
	return v.type == NITRO_TYPE_BOOL;
}

static inline bool
NitroValueIsInt(NitroValue v) {
	return v.type == NITRO_TYPE_INT;
}

static inline bool
NitroValueIsFloat(NitroValue v) {
	return v.type == NITRO_TYPE_FLOAT;
}

static inline bool
NitroValueIsArray(NitroValue v) {
	return v.type == NITRO_TYPE_ARRAY;
}

bool NitroValueIsArrayOf(NitroValue v, NitroType t);

static inline bool
NitroValueIsString(NitroValue v) {
	return v.type == NITRO_TYPE_STRING;
}


char NitroValueAsChar(NitroValue v);
bool NitroValueAsBool(NitroValue v);
int64_t NitroValueAsInt(NitroValue v);
double NitroValueAsFloat(NitroValue v);
NitroValue NitroValueArrayAt(NitroValue arr, int64_t index);
char NitroValueStringGetChar(NitroValue v, int64_t index);
char* NitroValueAsString(NitroValue v);
NitroValue NitroValueFunctionCall(NitroValue func, size_t argc, NitroValue* argv);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _NITRO_VALUE_H
