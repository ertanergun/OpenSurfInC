#include "fasthassian.h"

static int count = 0;

//! Constructor with image
void FastH(FastHessian *fhts, IplImage *img)
{
	int i;
	fhts->img = img;
	fhts->i_height = img->height;
	fhts->i_width = img->width;
	for(i=0; i<N; i++){
		fhts->ipts[i].x = 0;
		fhts->ipts[i].y = 0;
		fhts->ipts[i].clusterIndex = 0;
		fhts->ipts[i].dx = 0;
		fhts->ipts[i].dy = 0;
		fhts->ipts[i].laplacian = 0;
		fhts->ipts[i].orientation = 0;
		fhts->ipts[i].scale = 0;
	}
	fhts->octaves = OCTAVES; 
    fhts->intervals = INTERVALS; 
    fhts->init_sample = INIT_SAMPLE;
    fhts->thresh = THRES;	
}

int getIpoints(FastHessian *fh)
{
	int o,i,r,c;
	ResponseLayer *b, *m, *t;

	// filter index map // variables should be OCTAVES AND INTERVAL BUT THERE IS AN ERROR
	static const int filter_map [5][4] = {{0,1,2,3}, {1,3,4,5}, {3,5,6,7}, {5,7,8,9}, {7,9,10,11}};

	// Build the response map
	buildResponseMap(fh);
}

void buildResponseMap(FastHessian *fh)
{
	int w = 0,h = 0,s = 0;
	unsigned int i = 0;

	// Get image attributes
	w = (fh->i_width / fh->init_sample);
	h = (fh->i_height / fh->init_sample);
	s = (fh->init_sample);

	// Calculate approximated determinant of hessian values
  if (fh->octaves >= 1)
  {
		ResLayer(&fh->responseMap[0],w,   h,   s,   9);
		ResLayer2(&fh->responseMap[1],w,   h,   s,   15);
		ResLayer3(&fh->responseMap[2],w,   h,   s,   21);
		ResLayer4(&fh->responseMap[3],w,   h,   s,   27);
  }

   if (fh->octaves >= 2)
  {
		ResLayer5(&fh->responseMap[4],w/2, h/2, s*2, 39);
		ResLayer6(&fh->responseMap[5],w/2, h/2, s*2, 51);
  }

   if (fh->octaves >= 3)
  {
		ResLayer7(&fh->responseMap[6],w/4, h/4, s*4, 75);
		ResLayer8(&fh->responseMap[7],w/4, h/4, s*4, 99);
  }

	 if (fh->octaves >= 4)
  {
		ResLayer9(&fh->responseMap[8],w/8, h/8, s*8, 147);
		ResLayer10(&fh->responseMap[9],w/8, h/8, s*8, 195);
  }

	  if (fh->octaves >= 5)
  {
		ResLayer11(&fh->responseMap[10],w/16, h/16, s*16, 291);
		ResLayer12(&fh->responseMap[11],w/16, h/16, s*16, 387);
  }

	// Extract responses from the image
	for (i = 0; i<12 ; ++i)
	{
		buildResponseLayer(fh, &fh->responseMap[i]);
	}
}

void buildResponseLayer(FastHessian *fh, ResponseLayer *rl)
{
	float *responses;				// response storage
	unsigned char *laplacian;		// laplacian sign storage
	int step;						// step size for this filter
	int b;							// border for this filter
	int l;							// lobe for this filter (filter size / 3)
	int w;							// filter size
	float inverse_area;				// normalisation factor
	float Dxx, Dyy, Dxy;
	int r, c, ar = 0, index = 0,ac;

	responses = rl->responses;
	laplacian = rl->laplacian;
	step = rl->step;
	b = (rl->filter - 1) / 2;
	l = rl->filter / 3;
	w = rl->filter;
	inverse_area = 1.f/(w*w);

	for(ar = 0; ar < rl->height; ++ar) 
		{
			for(ac = 0; ac < rl->width; ++ac, index++) 
				{
					// get the image coordinates
					r = ar * step;
					c = ac * step; 

					// Compute response components
				  Dxx = BoxIntegral(fh->img, r - l + 1, c - b, 2*l - 1, w)
					  - BoxIntegral(fh->img, r - l + 1, c - l / 2, 2*l - 1, l)*3;

				  Dyy = BoxIntegral(fh->img, r - b, c - l + 1, w, 2*l - 1)
					  - BoxIntegral(fh->img, r - l / 2, c - l + 1, l, 2*l - 1)*3;

				  Dxy = + BoxIntegral(fh->img, r - l, c + 1, l, l)
						+ BoxIntegral(fh->img, r + 1, c - l, l, l)
						- BoxIntegral(fh->img, r - l, c - l, l, l)
						- BoxIntegral(fh->img, r + 1, c + 1, l, l);

				  // Normalise the filter responses with respect to their size
				  Dxx *= inverse_area;
				  Dyy *= inverse_area;
				  Dxy *= inverse_area;

				  // Get the determinant of hessian response & laplacian sign
					responses[index] = (Dxx * Dyy - 0.81f * Dxy * Dxy);
					laplacian[index] = (Dxx + Dyy >= 0 ? 1 : 0);

				}
		}
}

//! Performs one step of extremum interpolation. 
//void interpolateStep(int r, int c, ResponseLayer *t, ResponseLayer *m, ResponseLayer *b, double* xi, double* xr, double* xc )
void interpolateStep()
{
	CvMat* dD, * H, * H_inv, X;
	double x[3] = { 0 };

	/*dD = deriv3D( r, c, t, m, b );
	H = hessian3D( r, c, t, m, b );*/
	H_inv = CreateMat( 3, 3, CV_64FC1 );
	cvInvert( H, H_inv, CV_SVD );
	/*cvInitMatHeader( &X, 3, 1, CV_64FC1, x, CV_AUTOSTEP );
	cvGEMM( H_inv, dD, -1, NULL, 0, &X, 0 );

	cvReleaseMat( &dD );
	cvReleaseMat( &H );
	cvReleaseMat( &H_inv );

	*xi = x[2];
	*xr = x[1];
	*xc = x[0];*/
}