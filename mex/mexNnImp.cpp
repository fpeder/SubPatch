#include <mex.h>
#include "mat.hpp"
#include <x86intrin.h>

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#define US(x) (unsigned(x))

//#define RAND_SEARCH

int P;
int niter;


int dist(mat *a, mat *b, int ax, int ay, int bx, int by, int cutoff=INT_MAX)
{
    int ans = 0;

    for (int dy=0; dy<P; dy++) {
        for (int dx=0; dx<P; dx++) {
            unsigned int v1 = GET(a, ay+dy, ax+dx);
            unsigned int v2 = GET(b, by+dy, bx+dx);

            int dr = (v1&255) - (v2&255);
            int dg = ((v1>>8)&255) - ((v2>>8)&255);
            int db = (v1>>16) - (v2>>16);
            
            ans += dr*dr + dg*dg + db*db;

            if (ans > cutoff)
                return cutoff;
        }
    }

    return ans;
}


void improve_guess(mat *a, mat *b, int ax, int ay, int &xbest, int &ybest,
                   int &dbest, int bx, int by)
{
    int d = dist(a, b, ax, ay, bx, by, dbest);

    if (d < dbest) {
        dbest = d;
        xbest = bx;
        ybest = by;
    }
}

inline static
void copy_patch(mat *a, int *adata, int x, int y)
{
    for(int dy=0; dy<P; dy++) 
        for (int dx=0; dx<P; dx++) {
            adata[dy + dx*P] = GET(a, y+dy, x+dx);
        }
}

inline static
void getnn(mat *nn, int x, int y, int &xp, int &yp)
{
    int v = GET(nn, y, x);

    xp = INT_TO_X(v);
    yp = INT_TO_Y(v);
}

void attempt(mat *b, int *adata, int x, int y, int &xbest, int &ybest, int &dbest)
{
    int ans=0;

    for (int dy=0; dy<P; dy++)
        for (int dx=0; dx<P; dx++) {
            unsigned int v1 = adata[dy + dx*P];
            unsigned int v2 = GET(b, y+dy, x+dx);

            int dr = (v1&255) - (v2&255);
            int dg = ((v1>>8)&255) - ((v2>>8)&255);
            int db = (v1>>16) - (v2>>16);

            ans += dr*dr + dg*dg + db*db;

            if (ans > dbest)
                continue;
        }

    if (ans < dbest) {
        dbest = ans;
        xbest = x;
        ybest = y;
    }
}


void improve(mat *nn, mat *d, mat *a, mat *b)
{
    int aew = a->N-P+1;
    int aeh = a->M-P+1;
    int bew = b->N-P+1;
    int beh = b->M-P+1;

    for (int iter=0; iter < niter; iter++) {
        int ystart=0, yend=aeh, ychange=1;
        int xstart=0, xend=aew, xchange=1;
        
        if (iter % 2 == 1) {
            xstart=xend-1; xend=-1; xchange=-1;
            ystart=yend-1; yend=-1; ychange=-1;
        }

        int adata[P*P];
        
        for (int ay=ystart; ay!=yend; ay+=ychange){
            for (int ax=xstart; ax!=xend; ax+=xchange) {
                int xbest, ybest, dbest;

                dbest = GET(d, ay, ax);
                if (dbest == 0)
                    continue;

                getnn(nn, ax, ay, xbest, ybest);
                //copy_patch(a, adata, ax, ay);

                // propagate x
                if (US(ax - xchange) < US(aew)) {
                    int xp, yp;

                    getnn(nn, ax-xchange, ay, xp, yp);
                    xp += xchange;
                    
                    if ((xp != xbest || yp != ybest) &&
                        US(xp) < US(bew)) {
                        improve_guess(a, b, ax, ay, xbest, ybest, dbest, xp, yp);
                        //attempt(b, adata, xp, yp, xbest, ybest, dbest);
                        
                    }
                }
                
                // propagate y
                if (US(ay-ychange) < US(aeh)) {
                    int xp, yp;
                    
                    getnn(nn, ax, ay-ychange, xp, yp);
                    yp += ychange;
                    
                    if ((xp != xbest || yp != ybest) &&
                        US(yp) < US(beh)) {
                        improve_guess(a, b, ax, ay, xbest, ybest, dbest, xp, yp);
                        //attempt(b, adata, xp, yp, xbest, ybest, dbest);
                    }
                }

#ifdef RAND_SEARCH
                int rs_start = MAX(a->M/6, a->N/6);
                for (int mag=rs_start; mag >= 1; mag /= 2) {
                    int xmin = MAX(xbest-mag, 0);
                    int xmax = MIN(xbest+mag+1, bew);

                    int ymin = MAX(ybest-mag, 0);
                    int ymax = MIN(ybest+mag+1, beh);

                    int xp = xmin + rand()%(xmax - xmin);
                    int yp = ymin + rand()%(ymax - ymin);

                    improve_guess(a, b, ax, ay, xbest, ybest, dbest,
                                  xp, yp);
                }
                
#endif

                SET(nn, ay, ax) = XY_TO_INT(xbest, ybest);
                SET(d, ay, ax)= dbest;
            }
        }
    }
}



// void nn_n(mat *a, mat *b, mat *nn, mat *d)
// {
//     int aew = a->N-P+1;
//     int aeh = a->M-P+1;
//     int bew = b->N-P+1;
//     int beh = b->M-P+1;
    
//     for (int iter=0; iter<niter; iter++) {
//         int ystart=0, yend=aeh, ychange=1;
//         int xstart=0, xend=aew, xchange=1;
//         if (iter % 2 == 1) {
//             xstart=xend-1; xend=-1; xchange=-1;
//             ystart=yend-1; yend=-1; ychange=-1;
//         }
//         int dx=-xchange, dy=-ychange;

