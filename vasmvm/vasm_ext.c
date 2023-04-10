#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "vasm_ext.h"
#include "vasm_core.h"

#define WINDOW_SCALE 8
#define WINDOW_W (128 * WINDOW_SCALE)
#define WINDOW_H (64 * WINDOW_SCALE)
#define check(x) if ((x) < 0) { printf("Error: %s\n", SDL_GetError()); exit(1); }
#define BUFFERED_SIZE 1024

SDL_Renderer *renderer = NULL;
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
SDL_Rect rect = { 0 };
FILE *file_handler = NULL;
uint8_t file_buffer[BUFFERED_SIZE] = { 0 };
size_t file_buffer_offset = 0;
size_t file_buffer_end = 0;
// vasm runtime buffers
uint8_t register_buffer[REGISTER_COUNT] = { 0 };
uint16_t call_stack_buffer[CALL_STACK_COUNT] = { 0 };
uint8_t tile_table_buffer[TILE_TABLE_SIZE] = { 0 };
uint8_t working_buffer[WORKING_BUFFER_SIZE] = { 0 };

static void white() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
}

static void black() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
}

static void poll_sdl_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {  // poll until all events are handled!
        if (event.type == SDL_QUIT) {
            exit(0);
            break;
        }
    }
}

uint8_t vasm_code_read(uint16_t offset) {
    // check buffer
    if (offset >= file_buffer_offset && offset < file_buffer_end) {
        // read from buffer
        return file_buffer[offset - file_buffer_offset];
    } else {
        fseek(file_handler, offset, SEEK_SET);
        size_t readed = fread(file_buffer, 1, BUFFERED_SIZE, file_handler);
        if (readed == 0) {
            printf("Read Failed. Maybe reach the end of the program. offset: %u\n", offset);
            exit(1);
        }
        file_buffer_offset = offset;
        file_buffer_end = offset + readed;
        return file_buffer[0];
    }
}

uint8_t vasm_get_key(void) {
    int ksize = 0;
    const uint8_t *kbd = SDL_GetKeyboardState(&ksize);
    uint8_t value = 0;
    if (kbd[SDL_SCANCODE_SPACE]) {
        value |= KEY_MASK_A;
    } else if (kbd[SDL_SCANCODE_W]) {
        value |= KEY_MASK_UP;
    } else if (kbd[SDL_SCANCODE_S]) {
        value |= KEY_MASK_DOWN;
    } else if (kbd[SDL_SCANCODE_A]) {
        value |= KEY_MASK_LEFT;
    } else if (kbd[SDL_SCANCODE_D]) {
        value |= KEY_MASK_RIGHT;
    }
    return value;
}

uint8_t vasm_wait_key(uint8_t last_key) {
    uint8_t key = last_key;
    while ((key = vasm_get_key()) == last_key){
        poll_sdl_events();
    }
    return key;
}

uint8_t vasm_get_tick() {
    return SDL_GetTicks64() & 0xFF;
}

void vasm_draw_tile(uint8_t pos, const uint8_t mono_vlsb_data[8]) {
    uint8_t vx = (pos % VASM_TILE_GRID_WIDTH) * 8;
    uint8_t vy = (pos / VASM_TILE_GRID_WIDTH) * 8;
    uint8_t ix = 0;
    uint8_t iy = 0;
    uint8_t mask = 0;
    while (iy < 8) {
        mask = 0b1 << iy;
        ix = 0;
        while (ix < 8) {
            if (mono_vlsb_data[ix] & mask) {
                white();
            } else {
                black();
            }
            rect.x = (vx + ix) * WINDOW_SCALE + 1;
            rect.y = (vy + iy) * WINDOW_SCALE + 1;
            rect.w = WINDOW_SCALE - 2;
            rect.h = WINDOW_SCALE - 2;
            check(SDL_RenderFillRect(renderer, &rect));
            ix ++;
        }
        iy ++;
    }
    SDL_RenderPresent(renderer);
    check(SDL_UpdateWindowSurface(window));
}

void vasm_nope_callback() {
    printf("NOPE dump:\n");
    printf("==== PC ====\n");
    printf("0x%04x\n", vasm_get_program_counter_at_instruction_start());
    printf("==== Registers ====\n");
    printf("      +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +a +b +c +d +e +f \n");
    for (uint8_t row = 0; row < 8; row ++) {
        printf("r%03u: ", row * 16);
        for (uint8_t col = 0; col < 16; col ++) {
            uint8_t reg_value = register_buffer[row * 16 + col];
            printf("%02X ", reg_value);
        }
        printf("\n");
    }
    printf("==== dump end ====\n\n");
}

void init(void) {
    check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER))
    window = SDL_CreateWindow("VASM",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );
    check((int64_t)window - 1); // window not null
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    check((int64_t)renderer - 1); // renderer not null
    surface = SDL_GetWindowSurface(window);
    check((int64_t)surface - 1); // surface not null
    white();
    black();
    rect.x = 0;
    rect.y = 0;
    rect.w = 128 * WINDOW_SCALE;
    rect.h = 64 * WINDOW_SCALE;
    check(SDL_RenderClear(renderer));
    check(SDL_RenderFillRect(renderer, &rect));
    SDL_RenderPresent(renderer);
    check(SDL_UpdateWindowSurface(window));
}

void loop(void) {
    // main loop
    // init runtime
    file_handler = fopen("./a.out", "r");
    if (file_handler == NULL) {
        printf("Failed to open 'a.out'");
        exit(1);
    }
    vasm_init(register_buffer, call_stack_buffer, tile_table_buffer, working_buffer);
    uint8_t running = 1;
    uint64_t last_update_tick = SDL_GetTicks64();
    SDL_Delay(500);
    while (running) {
        uint8_t ret = vasm_execute_once();
        if (ret == VASM_RETURN_EXIT) {
            printf("vasm exit.\n");
            exit(0);
        }
        poll_sdl_events();
        if (SDL_GetTicks64() - last_update_tick >= 30) {
            check(SDL_UpdateWindowSurface(window));
            last_update_tick += 30;
        }
        sleep(0);
    }
    fclose(file_handler);
    SDL_Quit();
}
