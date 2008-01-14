/* The Removers'Library */
/* Copyright (C) 2006-2008 Seb/The Removers */
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

#include <jagdefs.h>
#include <display.h>
#include <stdlib.h>
#include <op.h>

void build_display_list_header(display *d,display_list_header *h, qphrase *list) {
  h->ob1.type = BRANCHOBJ;
  h->ob1.ypos = a_vde;
  h->ob1.cc = O_BRLT;
  h->ob1.link = (unsigned long)(&(h->ob7)) >> 3;
  //
  h->ob2.type = BRANCHOBJ;
  h->ob2.ypos = a_vdb;
  h->ob2.cc = O_BRGT;
  h->ob2.link = (unsigned long)(&(h->ob7)) >> 3;
  //
  h->ob3.type = BRANCHOBJ;
  h->ob3.ypos = (a_vdb + 1) & 0xfffe;
  h->ob3.cc = O_BREQ;
  h->ob3.link = (unsigned long)(&(h->ob5)) >> 3;
  //
  h->ob4.type = BRANCHOBJ;
  h->ob4.ypos = 0x7ff;
  h->ob4.cc = O_BREQ;
  h->ob4.link = (unsigned long)list >> 3;
  //
#if DISPLAY_USE_OP_IT
  h->ob5.data1 = (unsigned long)d;;
  h->ob5.type = GPUOBJ;
#else
  h->ob5.type = BRANCHOBJ;
  h->ob5.ypos = 0x7ff;
  h->ob5.cc = O_BREQ;
  h->ob5.link = (unsigned long)(&(h->ob4)) >> 3;
#endif
  //
  h->ob6.type = BRANCHOBJ;
  h->ob6.ypos = 0x7ff;
  h->ob6.cc = O_BREQ;
  h->ob6.link = (unsigned long)(&(h->ob4)) >> 3;
  //
  h->ob7.type = STOPOBJ;
  h->ob7.int_flag = 0;
  //
  op_stop_object *stop;
  stop = (void *)list;
  stop->type = STOPOBJ;
  stop->int_flag = 0;
}

// This is bad because it is hardcoded for 8 strips!!
void build_display_strip_tree(display *d,qphrase *list) {
  display_strip_tree *h = (display_strip_tree *)list;
  list += sizeof(display_strip_tree)/sizeof(qphrase);
  //
  h->ob1.type = BRANCHOBJ;
  h->ob1.ypos = d->strips[4].y<<1;
  h->ob1.cc = O_BRGT;
  h->ob1.link = (unsigned long)(&(h->ob8)) >> 3;
  //
  h->ob2.type = BRANCHOBJ;
  h->ob2.ypos = d->strips[6].y<<1;
  h->ob2.cc = O_BRGT;
  h->ob2.link = (unsigned long)(&(h->ob6)) >> 3;
  //
  h->ob3.type = BRANCHOBJ;
  h->ob3.ypos = d->strips[7].y<<1;
  h->ob3.cc = O_BRGT;
  h->ob3.link = (unsigned long)(list+d->strips[6].offset/sizeof(qphrase)) >> 3;
  //
  h->ob4.type = BRANCHOBJ;
  h->ob4.ypos = d->strips[8].y<<1;
  h->ob4.cc = O_BRGT;
  h->ob4.link = (unsigned long)(list+d->strips[7].offset/sizeof(qphrase)) >> 3;
  //
  h->ob5.type = STOPOBJ;
  h->ob5.int_flag = 0;
  //
  h->ob6.type = BRANCHOBJ;
  h->ob6.ypos = d->strips[5].y<<1;
  h->ob6.cc = O_BRGT;
  h->ob6.link = (unsigned long)(list+d->strips[4].offset/sizeof(qphrase)) >> 3;
  //
  h->ob7.type = BRANCHOBJ;
  h->ob7.ypos = 0x7ff;
  h->ob7.cc = O_BREQ;
  h->ob7.link = (unsigned long)(list+d->strips[5].offset/sizeof(qphrase)) >> 3;
  //
  h->ob8.type = BRANCHOBJ;
  h->ob8.ypos = d->strips[2].y<<1;
  h->ob8.cc = O_BRGT;
  h->ob8.link = (unsigned long)(&(h->ob11)) >> 3;
  //
  h->ob9.type = BRANCHOBJ;
  h->ob9.ypos = d->strips[3].y<<1;
  h->ob9.cc = O_BRGT;
  h->ob9.link = (unsigned long)(list+d->strips[2].offset/sizeof(qphrase)) >> 3;
  //
  h->ob10.type = BRANCHOBJ;
  h->ob10.ypos = 0x7ff;
  h->ob10.cc = O_BREQ;
  h->ob10.link = (unsigned long)(list+d->strips[3].offset/sizeof(qphrase)) >> 3;
  //
  h->ob11.type = BRANCHOBJ;
  h->ob11.ypos = (d->strips[1].y<<1) - 1;
  h->ob11.cc = O_BRLT;
  h->ob11.link = (unsigned long)(list+d->strips[1].offset/sizeof(qphrase)) >> 3;
  //
  h->ob12.type = BRANCHOBJ;
  h->ob12.ypos = (d->strips[0].y<<1) - 1;
  h->ob12.cc = O_BRLT;
  h->ob12.link = (unsigned long)(list+d->strips[0].offset/sizeof(qphrase)) >> 3;
  //
  h->ob13.type = STOPOBJ;
  h->ob13.int_flag = 0;
  //
  int i;
  op_stop_object *stop;
  for(i = 0; i < DISPLAY_NB_STRIPS; i++) {
    stop = (void *)(list+d->strips[i].offset/sizeof(qphrase));
    stop->type = STOPOBJ;
    stop->int_flag = 0;
  }
}

