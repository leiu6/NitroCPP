#pragma once

#define NITRO_DISABLE_COPY(Class) \
	Class (const Class &) = delete;\
	Class &operator=(const Class &) = delete;

#define NITRO_DISABLE_MOVE(Class) \
	Class (Class &&) = delete;\
	Class &operator=(Class &&) = delete;

#define NITRO_DISABLE_COPY_MOVE(Class) \
	NITRO_DISABLE_COPY(Class)\
	NITRO_DISABLE_MOVE(Class)

#define NITRO_DEFAULT_COPY(Class) \
	Class (const Class &) = default;\
	Class &operator=(const Class &) = default;

#define NITRO_DEFAULT_MOVE(Class) \
	Class (Class &&) = default;\
	Class &operator=(Class &&) = default;

#define NITRO_DEFAULT_COPY_MOVE(Class) \
	NITRO_DEFAULT_COPY(Class)\
	NITRO_DEFAULT_MOVE(Class)

