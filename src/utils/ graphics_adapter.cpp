#include "graphics_adapter.h"

std::vector<chart_data_t> get_graphics_X(const config_singleton_t *config) {
    return std::vector<chart_data_t>();
}

std::vector<chart_data_t> get_graphics_T(const config_singleton_t *config) {
    return std::vector<chart_data_t>();
}

std::vector<chart_data_t> get_graphics_W(const config_singleton_t *config) {
    return std::vector<chart_data_t>();
}

std::vector<chart_data_t> get_graphics_all(const config_singleton_t *config) {
    return std::vector<chart_data_t>();
}

chart_data_t::chart_data_t(const std::string &rgb_color, const std::vector<double> &x_data,
                           const std::vector<double> &y_data) : rgb_color(rgb_color), x_data(x_data), y_data(y_data) {}
