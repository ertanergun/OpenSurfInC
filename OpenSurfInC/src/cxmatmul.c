#include "cxtypes.h"

CV_IMPL void cvGEMM( const CvArr* Aarr, const CvArr* Barr, double alpha,
                     const CvArr* Carr, double beta, CvArr* Darr, int flags )
{
    //cv::Mat A = cv::cvarrToMat(Aarr), B = cv::cvarrToMat(Barr);
    //cv::Mat C, D = cv::cvarrToMat(Darr);

    //if( Carr )
    //    C = cv::cvarrToMat(Carr);

   // CV_Assert( (D.rows == ((flags & CV_GEMM_A_T) == 0 ? A.rows : A.cols)) &&
    //           (D.cols == ((flags & CV_GEMM_B_T) == 0 ? B.cols : B.rows)) &&
    //           D.type() == A.type() );

    //gemm( A, B, alpha, C, beta, D, flags );
}