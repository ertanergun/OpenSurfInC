#include <time.h>
#include "surflib.h"

char imageData[262144];

int mainImage()
{
	int i,ipts_size = 0;
	float tclock;
	clock_t start, end;
	Ipoint IpArray[N];
	IplImage img;
	int x,y, count = 0;
	char someChar;
	char *pChar  = &someChar;

	// Detect and describe interest points in the image
	start = clock();
	ipts_size = surfDetDes(&img, IpArray, 0, 5, 4, 2, 0.0004f, N);
	end = clock();

	tclock = (float)(end - start) / CLOCKS_PER_SEC;

	printf("OpenSURF found: %d interest points \n",ipts_size);
	printf("OpenSURF took: %f seconds \n",tclock);

}

int main()
{
	return 0;
}