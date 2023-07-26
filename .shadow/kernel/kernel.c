#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>

#include "graph_data.h"
#include "data_conv.h"

#define SIDE 16

static int w, h;  // Screen size

#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = { AM_KEYS(KEYNAME) };

static inline void puts(const char *s) {
  for (; *s; s++) putch(*s);
}

bool wait_escape() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode == AM_KEY_ESCAPE && event.keydown) {
    return true;
  }
  return false;
}

void print_key() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
  }
}

static void draw_tile(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: large stack-allocated memory
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  ioe_write(AM_GPU_FBDRAW, &event);
}

void display_bmp(const char* bmp_data, size_t bmp_data_len)
{
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;

#define RGB_PIXEL_SIZE 3
  unsigned int bmp_width = ptr_le_u32(bmp_data+18);
  unsigned int bmp_high = ptr_le_u32(bmp_data+22);
  unsigned int row_size = ROUNDUP(RGB_PIXEL_SIZE * bmp_width, 4);
  unsigned int padding_size = row_size - RGB_PIXEL_SIZE * bmp_width;
#define BITMAP_FILE_HEADER_SIZE 14
#define DIB_HEADER_SIZE 54 
  unsigned int offset = BITMAP_FILE_HEADER_SIZE+DIB_HEADER_SIZE;
  const char* cur_ptr = bmp_data + offset;
  for (int x = 0; x <= w && x <= bmp_width; x++){
    for (int y = 0; y <= h && y <= bmp_high; y++){
      draw_tile(x,y,1,1,ptr_le_u32(cur_ptr));
      cur_ptr += RGB_PIXEL_SIZE;
    }
    cur_ptr += padding_size;
  }


}


void splash() {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;

  for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      if ((x & 1) ^ (y & 1)) {
        draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
      }
    }
  }
}

// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");

  display_bmp((const char*)_home_temp_cover_bmp, _home_temp_cover_bmp_len);
  // splash();

  puts("Press any key to see its key code...\n");
  while (1) {
    if (wait_escape())
      break;
  }
  return 0;
}
