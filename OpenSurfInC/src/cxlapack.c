#include "cxmat.h"

double
cvInvert( CvArr* srcarr, CvArr* dstarr, int method )
{
	Mat src , dst;
	src = cvarrToMat(srcarr,0,1,0);
	dst = cvarrToMat(dstarr,0,1,0);

	assert( CV_MAT_TYPE(src.flags) == CV_MAT_TYPE(dst.flags) && src.rows == dst.cols && src.cols == dst.rows );
	//invert(&src,&dst,method);
	return invert( &src, &dst, method == CV_CHOLESKY ? DECOMP_CHOLESKY :
        method == CV_SVD || method == CV_SVD_SYM ? DECOMP_SVD : DECOMP_LU );
}

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

SVD* CreateSVD(Mat* mat)
{
	Mat* _a;
	SVD* svd;
	int flags = 0;
	int m = mat->rows, n = mat->cols, mn = max(m, n), nm = min(m, n);
	int type = getType(mat), elem_size = (int)getElemSize(mat);
	int a_ofs = 0, work_ofs=0, iwork_ofs=0, buf_size = 0;
	int temp_a = 0;
	double u1=0, v1=0, work1=0;
    float uf1=0, vf1=0, workf1=0;
	int lda, ldu, ldv, lwork=-1, iwork1=0, info=0, *iwork=0;
	char mode[1];
	Size size_mat, size_u, size_vt;
    

	if( flags & NO_UV )
    {
		free(svd->u);
		free(svd->vt);
    }
	 else
    {
		svd->u = MatCreate((int)m, (int)((flags & FULL_UV) ? m : nm), type);
		svd->vt = MatCreate((int)((flags & FULL_UV) ? n : nm), n, type );
    }

	svd->w = MatCreate(nm, 1, type);
	_a = mat;
	mode[0] = svd->u->data || svd->vt->data ? 'S' : 'N', '\0';

	if( m != n && !(flags & NO_UV) && (flags & FULL_UV) )
        mode[0] = 'A';

	size_mat = getSize(mat->cols, mat->rows);
	size_vt = getSize(svd->vt->cols, svd->vt->rows);
	size_u = getSize(svd->u->cols, svd->u->rows);

	if( !(flags & MODIFY_A) )
    {
		if( mode[0] == 'N' || mode[0] == 'A' )
            temp_a = 1;
		else if( ((svd->vt->data && CompareSize(&size_mat, &size_vt)) || (svd->u->data && CompareSize(&size_mat, &size_u))) &&
                  mode[0] == 'S' )
            mode[0] = 'O';
	}

	lda = mat->cols;
    ldv = ldu = mn;

	if( type == CV_32F )
	{

	}

	return svd;
}


#define Sf( y, x ) ((float*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((double*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((float*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((double*)(dstdata + y*dststep))[x]
double invert( CvMat *src, CvMat *dst, int method )
{
	double result = 0;
	int type = getType(src);

	assert( method == DECOMP_LU || method == DECOMP_CHOLESKY || method == DECOMP_SVD );

	if( method == DECOMP_SVD )
    {
        int n = min(src->rows, src->cols);
		SVD* svd;
		svd = CreateSVD(src);
    }

	return result;
}