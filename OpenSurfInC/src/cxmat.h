//#include "cxcore.h"
#include "mixChannels_template.h"

typedef void (*MixChannelsFunc)( const void** src, const int* sdelta0,
        const int* sdelta1, void** dst, const int* ddelta0, const int* ddelta1, int n, Size size );

int isContinuous(Mat* mat)
{
	return (mat->flags & CONTINUOUS_FLAG) != 0;
}

int getChannels(Mat* mat)
{
	return CV_MAT_CN(mat->flags);
}

int getDepth(Mat* mat)
{
	return CV_MAT_DEPTH(mat->flags);
}

int getType(Mat* mat)
{
	return CV_MAT_TYPE(mat->flags);
}

static _inline size_t alignSize(size_t sz, int n)
{
    return (sz + n-1) & -n;
}

_inline size_t getElemSize(Mat* mat)
{
	return CV_ELEM_SIZE(mat->flags);
}

_inline void MatRelease(Mat* mat)
{
	if( mat->refcount && CV_XADD(mat->refcount, -1) == 1 )
        free(mat->datastart);
    mat->data = mat->datastart = mat->dataend = 0;
    mat->step = mat->rows = mat->cols = 0;
    mat->refcount = 0;
}

void createAutoAutoBuffer_uchar(AutoBuffer_uchar *a, size_t _size)
{
	uchar temp[fixed_size]; //control the size and arrange an average size

	if(_size > fixed_size)
	{
		a->ptr = temp;
		a->size = _size;
	}
	else
	{
		a->ptr = a->buf;
		a->size = fixed_size;
	}
}

Size getSize(int cols, int rows)
{
	Size size;
	size.width = cols;
	size.height = rows;
	return size;
}

int CompareSize(Size *a, Size *b)
{
	int result = 0;
	if(a->width == b->width)
	{
		if(a->height == b->height )
			result = 1;
	}
	return result;
}

void mixChannels(const Mat* src, int nsrcs, Mat* dst, int ndsts, const int* fromTo, size_t npairs)
{
	size_t i;
	int depth, esz1, _isContinuous;
	Size size, tempSize;
	AutoBuffer_uchar buf;
	void** srcs;
	void** dsts;
	int *s0,*s1,*d0,*d1;
	MixChannelsFunc func = 0;
    
    if( npairs == 0 )
        return;

	assert( src && nsrcs > 0 && dst && ndsts > 0 && fromTo && npairs > 0 );

	depth = CV_MAT_DEPTH(dst[0].flags); 
	esz1 = (int)CV_ELEM_SIZE1(dst[0].flags);
	size = getSize(dst[0].cols,dst[0].rows);
	
	createAutoAutoBuffer_uchar(&buf,npairs*(sizeof(void*)*2 + sizeof(int)*4));

	srcs = (void**)(uchar*)&buf;
    dsts = srcs + npairs;

	*s0 = (int*)(dsts + npairs);
	*s1 = s0 + npairs;
	*d0 = s1 + npairs;
	*d1 = d0 + npairs;

    _isContinuous = 1; //bool isContinuous = true;

	for( i = 0; i < npairs; i++ )
	{
		int i0 = fromTo[i*2], i1 = fromTo[i*2+1], j;
		if( i0 >= 0 )
        {
			for( j = 0; j < nsrcs; i0 -= getChannels(&src[j]), j++ )
                if( i0 < CV_MAT_CN(src[j].flags) )
                    break;

			tempSize = getSize(src[j].cols, src[j].rows);
			assert(j < nsrcs && CompareSize(&tempSize, &size) && getDepth(&src[j]) == depth);
			_isContinuous = _isContinuous && isContinuous(&src[j]);
			srcs[i] = src[j].data + i0*esz1;
			s1[i] = getChannels(&src[j]); s0[i] = (int)src[j].step/esz1 - size.width*getChannels(&src[j]);
		}
		else
        {
            srcs[i] = 0; s1[i] = s0[i] = 0;
        }

		for( j = 0; j < ndsts; i1 -= getChannels(&dst[j]), j++ )
            if( i1 < getChannels(&dst[j]) )
                break;
		tempSize = getSize(dst[j].cols, dst[j].rows);
		assert(i1 >= 0 && j < ndsts && CompareSize(&tempSize, &size) && getDepth(&dst[j]) == depth);
		_isContinuous = _isContinuous && isContinuous(&dst[j]);
		dsts[i] = dst[j].data + i1*esz1;
		d1[i] = getChannels(&dst[j]) ; d0[i] = (int)dst[j].step/esz1 - size.width*getChannels(&dst[j]);
	}

	
    if( esz1 == 1 )
        func = mixChannels_uchar;
    else if( esz1 == 2 )
		func = mixChannels_ushort;
    else if( esz1 == 4 )  
		func = mixChannels_int;
    else if( esz1 == 8 )
		func = mixChannels_int64;
    else
        return -1;    

    if( _isContinuous )
    {
        size.width *= size.height;
        size.height = 1;
    }
    func( (const void**)srcs, s0, s1, dsts, d0, d1, (int)npairs, size );
}

