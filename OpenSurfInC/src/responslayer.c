#include "responselayer.h"

#define R1 279040
#define R2 69760
#define R3 17280
#define R4 4320
#define R5 1040
#define L1 69760
#define L2 17440
#define L3 4320
#define L4 1080
#define L5 260

float res1a[R1];
float res1b[R1];
float res1c[R1];
float res1d[R1];

float res2a[R2];
float res2b[R2];

float res3a[R3];
float res3b[R3];

float res4a[R4];
float res4b[R4];

float res5a[R5];
float res5b[R5];

unsigned char lap1a[L1];
unsigned char lap1b[L1];
unsigned char lap1c[L1];
unsigned char lap1d[L1];

unsigned char lap2a[L2];
unsigned char lap2b[L2];

unsigned char lap3a[L3];
unsigned char lap3b[L3];

unsigned char lap4a[L4];
unsigned char lap4b[L4];

unsigned char lap5a[L5];
unsigned char lap5b[L5];

void ResLayer(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res1a , lap1a};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer2(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res1b , lap1b};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer3(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res1c , lap1c};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer4(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res1d , lap1d};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer5(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res2a , lap2a};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer6(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res2b , lap2b};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer7(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res3a , lap3a};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer8(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res3b , lap3b};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer9(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res4a , lap4a};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer10(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res4b , lap4b};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer11(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res5a , lap5a};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}

void ResLayer12(ResponseLayer *Res, int width, int height, int step, int filter)
{
	ResponseLayer es = {width, height, step, filter , res5b , lap5b};

	assert(width > 0 && height > 0);

	Res->width = width;
	Res->height = height;
	Res->step = step;
	Res->filter = filter;

	memset(es.responses,0,sizeof(float)*width*height);
    memset(es.laplacian,0,sizeof(unsigned char)*width*height);

	Res->responses = es.responses;
	Res->laplacian = es.laplacian;
}


