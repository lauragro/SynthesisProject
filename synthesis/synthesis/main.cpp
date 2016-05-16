#include "patchmatch.h"

#define MAGENTA -65281

// return the two's complement interpretation of a hex value
int twos_complement(int hex_value) {
	return hex_value - 0xffffff - 1;
}

// return the RGB values from a two's complement integer
int * get_RGB(int value) {
	int * rgb = new int[3];

	//value += (0xffffff + 1);	// converting first from two's complement is redundant

	rgb[0] = value & 0xff;			// red
	rgb[1] = (value >> 8) & 0xff;	// green
	rgb[2] = (value >> 16) & 0xff;	// blue

	return rgb;
}

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

	// testing
	cout << 0xff00ff << endl;

	// assign values to mask bitmap: <0 = hole, otherwise = not a hole
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {

			// testing
			/*if ((*image)[i][j] != -1) {
				if ((*image)[i][j] != -16777216){
					int temp = (*image)[i][j];
				}
			}*/

			// if the image is magenta here: twos_complement(0xff00ff) = -65281
			if ((*image)[i][j] == MAGENTA) {

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

// reconstruct target with source
BITMAP * e_step(BITMAP * source, BITMAP * target, BITMAP * mask, BITMAP * nnf, BITMAP * nnfd) {
	int nn_coords, x, y, red, green, blue, ave, colour;
	int * rgb;
	int w = target->w;
	int h = target->h;

	// capture the nearest neighbour field and distances to reconstruct target from source
	patchmatch(target, source, nnf, nnfd);

	// naive reconstruction
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {

			// replace target patch with nearest source patch
			nn_coords = (*nnf)[i][j];

			// if this element in the target is a hole
			if ( (*mask)[i][j] < 0 ){

				// find the average colour in the patch
				x = INT_TO_X(nn_coords);	// x-coord of pixel at centre of patch
				y = INT_TO_Y(nn_coords);	// y-coord of pixel at centre of patch
				red = green = blue = 0;	// reset for new calculation

				for (int ip = -patch_w / 2; ip < patch_w / 2; ip++) {
					for (int jp = -patch_w / 2; jp < patch_w / 2; jp++) {

						// get the current pixel colour
						colour = (*source)[y + ip][x + jp];

						// ignore values from the hole
						if (colour != MAGENTA) {

							// update the sums
							rgb = get_RGB(colour);
							red += rgb[0];
							green += rgb[1];
							blue += rgb[2];
						}
						
						//ave += (*source)[y + ip][x + jp];
					}
				}

				//ave = ave / (patch_w*patch_w);

				// calculate the average colour using rgb values
				ave = (red / (patch_w*patch_w))
					+ ((green / (patch_w*patch_w)) << 8)
					+ ((blue / (patch_w*patch_w)) << 16);

				// fill the hole
				(*target)[i][j] = ave;

				//int average = (*source)[INT_TO_Y(temp)][INT_TO_X(temp)]
				//(*target)[i][j] = (*source)[INT_TO_Y(temp)][INT_TO_X(temp)];	// TODO: use the average of pixels in NNF patch instead of hole patch
			}
			
			// testing
			//cout << (*mask)[i][j];
		}

		// testing
		//cout << endl;
	}

	return 0;
}

// minimize the energy (screened poisson equation)
int m_step(BITMAP * target) {

	BITMAP * gradient = NULL;

	

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

	// testing
	//int * rbg = get_RGB(-65280);

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

	// check parameters
	argc--;
	argv++;
	if (argc != 1) {
		fprintf(stderr, "Usage: synthesis source\n"
			"Where source is the image containing a magenta hole to be filled, outputs completed image."); 
		exit(1);
	}

	// load source image
	printf("Loading input image\n");
	BITMAP *source = load_bitmap(argv[0]);	// a copy for checking
	BITMAP *image = load_bitmap(argv[0]);	// a copy for editing

	// build mask: value<0 = hole, otherwise = not a hole
	printf("Building mask\n");
	BITMAP *mask = build_mask(image);		// TODO: build masks for all scales
	save_bitmap(mask, "mask.jpg");			// save to check logic

	printf("Running hole filling algorithm\n");

	// reduce the size to fill hole with fewer details first
	scale_down();	// not yet implemented
	initial_fill(image, mask);	// initialize hole indices to diagonal neighbour's value (upper-left). TODO: use Laplace or similar instead

	// implement an EM-algorithm
	for (i = 0; i < num_scales; i++) {

		// variables for capturing nearest neighbour field and distances at current scale
		BITMAP *ann = NULL;
		BITMAP *annd = NULL;

		for (j = 0; j < num_em; j++) {

			e_step(source, image, mask, ann, annd);	// replaces hole patches with nnf patches. TODO: use channels and gradients like image melding does
			m_step(image);	// in progress
		}

		output_image(); // not yet implemented, probably redundant. TODO: use save_bitmap() instead
		upscale();	// not yet implemented

		// clear variables from current scale
		//delete ann;
		//delete annd;
		//delete image;
	}
	save_bitmap(image, "output.jpg");

	return 0;
}