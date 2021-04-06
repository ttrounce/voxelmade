#ifndef VHM_TYPES_H
#define VHM_TYPES_H

#include <stdint.h>
#include <cglm/types.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef double f64;
typedef float f32;

#define col(x) (x/255.0)

#endif // VHM_TYPES_H