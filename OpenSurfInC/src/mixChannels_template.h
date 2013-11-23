#include "cxcore.h"
static void mixChannels_uchar( const void** _src, const int* sdelta0,
              const int* sdelta1, void** _dst,
              const int* ddelta0, const int* ddelta1,
              int n, Size size )
{
    const uchar** src = (const uchar**)_src;
    uchar** dst = (uchar**)_dst;
    int i, k;
    int block_size0 = n == 1 ? size.width : 1024;

    for( ; size.height--; )
    {
        int remaining = size.width;
        for( ; remaining > 0; )
        {
            int block_size = MIN( remaining, block_size0 );
            for( k = 0; k < n; k++ )
            {
                const uchar* s = src[k];
                uchar* d = dst[k];
                int ds = sdelta1[k], dd = ddelta1[k];
                if( s )
                {
                    for( i = 0; i <= block_size - 2; i += 2, s += ds*2, d += dd*2 )
                    {
                        uchar t0 = s[0], t1 = s[ds];
                        d[0] = t0; d[dd] = t1;
                    }
                    if( i < block_size )
                        d[0] = s[0], s += ds, d += dd;
                    src[k] = s;
                }
                else
                {
                    for( i=0; i <= block_size-2; i+=2, d+=dd*2 )
                        d[0] = d[dd] = 0;
                    if( i < block_size )
                        d[0] = 0, d += dd;
                }
                dst[k] = d;
            }
            remaining -= block_size;
        }
        for( k = 0; k < n; k++ )
            src[k] += sdelta0[k], dst[k] += ddelta0[k];
    }
}

static void mixChannels_int( const void** _src, const int* sdelta0,
              const int* sdelta1, void** _dst,
              const int* ddelta0, const int* ddelta1,
              int n, Size size )
{
    const int** src = (const int**)_src;
    int** dst = (int**)_dst;
    int i, k;
    int block_size0 = n == 1 ? size.width : 1024;

    for( ; size.height--; )
    {
        int remaining = size.width;
        for( ; remaining > 0; )
        {
            int block_size = MIN( remaining, block_size0 );
            for( k = 0; k < n; k++ )
            {
                const int* s = src[k];
                int* d = dst[k];
                int ds = sdelta1[k], dd = ddelta1[k];
                if( s )
                {
                    for( i = 0; i <= block_size - 2; i += 2, s += ds*2, d += dd*2 )
                    {
                        int t0 = s[0], t1 = s[ds];
                        d[0] = t0; d[dd] = t1;
                    }
                    if( i < block_size )
                        d[0] = s[0], s += ds, d += dd;
                    src[k] = s;
                }
                else
                {
                    for( i=0; i <= block_size-2; i+=2, d+=dd*2 )
                        d[0] = d[dd] = 0;
                    if( i < block_size )
                        d[0] = 0, d += dd;
                }
                dst[k] = d;
            }
            remaining -= block_size;
        }
        for( k = 0; k < n; k++ )
            src[k] += sdelta0[k], dst[k] += ddelta0[k];
    }
}

static void mixChannels_ushort( const void** _src, const int* sdelta0,
              const int* sdelta1, void** _dst,
              const int* ddelta0, const int* ddelta1,
              int n, Size size )
{
    const ushort** src = (const ushort**)_src;
    ushort** dst = (ushort**)_dst;
	ushort t0, t1;
	ushort* d;
	const ushort* s;
	int ds , dd;
    int i, k;
    int block_size0 = n == 1 ? size.width : 1024;

    for( ; size.height--; )
    {
        int remaining = size.width;
        for( ; remaining > 0; )
        {
            int block_size = MIN( remaining, block_size0 );
            for( k = 0; k < n; k++ )
            {
                s = src[k];
                d = dst[k];
                ds = sdelta1[k], dd = ddelta1[k];
                if( s )
                {
                    for( i = 0; i <= block_size - 2; i += 2, s += ds*2, d += dd*2 )
                    {
                        t0 = s[0], t1 = s[ds];
                        d[0] = t0; d[dd] = t1;
                    }
                    if( i < block_size )
                        d[0] = s[0], s += ds, d += dd;
                    src[k] = s;
                }
                else
                {
                    for( i=0; i <= block_size-2; i+=2, d+=dd*2 )
                        d[0] = d[dd] = 0;
                    if( i < block_size )
                        d[0] = 0, d += dd;
                }
                dst[k] = d;
            }
            remaining -= block_size;
        }
        for( k = 0; k < n; k++ )
            src[k] += sdelta0[k], dst[k] += ddelta0[k];
    }
}

static void mixChannels_int64( const void** _src, const int* sdelta0,
              const int* sdelta1, void** _dst,
              const int* ddelta0, const int* ddelta1,
              int n, Size size )
{
    const int64** src = (const int64**)_src;
    int64** dst = (int64**)_dst;
    int i, k;
    int block_size0 = n == 1 ? size.width : 1024;

    for( ; size.height--; )
    {
        int remaining = size.width;
        for( ; remaining > 0; )
        {
            int block_size = MIN( remaining, block_size0 );
            for( k = 0; k < n; k++ )
            {
                const int64* s = src[k];
                int64* d = dst[k];
                int ds = sdelta1[k], dd = ddelta1[k];
                if( s )
                {
                    for( i = 0; i <= block_size - 2; i += 2, s += ds*2, d += dd*2 )
                    {
                        int64 t0 = s[0], t1 = s[ds];
                        d[0] = t0; d[dd] = t1;
                    }
                    if( i < block_size )
                        d[0] = s[0], s += ds, d += dd;
                    src[k] = s;
                }
                else
                {
                    for( i=0; i <= block_size-2; i+=2, d+=dd*2 )
                        d[0] = d[dd] = 0;
                    if( i < block_size )
                        d[0] = 0, d += dd;
                }
                dst[k] = d;
            }
            remaining -= block_size;
        }
        for( k = 0; k < n; k++ )
            src[k] += sdelta0[k], dst[k] += ddelta0[k];
    }
}