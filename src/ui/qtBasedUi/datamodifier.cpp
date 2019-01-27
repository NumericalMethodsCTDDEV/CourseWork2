#include "datamodifier.h"
#include "graphics_adapter.h"

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

ScatterDataModifier::ScatterDataModifier(const std::vector<QComboBox *> &boxes, QChartView *depTz,
                                         QChartView *depXz, QChartView *depWT, QChartView *depAll)
        : equetionParametres(boxes),
          depTz(depTz), depXz(depXz), depWT(depWT), depAll(depAll) {

}

ScatterDataModifier::~ScatterDataModifier() = default;

template<typename T>
static inline std::pair<T, T> _find_min_max(const std::vector<T> &v) {
    T mi = std::numeric_limits<T>::max();
    T ma = std::numeric_limits<T>::min();
    for (const auto &x : v) {
        if (x > ma)
            ma = x;
        if (x < mi)
            mi = x;
    }
    return {mi, ma};
}

static inline void rebuildChartWithNewData(QChart *chart, const std::vector<chart_data_t> &datas) {
    chart->removeAllSeries();
//    std::pair<double, double> min_max_x = _find_min_max(x);
//    std::pair<double, double> min_max_y = _find_min_max(y);
//    chart->axisX()->setRange(min_max_x.first, min_max_x.second);
//    chart->axisY()->setRange(min_max_y.first, min_max_y.second);

    for (auto data : datas) {
        auto *series = new QSplineSeries();

        size_t minSz = std::min(data.x_data.size(), data.y_data.size());

        for (size_t i = 0; i != minSz; ++i) {
            series->append(data.x_data[i], data.y_data[i]);
        }

        QColor color(data.rgb_color.c_str());
        series->setColor(color);
        chart->addSeries(series);
    }
}


bool ScatterDataModifier::validateParametersSetting(QString *error_msg) const {
    for (const QComboBox *paramBox : equetionParametres) {
        bool is_ok;
        paramBox->currentText().toDouble(&is_ok);
        if (!is_ok) {
            *error_msg = "Not all configuration fields are set";
            return false;
        }
    }

    bool okMethodSet = false;
    for (const QRadioButton *methodB : methodButtons) {
        if (methodB->isChecked()) {
            okMethodSet = true;
        }
    }

    if (!okMethodSet) {
        *error_msg = "Set any available method";
        return false;
    }

    return true;
}

const config_singleton_t *ScatterDataModifier::loadConfig() const {
    const double K = equetionParametres[0]->currentText().toDouble();
    const double E = equetionParametres[1]->currentText().toDouble();
    const double alpha = equetionParametres[2]->currentText().toDouble();
    const double Q = equetionParametres[3]->currentText().toDouble();
    const double T0 = equetionParametres[4]->currentText().toDouble();
    const double rho = equetionParametres[5]->currentText().toDouble();
    const double C = equetionParametres[6]->currentText().toDouble();
    const double lambda = equetionParametres[7]->currentText().toDouble();
    const double D = equetionParametres[8]->currentText().toDouble();
    const double R = equetionParametres[9]->currentText().toDouble();
    const double MAX_Z = equetionParametres[10]->currentText().toDouble();
    const double MAX_T = equetionParametres[11]->currentText().toDouble();
    const double dz = equetionParametres[12]->currentText().toDouble();
    const double dt = equetionParametres[13]->currentText().toDouble();
    const double threshold = equetionParametres[14]->currentText().toDouble();

    return config_singleton_t::createConfigs(K, E, alpha, Q, T0, rho, C, lambda, D, R, MAX_Z,
                                             MAX_T, dz, dt, threshold);
}

static inline void showMessage(const QString &msg) {
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.exec();
}

void ScatterDataModifier::replot() {
    config_singleton_t::releaseConfigs();
    loadConfig();
    const config_singleton_t *configs = config_singleton_t::getConfigs();

    std::vector<chart_data_t> data_TZ;
    std::vector<chart_data_t> data_XZ;
    std::vector<chart_data_t> data_WT;
    std::vector<chart_data_t> data_All;

    try {
        data_TZ = get_graphics_T(configs);
        data_XZ = get_graphics_X(configs);
        data_WT = get_graphics_W(configs);
        data_All = get_graphics_all(configs);
    } catch (std::runtime_error &e) {
        showMessage(e.what());
    }

    rebuildChartWithNewData(depTz->chart(), data_TZ);
    rebuildChartWithNewData(depXz->chart(), data_XZ);
    rebuildChartWithNewData(depTz->chart(), data_WT);
    rebuildChartWithNewData(depTz->chart(), data_All);
}
