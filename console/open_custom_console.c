#include <stdlib.h>
#include <console.h>

#define USE_BLITTER 0

#if USE_BLITTER
// scrolling is made with the Blitter
#include <blit.h>
#else
// scrolling is made with the 68k
#include <string.h>
#endif

typedef struct {
  short int x;
  short int y;
  short int width;
  short int height;
  sprite *s;
} console;

static char ascii_fnt[256*8] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x18, 0x24, 0x42, 0x81, 0xE7, 0x24, 0x24, 0x3C, 
  0x3C, 0x24, 0x24, 0xE7, 0x81, 0x42, 0x24, 0x18, 
  0x18, 0x14, 0xF2, 0x81, 0x81, 0xF2, 0x14, 0x18, 
  0x18, 0x28, 0x4F, 0x81, 0x81, 0x4F, 0x28, 0x18, 
  0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF, 
  0xF8, 0x88, 0x8F, 0x89, 0xF9, 0x41, 0x41, 0x7F, 
  0xFF, 0x89, 0x89, 0x89, 0xF9, 0x81, 0x81, 0xFF, 
  0x01, 0x03, 0x06, 0x8C, 0xD8, 0x70, 0x20, 0x00, 
  0x7E, 0xC3, 0xD3, 0xD3, 0xDB, 0xC3, 0xC3, 0x7E, 
  0x18, 0x3C, 0x2C, 0x2C, 0x7E, 0x18, 0x18, 0x00, 
  0x10, 0x1C, 0x12, 0x10, 0x10, 0x70, 0xF0, 0x60, 
  0xF0, 0xC0, 0xFE, 0xD8, 0xDE, 0x18, 0x18, 0x00, 
  0x70, 0xC8, 0xDE, 0xDB, 0xDB, 0x7E, 0x1B, 0x1B, 
  0x03, 0x00, 0x0F, 0x03, 0x03, 0x03, 0x03, 0x0F, 
  0x90, 0x20, 0xD0, 0xA0, 0xD0, 0xA8, 0xD0, 0xE0, 
  0x7C, 0xC6, 0xC6, 0x00, 0xC6, 0xC6, 0x7C, 0x00, 
  0x06, 0x06, 0x06, 0x00, 0x06, 0x06, 0x06, 0x00, 
  0x7C, 0x06, 0x06, 0x7C, 0xC0, 0xC0, 0x7C, 0x00, 
  0x7C, 0x06, 0x06, 0x7C, 0x06, 0x06, 0x7C, 0x00, 
  0xC6, 0xC6, 0xC6, 0x7C, 0x06, 0x06, 0x06, 0x00, 
  0x7C, 0xC0, 0xC0, 0x7C, 0x06, 0x06, 0x7C, 0x00, 
  0x7C, 0xC0, 0xC0, 0x7C, 0xC6, 0xC6, 0x7C, 0x00, 
  0x7C, 0x06, 0x06, 0x00, 0x06, 0x06, 0x06, 0x00, 
  0x7C, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0x7C, 0x00, 
  0x7C, 0xC6, 0xC6, 0x7C, 0x06, 0x06, 0x7C, 0x00, 
  0x00, 0x3C, 0x46, 0x06, 0x7E, 0x66, 0x3C, 0x00, 
  0x78, 0x66, 0x7D, 0x64, 0x7E, 0x03, 0x0B, 0x06, 
  0x07, 0x0F, 0x1F, 0x18, 0x18, 0x10, 0x1E, 0x17, 
  0xF0, 0xF8, 0xEC, 0x04, 0x04, 0x04, 0x3C, 0x54, 
  0x11, 0x0B, 0x0D, 0x06, 0x07, 0x2E, 0x39, 0x38, 
  0xFC, 0xFC, 0xFF, 0xE1, 0xE1, 0x21, 0x3F, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00, 
  0x66, 0x66, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x00, 
  0x14, 0x3E, 0x55, 0x3C, 0x1E, 0x55, 0x3E, 0x14, 
  0x62, 0x66, 0x0C, 0x18, 0x30, 0x66, 0x46, 0x00, 
  0x78, 0xCC, 0x61, 0xCE, 0xCC, 0xCC, 0x78, 0x00, 
  0x18, 0x18, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x04, 0x08, 0x18, 0x18, 0x18, 0x18, 0x08, 0x04, 
  0x20, 0x10, 0x18, 0x18, 0x18, 0x18, 0x10, 0x20, 
  0x00, 0x54, 0x38, 0xFE, 0x38, 0x54, 0x00, 0x00, 
  0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x20, 
  0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 
  0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x00, 
  0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00, 
  0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
  0x3C, 0x66, 0x0E, 0x1C, 0x38, 0x70, 0x7E, 0x00, 
  0x7E, 0x0C, 0x18, 0x3C, 0x06, 0x46, 0x3C, 0x00, 
  0x0C, 0x1C, 0x2C, 0x4C, 0x7E, 0x0C, 0x0C, 0x00, 
  0x7E, 0x60, 0x7C, 0x06, 0x06, 0x46, 0x3C, 0x00, 
  0x1C, 0x20, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00, 
  0x7E, 0x06, 0x0E, 0x1C, 0x18, 0x18, 0x18, 0x00, 
  0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00, 
  0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00, 
  0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 
  0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x10, 0x00, 
  0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00, 
  0x00, 0x00, 0x3C, 0x00, 0x00, 0x3C, 0x00, 0x00, 
  0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00, 
  0x3C, 0x46, 0x06, 0x0C, 0x18, 0x18, 0x00, 0x18, 
  0x3C, 0x66, 0x6E, 0x6A, 0x6E, 0x60, 0x3C, 0x00, 
  0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 
  0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00, 
  0x3C, 0x62, 0x60, 0x60, 0x60, 0x62, 0x3C, 0x00, 
  0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 
  0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00, 
  0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x00, 
  0x3C, 0x62, 0x60, 0x6E, 0x66, 0x66, 0x3E, 0x00, 
  0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
  0x06, 0x06, 0x06, 0x06, 0x06, 0x46, 0x3C, 0x00, 
  0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00, 
  0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7C, 0x00, 
  0xFC, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0xC6, 0x00, 
  0x62, 0x72, 0x7A, 0x5E, 0x4E, 0x46, 0x42, 0x00, 
  0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00, 
  0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x06, 
  0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x00, 
  0x3C, 0x62, 0x70, 0x3C, 0x0E, 0x46, 0x3C, 0x00, 
  0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
  0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x66, 0x66, 0x66, 0x66, 0x66, 0x64, 0x78, 0x00, 
  0xC6, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFC, 0x00, 
  0x66, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x00, 
  0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00, 
  0x7E, 0x0E, 0x1C, 0x38, 0x70, 0x60, 0x7E, 0x00, 
  0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00, 
  0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00, 
  0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00, 
  0x10, 0x38, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 
  0x00, 0xC0, 0xC0, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x00, 
  0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 
  0x00, 0x3C, 0x62, 0x60, 0x60, 0x62, 0x3C, 0x00, 
  0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00, 
  0x00, 0x3C, 0x66, 0x7E, 0x60, 0x62, 0x3C, 0x00, 
  0x1E, 0x30, 0x7C, 0x30, 0x30, 0x30, 0x30, 0x00, 
  0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x46, 0x3C, 
  0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 
  0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 
  0x00, 0x08, 0x18, 0x18, 0x18, 0x18, 0x58, 0x30, 
  0x60, 0x64, 0x68, 0x70, 0x78, 0x6C, 0x66, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0C, 0x00, 
  0x00, 0xFC, 0xD6, 0xD6, 0xD6, 0xD6, 0xC6, 0x00, 
  0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 
  0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x00, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 
  0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 
  0x00, 0x6C, 0x70, 0x60, 0x60, 0x60, 0x60, 0x00, 
  0x00, 0x3C, 0x72, 0x38, 0x1C, 0x4E, 0x3C, 0x00, 
  0x18, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x0C, 0x00, 
  0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00, 
  0x00, 0x66, 0x66, 0x66, 0x66, 0x64, 0x78, 0x00, 
  0x00, 0xC6, 0xC6, 0xD6, 0xD6, 0xD6, 0xFC, 0x00, 
  0x00, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x00, 
  0x00, 0x66, 0x66, 0x66, 0x26, 0x1E, 0x46, 0x3C, 
  0x00, 0x7E, 0x0E, 0x1C, 0x38, 0x70, 0x7E, 0x00, 
  0x0E, 0x18, 0x18, 0x30, 0x18, 0x18, 0x0E, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0x70, 0x18, 0x18, 0x0C, 0x18, 0x18, 0x70, 0x00, 
  0x00, 0x60, 0xF2, 0x9E, 0x0C, 0x00, 0x00, 0x00, 
  0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0xFE, 
  0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 
  0x24, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00, 
  0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 
  0x24, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3E, 0x00, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0xFF, 0xFF, 
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 
  0x03, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x03, 
  0x24, 0x00, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x00, 
  0xFF, 0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF, 
  0x00, 0x00, 0x7E, 0x1B, 0x7F, 0xD8, 0x7E, 0x00, 
  0x3F, 0x78, 0xD8, 0xDE, 0xF8, 0xD8, 0xDF, 0x00, 
  0x18, 0x34, 0x00, 0x3C, 0x66, 0x66, 0x3C, 0x00, 
  0x24, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x30, 0x18, 0x00, 0x3C, 0x66, 0x66, 0x3C, 0x00, 
  0x18, 0x24, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x30, 0x18, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x66, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x46, 0x3C, 
  0x66, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x66, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, 
  0x18, 0x3C, 0x62, 0x60, 0x60, 0x62, 0x3C, 0x18, 
  0x1C, 0x3A, 0x30, 0x7C, 0x30, 0x30, 0x7E, 0x00, 
  0x66, 0x66, 0x3C, 0x18, 0x3C, 0x18, 0x18, 0x00, 
  0x3C, 0x66, 0x66, 0x6C, 0x66, 0x66, 0xEC, 0x00, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0x0C, 0x18, 0x00, 0x3C, 0x06, 0x7E, 0x3E, 0x00, 
  0x0C, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 
  0x0C, 0x18, 0x00, 0x3C, 0x66, 0x66, 0x3C, 0x00, 
  0x0C, 0x18, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x00, 
  0x34, 0x58, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x00, 
  0x1A, 0x2C, 0x62, 0x72, 0x5A, 0x4E, 0x46, 0x00, 
  0x00, 0x3C, 0x46, 0x3E, 0x66, 0x3E, 0x00, 0x7E, 
  0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, 0x7E, 
  0x00, 0x18, 0x00, 0x18, 0x30, 0x60, 0x66, 0x3C, 
  0x00, 0x00, 0x00, 0x3E, 0x30, 0x30, 0x30, 0x00, 
  0x00, 0x00, 0x00, 0x7C, 0x0C, 0x0C, 0x0C, 0x00, 
  0x62, 0xE4, 0x68, 0x76, 0x2B, 0x43, 0x86, 0x0F, 
  0x62, 0xE4, 0x68, 0x76, 0x2E, 0x56, 0x9F, 0x06, 
  0x00, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0x1B, 0x36, 0x6C, 0xD8, 0x6C, 0x36, 0x1B, 0x00, 
  0xD8, 0x6C, 0x36, 0x1B, 0x36, 0x6C, 0xD8, 0x00, 
  0x34, 0x58, 0x00, 0x3C, 0x06, 0x7E, 0x3E, 0x00, 
  0x34, 0x58, 0x00, 0x3C, 0x66, 0x66, 0x3C, 0x00, 
  0x02, 0x3C, 0x66, 0x6E, 0x76, 0x66, 0x3C, 0x40, 
  0x00, 0x02, 0x3C, 0x6E, 0x76, 0x66, 0x3C, 0x40, 
  0x00, 0x00, 0x7E, 0xDB, 0xDE, 0xD8, 0x7F, 0x00, 
  0x00, 0x7E, 0xD8, 0xD8, 0xFC, 0xD8, 0xD8, 0xDE, 
  0x20, 0x10, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 
  0x34, 0x58, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 
  0x34, 0x58, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 
  0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x10, 0x38, 0x10, 0x10, 0x10, 0x00, 0x00, 
  0x7A, 0xCA, 0xCA, 0xCA, 0x7A, 0x0A, 0x0A, 0x0A, 
  0x3C, 0x42, 0x99, 0xB5, 0xB1, 0x9D, 0x42, 0x3C, 
  0x3C, 0x42, 0xB9, 0xB5, 0xB9, 0xB5, 0x42, 0x3C, 
  0xF1, 0x5B, 0x55, 0x51, 0x51, 0x00, 0x00, 0x00, 
  0x66, 0x00, 0xE6, 0x66, 0x66, 0xF6, 0x06, 0x1C, 
  0xF6, 0x66, 0x66, 0x66, 0x66, 0xF6, 0x06, 0x1C, 
  0x00, 0x66, 0x76, 0x3C, 0x6E, 0x66, 0x00, 0x00, 
  0x00, 0x7C, 0x0C, 0x0C, 0x0C, 0x7E, 0x00, 0x00, 
  0x00, 0x1E, 0x06, 0x0E, 0x1E, 0x36, 0x00, 0x00, 
  0x00, 0x7E, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 
  0x00, 0x7C, 0x06, 0x66, 0x66, 0x66, 0x00, 0x00, 
  0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 
  0x00, 0x1E, 0x0C, 0x06, 0x06, 0x06, 0x00, 0x00, 
  0x00, 0x7E, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 
  0x60, 0x6E, 0x66, 0x66, 0x66, 0x7E, 0x00, 0x00, 
  0x00, 0x3C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x3E, 0x06, 0x06, 0x06, 0x3E, 0x00, 0x00, 
  0x60, 0x7E, 0x06, 0x06, 0x06, 0x0E, 0x00, 0x00, 
  0x00, 0x6C, 0x3E, 0x66, 0x66, 0x6E, 0x00, 0x00, 
  0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00, 
  0x00, 0x3E, 0x36, 0x36, 0x36, 0x1C, 0x00, 0x00, 
  0x00, 0x36, 0x36, 0x36, 0x36, 0x7E, 0x00, 0x00, 
  0x00, 0x7E, 0x66, 0x76, 0x06, 0x7E, 0x00, 0x00, 
  0x00, 0x66, 0x66, 0x3C, 0x0E, 0x7E, 0x00, 0x00, 
  0x00, 0x3E, 0x06, 0x36, 0x36, 0x34, 0x30, 0x00, 
  0x00, 0x78, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 
  0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xFE, 0x00, 0x00, 
  0x00, 0x7C, 0x6C, 0x6C, 0x6C, 0xEC, 0x00, 0x00, 
  0x00, 0x1C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x00, 
  0x00, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 
  0x00, 0xFE, 0x66, 0x66, 0x66, 0x7E, 0x00, 0x00, 
  0x00, 0x7E, 0x66, 0x76, 0x06, 0x06, 0x06, 0x00, 
  0x00, 0x36, 0x36, 0x1C, 0x0C, 0x0C, 0x0C, 0x00, 
  0x1C, 0x32, 0x3C, 0x66, 0x66, 0x3C, 0x4C, 0x38, 
  0x00, 0x10, 0x38, 0x6C, 0xC6, 0x82, 0x00, 0x00, 
  0x66, 0xF7, 0x99, 0x99, 0xEF, 0x66, 0x00, 0x00, 
  0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00, 
  0x1C, 0x36, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x60, 
  0x00, 0xFE, 0x66, 0x62, 0x60, 0x60, 0x60, 0xF8, 
  0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x48, 
  0xFE, 0x66, 0x30, 0x18, 0x30, 0x66, 0xFE, 0x00, 
  0x00, 0x1E, 0x38, 0x6C, 0x6C, 0x6C, 0x38, 0x00, 
  0x00, 0x00, 0x6C, 0x6C, 0x6C, 0x6C, 0x7F, 0xC0, 
  0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x10, 
  0x3C, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x3C, 
  0x00, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x3C, 0x00, 
  0x00, 0x3C, 0x66, 0x66, 0x66, 0x24, 0x66, 0x00, 
  0x1C, 0x36, 0x78, 0xDC, 0xCC, 0xEC, 0x78, 0x00, 
  0x0C, 0x18, 0x38, 0x54, 0x54, 0x38, 0x30, 0x60, 
  0x00, 0x10, 0x7C, 0xD6, 0xD6, 0xD6, 0x7C, 0x10, 
  0x3E, 0x70, 0x60, 0x7E, 0x60, 0x70, 0x3E, 0x00, 
  0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 
  0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 
  0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00, 
  0x30, 0x18, 0x0C, 0x18, 0x30, 0x00, 0x7E, 0x00, 
  0x0C, 0x18, 0x30, 0x18, 0x0C, 0x00, 0x7E, 0x00, 
  0x00, 0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 
  0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70, 0x00, 
  0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00, 
  0x00, 0x32, 0x4C, 0x00, 0x32, 0x4C, 0x00, 0x00, 
  0x38, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x38, 0x7C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 
  0x00, 0x00, 0x0F, 0x18, 0xD8, 0x70, 0x30, 0x00, 
  0x38, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 
  0x38, 0x6C, 0x18, 0x30, 0x7C, 0x00, 0x00, 0x00, 
  0x78, 0x0C, 0x38, 0x0C, 0x78, 0x00, 0x00, 0x00, 
  0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static inline void output_char(console *co, char c) {
  if(co->y == co->height) {
    char *adr = (char *) co->s->data;
#if USE_BLITTER
    blitmove(adr+co->width*8,adr,co->width*(co->height-1)*8);
    blitset(adr+co->width*(co->height-1)*8,0,co->width*8);
#else
    bcopy(adr+co->width*8,adr,co->width*(co->height-1)*8);
    bzero(adr+co->width*(co->height-1)*8,co->width*8);
#endif
    co->y--;
  }

  switch(c) {
  case '\n': {
    co->x = 0;
    output_char(co,'\f');
    /*
    while(co->x != 0) {
      output_char(co,' ');
    }
    */
    /*
    co->x = 0;
    if(++(co->y) == co->height) {
    co->y = 0;
    }
    */
    break;
  }
  case '\f': {
    if(++(co->y) == co->height) {
      //      co->y = 0;
    }
    break;
  }
  case '\r': {
    co->x = 0;
    break;
  }
  case '\t': {
    int i = 4;
    i -= co->x % 4;
    for(;i > 0; i--) {
      output_char(co,' ');
    }
    break;
  }
  default: {
    char *adr = (char *) co->s->data;
    char *scr = adr + (co->x + co->y * co->width * 8);
    char *chr = ascii_fnt + 8*c;
    int i;
    for(i = 0; i < 8; i++) {
      *scr = *chr++;
      scr += co->width;
    }
    if(++(co->x) == co->width) {
      co->x = 0;
      if(++(co->y) == co->height) {
	//	co->y = 0;
      }
    }
  }
  }
}

