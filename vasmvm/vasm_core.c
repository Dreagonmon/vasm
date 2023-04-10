#include <stddef.h>
#include "vasm_core.h"

__attribute__((weak)) void vasm_save(const uint8_t data[16]) {
    // empty
}
__attribute__((weak)) void vasm_load(uint8_t data[16]) {
    // empty
}
__attribute__((weak)) uint8_t vasm_wait_key(uint8_t last_key) {
    uint8_t key = last_key;
    while ((key = vasm_get_key()) == last_key) ; // busy loop
    return key;
}
__attribute__((weak)) void vasm_nope_callback() {
    // empty
}
__attribute__((weak)) uint8_t vasm_get_rand(uint8_t seed) {
    return (1103515245 * seed + 12345) % 0x100;
}

#define VASM_SAVE_REGISTRT_OFFSET (REGISTER_COUNT - 16)
const uint8_t VASM_FONT[] = {
    0x00, 0x00, 0x00, 0x4F, 0x4F, 0x00, 0x00, 0x00, // (!)
    0x00, 0x07, 0x07, 0x00, 0x00, 0x07, 0x07, 0x00, // (")
    0x14, 0x7F, 0x7F, 0x14, 0x14, 0x7F, 0x7F, 0x14, // (#)
    0x00, 0x24, 0x2E, 0x6B, 0x6B, 0x3A, 0x12, 0x00, // ($)
    0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00, // (%)
    0x00, 0x32, 0x7F, 0x4D, 0x4D, 0x77, 0x72, 0x50, // (&)
    0x00, 0x00, 0x00, 0x04, 0x06, 0x03, 0x01, 0x00, // (')
    0x00, 0x00, 0x1C, 0x3E, 0x63, 0x41, 0x00, 0x00, // (()
    0x00, 0x00, 0x41, 0x63, 0x3E, 0x1C, 0x00, 0x00, // ())
    0x08, 0x2A, 0x3E, 0x1C, 0x1C, 0x3E, 0x2A, 0x08, // (*)
    0x00, 0x08, 0x08, 0x3E, 0x3E, 0x08, 0x08, 0x00, // (+)
    0x00, 0x00, 0x80, 0xE0, 0x60, 0x00, 0x00, 0x00, // (,)
    0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // (-)
    0x00, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x00, // (.)
    0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, // (/)
    0x00, 0x3E, 0x7F, 0x49, 0x45, 0x7F, 0x3E, 0x00, // (0)
    0x00, 0x40, 0x44, 0x7F, 0x7F, 0x40, 0x40, 0x00, // (1)
    0x00, 0x62, 0x73, 0x51, 0x49, 0x4F, 0x46, 0x00, // (2)
    0x00, 0x22, 0x63, 0x49, 0x49, 0x7F, 0x36, 0x00, // (3)
    0x00, 0x18, 0x18, 0x14, 0x16, 0x7F, 0x7F, 0x10, // (4)
    0x00, 0x27, 0x67, 0x45, 0x45, 0x7D, 0x39, 0x00, // (5)
    0x00, 0x3E, 0x7F, 0x49, 0x49, 0x7B, 0x32, 0x00, // (6)
    0x00, 0x03, 0x03, 0x79, 0x7D, 0x07, 0x03, 0x00, // (7)
    0x00, 0x36, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, // (8)
    0x00, 0x26, 0x6F, 0x49, 0x49, 0x7F, 0x3E, 0x00, // (9)
    0x00, 0x00, 0x00, 0x24, 0x24, 0x00, 0x00, 0x00, // (:)
    0x00, 0x00, 0x80, 0xE4, 0x64, 0x00, 0x00, 0x00, // (;)
    0x00, 0x08, 0x1C, 0x36, 0x63, 0x41, 0x41, 0x00, // (<)
    0x00, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x00, // (=)
    0x00, 0x41, 0x41, 0x63, 0x36, 0x1C, 0x08, 0x00, // (>)
    0x00, 0x02, 0x03, 0x51, 0x59, 0x0F, 0x06, 0x00, // (?)
    0x00, 0x3E, 0x7F, 0x41, 0x4D, 0x4F, 0x2E, 0x00, // (@)
    0x00, 0x7C, 0x7E, 0x0B, 0x0B, 0x7E, 0x7C, 0x00, // (A)
    0x00, 0x7F, 0x7F, 0x49, 0x49, 0x7F, 0x36, 0x00, // (B)
    0x00, 0x3E, 0x7F, 0x41, 0x41, 0x63, 0x22, 0x00, // (C)
    0x00, 0x7F, 0x7F, 0x41, 0x63, 0x3E, 0x1C, 0x00, // (D)
    0x00, 0x7F, 0x7F, 0x49, 0x49, 0x41, 0x41, 0x00, // (E)
    0x00, 0x7F, 0x7F, 0x09, 0x09, 0x01, 0x01, 0x00, // (F)
    0x00, 0x3E, 0x7F, 0x41, 0x49, 0x7B, 0x3A, 0x00, // (G)
    0x00, 0x7F, 0x7F, 0x08, 0x08, 0x7F, 0x7F, 0x00, // (H)
    0x00, 0x00, 0x41, 0x7F, 0x7F, 0x41, 0x00, 0x00, // (I)
    0x00, 0x20, 0x60, 0x41, 0x7F, 0x3F, 0x01, 0x00, // (J)
    0x00, 0x7F, 0x7F, 0x1C, 0x36, 0x63, 0x41, 0x00, // (K)
    0x00, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, // (L)
    0x00, 0x7F, 0x7F, 0x06, 0x0C, 0x06, 0x7F, 0x7F, // (M)
    0x00, 0x7F, 0x7F, 0x0E, 0x1C, 0x7F, 0x7F, 0x00, // (N)
    0x00, 0x3E, 0x7F, 0x41, 0x41, 0x7F, 0x3E, 0x00, // (O)
    0x00, 0x7F, 0x7F, 0x09, 0x09, 0x0F, 0x06, 0x00, // (P)
    0x00, 0x1E, 0x3F, 0x21, 0x61, 0x7F, 0x5E, 0x00, // (Q)
    0x00, 0x7F, 0x7F, 0x19, 0x39, 0x6F, 0x46, 0x00, // (R)
    0x00, 0x26, 0x6F, 0x49, 0x49, 0x7B, 0x32, 0x00, // (S)
    0x00, 0x01, 0x01, 0x7F, 0x7F, 0x01, 0x01, 0x00, // (T)
    0x00, 0x3F, 0x7F, 0x40, 0x40, 0x7F, 0x3F, 0x00, // (U)
    0x00, 0x1F, 0x3F, 0x60, 0x60, 0x3F, 0x1F, 0x00, // (V)
    0x00, 0x7F, 0x7F, 0x30, 0x18, 0x30, 0x7F, 0x7F, // (W)
    0x00, 0x63, 0x77, 0x1C, 0x1C, 0x77, 0x63, 0x00, // (X)
    0x00, 0x07, 0x0F, 0x78, 0x78, 0x0F, 0x07, 0x00, // (Y)
    0x00, 0x61, 0x71, 0x59, 0x4D, 0x47, 0x43, 0x00, // (Z)
    0x00, 0x00, 0x7F, 0x7F, 0x41, 0x41, 0x00, 0x00, // ([)
    0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, // (\)
    0x00, 0x00, 0x41, 0x41, 0x7F, 0x7F, 0x00, 0x00, // (])
    0x00, 0x08, 0x0C, 0x06, 0x06, 0x0C, 0x08, 0x00, // (^)
    0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, // (_)
    0x00, 0x00, 0x01, 0x03, 0x06, 0x04, 0x00, 0x00, // (`)
    0x00, 0x20, 0x74, 0x54, 0x54, 0x7C, 0x78, 0x00, // (a)
    0x00, 0x7F, 0x7F, 0x44, 0x44, 0x7C, 0x38, 0x00, // (b)
    0x00, 0x38, 0x7C, 0x44, 0x44, 0x6C, 0x28, 0x00, // (c)
    0x00, 0x38, 0x7C, 0x44, 0x44, 0x7F, 0x7F, 0x00, // (d)
    0x00, 0x38, 0x7C, 0x54, 0x54, 0x5C, 0x58, 0x00, // (e)
    0x00, 0x08, 0x7E, 0x7F, 0x09, 0x03, 0x02, 0x00, // (f)
    0x00, 0x98, 0xBC, 0xA4, 0xA4, 0xFC, 0x7C, 0x00, // (g)
    0x00, 0x7F, 0x7F, 0x04, 0x04, 0x7C, 0x78, 0x00, // (h)
    0x00, 0x00, 0x00, 0x7D, 0x7D, 0x00, 0x00, 0x00, // (i)
    0x00, 0x40, 0xC0, 0x80, 0x80, 0xFD, 0x7D, 0x00, // (j)
    0x00, 0x7F, 0x7F, 0x30, 0x38, 0x6C, 0x44, 0x00, // (k)
    0x00, 0x00, 0x41, 0x7F, 0x7F, 0x40, 0x00, 0x00, // (l)
    0x00, 0x7C, 0x7C, 0x18, 0x30, 0x18, 0x7C, 0x7C, // (m)
    0x00, 0x7C, 0x7C, 0x04, 0x04, 0x7C, 0x78, 0x00, // (n)
    0x00, 0x38, 0x7C, 0x44, 0x44, 0x7C, 0x38, 0x00, // (o)
    0x00, 0xFC, 0xFC, 0x24, 0x24, 0x3C, 0x18, 0x00, // (p)
    0x00, 0x18, 0x3C, 0x24, 0x24, 0xFC, 0xFC, 0x00, // (q)
    0x00, 0x7C, 0x7C, 0x04, 0x04, 0x0C, 0x08, 0x00, // (r)
    0x00, 0x48, 0x5C, 0x54, 0x54, 0x74, 0x20, 0x00, // (s)
    0x04, 0x04, 0x3F, 0x7F, 0x44, 0x64, 0x20, 0x00, // (t)
    0x00, 0x3C, 0x7C, 0x40, 0x40, 0x7C, 0x3C, 0x00, // (u)
    0x00, 0x1C, 0x3C, 0x60, 0x60, 0x3C, 0x1C, 0x00, // (v)
    0x00, 0x1C, 0x7C, 0x30, 0x18, 0x30, 0x7C, 0x1C, // (w)
    0x00, 0x44, 0x6C, 0x38, 0x38, 0x6C, 0x44, 0x00, // (x)
    0x00, 0x9C, 0xBC, 0xA0, 0xA0, 0xFC, 0x7C, 0x00, // (y)
    0x00, 0x44, 0x64, 0x74, 0x5C, 0x4C, 0x44, 0x00, // (z)
    0x00, 0x08, 0x08, 0x3E, 0x77, 0x41, 0x41, 0x00, // ({)
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, // (|)
    0x00, 0x41, 0x41, 0x77, 0x3E, 0x08, 0x08, 0x00, // (})
    0x00, 0x02, 0x03, 0x01, 0x03, 0x02, 0x03, 0x01, // (~)
};
const uint8_t EMPTY_TILE[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

uint8_t *regs = NULL;
uint16_t *call_stack = NULL;
uint8_t *tile_table = NULL;
uint8_t *wbuf = NULL;
uint16_t call_stack_top = 0;
uint16_t pc = 0; // program counter
uint16_t pc_inst_st = 0; // program counter at instruction start
uint8_t last_key_status = 0;

static uint8_t parse_key_event(uint8_t key_status) {
    uint8_t mask = 0b1;
    for (uint8_t i = 0; i < 5; i++) {
        if (!(last_key_status & mask) && (key_status & mask)) {
            // key down
            last_key_status |= mask;
            return (EVENT_KEYDOWN << 6) | mask;
        } else if ((last_key_status & mask) && !(key_status & mask)) {
            // key up
            last_key_status &= (~mask);
            return (EVENT_KEYUP << 6) | mask;
        }
        mask <<= 1;
    }
    last_key_status = key_status & 0b00111111;
    return last_key_status;
}

static void set_reg(uint8_t reg, uint8_t val) {
    if (reg < 128) {
        regs[reg] = val;
    } else {
        #ifdef ENABLE_SAFE_CHECK
        uint8_t rreg = regs[reg - 128];
        if (rreg >= 128) {
            VASM_PANIC("write to register out of range.\n");
        }
        regs[rreg] = val;
        #else
        regs[regs[reg - 128]] = val;
        #endif
    }
}

static uint8_t get_reg(uint8_t reg) {
    if (reg < 128) {
        return regs[reg];
    } else {
        #ifdef ENABLE_SAFE_CHECK
        uint8_t rreg = regs[reg - 128];
        if (rreg >= 128) {
            VASM_PANIC("read from register out of range.\n");
        }
        return regs[rreg];
        #else
        return regs[regs[reg - 128]];
        #endif
    }
}

static void vasm_put_ch(uint8_t pos, uint8_t ch) {
    if (pos > TILE_POS_LIMIT) {
        return;
    }
    if (ch >= 0x21 && ch <= 0x7E) {
        uint8_t font_offset = ch - 0x21;
        vasm_draw_tile(pos, VASM_FONT + (font_offset * 8));
    } else {
        vasm_draw_tile(pos, EMPTY_TILE);
    }
}

static void vasm_put_unum(uint8_t pos,uint8_t num) {
    uint8_t mask = 100;
    uint8_t digital = 0;
    uint8_t after_first_digital = 0;
    while (mask > 0 && pos < TILE_POS_LIMIT)
    {
        digital = (num / mask) % 10;
        if (after_first_digital || digital > 0) {
            vasm_put_ch(pos, (uint8_t)((uint8_t)'0' + digital));
            after_first_digital = 1;
            pos ++;
        }
        mask /= 10;
    }
    if (!after_first_digital) {
        vasm_put_ch(pos, (uint8_t)'0');
    }
}

static uint8_t u8_string_size(const uint8_t *text) {
    if (text == NULL) return 0;
    uint8_t len = 0;
    while (text[len] != '\0') {
        len ++;
    }
    return len;
}

static uint8_t u8_string_group_size(const uint8_t *text_group) {
    if (text_group == NULL) return 0;
    const uint8_t *pointer = text_group;
    uint8_t len = 0;
    while (*pointer != '\0') {
        while (*pointer != '\0') {
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return len;
}

static const uint8_t *u8_string_group_get(const uint8_t *text_group, uint8_t index) {
    if (text_group == NULL) return NULL;
    const uint8_t *pointer = text_group;
    uint8_t len = 0;
    while (*pointer != '\0') {
        if (index == len) {
            return pointer;
        }
        while (*pointer != '\0') {
            pointer ++;
        }
        pointer ++;
        len ++;
    }
    return NULL;
}

static uint8_t vasm_select(uint8_t pos, const uint8_t *options) {
    if (pos > TILE_POS_LIMIT) {
        pos = TILE_POS_LIMIT - 1;
    }
    pos = (pos / VASM_TILE_GRID_WIDTH) * VASM_TILE_GRID_WIDTH;
    const uint8_t *text = NULL;
    uint8_t ipos = pos;
    uint8_t count = u8_string_group_size(options);
    uint8_t cursor = 0;
    uint8_t kevt = 0;
    uint8_t need_render = 1;
    while (1) {
        // render
        if (need_render) {
            ipos = pos;
            vasm_put_ch(ipos++, '<');
            text = u8_string_group_get(options, cursor);
            if (text == NULL) {
                for (uint8_t i = 0; i < (VASM_TILE_GRID_WIDTH - 2); i++) {
                    vasm_put_ch(ipos++, ' ');
                }
            } else {
                uint8_t text_len = u8_string_size(text);
                uint8_t center_offset = (VASM_TILE_GRID_WIDTH - 2 - text_len) / 2;
                if (text_len > VASM_TILE_GRID_WIDTH - 2) {
                    center_offset = 0;
                }
                for (uint8_t i = 0; i < (VASM_TILE_GRID_WIDTH - 2); i++) {
                    if (i >= center_offset && i < text_len + center_offset) {
                        vasm_put_ch(ipos++, (uint8_t)text[i - center_offset]);
                    } else {
                        vasm_put_ch(ipos++, ' ');
                    }
                }
            }
            vasm_put_ch(ipos, '>');
            need_render = 0;
        }
        // key
        kevt = parse_key_event(vasm_wait_key(last_key_status));
        if (kevt >> 6 == EVENT_KEYDOWN) {
            kevt = kevt & 0b00111111; // key code
            if (kevt & KEY_MASK_A) {
                return cursor;
            } else if (kevt & KEY_MASK_LEFT) {
                cursor = (cursor + count - 1) % count;
                need_render = 1;
            } else if (kevt & KEY_MASK_RIGHT) {
                cursor = (cursor + 1) % count;
                need_render = 1;
            }
        }
    }
}

void vasm_init(
        uint8_t register_buffer[REGISTER_COUNT],
        uint16_t call_stack_buffer[CALL_STACK_COUNT],
        uint8_t tile_table_buffer[TILE_TABLE_SIZE],
        uint8_t working_buffer[WORKING_BUFFER_SIZE]
    ) {
    regs = register_buffer;
    call_stack = call_stack_buffer;
    tile_table = tile_table_buffer;
    wbuf = working_buffer;
}

uint8_t vasm_execute_once() {
    #ifdef ENABLE_SAFE_CHECK
    if (regs == NULL || call_stack == NULL || tile_table == NULL || wbuf == NULL) {
        VASM_PANIC("vasm not inited.\n");
    }
    #endif
    pc_inst_st = pc;
    uint8_t inst = vasm_code_read(pc++);
    uint8_t param1 = 0;
    uint8_t param2 = 0;
    uint8_t pos = 0;
    switch (inst)
    {
    case NOPE:
        vasm_nope_callback();
        break;
    case EXIT:
        return VASM_RETURN_EXIT;
        break;
    case WKEY:
        set_reg(0, parse_key_event(vasm_wait_key(last_key_status)));
        break;
    case GKEY:
        set_reg(0, parse_key_event(vasm_get_key()));
        break;
    case TICK:
        set_reg(0, vasm_get_tick());
    case SAVE:
        vasm_save(regs + VASM_SAVE_REGISTRT_OFFSET);
        break;
    case LOAD:
        vasm_load(regs + VASM_SAVE_REGISTRT_OFFSET);
        break;
    case TILE:
        param1 = vasm_code_read(pc++);
        #ifdef ENABLE_SAFE_CHECK
        if (param1 >= CALL_STACK_COUNT) {
            VASM_PANIC("tile index out of range.\n");
        }
        #endif
        uint16_t t_offset = param1 * 8;
        uint16_t t_end = t_offset + 8;
        while (t_offset < t_end) {
            tile_table[t_offset] = vasm_code_read(pc++);
            t_offset ++;
        }
        break;
    case STOR:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        set_reg(param2, param1);
        break;
    case MOVE:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        set_reg(param2, get_reg(param1));
        break;
    case ADDI:
        param1 = vasm_code_read(pc++);
        set_reg(0, get_reg(0) + get_reg(param1));
        break;
    case SUBI:
        param1 = vasm_code_read(pc++);
        set_reg(0, get_reg(0) - get_reg(param1));
        break;
    case MULI:
        param1 = vasm_code_read(pc++);
        set_reg(0, get_reg(0) * get_reg(param1));
        break;
    case DIVI:
        param1 = vasm_code_read(pc++);
        set_reg(0, get_reg(0) / get_reg(param1));
        break;
    case MODI:
        param1 = vasm_code_read(pc++);
        set_reg(0, get_reg(0) % get_reg(param1));
        break;
    case INVI:
        set_reg(0, ~get_reg(0));
        break;
    case JUMP:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        pc = (param1 << 8) | param2;
        break;
    case JPEZ:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        if (get_reg(0) == 0) {
            pc = (param1 << 8) | param2;
        }
        break;
    case JPGZ:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        if (get_reg(0) > 0 && get_reg(0) < 128) {
            pc = (param1 << 8) | param2;
        }
        break;
    case JPLZ:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        if (get_reg(0) >= 128) {
            pc = (param1 << 8) | param2;
        }
        break;
    case CALL:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        #ifdef ENABLE_SAFE_CHECK
        if (call_stack_top >= CALL_STACK_COUNT) {
            VASM_PANIC("call stack overflow.\n");
        }
        #endif
        uint16_t target_pc = (param1 << 8) | param2;
        call_stack[call_stack_top++] = pc;
        pc = target_pc;
        break;
    case RETN:
        #ifdef ENABLE_SAFE_CHECK
        if (call_stack_top == 0) {
            VASM_PANIC("call stack underflow.\n");
        }
        #endif
        pc = call_stack[--call_stack_top];
        break;
    case DTIL:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        pos = get_reg(param1);
        uint16_t tile_offset = get_reg(param2) * 8;
        #ifdef ENABLE_SAFE_CHECK
        if (tile_offset > TILE_TABLE_SIZE - 8) {
            VASM_PANIC("tile index out of range.\n");
        }
        #endif
        vasm_draw_tile(pos, tile_table + tile_offset);
        break;
    case DNUM:
        param1 = vasm_code_read(pc++);
        param2 = vasm_code_read(pc++);
        pos = get_reg(param1);
        vasm_put_unum(pos, get_reg(param2));
        break;
    case DTXT:
        param1 = vasm_code_read(pc++);
        pos = get_reg(param1);
        param1 = vasm_code_read(pc++);
        while (param1 > 0) {
            vasm_put_ch(pos++, param1);
            param1 = vasm_code_read(pc++);
        }
        break;
    case DSEL:
        param1 = vasm_code_read(pc++);
        pos = get_reg(param1);
        uint8_t text_len = 0;
        param1 = vasm_code_read(pc++);
        while (param1 != '\0' && text_len < WORKING_BUFFER_SIZE) {
            while (param1 != '\0' && text_len < WORKING_BUFFER_SIZE) {
                wbuf[text_len] = param1;
                text_len ++;
                param1 = vasm_code_read(pc++);
            }
            if (text_len < WORKING_BUFFER_SIZE) {
                wbuf[text_len] = '\0';
                text_len ++;
                param1 = vasm_code_read(pc++);
            }
        }
        wbuf[WORKING_BUFFER_SIZE - 1] = '\0';
        wbuf[WORKING_BUFFER_SIZE - 2] = '\0';
        param1 = vasm_select(pos, wbuf);
        set_reg(0, param1);
        break;
    case RAND:
        set_reg(0, vasm_get_rand(get_reg(0)));
        break;
    
    default:
        #ifdef ENABLE_SAFE_CHECK
        VASM_PANIC("unknown instruction.\n");
        #endif
        break;
    }
    return VASM_RETURN_NORMAL;
}

uint16_t vasm_get_program_counter() {
    return pc;
}

uint16_t vasm_get_program_counter_at_instruction_start() {
    return pc_inst_st;
}
