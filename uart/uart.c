/* The Removers'Library */
/* Copyright (C) 2006-2011 Seb/The Removers */
/* http://removers.atari.org/ */

/* This library is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU Lesser General Public */
/* License as published by the Free Software Foundation; either */
/* version 2.1 of the License, or (at your option) any later version. */

/* This library is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/* Lesser General Public License for more details. */

/* You should have received a copy of the GNU Lesser General Public */
/* License along with this library; if not, write to the Free Software */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA */

#include "uart.h"
#include <jagdefs.h>
#include <stdlib.h>

#define BAUDRATE(n) ((SYSTEM_FREQUENCY / (16 * n)) - 1)

struct baudrate_entry_t {
  enum uart_baudrate_t bd;
  uint16_t clk;
};

static const struct baudrate_entry_t baudrate_table[] = {
  { bd: B9600, clk: BAUDRATE(9600) },
  { bd: B19200, clk: BAUDRATE(19200) },
  { bd: B38400, clk: BAUDRATE(38400) },
  { bd: B57600, clk: BAUDRATE(57600) },
  { bd: B115200, clk: BAUDRATE(115200) }
};

#define arraysizeof(a$) (sizeof(a$) / sizeof(a$[0]))

static uint16_t get_clk(enum uart_baudrate_t bd) {
  int i;
  for(i = 0; i < arraysizeof(baudrate_table); i++) {
    if(baudrate_table[i].bd == bd) {
      return baudrate_table[i].clk;
    }
  }
  return baudrate_table[0].clk;
}

static uint16_t get_ctrl(enum uart_baudrate_t p) {
  switch(p) {
  case PODD:
    return ASI_ODD|ASI_PAREN;
    break;
  case PEVEN:
    return ASI_PAREN;
    break;
  case PNONE:
  default:
    return 0;
  }
}

static int putc(FILE *fp, int c) {
  while(!(JERRYREGS->asistat & ASI_TBE)) {
  }
  JERRYREGS->asidata = c & 0xff;
  return c;
}

static int eof(FILE *fp) {
  return 0;
}

static int getc(FILE *fp) {
  return EOF;
}

static int flush(FILE *fp) {
  while(!(JERRYREGS->asidata & ASI_TBE)) {
  }
  return 0;
}

FILE *open_uart(enum uart_baudrate_t bd, enum uart_parity_t p) {
  FILE *fp = calloc(1, sizeof(FILE));
  if(fp == NULL) {
    return NULL;
  }

  JERRYREGS->asiclk = get_clk(bd);
  JERRYREGS->asictrl = ASI_CLRERR;
  {
    uint16_t tmp;
    tmp = JERRYREGS->asidata;
    tmp = JERRYREGS->asidata;
  }
  JERRYREGS->asictrl = get_ctrl(p);

  fp->data = NULL;
  // output actions
  fp->putc = putc;
  fp->puts = NULL; // use default implementation
  fp->write = NULL; // use default implementation
  // input actions
  fp->getc = getc;
  fp->gets = NULL; // use default implementation
  fp->read = NULL; // use default implementation
  //
  fp->seek = NULL;
  fp->tell = NULL;
  fp->eof = eof;
  fp->flush = flush;
  fp->close = NULL;

  return fp;
}

