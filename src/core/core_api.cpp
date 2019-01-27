#include "core_api.h"
#include <fstream>


void solve(const config_singleton_t *configs) {
    for (int i = 0; i < 3; i++) {
        std::string name = "outputTz";
        std::ofstream fout(name + std::to_string(i));
        for (int j = 1; j < 100; j++) {
            switch (i) {
                case 0: {
                    fout << j << " " << j + 10 << std::endl;
                    break;
                }
                case 1: {
                    fout << j << " " << j * j << std::endl;
                    break;
                }
                case 2: {
                    fout << j << " " << 1 / static_cast<double>(j) << std::endl;
                    break;
                }
                default:
                    break;
            }
        }
    }
}
