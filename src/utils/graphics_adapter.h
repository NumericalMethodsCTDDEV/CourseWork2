#ifndef COURSEWORK2_GRAPHICS_ADAPTER_H
#define COURSEWORK2_GRAPHICS_ADAPTER_H

#include <vector>
#include <string>
#include "config.h"

struct chart_data_t {
    std::string rgb_color; // например “#00ff4d”, можно брать отсюда из поля НЕХ: https://colorscheme.ru/color-converter.html

    std::vector<double> x_data;
    std::vector<double> y_data;

    chart_data_t(const std::string &rgb_color, const std::vector<double> &x_data, const std::vector<double> &y_data);
};

std::vector<chart_data_t> get_graphics_X();

std::vector<chart_data_t> get_graphics_T();

std::vector<chart_data_t> get_graphics_W();

std::vector<chart_data_t> get_graphics_all();

#endif //COURSEWORK2_GRAPHICS_ADAPTER_H