//         //int adata[P*P];
//         for (int y=ystart; y!=yend; y+=ychange) {
//             for (int x=xstart; x!=xend; x+=xchange) {
//                 int xbest, ybest, err;
                
//                 getnn(nn, x, y, xbest, ybest);
//                 err = GET(d, y, x);
//                 if (err == 0)
//                     continue;

//                 if (US(x+dx) < US(nn->N-P)) {
//                     int xpp, ypp;

//                     getnn(nn, x+dx, y, xpp, ypp);
//                     xpp -= dx;

//                     if ((xpp != xbest || ypp != ybest) &&
//                         US(xpp) < US(b->N-P+1)) {

//                         int err0 = GET(d, y, x+dx);
//                         int xa = dx, xb = 0;
//                         if (dx > 0) { xa = 0; xb = dx; }
//                         int partial = 0;
//                         for (int yi = 0; yi < P; yi++) {
//                             int c1 = GET(a, y+yi, x+xa);
//                             int c2 = GET(b, ypp+yi, xpp+xa);
//                             int c3 = GET(a, y+yi, x+xb+P-1);
//                             int c4 = GET(b, ypp+yi, xpp+xb+P-1);

//                             int dr12 = (c1&255)-(c2&255);
//                             int dg12 = ((c1>>8)&255)-((c2>>8)&255);
//                             int db12 = (c1>>16)-(c2>>16);
//                             int dr34 = (c3&255)-(c4&255);
//                             int dg34 = ((c3>>8)&255)-((c4>>8)&255);
//                             int db34 = (c3>>16)-(c4>>16);
//                             partial +=  dr34*dr34+dg34*dg34+db34*db34
//                              -dr12*dr12-dg12*dg12-db12*db12;
//                         }
//                         err0 += (dx < 0) ? partial: -partial; 
//                         if (err0 < err) {
//                             err = err0;
//                             xbest = xpp;
//                             ybest = ypp;
//                         }
//                     }
//                 }


//                 if ((unsigned) (y+dy) < (unsigned) (nn->M-P)) {
//                     int xpp, ypp;
                    
//                     getnn(nn, x, y+dy, xpp, ypp);
//                     ypp -= dy;
                    
//                     if ((xpp != xbest || ypp != ybest) &&
//                         (unsigned) ypp < (unsigned) (b->M-P+1)) {

//                         int err0 = GET(d, y+dy, x);
//                         int ya = dy, yb = 0;
//                         if (dy > 0) { ya = 0; yb = dy; }
//                         int partial = 0;
//                         for (int xi = 0; xi < P; xi++) {
//                             int c1 = GET(a, y+ya, x+xi);
//                             int c2 = GET(b, ypp+ya, xpp+xi);
//                             int c3 = GET(a, y+yb+P-1, x+xi);
//                             int c4 = GET(b, ypp+yb+P-1, xpp+xi);
//                             int dr12 = (c1&255)-(c2&255);
//                             int dg12 = ((c1>>8)&255)-((c2>>8)&255);
//                             int db12 = (c1>>16)-(c2>>16);
//                             int dr34 = (c3&255)-(c4&255);
//                             int dg34 = ((c3>>8)&255)-((c4>>8)&255);
//                             int db34 = (c3>>16)-(c4>>16);
//                             partial +=  dr34*dr34+dg34*dg34+db34*db34
//                                 -dr12*dr12-dg12*dg12-db12*db12;
//                         }
//                         err0 += (dy < 0) ? partial: -partial;
//                         if (err0 < err) {
//                             err = err0;
//                             xbest = xpp;
//                             ybest = ypp;
//                         }
//                     }
//                 }

//                 GET(nn, y, x) = XY_TO_INT(xbest, ybest);
//                 GET(d, y, x) = err;
                
//             } 
//         }
//     }
// }


void mexFunction(int no, mxArray *po[], int ni, const mxArray *pi[])
{
    const mxArray *nn, *a, *b, *d;

    if (ni == 6) {
        if (mxIsEmpty(pi[0]))  mexErrMsgTxt("nn is empty");
        if (!mxIsInt32(pi[0])) mexErrMsgTxt("nn not int32");

        if (mxIsEmpty(pi[1]))  mexErrMsgTxt("d is empty");
        if (!mxIsInt32(pi[1])) mexErrMsgTxt("d not int32");

        if (mxIsEmpty(pi[2]))  mexErrMsgTxt("a is empty");
        if (!mxIsUint8(pi[2])) mexErrMsgTxt("a not uint8");

        if (mxIsEmpty(pi[3]))  mexErrMsgTxt("b is empty");
        if (!mxIsUint8(pi[3])) mexErrMsgTxt("b not uint8");

        if (mxIsEmpty(pi[4]))  mexErrMsgTxt("P is empty");
        P = mxGetScalar(pi[4]);
        
        if (mxIsEmpty(pi[5]))  mexErrMsgTxt("niter is empty");        
        niter = mxGetScalar(pi[5]);

    } else
        mexErrMsgTxt("usage: ...\n");
    
    nn = pi[0];
    d  = pi[1];
    a  = pi[2];
    b  = pi[3];

    mat aa  = mx2mat3(a);
    mat bb  = mx2mat3(b);
    mat nnn = mx2mat2(nn);
    mat dd  = mx2mat1(d);

    improve(&nnn, &dd, &aa, &bb);
    //nn_n(&aa, &bb, &nnn, &dd);
    
    mxArray *out1 = mat2mx(&nnn);
    mxArray *out2 = mat1mx(&dd);

    if (no == 1)
        po[0] = out1;
    else if (no == 2) {
        po[0] = out1;
        po[1] = out2;
    }
        
}
