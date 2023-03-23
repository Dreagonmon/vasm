#pragma once
#include <stdint.h>

#define ENABLE_SAFE_CHECK

#ifdef ENABLE_SAFE_CHECK
    #ifndef VASM_PANIC
        #include <stdio.h>
        #include <stdlib.h>
        #define VASM_PANIC(info) do { printf(info); exit (1); } while (0)
    #endif
#endif

// required interface
uint8_t vasm_code_read(uint16_t offset);
uint8_t vasm_get_tick(void);
uint8_t vasm_get_key(void);
void vasm_draw_tile(uint8_t pos, const uint8_t mono_vlsb_data[8]);

// optional interface
void vasm_save(const uint8_t data[16]);
void vasm_load(uint8_t data[16]);
uint8_t vasm_get_rand(uint8_t seed);
uint8_t vasm_wait_key(uint8_t last_key);
void vasm_nope_callback(void);

// port specific interface
void init(void);
void loop(void);