static inline void output_string(console *co,const char *s) {
  while(*s != 0) {
    output_char(co,*s++);
  }
}

static int putc(FILE *stream, char c) {
  console *co = stream->data;
  if(co != NULL) {
    output_char(co,c);
    return c;
  }
  return EOF;
}

static int puts(FILE *stream, const char *s) {
  console *co = stream->data;
  if(co != NULL) {
    output_string(co,s);
    return 0;
  }
  return EOF;
}

/* static size_t write(FILE *stream, const  void  *ptr,  size_t  size,  size_t  nmemb) { */
/*   console *co = stream->data; */
/*   if(co != NULL) { */
/*     int i; */
/*     char *s = (char *) ptr; */
/*     for(i = 0; i < nmemb * size; i++) { */
/*       output_char(co, *s++); */
/*     } */
/*     return nmemb; */
/*   } */
/*   return 0; */
/* } */

static int close(FILE *fp) {
  console *co = fp->data;
  if(co != NULL) {
    sprite *s = co->s;
    detach_sprite_from_display(s);
    free(s->data);
    free(s);
    free(co);
    fp->data = NULL;
    return 0;
  }
  return EOF;
}

static int eof(FILE *fp) {
  console *co = fp->data;
  return (co != NULL);
}

FILE *open_custom_console(display *d, int x, int y, int idx, int width, int height, int layer) {
  if(width % 8 != 0) {
    width += 8-(width % 8);
  }
  console *co = malloc(sizeof(console));
  phrase *scr_addr = calloc(8,(width / 8) * (height * 8));
  sprite *s = new_sprite(width*8,height*8,x,y,DEPTH1,scr_addr);
  s->trans = 0;
  s->index = idx;
  co->x = 0;
  co->y = 0;
  co->width = width;
  co->height = height;
  co->s = s;
  attach_sprite_to_display_at_layer(s,d,layer);
  SET_CLUT_RGB(idx,0,0,0);
  SET_CLUT_RGB(idx+1,255,255,255);
  //  SET_SHORT_INT(0,(CLUT+2*idx));
  //  SET_SHORT_INT(0xffff,(CLUT+2*idx+2));
  FILE *fp = malloc(sizeof(FILE));
  fp->data = co;
  fp->eof = eof;
  fp->putc = putc;
  fp->puts = puts;
  fp->write = NULL; // use default implementation
  fp->flush = NULL;
  fp->close = close;
  return fp;
}

