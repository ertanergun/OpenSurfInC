#include <assert.h>
typedef struct {
	
	int width;
	int	height; 
	int	step;
	int	filter;
	float *responses;
	unsigned char *laplacian;

}ResponseLayer;

void ResLayer(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer2(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer3(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer4(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer5(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer6(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer7(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer8(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer9(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer10(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer11(ResponseLayer *RL, int width, int height, int step, int filter);
void ResLayer12(ResponseLayer *RL, int width, int height, int step, int filter);
//_inline float getResponse(unsigned int row, unsigned int column, ResponseLayer *src, ResponseLayer *R);

_inline float get_Response(unsigned int row, unsigned int column, ResponseLayer *R)
  {
    return R->responses[row * R->width + column];
  }


_inline float getResponse(unsigned int row, unsigned int column, ResponseLayer *src, ResponseLayer *R)
  {
    int scale;
    scale = R->width / src->width;

    #ifdef RL_DEBUG
    assert(src->getCoords(row, column) == this->getCoords(scale * row, scale * column));
    #endif

    return R->responses[(scale * row) * R->width + (scale * column)];
  }

_inline unsigned char getLaplacian(unsigned int row, unsigned int column, ResponseLayer *src, ResponseLayer *R)
  {
    int scale = R->width / src->width;

    #ifdef RL_DEBUG
    assert(src->getCoords(row, column) == this->getCoords(scale * row, scale * column));
    #endif

    return R->laplacian[(scale * row) * R->width + (scale * column)];
  }