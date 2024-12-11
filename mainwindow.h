#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QStackedWidget>
#include <QProgressBar>
#include <QTimer>
#include <QString>
#include <QtCharts/QChartView>
#include <QTabWidget>
#include "Device.h"

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onWelcomeCreateProfileClicked();
    void onCreateProfileClicked();
    void onWelcomeLoginClicked();
    void onLoginClicked();
    void onLogoutClicked();
    void onBackToMenuClicked();
    void onStartMeasurementClicked();
    void onViewHistoricalDataClicked();
    void onViewResultsClicked();
    void onUpdateProfileClicked();
    void onDeleteProfileClicked();
    void onTakeMeasurementClicked();
    void onViewMetricsClicked();
    void onShowRangesClicked();
    void onViewHistoryClicked();
    void onMeasurementNowClicked();
    void onDisplayProfilesClicked();
    void onSkinOnClicked();
    void onSkinOffClicked();

private:
    QString currentUser;
    int currentMeasurementIndex;
    void showUpdateProfileDialog();
    void setupWelcomePage();
    void setupLoginPage();
    void setupMainMenu();
    void setupCreateProfilePage();
    void setupPostLoginPage();
    void setupMeasurementPage();
    void setupResultsPage();
    void setupViewMetricsPage();
    void setupUpdateProfilePage();
    QString processMetricsForRecommendations(const std::vector<Metric> &metrics);

    void createBodyChart();
    void createPieChart(const std::vector<HealthData> &allData);
    void clearCharts();

    // New functions to handle record-specific charts
    QChartView* createBodyChartForRecord(const HealthData &record);
    QChartView* createPieChartForRecord(const HealthData &record);

    Device myDevice;
    QStackedWidget *stackedWidget;

    QWidget *welcomePage;
    QWidget *loginPage;
    QWidget *mainMenuPage;
    QWidget *createProfilePage;
    QWidget *postLoginPage;
    QWidget *measurementPage;
    QWidget *resultsPage;
    QWidget *viewMetricsPage;
    QWidget *updateProfilePage;

    // For viewMetricsPage
    QTabWidget *metricsTabWidget;

    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLineEdit *nameInput;
    QLineEdit *heightInput;
    QLineEdit *weightInput;
    QLineEdit *dobInput;
    QLineEdit *updateNameInput;
    QLineEdit *updateHeightInput;
    QLineEdit *updateWeightInput;
    QLineEdit *updateDobInput;

    QPushButton *confirmButton;
    QPushButton *logoutButton;
    QPushButton *startMeasurementButton;
    QPushButton *viewHistoricalDataButton;
    QPushButton *viewResultsButton;
    QPushButton *updateButton;
    QPushButton *deleteButton;
    QPushButton *displayButton;
    QPushButton *takeMeasurementButton;
    QPushButton *viewMetricsButton;
    QPushButton *showRangesButton;
    QPushButton *historyButton;
    QPushButton *backButton;
    QPushButton *confirmUpdateButton;

    QTextEdit *resultsOutput;
    QProgressBar *measurementProgressBar;
    QTextEdit *measurementOutput;
    QLabel *currentMeasurementLabel;
    QLabel *skinContactStatusLabel;
    QPushButton *skinOnButton;
    QPushButton *skinOffButton;

    // For incremental measurements
    int currentMeridianIndex;
    std::vector<Metric> allMeridianMetrics;

    // For the view metrics page, previously we had metricsLabel, now replaced by a QTabWidget
};

#endif // MAINWINDOW_H
