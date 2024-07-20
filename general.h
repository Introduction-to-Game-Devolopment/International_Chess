#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>
#include <string>
#include <string.h>
#include "raylib/src/raylib.h"

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int y;
    int x;
} XY;

typedef struct {
    int w, h;
} size;

const Color ODD_CELL_COLOR = {54, 54, 54, 255};
const Color EVEN_CELL_COLOR = {217, 217, 217, 255};
const Color CHOSEN_CELL_COLOR = {165, 111, 111, 255};
const Color HOVERED_CELL_COLOR = {165, 111, 111, 125};

const size FRAME_SIZE = {1650, 980};
const int PADDING = 40;