_inline Mat* MatCreate(int _rows, int _cols, int _type)
{
	Mat* mat;
	_type &= TYPE_MASK;

	if( mat->rows == _rows && mat->cols == _cols && getType(mat) == _type && mat->data )
        return;

	if( mat->data )
        free(mat->data);

	assert( _rows >= 0 && _cols >= 0 );

	if( _rows > 0 && _cols > 0 )
	{
		int64 _nettosize;
		size_t nettosize, datasize;
		mat->flags = MAGIC_VAL + CONTINUOUS_FLAG + _type;
        mat->rows = _rows;
        mat->cols = _cols;
		mat->step = getElemSize(mat)*mat->cols;
		_nettosize = (int64)mat->step*mat->rows;
        nettosize = (size_t)_nettosize;
		if( _nettosize != (int64)nettosize )
			return -1;
		datasize = alignSize(nettosize, (int)sizeof(*mat->refcount));
        mat->datastart = mat->data = (uchar*)malloc(datasize + sizeof(*mat->refcount));
        mat->dataend = mat->data + nettosize;
        mat->refcount = (int*)(mat->data + datasize);
        *mat->refcount = 1;
	}

	return mat;
}

void CreateMatFromInput(Mat* mat,int _rows, int _cols, int _type)
{
	int64 _nettosize;
	size_t nettosize;
	size_t datasize;

	_type = TYPE_MASK;
	if( mat->rows == _rows && mat->cols == _cols && CV_MAT_TYPE(mat->flags) == _type && mat->data )
        return;

	if( mat->data )
        MatRelease(mat);

	assert(_rows >= 0 && _cols >= 0 );

	if( _rows > 0 && _cols > 0 )
	{
		mat->flags = MAGIC_VAL + CONTINUOUS_FLAG + _type;
        mat->rows = _rows;
        mat->cols = _cols;
        mat->step = CV_ELEM_SIZE(mat->flags)*mat->cols;
		_nettosize = (int64)mat->step*mat->rows;
        nettosize = (size_t)_nettosize;
		if( _nettosize != (int64)nettosize )
			return -1;
		datasize = alignSize(nettosize, (int)sizeof(*mat->refcount));
		mat->datastart = mat->data = (uchar*)malloc(datasize + sizeof(*mat->refcount));
		mat->dataend = mat->data + nettosize;
        mat->refcount = (int*)(mat->data + datasize);
        *mat->refcount = 1;
	}
}

void copyMat(Mat* src, Mat* dst)
{
	const uchar* sptr;
	uchar* dptr;
	Size sz;

	if( src->data == dst->data )
        return;

	CreateMatFromInput(src, src->rows, src->cols, CV_MAT_TYPE(src->flags) );
    sptr = src->data;
    dptr = dst->data;

	sz.width *= (int)CV_ELEM_SIZE(src->flags);
    if( isContinuous(src) && isContinuous(dst) )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

	for( ; sz.height--; sptr += src->step, dptr += dst->step )
        memcpy( dptr, sptr, sz.width );
}

_inline Mat CreateMatCopyObject(int _rows, int _cols, int _type, void* _data, size_t _step)
{
	Mat mat;
	size_t minstep ;
	mat.flags = MAGIC_VAL + (_type & TYPE_MASK);
	mat.rows = _rows;
	mat.cols = _cols;
	mat.datastart = (uchar*) _data;
	mat.dataend = (uchar*) _data;

	minstep = mat.cols*CV_ELEM_SIZE(mat.flags);
    if( mat.step == AUTO_STEP )
    {
        mat.step = minstep;
        mat.flags |= CONTINUOUS_FLAG;
    }
	else
    {
        if( mat.rows == 1 ) mat.step = minstep;
        assert( mat.step >= minstep );
        mat.flags |= mat.step == minstep ? CONTINUOUS_FLAG : 0;
    }
    mat.dataend += mat.step*(mat.rows-1) + minstep;

	return mat;
}


