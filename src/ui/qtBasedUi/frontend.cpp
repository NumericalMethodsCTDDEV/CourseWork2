#include "datamodifier.h"
#include "../frontend.h"

static inline QComboBox *createComboBox() {
    auto *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

QT_CHARTS_USE_NAMESPACE

static inline QChartView *createChartView(const QString &title, const QString &xAxisName, const QString &yAxisName) {
    auto *series = new QSplineSeries();
//    QPen pen(QColor(Qt::green));
//    pen.setWidth(5);
//    series->setPen(pen);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->createDefaultAxes();
    chart->axisX()->setTitleText(xAxisName);
    chart->axisY()->setTitleText(yAxisName);
//    chart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

int showUi(int argc, char **argv) {
    QApplication app(argc, argv);

    auto *chartsLayout = new QGridLayout;
    QChartView *qchartTz = createChartView("T(t, z)", "z", "T");
    chartsLayout->addWidget(qchartTz, 0, 0);
    QChartView *qchartXz = createChartView("X(t, z)", "z", "X");
    chartsLayout->addWidget(qchartXz, 0, 1);
    QChartView *qchartWT = createChartView("W(t, z)", "z", "W");
    chartsLayout->addWidget(qchartWT, 1, 0);
    QChartView *qchartAll = createChartView("All", "", "");
    chartsLayout->addWidget(qchartAll, 1, 1);


    QWidget *widget = new QWidget;
    auto *hLayout = new QHBoxLayout(widget);
    auto *vLayout = new QVBoxLayout();
    hLayout->addLayout(chartsLayout, 1);
    hLayout->addLayout(vLayout);

    QGroupBox *configGroupBox = new QGroupBox(QStringLiteral("Configs"));
    auto *configGroupBoxLayout = new QVBoxLayout;
    std::vector<QComboBox *> lineEdits(15, nullptr);
    configGroupBox->setMinimumWidth(300);
//    configGroupBox->setMaximumHeight(1000);

    {

        // Warning! Do not change places of labels and boxes at arrays

        QLabel *labels[15];
        labels[0] = new QLabel("K");
        labels[1] = new QLabel("E");
        labels[2] = new QLabel("alpha");
        labels[3] = new QLabel("Q");
        labels[4] = new QLabel("T0");
        labels[5] = new QLabel("rho");
        labels[6] = new QLabel("C");
        labels[7] = new QLabel("lambda");
        labels[8] = new QLabel("D");
        labels[9] = new QLabel("R");
        labels[10] = new QLabel("MAX_Z");
        labels[11] = new QLabel("MAX_T");
        labels[12] = new QLabel("dz");
        labels[13] = new QLabel("dt");
        labels[14] = new QLabel("threshold");


        for (size_t i = 0; i != lineEdits.size(); ++i)
            lineEdits[i] = createComboBox();

        lineEdits[0]->setCurrentText("1600000");
        lineEdits[1]->setCurrentText("80000");
        lineEdits[2]->setCurrentText("1");
        lineEdits[3]->setCurrentText("700000");
        lineEdits[4]->setCurrentText("293");
        lineEdits[5]->setCurrentText("830");
        lineEdits[6]->setCurrentText("1990");
        lineEdits[7]->setCurrentText("0.13");
        lineEdits[8]->setCurrentText("8e-16");
        lineEdits[9]->setCurrentText("8.314");
        lineEdits[10]->setCurrentText("0.02");
        lineEdits[11]->setCurrentText("2000");
        lineEdits[12]->setCurrentText("0.0001");
        lineEdits[13]->setCurrentText("0.0001");
        lineEdits[14]->setCurrentText("0.8");

//        lineEdits[3]->setCurrentText("10");
//        lineEdits[3]->lineEdit()->setReadOnly(true);

//        lineEdits[5]->setCurrentText("2.666666666");
//        lineEdits[5]->lineEdit()->setReadOnly(true);

        auto *gridLayout = new QGridLayout;

        for (int i = 0; i != 15; ++i) {
            gridLayout->addWidget(labels[i], i, 0);
            gridLayout->addWidget(lineEdits[static_cast<size_t>(i)], i, 1);
        }
        configGroupBoxLayout->addLayout(gridLayout);
    }

    std::vector<QRadioButton *> radioButtons(4, nullptr);

    auto *replotButton = new QPushButton(widget);
    {
        QPalette pal = replotButton->palette();
        pal.setColor(QPalette::Button, QColor(Qt::green));
        replotButton->setText(QStringLiteral("Solve"));
        replotButton->setAutoFillBackground(true);
        replotButton->setPalette(pal);

        QSize sz = replotButton->size();
        replotButton->setMinimumSize(sz.width() + 20, sz.height() + 20);

        configGroupBoxLayout->addWidget(replotButton);
    }

    configGroupBox->setLayout(configGroupBoxLayout);
    vLayout->addWidget(configGroupBox);

    widget->setWindowTitle(QStringLiteral("CourseWork2"));

    auto *modifier = new ScatterDataModifier(lineEdits, qchartTz, qchartXz, qchartWT, qchartAll);

    QObject::connect(replotButton, &QPushButton::clicked, modifier, &ScatterDataModifier::replot);

    const QRect screenGeometry = QApplication::desktop()->screenGeometry();
    widget->resize(screenGeometry.width(), screenGeometry.height());

    widget->show();
    return QApplication::exec();
}
