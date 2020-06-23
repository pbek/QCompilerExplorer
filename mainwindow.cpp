#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "godboltsvc.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initConnections();

    GodboltSvc::instance()->sendRequest(QGodBolt::Endpoints::Languages);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupLanguages(const QByteArray& data)
{
    const QJsonArray json = QJsonDocument::fromJson(data).array();
    for (const auto& value : json) {
        ui->languagesComboBox->addItem(value["name"].toString(), value["id"].toString());
    }
}

void MainWindow::updateCompilerComboBox(const QByteArray& data)
{
    const QJsonArray json = QJsonDocument::fromJson(data).array();
    for (const auto& value : json) {
        ui->compilerComboBox->addItem(value["name"].toString(), value["id"].toString());
    }
}

void MainWindow::initConnections()
{
    connect(GodboltSvc::instance(), &GodboltSvc::languages, this, &MainWindow::setupLanguages);
    connect(GodboltSvc::instance(), &GodboltSvc::compilers, this, &MainWindow::updateCompilerComboBox);
}

void MainWindow::on_languagesComboBox_currentIndexChanged(const QString& arg1)
{
    Q_UNUSED(arg1)
    const QString languageId = '/' + ui->languagesComboBox->currentData().toString();
    GodboltSvc::instance()->sendRequest(QGodBolt::Endpoints::Compilers, languageId);
}

void MainWindow::on_compileButton_clicked()
{
}