_inline Mat CreateMatObject(const CvMat* m, int copyData)
{
	Mat mat;
	size_t minstep;
	mat.flags = MAGIC_VAL + (m->type & (CV_MAT_TYPE_MASK|CV_MAT_CONT_FLAG));
	mat.rows = m->rows;
	mat.cols = m->cols;
	mat.step = m->step;
	mat.data = m->data.ptr;
	mat.refcount = 0;
	mat.datastart = m->data.ptr;
	mat.dataend = m->data.ptr;

	if(mat.step == 0)
		mat.step = mat.cols*CV_ELEM_SIZE(mat.flags);
	minstep = mat.cols * CV_ELEM_SIZE(mat.flags);
	mat.dataend += mat.step*(mat.rows-1) + minstep;

	if( copyData )
    {
        mat.data = mat.datastart = mat.dataend = 0;
        mat = CreateMatCopyObject(m->rows, m->cols, m->type, m->data.ptr, m->step);
        copyMat(&mat, &mat);        
    }

	return mat;
}
//incomplete*****************
Mat CreateMatImage(const IplImage* img, int copyData)
{
	Mat mat, *m;
	int depth, selectedPlane;
	size_t esz;
	//int ch[2];

	assert(CV_IS_IMAGE(img) && img->imageData != 0);
	depth = IPL2CV_DEPTH(img->depth);
    mat.step = img->widthStep;
    mat.refcount = 0;

	if(!img->roi)
    {
        assert(img->dataOrder == IPL_DATA_ORDER_PIXEL);
        mat.flags = MAGIC_VAL + CV_MAKETYPE(depth, img->nChannels);
        mat.rows = img->height; 
		mat.cols = img->width;
        mat.datastart = mat.data = (uchar*)img->imageData;
        esz = CV_ELEM_SIZE(mat.flags); 
    }
	else
	{
		assert(img->dataOrder == IPL_DATA_ORDER_PIXEL || img->roi->coi != 0);
        selectedPlane = img->roi->coi && img->dataOrder == IPL_DATA_ORDER_PLANE;
        mat.flags = MAGIC_VAL + CV_MAKETYPE(depth, selectedPlane ? 1 : img->nChannels);
        mat.rows = img->roi->height; 
		mat.cols = img->roi->width;
        esz = CV_ELEM_SIZE(mat.flags);
        mat.data = mat.datastart = (uchar*)img->imageData +
			(selectedPlane ? (img->roi->coi - 1)*mat.step*img->height : 0) +
			img->roi->yOffset*mat.step + img->roi->xOffset*esz;
	}
	mat.dataend = mat.datastart + mat.step*(mat.rows-1) + esz*mat.cols;
    mat.flags |= (mat.cols*esz == mat.step || mat.rows == 1 ? CONTINUOUS_FLAG : 0);

	if( copyData )
    {
        m = &mat;
        mat.rows = mat.cols = 0;
        if( !img->roi || !img->roi->coi || img->dataOrder == IPL_DATA_ORDER_PLANE)
		{
			copyMat(&mat,m);
		}
        else
        {
            int ch[] = {img->roi->coi - 1, 0};
			CreateMatFromInput(&mat,m->rows, m->cols, getType(m));
            mixChannels(&m, 1, &mat, 1, ch, 1);
        }
    }

	return mat;
}

//*********incomplete********
static _inline Mat cvarrToMat(const CvArr* arr, int copyData,
                             int allowND, int coiMode)
{
	Mat mat;
	const IplImage* iplimg;
	size_t minstep;
	coiMode = 0;
	allowND = 1;
	copyData = 0;


	if( CV_IS_MAT(arr) )
        return CreateMatObject((const CvMat*)arr, copyData );
	else if( CV_IS_IMAGE(arr) )
	{
		iplimg = (const IplImage*)arr;
		if( coiMode == 0 && iplimg->roi && iplimg->roi->coi > 0 )
			return;
		return CreateMatImage(iplimg, copyData);
	}
	else if( CV_IS_SEQ(arr) )
    {
		/*CvSeq* seq = (CvSeq*)arr;
		assert(seq->total > 0 && CV_ELEM_SIZE(seq->flags) == seq->elem_size);
		if(!copyData && seq->first->next == seq->first)
			return CreateMatCopyObject()*/
	}
	else
    {
        CvMat hdr, *cvmat = GetMat( arr, &hdr, 0, allowND ? 1 : 0 );
        if( cvmat )
            return CreateMatObject(cvmat, copyData);
    }

	return mat;
}

double invert( CvMat *src, CvMat *dst, int method );

void* AlignPtr( const void* ptr, int align CV_DEFAULT(32) )
{
    assert( (align & (align-1)) == 0 );
    return (void*)( ((size_t)ptr + align - 1) & ~(size_t)(align-1) );
}

