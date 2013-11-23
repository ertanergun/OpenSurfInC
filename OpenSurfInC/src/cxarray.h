#include "cxcore.h"

static struct
{
    Cv_iplCreateImageHeader  createHeader;
    Cv_iplAllocateImageData  allocateData;
    Cv_iplDeallocate  deallocate;
    Cv_iplCreateROI  createROI;
    Cv_iplCloneImage  cloneImage;
}
CvIPL;


CvMat* CreateMatHeader(int height, int widht, int type);
CvMat* CreateMat(int height, int widht, int type);
static void CheckHuge( CvMat* arr );
void CreateData( CvMat* arr );