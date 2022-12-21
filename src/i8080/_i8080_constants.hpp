#include <stdint.h>
#include <string>

extern const uint8_t opcodes_lengths[256];
extern const uint8_t opcodes_cycles[256];
extern const std::string opcodes_names[256];

#define MEMORY_SIZE (0x10000)