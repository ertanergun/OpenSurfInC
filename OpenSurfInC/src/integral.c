#include "utils.h"


float x_data[1048576];
IplImage *Integral(IplImage *img)
{
	IplImage *int_img = img;

	// set up variables for data access
  int height = img->height;
  int width = img->width;
  int j,i;
  int step = img->widthStep/sizeof(float);
  float *data   = (float *) img->imageData;  
  float *i_data = (float *) int_img->imageData = x_data; 

  // first row only
  float rs = 0.0f;
  for(j=0; j<width; j++) 
  {
    rs += data[j]; 
    i_data[j] = rs;
  }

  // remaining cells are sum above and to the left
  for(i=1; i<height; ++i) 
  {
    rs = 0.0f;
    for(j=0; j<width; ++j) 
    {
      rs += data[i*step+j]; 
      i_data[i*step+j] = rs + i_data[(i-1)*step+j];
    }
  }

  // release the gray image
  //cvReleaseImage(&img);
	
  // return the integral image
  return int_img;
}