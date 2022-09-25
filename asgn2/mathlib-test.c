#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

int main(int argc, char **argv) {
    int opt = 0;
    // booleans for sqrt, e, and pi function computations
    bool teste = false, testb = false, testm = false, testr = false, testv = false, testn = false;
    // boolean for statistic printing, and program synopsis page
    bool tests = false, testh = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': {
            teste = testb = testm = testr = testv = testn = true;
            break;
        }
        case 'e': {
            teste = true;
            break;
        }
        case 'r': {
            testr = true;
            break;
        }
        case 'b': {
            testb = true;
            break;
        }
        case 'm': {
            testm = true;
            break;
        }
        case 'v': {
            testv = true;
            break;
        }
        case 'n': {
            testn = true;
            break;
        }
        case 's': {
            tests = true;
            break;
        }
        case 'h': {
            testh = true;
            break;
        }
        default: {
            testh = true;
            break;
        }
        }
    }
    // boolean to test if any of the sqrt, e, or pi functions tests are being run.
    bool notest = (teste == false && testb == false && testm == false && testr == false
                   && testv == false && testn == false);
    if (testh || notest) {
        printf("SYNOPSIS\n");
        printf("   A test harness for the small numerical library.\n");
        printf("\nUSAGE\n");
        printf("   ./mathlib-test [-aebmrvnsh]\n");
        printf("\nOPTIONS\n");
        printf("  -a   Runs all tests.\n");
        printf("  -e   Runs e test.\n");
        printf("  -b   Runs BBP pi test.\n");
        printf("  -m   Runs Madhava pi test.\n");
        printf("  -r   Runs Euler pi test.\n");
        printf("  -v   Runs Viete pi test.\n");
        printf("  -n   Runs Newton square root tests.\n");
        printf("  -s   Print verbose statistics.\n");
        printf("  -h   Display program program synopsis and usage.\n");
        return 0;
    }
    if (teste) {
        double myresult = e();
        printf("e() = %16.15lf", myresult);
        printf(", M_E = %16.15lf", M_E);
        double diff = absolute(myresult - M_E);
        printf(", diff = %16.15lf\n", diff);
        if (tests) {
            int terms = e_terms();
            printf("e() terms = %d\n", terms);
        }
    }
    if (testr) {
        double myresult = pi_euler();
        printf("pi_euler() = %16.15lf", myresult);
        printf(", M_PI = %16.15lf", M_PI);
        double diff = absolute(myresult - M_PI);
        printf(", diff = %16.15lf\n", diff);
        if (tests) {
            int terms = pi_euler_terms();
            printf("pi_euler() terms = %d\n", terms);
        }
    }
    if (testb) {
        double myresult = pi_bbp();
        printf("pi_bbp() = %16.15lf", myresult);
        printf(", M_PI = %16.15lf", M_PI);
        double diff = absolute(myresult - M_PI);
        printf(", diff = %16.15lf\n", diff);
        if (tests) {
            int terms = pi_bbp_terms();
            printf("pi_bbp() terms = %d\n", terms);
        }
    }
    if (testm) {
        double myresult = pi_madhava();
        printf("pi_madhava() = %16.15lf", myresult);
        printf(", M_PI = %16.15lf", M_PI);
        double diff = absolute(myresult - M_PI);
        printf(", diff = %16.15lf\n", diff);
        if (tests) {
            int terms = pi_madhava_terms();
            printf("pi_madhava() terms = %d\n", terms);
        }
    }
    if (testv) {
        double myresult = pi_viete();
        printf("pi_viete() = %16.15lf", myresult);
        printf(", M_PI = %16.15lf", M_PI);
        double diff = absolute(myresult - M_PI);
        printf(", diff = %16.15lf\n", diff);
        if (tests) {
            int terms = pi_viete_factors();
            printf("pi_viete() terms = %d\n", terms);
        }
    }
    if (testn) {
        for (double x = 0.0; x <= 10.0; x += 0.1) {
            double myresult = sqrt_newton(x);
            printf("sqrt_newton(%8.6lf) = %16.15lf", x, myresult);
            double mathresult = sqrt(x);
            printf(", sqrt(%8.6lf) = %16.15lf", x, mathresult);
            double diff = absolute(myresult - mathresult);
            printf(", diff = %16.15lf\n", diff);
            if (tests) {
                int terms = sqrt_newton_iters();
                printf("sqrt_newton() terms = %d\n", terms);
            }
        }
    }
    return 0;
}
