// Basic program skeleton for a Sketch File (.sk) Viewer//
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState() {
  //TO DO

  state *s = malloc(sizeof(state));
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = 1;
  s->start = 0;
  s->data = 0;
  s->end = false;
  return s; // this is a placeholder only
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  //TO DO

  free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  //TO DO

  int opcode = 0;
  b = b >> 6;
  if(b == 2){
    opcode = TOOL;
  }
  else if(b == 0){
    opcode = DX;
  }
  else if(b == 1){
    opcode = DY;
  }
  else if(b == 3){
    opcode = DATA;
  }
  return opcode; // this is a placeholder only
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  //TO DO

  b = b & 0x3f;
  char h = b;
  if((b >> 5) == 1){
    h = h | 0xc0;
  }

  return (int)h; // this is a placeholder only
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {

  int opcode = getOpcode(op);
  int operand = getOperand(op);

  if(opcode == TOOL) {
    if(operand == 0){
      s->tool = NONE;
      s->data = 0;
    }

    else if(operand == 1){
      s->tool = LINE;
      s->data = 0;
    }

    else if(operand == 2){
     s->tool = BLOCK;
     s->data = 0;
   }

    else if(operand == 3){
     colour(d,s->data);
     s->data = 0;
   }

    else if(operand == 4){
     s->tx = s->data;
     s->data = 0;
   }

    else if(operand == 5){
     s->ty = s->data;
     s->data = 0;
    }
  }

  else if(opcode == DX){
    s->tx = s->tx + operand;
  }

  else if(opcode == DY){
    s->ty = s->ty + operand;
    if(s->tool == LINE){
      line(d,s->x,s->y,s->tx,s->ty);
    }
    else if(s->tool == BLOCK){
      block(d, s->x, s->y, (s->tx - s->x), (s->ty - s->y));
      s->data = 0;
    }
    s->x = s->tx;
    s->y = s->ty;
  }

  else if(opcode == DATA){
    s->data = s->data << 6 | (operand & 0x3f);

  }
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {

    state *s = (state*) data;
    char *filename = getName(d);
    FILE *firstin = fopen(filename, "rb");
    byte opfirst = fgetc(firstin);

    if (data == NULL){
      return (pressedKey == 27);
    }
    while(! feof(firstin)){
      obey(d, s, opfirst);
      opfirst = fgetc(firstin);
    }

  show(d);
  fclose(firstin);
  // reset STATE

  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = 1;
  s->start = 0;
  s->data = 0;
  s->end = false;
  return (pressedKey == 27);
  }



    //TO DO: OPEN, PROCESS/DRAW A SKETCH FILE BYTE BY BYTE, THEN CLOSE IT
    //NOTE: CHECK DATA HAS BEEN INITIALISED... if (data == NULL) return (pressedKey == 27);
    //NOTE: TO GET ACCESS TO THE DRAWING STATE USE... state *s = (state*) data;
    //NOTE: TO GET THE FILENAME... char *filename = getName(d);
    //NOTE: DO NOT FORGET TO CALL show(d); AND TO RESET THE DRAWING STATE APART FROM
    //      THE 'START' FIELD AFTER CLOSING THE FILE


// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
