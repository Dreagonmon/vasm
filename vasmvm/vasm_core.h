#pragma once
#include <stdint.h>
#include "vasm_ext.h"

#define KEY_MASK_A     0b00000001
#define KEY_MASK_UP    0b00000010
#define KEY_MASK_DOWN  0b00000100
#define KEY_MASK_LEFT  0b00001000
#define KEY_MASK_RIGHT 0b00010000

#define EVENT_NOPE     0b00
#define EVENT_KEYDOWN  0b01
#define EVENT_KEYUP    0b10

#define VASM_TILE_GRID_WIDTH 16
#define VASM_TILE_GRID_HEIGHT 8

#define REGISTER_COUNT 128
#define CALL_STACK_COUNT 64
#define TILE_TABLE_COUNT 64
#define TILE_TABLE_SIZE (TILE_TABLE_COUNT * 8)
#define WORKING_BUFFER_SIZE 128
#define TILE_POS_LIMIT (VASM_TILE_GRID_WIDTH * VASM_TILE_GRID_HEIGHT)

#define VASM_RETURN_NORMAL 0
#define VASM_RETURN_EXIT 1

#define NOPE 0x00
#define EXIT 0x01
#define WKEY 0x02
#define GKEY 0x03
#define TICK 0x04
#define SAVE 0x05
#define LOAD 0x06
#define TILE 0x07
#define STOR 0x08
#define MOVE 0x09
#define ADDI 0x0A
#define SUBI 0x0B
#define MULI 0x0C
#define DIVI 0x0D
#define MODI 0x0E
#define INVI 0x0F
#define JUMP 0x10
#define JPEZ 0x11
#define JPGZ 0x12
#define JPLZ 0x13
#define CALL 0x14
#define RETN 0x15
#define DTIL 0x16
#define DNUM 0x17
#define DTXT 0x18
#define DSEL 0x19
#define RAND 0x1A

void vasm_init(
    uint8_t register_buffer[REGISTER_COUNT],
    uint16_t call_stack_buffer[CALL_STACK_COUNT],
    uint8_t tile_table_buffer[TILE_TABLE_SIZE],
    uint8_t working_buffer[WORKING_BUFFER_SIZE]
);
uint8_t vasm_execute_once(void);
uint16_t vasm_get_program_counter(void);
uint16_t vasm_get_program_counter_at_instruction_start(void);
