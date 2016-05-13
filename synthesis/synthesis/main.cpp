#include "patchmatch.h"

// use the source image to return a bitmap whose data is <0 corresponding to the hole to be filled
BITMAP * build_mask(BITMAP * image) {
	// create mask bitmap same size as image bitmap
	int w = image->w;
	int h = image->h;
	BITMAP *mask = new BITMAP(w, h);
	memset(mask->data, 0, sizeof(int)*w*h);	// initialize all values to 0

	// pointers to access each data value in image and mask
	unsigned char *image_pointer = (unsigned char *)image->data;
	unsigned char *mask_pointer = (unsigned char *)mask->data;

	// assign values to mask bitmap: <0 = hole, otherwise = not a hole
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {

			/*if ((*image)[i][j] != -1) {
				if ((*image)[i][j] != -16777216){
					int temp = (*image)[i][j];
				}
			}*/

			// if the image is magenta here
			if ((*image)[i][j] == -65281) {

				// identify this point as a hole in the mask
				(*mask)[i][j] = -1;
			}
		}
	}

	return mask;
}

int scale_down() {
	return 0;
}

// fill in the hole of the image with an initial guess
int initial_fill(BITMAP * image, BITMAP * mask) {
	int temp;
	int w = image->w;
	int h = image->h;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {

			// if this is part of the hole
			if ((*mask)[i][j] < 0) {

				// initialize to diagonal neighbour's value (upper-left)
				temp = (*image)[i - 1][j - 1]; 
				(*image)[i][j] = temp;
			}
		}
	}

	return 0;
}

int e_step() {
	return 0;
}

int m_step() {
	return 0;
}

int output_image() {
	return 0;
}

int upscale() {
	return 0;
}


int main(int argc, char *argv[]) {
	/* Control variables */
	int num_scales, num_em, i, j;
	num_scales = 1;
	num_em = 1;

	/* Patch Match algorithm to produce nearest neighbour field and distance matrix */
	/*argc--;
	argv++;
	if (argc != 4) {
		fprintf(stderr, "pm_minimal a b ann annd\n"
			"Given input images a, b outputs nearest neighbor field 'ann' mapping a => b coords, and the squared L2 distance 'annd'\n"
			"These are stored as RGB 24-bit images, with a 24-bit int at every pixel. For the NNF we store (by<<12)|bx."); exit(1);
	}
	printf("Loading input images\n");
	BITMAP *a = load_bitmap(argv[0]);
	BITMAP *b = load_bitmap(argv[1]);
	BITMAP *ann = NULL, *annd = NULL;
	printf("Running PatchMatch\n");
	patchmatch(a, b, ann, annd);
	printf("Saving output images\n");
	save_bitmap(ann, argv[2]);
	save_bitmap(annd, argv[3]);*/

	/* Hole filling algorithm derived from Image Melding */
	argc--;
	argv++;
	if (argc != 1) {
		fprintf(stderr, "Usage: synthesis source\n"
			"Where source is the image containing a magenta hole to be filled, outputs completed image."); 
		exit(1);
	}
	printf("Loading input image\n");
	BITMAP *image = load_bitmap(argv[0]);	// load source image
	printf("Building mask\n");
	BITMAP *mask = build_mask(image);	// <0 = hole, otherwise = not a hole
	save_bitmap(mask, "mask.jpg");	// save mask to check results
	printf("Running hole filling algorithm\n");
	scale_down();	// not yet implemented
	initial_fill(image, mask);	// initialize hole indices to diagonal neighbour's value (upper-left)
	for (i = 0; i < num_scales; i++) {
		for (j = 0; j < num_em; j++) {
			e_step();	// not yet implemented
			m_step();	// not yet implemented
		}
		output_image();
		upscale();	// not yet implemented
	}
	save_bitmap(image, "output.jpg");

	return 0;
}