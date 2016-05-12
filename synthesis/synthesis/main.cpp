#include "patchmatch.h"

//int main(int argc, char *argv[]) {
//
//	/* Patch Match algorithm to produce nearest neighbour field and distance matrix */
//	argc--;
//	argv++;
//	if (argc != 4) {
//		fprintf(stderr, "pm_minimal a b ann annd\n"
//			"Given input images a, b outputs nearest neighbor field 'ann' mapping a => b coords, and the squared L2 distance 'annd'\n"
//			"These are stored as RGB 24-bit images, with a 24-bit int at every pixel. For the NNF we store (by<<12)|bx."); exit(1);
//	}
//	printf("Loading input images\n");
//	BITMAP *a = load_bitmap(argv[0]);
//	BITMAP *b = load_bitmap(argv[1]);
//	BITMAP *ann = NULL, *annd = NULL;
//	printf("Running PatchMatch\n");
//	patchmatch(a, b, ann, annd);
//	printf("Saving output images\n");
//	save_bitmap(ann, argv[2]);
//	save_bitmap(annd, argv[3]);
//
//	/* Texture Synthesis algorithm derived from Image Melding */
//
//
//	return 0;
//}