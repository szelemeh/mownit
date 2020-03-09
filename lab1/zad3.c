#include <stdio.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_ieee_utils.h>

int main() {

    float f = 1.1;

    for (int i = 0; i < 150; ++i) {
        f = f/2.0;
        printf (" f=");
        gsl_ieee_printf_float(&f);
        printf ("\n");
    }

    return 0;
}