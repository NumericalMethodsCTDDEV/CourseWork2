#include "graphics_adapter.h"
#include "core_api.h"
#include <fstream>

std::vector<chart_data_t> get_graphics_T() {
    std::vector<chart_data_t> chart_data;
    for (int i = 0; i < num; ++i) {
        std::string name = "outputTz";
        std::ifstream fin(name + std::to_string(i));
        if (!fin) {
            break;
        }
        double x, y;
        std::vector<double> x_data;
        std::vector<double> y_data;
        while (fin >> x >> y) {
            x_data.push_back(x);
            y_data.push_back(y);
        }
        chart_data.emplace_back("#d60d0d", x_data, y_data);
    }
    return chart_data;
}

std::vector<chart_data_t> get_graphics_X() {
    std::vector<chart_data_t> chart_data;
    for (int i = 0; i < num; ++i) {
        std::string name = "outputXz";
        std::ifstream fin(name + std::to_string(i));
        if (!fin) {
            break;
        }
        double x, y;
        std::vector<double> x_data;
        std::vector<double> y_data;
        while (fin >> x >> y) {
            x_data.push_back(x);
            y_data.push_back(y);
        }
        chart_data.emplace_back("#dbaa09", x_data, y_data);
    }
    return chart_data;
}

std::vector<chart_data_t> get_graphics_W() {
    std::vector<chart_data_t> chart_data;
    for (int i = 0; i < num; ++i) {
        std::string name = "outputWz";
        std::ifstream fin(name + std::to_string(i));
        if (!fin) {
            break;
        }
        double x, y;
        std::vector<double> x_data;
        std::vector<double> y_data;
        while (fin >> x >> y) {
            x_data.push_back(x);
            y_data.push_back(y);
        }
        chart_data.emplace_back("#0d23d1", x_data, y_data);
    }
    return chart_data;
}

std::vector<chart_data_t> get_graphics_all() {
    std::vector<chart_data_t> chart_data;
    for (int i = 0; i < num; ++i) {
        std::string name = "outputAll";
        std::ifstream fin(name + std::to_string(i));
        if (!fin) {
            break;
        }
        double z, T, X, W;
        std::vector<double> z_data;
        std::vector<double> T_data;
        std::vector<double> X_data;
        std::vector<double> W_data;
        while (fin >> z >> T >> X >> W) {
            z_data.push_back(z);
            T_data.push_back(T);
            X_data.push_back(X);
            W_data.push_back(W);
        }
        chart_data.emplace_back("#d60d0d", z_data, T_data);
        chart_data.emplace_back("#dbaa09", z_data, X_data);
        chart_data.emplace_back("#0d23d1", z_data, W_data);
    }
    return chart_data;
}

chart_data_t::chart_data_t(const std::string &rgb_color, const std::vector<double> &x_data,
                           const std::vector<double> &y_data) : rgb_color(rgb_color), x_data(x_data), y_data(y_data) {}