mblock *new_custom_display(unsigned int max_nb_sprites, int strips[]) {
  display *d;
  int i;

  if(max_nb_sprites == 0) {
    max_nb_sprites = DISPLAY_DFLT_MAX_SPRITE;
  }
  max_nb_sprites++; // for stop object
  mblock *result = memalign(sizeof(qphrase),sizeof(display)+2*(sizeof(display_strip_tree)+DISPLAY_NB_STRIPS*max_nb_sprites*sizeof(qphrase)));
  d = (display *)result->addr;

  d->phys = d->op_list;
  d->log = d->op_list + sizeof(display_strip_tree) + DISPLAY_NB_STRIPS*max_nb_sprites;

  d->x = 0;
  d->y = 0;

  for(i = 0; i < 1<<DISPLAY_NB_LAYER; i++) {
    d->layer[i].y = 0;
    d->layer[i].x = 0;
    d->layer[i].attribute.visible = 1;
    d->layer[i].attribute.reserved = 0;
    d->layer[i].sprites.previous = NULL;
    d->layer[i].sprites.next = NULL;
  }

  build_display_list_header(d,&d->h,d->phys);

  int y_min = (a_vdb+1)/2;

  for(i = 0; i <= DISPLAY_NB_STRIPS; i++) {
    d->strips[i].y = y_min + strips[i];
    d->strips[i].offset = i * max_nb_sprites*sizeof(qphrase);
  }

  // remove this !!
/*   for(i = 0; i < DISPLAY_NB_STRIPS; i++) { */
/*     d->strips[i].offset = 0; */
/*   } */
  //
  
  build_display_strip_tree(d,d->phys);
  build_display_strip_tree(d,d->log);

  return result;
}

mblock *new_display(unsigned int max_nb_sprites) {
  int strips[DISPLAY_NB_STRIPS+1];
  int y_min = (a_vdb+1)/2;
  int y_max = a_vde/2;
  int h_strip = (y_max - y_min) / DISPLAY_NB_STRIPS;
  int i;
  for(i = 0; i < DISPLAY_NB_STRIPS; i++) {
    strips[i] = i * h_strip;
  } 
  strips[DISPLAY_NB_STRIPS] = y_max-y_min;
  return new_custom_display(max_nb_sprites,strips);
}