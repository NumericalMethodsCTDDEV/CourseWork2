#ifndef SCATTERDATAMODIFIER_H
#define SCATTERDATAMODIFIER_H

#include "config.h"
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QtGui/QFont>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QRadioButton>
#include <vector>
#include <QtCharts>

using namespace QtDataVisualization;

class ScatterDataModifier : public QObject {
Q_OBJECT
public:
    explicit ScatterDataModifier(const std::vector<QComboBox *> &equetionParametres,
                                 QChartView *depTz,
                                 QChartView *depXz,
                                 QChartView *depWT,
                                 QChartView *depAll);

    ~ScatterDataModifier();

    void start();

    void replot();

private:
//    void addData(const Answer &ans);
    std::vector<QComboBox *> equetionParametres;
    std::vector<QRadioButton *> methodButtons;

    const config_singleton_t *loadConfig() const;

    bool validateParametersSetting(QString *error_msg) const;

    QChartView *depTz;
    QChartView *depXz;
    QChartView *depWT;
    QChartView *depAll;
};

#endif
