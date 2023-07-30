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
  unsigned int factor, temp_factor =0;
  w = info.width;
  h = info.height;

#define RGB_PIXEL_SIZE 3
  unsigned int bmp_width = ptr_le_u32(bmp_data+18);
  unsigned int bmp_high = ptr_le_u32(bmp_data+22);
  unsigned int row_size = ROUNDUP(RGB_PIXEL_SIZE * bmp_width, 4);
  unsigned int padding_size = row_size - RGB_PIXEL_SIZE * bmp_width;
#define DIB_HEADER_OFFSET 54 
  const char* cur_ptr = bmp_data + DIB_HEADER_OFFSET;
  

  // bmp size is big than os window
  if(bmp_width > w || bmp_high > h)
  {
    temp_factor = bmp_width / w;
    factor = bmp_high / h;
    factor = factor < temp_factor ? temp_factor : factor;
    bmp_width = bmp_width / factor;
    bmp_high = bmp_high / factor;
    for (int y = (h+bmp_high)>>1 ; y > (h-bmp_high)>>1; y--)
    {
      for (int x = (w+bmp_width)>>1 ; x > (w-bmp_width)>>1; x--)
      {
        unsigned int color = ptr_le_u24(cur_ptr);
        draw_tile(x,y,1,1,color);
        cur_ptr += factor*RGB_PIXEL_SIZE;
      }
      cur_ptr += factor*padding_size;
    }
  }
  else 
  { // bmp size is small than os window
    temp_factor = w / bmp_width;
    factor = h /bmp_high;
    factor = factor < temp_factor ? factor : temp_factor;
    bmp_width = bmp_width * factor;
    bmp_high = bmp_high * factor;
    for (int y = (h+bmp_high)>>1 ; y > (h-bmp_high)>>1; y-= factor)
    {
      for (int x = (w+bmp_width)>>1 ; x > (w-bmp_width)>>1; x-= factor)
      {
        unsigned int color = ptr_le_u24(cur_ptr);
        draw_tile(x,y,factor,factor,color);
        cur_ptr += RGB_PIXEL_SIZE;
      }
      cur_ptr += padding_size;
    }

  }

}

//  

// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");

  display_bmp((const char*)_home_temp_cover_bmp, _home_temp_cover_bmp_len);
  // splash();

  puts("Press Esc to quit...\n");
  while (1) {
    if (wait_escape())
      break;
  }
  return 0;
}
