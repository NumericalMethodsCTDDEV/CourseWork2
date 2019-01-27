#include "graphics_adapter.h"
#include <fstream>

std::vector<chart_data_t> get_graphics_X() {
    return std::vector<chart_data_t>();
}

std::vector<chart_data_t> get_graphics_T() {
    std::vector<chart_data_t> chart_data;
    for (int i = 0; i < 3; i++) {
        std::string name = "outputTz";
        std::ifstream fin(name + std::to_string(i));
        double x, y;
        std::vector<double> x_data;
        std::vector<double> y_data;
        while (fin >> x >> y) {
            x_data.push_back(x);
            y_data.push_back(y);
        }
        chart_data.emplace_back("#199e08", x_data, y_data);
    }
    return chart_data;
}

std::vector<chart_data_t> get_graphics_W() {
    return std::vector<chart_data_t>();
}

std::vector<chart_data_t> get_graphics_all() {
    return std::vector<chart_data_t>();
}

chart_data_t::chart_data_t(const std::string &rgb_color, const std::vector<double> &x_data,
                           const std::vector<double> &y_data) : rgb_color(rgb_color), x_data(x_data), y_data(y_data) {}
