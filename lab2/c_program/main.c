#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <gsl/gsl_blas.h>
#include <unistd.h>

typedef struct matrix {
    int rows;
    int cols;
    double *data;
}matrix;

typedef struct vector {
    int size;
    double *data;
}vector;

const int NUM_OF_STAMPS = 10000;


vector *matrix_by_vector(matrix *m, vector *v) {

    vector *result = malloc(sizeof(vector));
    result->size = v->size;
    result->data = calloc(result->size, sizeof(double));

    gsl_matrix_view M = gsl_matrix_view_array(m->data, m->rows, m->cols);
    gsl_vector_view V = gsl_vector_view_array(v->data, v->size);
    gsl_vector_view R = gsl_vector_view_array(result->data, result->size);

    gsl_blas_dgemv (CblasNoTrans,
                    1.0, &M.matrix, &V.vector,
                    0.0, &R.vector);

    return result;
}

double vector_by_vector(vector *v, vector *w) {
    double result;

    gsl_vector_view V = gsl_vector_view_array(v->data, v->size);
    gsl_vector_view W = gsl_vector_view_array(w->data, w->size);

    gsl_blas_ddot (&V.vector, &W.vector, &result);

    return result;
};

vector *create_vector(int size, double *data) {
    vector *v = malloc(sizeof(vector));
    v->size = size;
    v->data = data;
    return v;
}

matrix *create_matrix(int rows, int cols, double *data) {
    matrix *m = malloc(sizeof(matrix));
    m->rows = rows;
    m->cols = rows;
    m->data = data;
    return m;
}

vector *generate_vector(int size) {
    double *data = malloc(size*sizeof(double));
    for(int i=0; i<size; i++) {
        double coeff = (double)rand()/RAND_MAX*2.0-1.0;
        double value = (double)coeff * rand();
        data[i] = value;
    }
    return create_vector(size, data);
}

matrix *generate_matrix(int size) {
    double *data = malloc(size*sizeof(double));
    for(int i=0; i<size; i++) {
        double coeff = (double)rand()/RAND_MAX*2.0-1.0;
        double value = (double)coeff * rand();
        data[i] = value;
    }
    return create_matrix(size, size, data);
}

double get_time_diff_in_sec(clock_t start, clock_t end) {
    return (double) (end - start) / sysconf(_SC_CLK_TCK);
}

void delete_vector(vector *v) {
    free(v->data);
    free(v);
}

void delete_matrix(matrix *m) {
    free(m->data);
    free(m);
}


int main (void) {
    FILE *fp = fopen("results.csv", "w");
    srand ( time ( NULL));

    clock_t time_stamps[NUM_OF_STAMPS];
    struct tms **tms_time = malloc(NUM_OF_STAMPS * sizeof(struct tms *));
    for (int i = 0; i < NUM_OF_STAMPS; i++) {
        tms_time[i] = (struct tms *) malloc(sizeof(struct tms *));
    }

    time_stamps[0] = times(tms_time[0]);

    int count = 1;
    for(int size=50000; size <=1000000; size = size + 20000) {//49
        printf("%d\n", size);
        for(int meas=0; meas<10; meas++) {
            vector *v = generate_vector(size);
            vector *w = generate_vector(size);
            vector_by_vector(v, w);
            time_stamps[count] = times(tms_time[count]);
            double time = get_time_diff_in_sec(time_stamps[count-1], time_stamps[count]);
            fprintf(fp, "%d, %lf, %d\n", size, time, 1);
            count++;
            delete_vector(v);
            delete_vector(w);
        }
    }
    printf("\n\n\n\n");
    for(int size=80; size <=3600; size = size + 80) { //44
        printf("%d\n", size);
        for(int meas=0; meas<10; meas++) {
            matrix *m = generate_matrix(size*size);
            vector *v = generate_vector(size);
            time_stamps[count] = times(tms_time[count]);
            double time = get_time_diff_in_sec(time_stamps[count-1], time_stamps[count]);
            fprintf(fp, "%d, %lf, %d\n", size, time, 0);
            count++;
            delete_matrix(m);
            delete_vector(v);
        }
    }
    fclose(fp);
    return 0;
}