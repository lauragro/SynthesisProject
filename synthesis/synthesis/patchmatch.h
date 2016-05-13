#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <climits>	
#include <iostream>	

using namespace std;

#ifndef MAX
	#define MAX(a, b) ((a)>(b)?(a):(b))
	#define MIN(a, b) ((a)<(b)?(a):(b))
#endif

/* -------------------------------------------------------------------------
BITMAP: Minimal image class
------------------------------------------------------------------------- */

class BITMAP {
public:
	int w, h;
	int *data;
	BITMAP(int w_, int h_) :w(w_), h(h_) { data = new int[w*h]; }
	~BITMAP() { delete[] data; }
	int *operator[](int y) { return &data[y*w]; }
};

#define XY_TO_INT(x, y) (((y)<<12)|(x))
#define INT_TO_X(v) ((v)&((1<<12)-1))
#define INT_TO_Y(v) ((v)>>12)



void check_im();
BITMAP *load_bitmap(const char *filename);
void save_bitmap(BITMAP *bmp, const char *filename);
int dist(BITMAP *a, BITMAP *b, int ax, int ay, int bx, int by, int cutoff);
void improve_guess(BITMAP *a, BITMAP *b, int ax, int ay, int &xbest, int &ybest, int &dbest, int bx, int by);
void patchmatch(BITMAP *a, BITMAP *b, BITMAP *&ann, BITMAP *&annd);


