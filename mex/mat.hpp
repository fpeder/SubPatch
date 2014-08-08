#ifndef _mat_hpp
#define _mat_hpp

#define XY_TO_INT(x, y) (((y)<<12)|(x))
#define INT_TO_X(v) ((v)&((1<<12)-1))
#define INT_TO_Y(v) ((v)>>12)

#define GET(m, i, j) ((m)->data[(i) + (j)*(m->M)])
#define SET(m, i, j) GET(m, i, j)

typedef struct mat {
    int N;
    int M;
    int *data;
} mat; 

mat init_mat(int M, int N);
//void free_mat(mat *mat);

int get(mat *mat, int i, int j);
void set(mat *mat, int i, int j, int val);

mat mx2mat3(const mxArray *in);
mat mx2mat2(const mxArray *in);
mat mx2mat1(const mxArray *in);

mxArray *mat3mx(mat *m);
mxArray *mat2mx(mat *m);
mxArray *mat1mx(mat *m);

void print_mat(mat *m);

#endif
