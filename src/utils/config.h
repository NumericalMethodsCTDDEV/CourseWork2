#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class config_singleton_t {

    static const config_singleton_t *configSingletonPtr;

public:
    config_singleton_t(const double K, const double E, const double alpha, const double Q, const double T0,
                       const double rho, const double C, const double lambda, const double D, const double R,
                       const double MAX_Z, const double MAX_T, const double dz, const double dt)
            : K(K), E(E), alpha(alpha), Q(Q), T0(T0), rho(rho), C(C), lambda(lambda), D(D), R(R), MAX_Z(MAX_Z),
              MAX_T(MAX_T), dz(dz), dt(dt) {}

    config_singleton_t() : K(0), E(0), alpha(0), Q(0), T0(0), rho(0), C(0), lambda(0), D(0), R(0), MAX_Z(0),
                           MAX_T(0), dz(0), dt(0) {}

public:

    const double K;
    const double E;
    const double alpha;
    const double Q;
    const double T0;
    const double rho;
    const double C;
    const double lambda;
    const double D;
    const double R;
    const double MAX_Z;
    const double MAX_T;
    const double dz;
    const double dt;


    static const config_singleton_t *getConfigs() {
        if (!configSingletonPtr)
            configSingletonPtr = new config_singleton_t;
        return configSingletonPtr;
    }

    static const config_singleton_t *
    createConfigs(const double K, const double E, const double alpha, const double Q, const double T0,
                  const double rho, const double C, const double lambda, const double D, const double R,
                  const double MAX_Z, const double MAX_T, const double dz, const double dt) {
        if (configSingletonPtr)
            return configSingletonPtr;
        configSingletonPtr = new config_singleton_t(K, E, alpha, Q, T0, rho, C, lambda, D, R, MAX_Z,
                                                    MAX_T, dz, dt);
        return configSingletonPtr;
    }

    static void releaseConfigs() {
        delete configSingletonPtr;
        configSingletonPtr = nullptr;
    }
};

#endif // CONFIG_H
