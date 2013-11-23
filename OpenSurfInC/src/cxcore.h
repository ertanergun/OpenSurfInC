#include "cxtypes.h"

#if defined _MSC_VER && !defined WIN64 && !defined _WIN64
    static _inline int CV_XADD( int* addr, int delta )
    {
        int tmp;
        __asm
        {
            mov edx, addr
            mov eax, delta
            lock xadd [edx], eax
            mov tmp, eax
        }
        return tmp;
    }
  #else
    #include "windows.h"
    #undef min
    #undef max
    #define CV_XADD(addr,delta) InterlockedExchangeAdd((LONG volatile*)(addr), (delta))
  #endif

#define CV_AUTOSTEP  0x7fffffff

typedef IplImage* (CV_STDCALL* Cv_iplCreateImageHeader)
                            (int,int,int,char*,char*,int,int,int,int,int,
                            IplROI*,IplImage*,void*,IplTileInfo*);
typedef void (CV_STDCALL* Cv_iplAllocateImageData)(IplImage*,int,int);
typedef void (CV_STDCALL* Cv_iplDeallocate)(IplImage*,int);
typedef IplROI* (CV_STDCALL* Cv_iplCreateROI)(int,int,int,int,int);
typedef IplImage* (CV_STDCALL* Cv_iplCloneImage)(const IplImage*);


enum { MAGIC_VAL=0x42FF0000, AUTO_STEP=0, CONTINUOUS_FLAG=CV_MAT_CONT_FLAG };
enum { MAGIC_MASK=0xFFFF0000, TYPE_MASK=0x00000FFF, DEPTH_MASK=7 };
enum { MODIFY_A=1, NO_UV=2, FULL_UV=4 };

typedef struct _Mat
{
    // includes several bit-fields:
    //  * the magic signature
    //  * continuity flag
    //  * depth
    //  * number of channels
    int flags;
    // the number of rows and columns
    int rows, cols;
    // a distance between successive rows in bytes; includes the gap if any
    size_t step;
    // pointer to the data
    uchar* data;

	// pointer to the reference counter;
    // when matrix points to user-allocated data, the pointer is NULL
    int* refcount;
    
    // helper fields used in locateROI and adjustROI
    uchar* datastart;
    uchar* dataend;
}
Mat;

typedef struct Size_
{
	int width, height;
}
Size;

typedef struct _AutoBuffer_uchar
{
	uchar* ptr;
    size_t size;
    uchar buf[fixed_size];
}
AutoBuffer_uchar;

typedef struct _SVD
{
	Mat* u, *w, *vt;
}
SVD;
 

