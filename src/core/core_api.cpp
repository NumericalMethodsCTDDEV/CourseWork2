#include "core_api.h"
#include <cstdio>
#include <iostream>
#include <cmath>
#include <map>
#include <iomanip>
#include <cassert>
#include <cstring>
#include <climits>
#include <algorithm>

using namespace std;

const double EPS = 1e-9;

double K = 16 * 1e5;
double E = 6 * 1e4;
double alpha = 2;
double Q = 7 * 1e5;
double T0 = 293;
double rho = 830;
double C = 1990;
double lambda = 0.13;
double D = 8e-12;
double R = 8.314;
double kappa;

double MAX_Z = 0.06;
double MAX_T = 2000;
double dz = 0.0001;
double dt = 0.0001;

double threshold = 0.3;

void parse_config(const config_singleton_t *configs) {
    K = configs -> K;
    E = configs -> E;
    alpha = configs -> alpha;
    Q = configs -> Q;
    T0 = configs -> T0;
    rho = configs -> rho;
    C = configs -> C;
    lambda = configs -> lambda;
    D = configs -> D;
    R = configs -> R;
    MAX_Z = configs -> MAX_Z;
    MAX_T = configs -> MAX_T;
    dz = configs -> dz;
    dt = configs -> dt;
    threshold = configs -> threshold;

    kappa = lambda / (rho * C);
}

void solve_matrix(double *a, double *b, double *c, double *d, double *result, int n) {
    c[0] /= b[0];
    d[0] /= b[0];
    b[0] = 1;
    for (int i = 1; i < n; ++i) {
        b[i] -= c[i - 1] * a[i];
        d[i] -= d[i - 1] * a[i];
        a[i] = 0;
        c[i] /= b[i];
        d[i] /= b[i];
    }

    result[n - 1] = d[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        result[i] = d[i] - result[i + 1] * c[i];
    }
}

bool eq(double a, double b) {
    return fabs(b - a) < EPS;
}

void solve_matrix_with_check(double *a, double *b, double *c, double *d, double *result, int n) {
    double *na = new double[n];
    memcpy(na, a, sizeof(double) * n);
    double *nb = new double[n];
    memcpy(nb, b, sizeof(double) * n);
    double *nc = new double[n];
    memcpy(nc, c, sizeof(double) * n);
    double *nd = new double[n];
    memcpy(nd, d, sizeof(double) * n);

    solve_matrix(na, nb, nc, nd, result, n);

    assert(eq(b[0] * result[0] + c[0] * result[1], d[0]));
    for (int i = 1; i + 1 < n; ++i) {
        assert(eq(a[i] * result[i - 1] + b[i] * result[i] + c[i] * result[i + 1], d[i]));
    }
    assert(eq(a[n - 1] * result[n - 2] + b[n - 1] * result[n - 1], d[n - 1]));

    delete[] na;
    delete[] nb;
    delete[] nc;
    delete[] nd;
}

void build_matrix_z(double *a, double *b, double *c, double *d, const double *prev_z, const double *prev_t, int n) {
    double ac = -D / (dz * dz);
    a[0] = 0;
    for (int i = 1; i < n; ++i) {
        a[i] = ac;
    }
    for (int i = 0; i + 1 < n; ++i) {
        c[i] = ac;
    }
    c[n - 1] = 0;

    for (int i = 0; i < n; ++i) {
        d[i] = prev_z[i] / dt;
    }
    d[n - 1] -= ac;

    double bb = 1.0 / dt - 2.0 * ac;
    for (int i = 0; i < n; ++i) {
        b[i] = bb + K * pow(prev_z[i], alpha - 1) * exp(-E / (R * prev_t[i]));
    }
}

void build_matrix_t(double *a, double *b, double *c, double *d, const double *z, const double *prev_t, int n) {
    double ac = -kappa / (dz * dz);
    a[0] = ac;
    for (int i = 1; i < n; ++i) {
        a[i] = ac;
    }

    double bb = 1.0 / dt - 2.0 * ac;
    for (int i = 0; i < n; ++i) {
        b[i] = bb;
    }

    for (int i = 0; i + 1 < n; ++i) {
        c[i] = ac;
    }
    c[n - 1] = 0;

    double dt_1 = 1.0 / dt;
    double qck = (Q / C) * K;
    for (int i = 0; i < n; ++i) {
        d[i] = prev_t[i] * dt_1 + qck * pow(z[i], alpha) * exp(-E / (R * prev_t[i]));
    }
    d[0] -= ac * (T0 + Q / C);
    d[n - 1] -= ac * T0;
}

void print_answer(int layer, double *z, double *t, double *G, int n) {
    freopen(("outputTz" + to_string(num)).c_str(), "w", stdout);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(13);
    for (int i = 0; i < n; ++i) {
        cout << dz * i << " " << t[i] << "\n";
    }

    freopen(("outputXz" + to_string(num)).c_str(), "w", stdout);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(13);
    for (int i = 0; i < n; ++i) {
        cout << dz * i << " " << z[i] << "\n";
    }

    freopen(("outputWz" + to_string(num)).c_str(), "w", stdout);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(13);
    for (int i = 0; i < n; ++i) {
        cout << dz * i << " " << G[i] << "\n";
    }

    freopen(("outputAll" + to_string(num)).c_str(), "w", stdout);
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(13);
    for (int i = 0; i < n; ++i) {
        cout << dz * i << " " << t[i] << " " << z[i] * T0 << " " << G[i] * T0 << "\n";
    }

    ++num;
}

void solve_old() {
    int n = (int)((MAX_Z / dz) + 0.5);

    double *a = new double[n];
    double *b = new double[n];
    double *c = new double[n];
    double *d = new double[n];

    double *prev_z = new double[n];
    double *prev_t = new double[n];
    double *z = new double[n];
    double *t = new double[n];

    for (int i = 0; i < n; ++i) {
        prev_z[i] = 1;
        prev_t[i] = T0;
    }

    int plot_num = 1;
    for (int layer = 1; ; ++layer) {
        build_matrix_z(a, b, c, d, prev_z, prev_t, n);
        solve_matrix(a, b, c, d, z, n);

        build_matrix_t(a, b, c, d, z, prev_t, n);
        solve_matrix(a, b, c, d, t, n);

        a[n - 1] = -INT_MAX;
        for (int i = 0; i + 1 < n; ++i) {
            a[i] = t[i - 1] - t[i];
        }

        int pos = max_element(a + 1, a + n) - a;
        if (pos >= n * threshold * plot_num / 10) {
            for (int i = 0; i < n; ++i) {
                b[i] = K * pow(z[i], alpha) * exp(-E / (R * t[i]));
            }
            print_answer(layer, z, t, b, n);
            plot_num++;
            if (pos >= n * threshold) {
                break;
            }
        }

        if (layer % 100000 == 0) {
            for (int i = 0; i < n; ++i) {
                b[i] = K * pow(z[i], alpha) * exp(-E / (R * t[i]));
            }
            print_answer(layer, z, t, b, n);
        }

        swap(prev_z, z);
        swap(prev_t, t);
    }
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
    delete[] prev_z;
    delete[] prev_t;
    delete[] z;
    delete[] t;
}

long long num;

void solve(const config_singleton_t *configs) {
    num = 0;
    parse_config(configs);
    solve_old();
}
