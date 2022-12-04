#include <stdint.h>

extern const uint8_t opcodes_lengths[256];
extern const uint8_t opcodes_cycles[256];
extern const char *opcodes_names[256];

#define MEMORY_SIZE (0x10000)