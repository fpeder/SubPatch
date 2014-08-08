#include <mex.h>
#include <stdint.h>

#define GET(mat, i, j, n) (mat[(i)+((j)+(n)*N)*M])
#define SET(mat, i, j, n) (mat[(i)+((j)+(n)*N2)*M2])


mxArray *img_to_pvec(const mxArray *img, mxArray *pvec, mxArray *xy,
                     int P, int M, int N, int M2, int N2, int nch,
                     int s)
{
    uint8_t *imgd, *pvecd;
    int32_t *xyd;
    imgd  = (uint8_t *)mxGetData(img);
    pvecd = (uint8_t *)mxGetData(pvec);
    xyd   = (int32_t *)mxGetData(xy);
    
    int i3=0, j3=0;
    for (int i=0; i < M-P+1; i+=s+1) {
        for (int j=0; j < N-P+1; j+=s+1) {
            
            j3 = 0;
            for (int n = 0; n < nch; n++) {
                for (int i2 = 0; i2 < P; i2++)
                    for (int j2 = 0; j2 < P; j2++) {
                        uint8_t v = GET(imgd, i+i2, j+j2, n);
                        SET(pvecd, i3, j3++, 0) = v;
                    }
            }
            SET(xyd, i3, 0, 0) = j;
            SET(xyd, i3, 1, 0) = i;
            i3++;
        }
    }

    return pvec;
}


void mexFunction(int no, mxArray *po[], int ni, const mxArray *pi[])
{
    const mxArray *img;
    int M, N, nch, P, skip;

    if (ni == 3) {
        img = pi[0];
        if (mxIsEmpty(img) || !mxIsUint8(img))
            mexErrMsgTxt("input mat...\n");
        M = mxGetDimensions(img)[0];
        N = mxGetDimensions(img)[1];
        nch = mxGetDimensions(img)[2];

        if (mxIsEmpty(pi[1]))
            mexErrMsgTxt("patch size...\n");
        P = mxGetScalar(pi[1]);

        if (mxIsEmpty(pi[2]))
            mexErrMsgTxt("skip size...\n");
        skip = mxGetScalar(pi[2]);
        
    } else
        mexErrMsgTxt("usage:...\n");
    
    int M2, N2, dims[2];
    mxArray *pvec, *xy;

    M2 = ((M-P)/(skip+1)+1)*((N-P)/(skip+1)+1);
    N2 = P*P*nch;

    dims[0] = M2;
    dims[1] = N2;
    pvec = mxCreateNumericArray(2, dims, mxUINT8_CLASS, mxREAL);

    dims[1] = 2;
    xy = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
                                
    img_to_pvec(img, pvec, xy, P, M, N, M2, N2, nch, skip);

    if (no == 1)
        po[0] = pvec;
    if (no == 2) {
        po[0] = pvec;
        po[1] = xy;
    }
            
}

