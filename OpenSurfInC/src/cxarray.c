#include "cxarray.h"

CvMat* 
CreateMat(int height, int widht, int type)
{
	CvMat* arr = CreateMatHeader(height, widht, type);
	CreateData(arr);

	return arr;
}

CvMat* 
CreateMatHeader(int rows, int cols, int type)
{
	CvMat *arr;
	int min_step;

	if( rows <= 0 || cols <= 0 )
		return -1;

	min_step = CV_ELEM_SIZE(type)*cols;
	if( min_step <= 0 )
		return -1;

	arr = (CvMat*)malloc( sizeof(*arr));

	arr->step = min_step;
    arr->type = CV_MAT_MAGIC_VAL | type | CV_MAT_CONT_FLAG;
	arr->rows = rows;
    arr->cols = cols;
    arr->data.ptr = 0;
    arr->refcount = 0;
    arr->hdr_refcount = 1;

	CheckHuge(arr);

	return arr;
}

static void CheckHuge( CvMat* arr )
{
    if( (int64)arr->step*arr->rows > INT_MAX )
        arr->type &= ~CV_MAT_CONT_FLAG;
}

// Allocates underlying array data
void 
CreateData( CvMat* arr )
{
	if( CV_IS_MAT_HDR( arr ))
    {
		size_t step, total_size;
		int64 _total_size;
        CvMat* mat = (CvMat*)arr;
        step = mat->step;

		if( mat->data.ptr != 0 )
			return -1;

		if( step == 0 )
            step = CV_ELEM_SIZE(mat->type)*mat->cols;

		_total_size = (int64)step*mat->rows + sizeof(int) + CV_MALLOC_ALIGN;
		total_size = (size_t)_total_size;

		if(_total_size != (int64)total_size)
			return -1;

		mat->refcount = (int*)malloc( (size_t)total_size );
		mat->data.ptr = (uchar*)AlignPtr( mat->refcount + 1, CV_MALLOC_ALIGN );
		*mat->refcount = 1;
	}
	else if( CV_IS_IMAGE_HDR(arr))
	{
		IplImage* img = (IplImage*)arr;

		if( img->imageData != 0 )
			return -1;

		if( !CvIPL.allocateData )
        {
            img->imageData = img->imageDataOrigin = 
                        (char*)malloc( (size_t)img->imageSize );
        }
		else
        {
            int depth = img->depth;
            int width = img->width;

            if( img->depth == IPL_DEPTH_32F || img->depth == IPL_DEPTH_64F )
            {
                img->width *= img->depth == IPL_DEPTH_32F ? sizeof(float) : sizeof(double);
                img->depth = IPL_DEPTH_8U;
            }

            CvIPL.allocateData( img, 0, 0 );

            img->width = width;
            img->depth = depth;
        }
	}
	else if( CV_IS_MATND_HDR( arr ))
	{
		CvMatND* mat = (CvMatND*)arr;
        int i;
        size_t total_size = CV_ELEM_SIZE(mat->type);

		if( mat->data.ptr != 0 )
			return -1;
		
		if( CV_IS_MAT_CONT( mat->type ))
        {
            total_size = (size_t)mat->dim[0].size*(mat->dim[0].step != 0 ?
                         mat->dim[0].step : total_size);
        }
		else
        {
            for( i = mat->dims - 1; i >= 0; i-- )
            {
                size_t size = (size_t)mat->dim[i].step*mat->dim[i].size;

                if( total_size < size )
                    total_size = size;
            }
        }

		mat->refcount = (int*)malloc( total_size +
                                        sizeof(int) + CV_MALLOC_ALIGN );
        mat->data.ptr = (uchar*)AlignPtr( mat->refcount + 1, CV_MALLOC_ALIGN );
        *mat->refcount = 1;
	}
	else
		return -1;
}

