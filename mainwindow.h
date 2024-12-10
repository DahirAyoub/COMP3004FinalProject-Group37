#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QString>
#include <QFormLayout>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QValueAxis>
#include "Device.h"

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onLogoutClicked();
    void onUpdateProfileClicked();
    void onDeleteProfileClicked();
    void onDisplayProfilesClicked();
    void onTakeMeasurementClicked();
    void onViewMetricsClicked();
    void onConfirmActionClicked();
    void onBackToMenuClicked();
    void onApplyToSkinClicked();
    void onLiftOffSkinClicked();
    void clearCharts();
    void onShowRangesClicked();
    void onViewHistoryClicked();

private:
    QVBoxLayout *mainLayout;
    QChartView *chartView;          // For line chart
    QChartView *bodyChartView;      // For body chart visualization
    QChartView *pieChartView;       // For pie chart visualization

    void showInitialButtons();
    void showLoginMenu();
    void showMainMenu();
    void showField(QLineEdit *field, bool show);

    Device myDevice;

    // Input fields
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLineEdit *nameInput;
    QLineEdit *heightInput;
    QLineEdit *weightInput;
    QLineEdit *dobInput;

    // Buttons
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *logoutButton;
    QPushButton *updateButton;
    QPushButton *deleteButton;
    QPushButton *displayButton;
    QPushButton *confirmButton;
    QPushButton *backButton;
    QPushButton *viewMetricsButton;
    QPushButton *takeMeasurementButton;
    QPushButton *showRangesButton;
    QPushButton *historyButton;


    // Layout pointers
    QFormLayout *formLayout;

    // Output area
    QTextEdit *outputArea;

    // Action tracking
    QString currentAction;

    // Helper methods
    void createBodyChart();
    void createPieChart(const std::vector<HealthData> &allData);
    QChartView* createBodyChartView();
    QChartView* createPieChartView();
};

#endif // MAINWINDOW_H