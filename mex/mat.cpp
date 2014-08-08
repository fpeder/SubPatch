#include <stdlib.h>
#include <stdint.h>
#include <mex.h>

#include "mat.hpp" 


mat init_mat(int M, int N)
{
    mat m;

    m.M = M;
    m.N = N;
    m.data = (int *)mxMalloc(sizeof(int)*N*M);

    return m;
}

// void free_mat(mat *m)
// {
//     free(m->data);
// }

// int get(mat *m, int i, int j)
// {
//     return m->data[i + m->M*j];
// }

// void set(mat *m, int i, int j, int val)
// {
//     m->data[i + m->M*j] = val;
// }

mat mx2mat3(const mxArray *in)
{
    int M = mxGetDimensions(in)[0];
    int N = mxGetDimensions(in)[1];
    uint8_t *data = (uint8_t *)mxGetData(in);

    mat out = init_mat(M, N);
    mat *pout = &out;

    for (int i=0; i<M; i++) {
        for (int j=0; j<N; j++) {
            unsigned int r = data[i + (j+0*N)*M];
            unsigned int g = data[i + (j+1*N)*M];
            unsigned int b = data[i + (j+2*N)*M];

            int v = r | (g<<8) | (b<<16);
           
            GET(pout, i, j) = v;
        }
    }

    return out;
}

mat mx2mat2(const mxArray *in)
{
    int M = mxGetDimensions(in)[0];
    int N = mxGetDimensions(in)[1];
    int32_t *data = (int32_t *)mxGetData(in);

    mat out = init_mat(M, N);
    mat *pout = &out;

    for (int i=0; i<M; i++) {
        for (int j=0 ; j<N; j++) {
            int x = data[i + (j + 0*N)*M];
            int y = data[i + (j + 1*N)*M];

            GET(pout, i, j) = XY_TO_INT(x, y);
            
        }
    }

    return out;
}

mat mx2mat1(const mxArray *in)
{
    int M = mxGetDimensions(in)[0];
    int N = mxGetDimensions(in)[1];
    int32_t *data = (int32_t *)mxGetData(in);

    mat out = init_mat(M, N);
    mat *pout = &out;

    for (int i=0; i<M; i++) {
        for (int j=0 ; j<N; j++) {
            int v = data[i + (j + 0*N)*M];

            GET(pout, i, j) = v;
        }
    }

    return out;
}

mxArray *mat3mx(mat *m)
{
    int dims[] = {m->M, m->N, 3};
    mxArray *out = mxCreateNumericArray(3, dims, mxINT32_CLASS, mxREAL);
    int *data = (int *)mxGetData(out);

    for (int i=0; i < m->M; i++) {
        for (int j=0; j < m->N; j++) {
            int v = GET(m, i, j);
            
            data[i + (j + 0*m->N)*m->M] = v & 255;
            data[i + (j + 1*m->N)*m->M] = (v>>8)&255;
            data[i + (j + 2*m->N)*m->M] = v >> 16;
            
        }
    }

    return out;
}

mxArray *mat2mx(mat *m)
{
    int dims[] = {m->M, m->N, 2};
    mxArray *out = mxCreateNumericArray(3, dims, mxINT32_CLASS, mxREAL);
    int *data = (int *)mxGetData(out);

    for (int i=0; i < m->M; i++) {
        for (int j=0; j < m->N; j++) {
            int v = GET(m, i, j);
            
            data[i + (j + 0*m->N)*m->M] = INT_TO_X(v);
            data[i + (j + 1*m->N)*m->M] = INT_TO_Y(v);
        }
    }

    return out;
}

mxArray *mat1mx(mat *m)
{
    int dims[] = {m->M, m->N, 1};
    mxArray *out = mxCreateNumericArray(3, dims, mxINT32_CLASS, mxREAL);
    int *data = (int *)mxGetData(out);

    for (int i=0; i < m->M; i++) {
        for (int j=0; j < m->N; j++) {
            
            data[i + (j)*m->M] = GET(m, i, j);
        }
    }

    return out;
}

void print_mat(mat *m)
{
    for (int i=0; i<m->M; i++) {
        for (int j=0; j<m->N; j++) {
            mexPrintf("%d ", GET(m, i, j));
        }
        mexPrintf("\n");
    }
}