// Initializes CvMat header, allocated by the user
CvMat* InitMatHeader( CvMat* arr, int rows, int cols, int type, void* data, int step )
{
	int pix_size, min_step;

	if( !arr )
        return -1;

    if( (unsigned)CV_MAT_DEPTH(type) > CV_DEPTH_MAX )
        return -1;

    if( rows <= 0 || cols <= 0 )
        return -1;

	type = CV_MAT_TYPE( type );
    arr->type = type | CV_MAT_MAGIC_VAL;
    arr->rows = rows;
    arr->cols = cols;
    arr->data.ptr = (uchar*)data;
    arr->refcount = 0;
    arr->hdr_refcount = 0;

	pix_size = CV_ELEM_SIZE(type);
    min_step = arr->cols*pix_size;

	if( step != CV_AUTOSTEP && step != 0 )
	{
		if( step < min_step )
            return -1;
        arr->step = step;
	}
	else
    {
        arr->step = min_step;
    }
	
	arr->type = CV_MAT_MAGIC_VAL | type |
        (arr->rows == 1 || arr->step == min_step ? CV_MAT_CONT_FLAG : 0);

    CheckHuge( arr );
    return arr;
}

CvMat* GetMat( const CvArr* array, CvMat* mat, int* pCOI, int allowND )
{
	CvMat* result = 0;
    CvMat* src = (CvMat*)array;
    int coi = 0;
	
	if( !mat || !src )
		return -1;

	if( CV_IS_MAT_HDR(src))
    {
        if( !src->data.ptr )
            return -1;
        
        result = (CvMat*)src;
    }
	else if( CV_IS_IMAGE_HDR(src) )
	{
		const IplImage* img = (const IplImage*)src;
        int depth, order;

		if( img->imageData == 0 )
            return -1;

		depth = IPL2CV_DEPTH( img->depth );
        if( depth < 0 )
            return -1;

		order = img->dataOrder & (img->nChannels > 1 ? -1 : 0);

		if( img->roi )
		{
			if( order == IPL_DATA_ORDER_PLANE )
			{
				int type = depth;

                if( img->roi->coi == 0 )
					return -1;

				InitMatHeader( mat, img->roi->height,
                                img->roi->width, type,
                                img->imageData + (img->roi->coi-1)*img->imageSize +
                                img->roi->yOffset*img->widthStep +
                                img->roi->xOffset*CV_ELEM_SIZE(type),
                                img->widthStep );
			}
			else /* pixel order */
            {
				int type = CV_MAKETYPE( depth, img->nChannels );
                coi = img->roi->coi;

                if( img->nChannels > CV_CN_MAX )
                        return -1;

                InitMatHeader( mat, img->roi->height, img->roi->width,
                                 type, img->imageData +
                                 img->roi->yOffset*img->widthStep +
                                 img->roi->xOffset*CV_ELEM_SIZE(type),
                                 img->widthStep );
			}
		}
		else
        {
            int type = CV_MAKETYPE( depth, img->nChannels );

            if( order != IPL_DATA_ORDER_PIXEL )
                return -1;

            InitMatHeader( mat, img->height, img->width, type,
                             img->imageData, img->widthStep );
        }

        result = mat;
	}
	else if( allowND && CV_IS_MATND_HDR(src) )
    {
		CvMatND* matnd = (CvMatND*)src;
        int i;
        int size1 = matnd->dim[0].size, size2 = 1;

		if( !src->data.ptr )
            return -1;

        if( !CV_IS_MAT_CONT( matnd->type ))
            return -1;

        if( matnd->dims > 2 )
            for( i = 1; i < matnd->dims; i++ )
                size2 *= matnd->dim[i].size;
        else
            size2 = matnd->dims == 1 ? 1 : matnd->dim[1].size;

        mat->refcount = 0;
        mat->hdr_refcount = 0;
        mat->data.ptr = matnd->data.ptr;
        mat->rows = size1;
        mat->cols = size2;
        mat->type = CV_MAT_TYPE(matnd->type) | CV_MAT_MAGIC_VAL | CV_MAT_CONT_FLAG;
        mat->step = size2*CV_ELEM_SIZE(matnd->type);
        mat->step &= size1 > 1 ? -1 : 0;

        CheckHuge( mat );
        result = mat;
	}
	else
        return -1;

    if( pCOI )
        *pCOI = coi;

	return result;
}