#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ensure graph fills the main window
    MainWindow::setCentralWidget(ui->widget);

    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::blue));
    //add another graph to visualize a separate variable
    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(QPen(Qt::red));

    //creates a ticker that increments in seconds of time
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%s");
    ui->widget->xAxis->setTicker(timeTicker);

    ui->widget->axisRect()->setupFullAxesBox();

    //display ticks on both x and y axes
    ui->widget->xAxis->setTickLabelFont(QFont(QFont().family(),8));
    ui->widget->yAxis->setTickLabelFont(QFont(QFont().family(),8));

    //display label on x axis
    ui->widget->xAxis->setLabel("Time(s)");

    //make top and right axes visible without ticks and labels
    ui->widget->xAxis2->setVisible(true);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->xAxis2->setTicks(false);
    ui->widget->yAxis2->setTicks(false);
    ui->widget->xAxis2->setTickLabels(false);
    ui->widget->yAxis2->setTickLabels(false);

    //timer repeatedly calls realtimeDataSlot() as fast as possible
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0);
}

void MainWindow::realtimeDataSlot() {
    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;
    //add a new data point every 1 millisecond
    if (key - lastPointKey > 0.001) {
        ui->widget->graph(0)->addData(key, ((double)rand() / RAND_MAX)*99.0);
        ui->widget->graph(1)->addData(key, ((double)rand() / RAND_MAX)*99.0);
        ui->widget->rescaleAxes();
        ui->widget->replot();
        lastPointKey = key;
    }

    //make key axis range scroll right with the data at a constant range of 8
    ui->widget->graph(0)->rescaleValueAxis();
    ui->widget->graph(1)->rescaleValueAxis();
    ui->widget->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->widget->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
