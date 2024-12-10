#include "mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QCategoryAxis>
#include <QToolTip>
#include <algorithm>

// If needed, normalizeValue can still be used in mainwindow, but it's best done in Device.
// Keeping it here for reference.
static float normalizeValue(float rawValue, float rawMin=0.0f, float rawMax=200.0f, float targetMin=0.0f, float targetMax=200.0f) {
    return ((rawValue - rawMin) / (rawMax - rawMin)) * (targetMax - targetMin) + targetMin;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), myDevice("RadoTechSim"), mainLayout(new QVBoxLayout()),
      chartView(nullptr), bodyChartView(nullptr), pieChartView(nullptr)
{
    // Create central widget and assign layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Add spacing and margins
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Output area for displaying messages
    outputArea = new QTextEdit(this);
    outputArea->setReadOnly(true);
    outputArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(outputArea);

    // Input fields layout
    formLayout = new QFormLayout();
    formLayout->setSpacing(5);

    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);

    nameInput = new QLineEdit(this);
    heightInput = new QLineEdit(this);
    weightInput = new QLineEdit(this);
    dobInput = new QLineEdit(this);

    formLayout->addRow("Username:", usernameInput);
    formLayout->addRow("Password:", passwordInput);
    formLayout->addRow("Name:", nameInput);
    formLayout->addRow("Height (cm):", heightInput);
    formLayout->addRow("Weight (kg):", weightInput);
    formLayout->addRow("Date of Birth (YYYY-MM-DD):", dobInput);

    mainLayout->addLayout(formLayout);

    // Hide form fields initially
    for (auto field : {usernameInput, passwordInput, nameInput, heightInput, weightInput, dobInput}) {
        showField(field, false);
    }

    // Buttons layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(5);

    historyButton = new QPushButton("View History", this);
    historyButton->setVisible(false);
    buttonLayout->addWidget(historyButton);

    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    logoutButton = new QPushButton("Logout", this);
    logoutButton->setVisible(false);

    confirmButton = new QPushButton("Confirm Action", this);
    backButton = new QPushButton("Back to Menu", this);
    confirmButton->setVisible(false);
    backButton->setVisible(false);

    updateButton = new QPushButton("Update Profile", this);
    deleteButton = new QPushButton("Delete Profile", this);
    displayButton = new QPushButton("Display Profiles", this);

    takeMeasurementButton = new QPushButton("Take Measurement", this);
    takeMeasurementButton->setVisible(false); // Hidden until logged in

    viewMetricsButton = new QPushButton("View My Metrics", this);
    viewMetricsButton->setVisible(false);

    // Button to show healthy ranges
    showRangesButton = new QPushButton("Show Healthy Ranges", this);
    showRangesButton->setVisible(false);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(logoutButton);
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(displayButton);
    buttonLayout->addWidget(takeMeasurementButton);
    buttonLayout->addWidget(viewMetricsButton);
    buttonLayout->addWidget(showRangesButton);

    mainLayout->addLayout(buttonLayout);

    // Increase window size
    setMinimumSize(1200, 800);
    resize(1400, 1000);

    // Connect buttons
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(confirmButton, &QPushButton::clicked, this, &MainWindow::onConfirmActionClicked);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateProfileClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteProfileClicked);
    connect(displayButton, &QPushButton::clicked, this, &MainWindow::onDisplayProfilesClicked);
    connect(takeMeasurementButton, &QPushButton::clicked, this, &MainWindow::onTakeMeasurementClicked);
    connect(viewMetricsButton, &QPushButton::clicked, this, &MainWindow::onViewMetricsClicked);
    connect(showRangesButton, &QPushButton::clicked, this, &MainWindow::onShowRangesClicked);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::onViewHistoryClicked);

    showInitialButtons();
}

MainWindow::~MainWindow() = default;

void MainWindow::showInitialButtons() {
    outputArea->clear();
    outputArea->append("Welcome! Please log in or register.\n");

    loginButton->setVisible(true);
    registerButton->setVisible(true);

    for (auto field : {usernameInput, passwordInput, nameInput, heightInput, weightInput, dobInput}) {
        showField(field, false);
    }

    logoutButton->setVisible(false);
    confirmButton->setVisible(false);
    backButton->setVisible(false);
    updateButton->setVisible(false);
    deleteButton->setVisible(false);
    displayButton->setVisible(false);
    takeMeasurementButton->setVisible(false);
    showRangesButton->setVisible(false);
}

void MainWindow::showMainMenu() {
    outputArea->clear();
    outputArea->append("Main Menu:\n");
    outputArea->append("1. Update Profile\n");
    outputArea->append("2. Delete Profile\n");
    outputArea->append("3. Display Profiles\n");
    outputArea->append("4. View My Metrics\n");

    logoutButton->setVisible(true);
    updateButton->setVisible(true);
    deleteButton->setVisible(true);
    displayButton->setVisible(true);
    takeMeasurementButton->setVisible(true);
    viewMetricsButton->setVisible(true);
    showRangesButton->setVisible(true);
    historyButton->setVisible(true); // Ensure the history button is visible

    for (auto field : {usernameInput, passwordInput, nameInput, heightInput, weightInput, dobInput}) {
        showField(field, false);
    }

    loginButton->setVisible(false);
    registerButton->setVisible(false);
    confirmButton->setVisible(false);
    backButton->setVisible(false);
}

void MainWindow::showField(QLineEdit *field, bool show) {
    field->setVisible(show);
    if (formLayout->labelForField(field)) {
        formLayout->labelForField(field)->setVisible(show);
    }
}

void MainWindow::showLoginMenu() {
    outputArea->clear();
    outputArea->append("Welcome! Please log in or register.\n");
    showInitialButtons();
}

void MainWindow::onLoginClicked() {
    outputArea->clear();
    outputArea->append("Please enter your username and password to log in.\n");

    showField(usernameInput, true);
    showField(passwordInput, true);

    for (auto field : {nameInput, heightInput, weightInput, dobInput}) {
        showField(field, false);
    }

    confirmButton->setText("Login");
    confirmButton->setVisible(true);
    backButton->setVisible(true);

    loginButton->setVisible(false);
    registerButton->setVisible(false);

    currentAction = "login";
}

void MainWindow::onRegisterClicked() {
    outputArea->clear();
    outputArea->append("Please fill out all the fields to register and create your profile.\n");

    for (auto field : {usernameInput, passwordInput, nameInput, heightInput, weightInput, dobInput}) {
        showField(field, true);
    }

    confirmButton->setText("Register");
    confirmButton->setVisible(true);
    backButton->setVisible(true);

    loginButton->setVisible(false);
    registerButton->setVisible(false);

    currentAction = "register";
}

void MainWindow::onLogoutClicked() {
    myDevice.logout();
    QMessageBox::information(this, "Logout", "You have logged out.");
    showLoginMenu();
}

void MainWindow::onUpdateProfileClicked() {
    showField(usernameInput, false);
    showField(passwordInput, false);
    showField(nameInput, true);
    showField(heightInput, true);
    showField(weightInput, true);
    showField(dobInput, true);

    confirmButton->setText("Update");
    confirmButton->setVisible(true);
    backButton->setVisible(true);
    currentAction = "update";
}

void MainWindow::onDeleteProfileClicked() {
    for (auto field : {nameInput, heightInput, weightInput, dobInput}) {
        showField(field, false);
    }

    confirmButton->setText("Delete");
    confirmButton->setVisible(true);
    backButton->setVisible(true);
    currentAction = "delete";
}

void MainWindow::onDisplayProfilesClicked() {
    outputArea->clear();
    auto profiles = myDevice.getAllProfiles();

    if (profiles.empty()) {
        outputArea->append("No profiles available.");
    } else {
        for (const auto &profile : profiles) {
            outputArea->append(QString("ID: %1, Name: %2, Age: %3, Height: %4 cm, Weight: %5 kg, DOB: %6")
                                   .arg(profile.getUserID())
                                   .arg(QString::fromStdString(profile.getName()))
                                   .arg(profile.calculateAge())
                                   .arg(profile.getHeight())
                                   .arg(profile.getWeight())
                                   .arg(QString::fromStdString(profile.getDob())));
        }
    }

    backButton->setVisible(true);
}

void MainWindow::onApplyToSkinClicked() {
    myDevice.applyToSkin();
    QMessageBox::information(this, "Skin Contact", "Device applied to skin.");
}

void MainWindow::onLiftOffSkinClicked() {
    myDevice.liftOffSkin();
    QMessageBox::information(this, "Skin Contact", "Device lifted off skin.");
}

void MainWindow::onTakeMeasurementClicked() {
    outputArea->clear();
    outputArea->append("Starting measurement procedure...");
    outputArea->append("Applying device to skin...");
    myDevice.applyToSkin();
    outputArea->append("Device applied to skin. Checking skin contact...");

    if (!myDevice.checkSkinContact()) {
        QMessageBox::warning(this, "Measurement", "Device is not applied to skin.");
        return;
    }

    outputArea->append(QString("Battery level: %1%").arg(myDevice.getBatteryLevel()));

    outputArea->append("Skin contact confirmed. Collecting and processing data...");
    myDevice.startMeasurement();

    outputArea->append(QString("Battery level after measurement: %1%").arg(myDevice.getBatteryLevel()));

    outputArea->append("Measurement complete. Retrieving updated metrics...");
    std::vector<Metric> metrics = myDevice.getBodyRegionMetrics();
    for (const auto& metric : metrics) {
        outputArea->append(QString("%1: %2 μA (%3)")
                           .arg(QString::fromStdString(metric.getName()))
                           .arg(metric.getValue())
                           .arg(QString::fromStdString(metric.getStatus())));
    }

    outputArea->append("Lifting device off skin...");
    myDevice.liftOffSkin();
    outputArea->append("Device lifted off skin. Measurement concluded.");
    QMessageBox::information(this, "Measurement", "Measurement process completed successfully.");
}

void MainWindow::clearCharts() {
    if (chartView) {
        mainLayout->removeWidget(chartView);
        delete chartView;
        chartView = nullptr;
    }
    if (bodyChartView) {
        mainLayout->removeWidget(bodyChartView);
        delete bodyChartView;
        bodyChartView = nullptr;
    }
    if (pieChartView) {
        mainLayout->removeWidget(pieChartView);
        delete pieChartView;
        pieChartView = nullptr;
    }
}

void MainWindow::onViewMetricsClicked() {
    outputArea->clear();
    clearCharts();

    std::vector<HealthData> allData = myDevice.getAllUserData();
    if (allData.empty()) {
        outputArea->append("No historical data available for this user.");
        backButton->setVisible(true);
        return;
    }

    outputArea->append("User Health Metrics (Historical):\n");

    // Example thresholds and suggestions
    float heartRateMin = 60.0f, heartRateMax = 100.0f;
    float bloodPressureMax = 120.0f;
    float oxygenMin = 95.0f;
    float respirationMax = 20.0f;

    for (const auto &record : allData) {
        QString timestamp = QString::fromStdString(record.getTimestamp());
        outputArea->append(QString("Timestamp: %1").arg(timestamp));
        outputArea->append("-----------------------------------------------------");
        for (const auto &metric : record.getMetrics()) {
            QString name = QString::fromStdString(metric.getName());
            float value = metric.getValue();
            QString status = QString::fromStdString(metric.getStatus());
            QString desc = QString::fromStdString(metric.getDescription());

            outputArea->append(QString("  %1 - Value: %2 μA, Status: %3\n    Description: %4")
                               .arg(name).arg(value).arg(status).arg(desc));

            // Provide simple suggestions based on threshold
            if (name == "Heart Rate") {
                if (value < heartRateMin) {
                    outputArea->append("    Suggestion: Heart rate low. Light exercise or consult a doctor.\n");
                } else if (value > heartRateMax) {
                    outputArea->append("    Suggestion: Heart rate high. Try relaxation techniques.\n");
                } else {
                    outputArea->append("    Good job! Heart rate in normal range.\n");
                }
            } else if (name == "Blood Pressure") {
                if (value > bloodPressureMax) {
                    outputArea->append("    Suggestion: High blood pressure. Consider reducing salt intake.\n");
                } else {
                    outputArea->append("    Blood pressure normal.\n");
                }
            } else if (name == "Oxygen Saturation") {
                if (value < oxygenMin) {
                    outputArea->append("    Suggestion: Low O2 saturation. Deep breathing or consult if persists.\n");
                } else {
                    outputArea->append("    Oxygen saturation healthy.\n");
                }
            } else if (name == "Respiration Rate") {
                if (value > respirationMax) {
                    outputArea->append("    Suggestion: High respiration rate. Practice calming exercises.\n");
                } else {
                    outputArea->append("    Respiration rate normal.\n");
                }
            }
        }
        outputArea->append("\n");
    }

    outputArea->append("Summary:\nFocus on maintaining normal heart rate and blood pressure.\n"
                       "Consider regular exercise and stress management.\n"
                       "If values stay abnormal, consult a healthcare provider.");

    outputArea->append("\nNote: Environmental factors (e.g., temperature, hydration) may affect measurements.\n");

    createBodyChart();
    createPieChart(allData);

    backButton->setVisible(true);
}

void MainWindow::onConfirmActionClicked() {
    if (currentAction == "login") {
        QString username = usernameInput->text().trimmed();
        QString password = passwordInput->text().trimmed();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Login", "Please enter both username and password.");
            return;
        }

        if (myDevice.login(username.toStdString(), password.toStdString())) {
            QMessageBox::information(this, "Login", "Login successful.");
            showMainMenu();
        } else {
            QMessageBox::warning(this, "Login", "Invalid username or password.");
            showLoginMenu();
        }
    }
    else if (currentAction == "register") {
        QString username = usernameInput->text().trimmed();
        QString password = passwordInput->text().trimmed();
        QString name = nameInput->text().trimmed();
        QString dob = dobInput->text().trimmed();
        float height = heightInput->text().toFloat();
        float weight = weightInput->text().toFloat();

        if (username.isEmpty() || password.isEmpty() || name.isEmpty() || dob.isEmpty() || height <= 0 || weight <= 0) {
            QMessageBox::warning(this, "Register", "All fields must be filled with valid data.");
            return;
        }

        if (myDevice.registerAccount(username.toStdString(), password.toStdString())) {
            myDevice.createUserProfile(name.toStdString(), height, weight, dob.toStdString());
            QMessageBox::information(this, "Register", "Registration and profile creation successful.");
            showLoginMenu();
        } else {
            QMessageBox::warning(this, "Register", "Username already exists.");
        }
    }
    else if (currentAction == "update") {
        std::string currentUser = myDevice.getLoggedInUser();
        QString name = nameInput->text().trimmed();
        QString dob = dobInput->text().trimmed();
        float height = heightInput->text().toFloat();
        float weight = weightInput->text().toFloat();

        if (currentUser.empty() || name.isEmpty() || dob.isEmpty() || height <= 0 || weight <= 0) {
            QMessageBox::warning(this, "Update", "All fields must be filled with valid data.");
            return;
        }

        myDevice.updateUserProfile(currentUser, name.toStdString(), height, weight, dob.toStdString());
        QMessageBox::information(this, "Update", "Profile updated successfully.");
        showMainMenu();
    }
    else if (currentAction == "delete") {
        auto profiles = myDevice.getAllProfiles();
        std::string currentUser = myDevice.getLoggedInUser();
        int userIDToDelete = -1;
        for (auto &p : profiles) {
            if (p.getName() == currentUser) {
                userIDToDelete = p.getUserID();
                break;
            }
        }

        if (userIDToDelete != -1) {
            myDevice.deleteUserProfile(userIDToDelete);
            QMessageBox::information(this, "Delete", "Profile deleted successfully.");
        } else {
            QMessageBox::warning(this, "Delete", "No profile found for current user.");
        }
        showLoginMenu();
    }
}

void MainWindow::createBodyChart() {
    if (bodyChartView) {
        mainLayout->removeWidget(bodyChartView);
        delete bodyChartView;
        bodyChartView = nullptr;
    }

    QChart *bodyChart = new QChart();
    bodyChart->setTitle("Electrical Conductivity by Body Region (Ryodoraku Scale)");

    std::vector<std::pair<QString, float>> bodyRegions = {
        {"Head", 130.0f},
        {"Neck", 110.0f},
        {"Left Shoulder", 90.0f},
        {"Right Shoulder", 85.0f},
        {"Heart", 140.0f},
        {"Lungs", 70.0f},
        {"Stomach", 50.0f},
        {"Left Kidney", 95.0f},
        {"Right Kidney", 100.0f},
        {"Left Foot", 40.0f},
        {"Right Foot", 30.0f}
    };

    QBarSet *set = new QBarSet("Conductivity (μA)");
    QStringList categories;
    for (auto &region : bodyRegions) {
        float value = region.second;
        *set << value;
        categories << region.first;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    bodyChart->addSeries(series);

    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(categories);
    xAxis->setTitleText("Body Regions");

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setTitleText("Conductivity (μA)");
    yAxis->setRange(0, 200);

    bodyChart->addAxis(xAxis, Qt::AlignBottom);
    bodyChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    // Highlight abnormal regions
    for (int i = 0; i < (int)bodyRegions.size(); ++i) {
        float val = bodyRegions[i].second;
        if (val < 80.0f || val > 120.0f) {
            QString msg = val < 80.0f ? "Low conductivity. Consider professional advice." :
                                        "High conductivity. Consider relaxation/stress reduction.";
            outputArea->append(QString("%1: %2 μA - %3").arg(bodyRegions[i].first)
                              .arg(val).arg(msg));
        }
    }

    bodyChartView = new QChartView(bodyChart);
    bodyChartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(bodyChartView);
}

void MainWindow::createPieChart(const std::vector<HealthData> &allData) {
    if (pieChartView) {
        mainLayout->removeWidget(pieChartView);
        delete pieChartView;
        pieChartView = nullptr;
    }

    std::map<std::string, float> metricTotals;
    for (const auto &record : allData) {
        for (const auto &metric : record.getMetrics()) {
            if (metric.getValue() > 0) {
                metricTotals[metric.getName()] += metric.getValue();
            }
        }
    }

    std::vector<std::pair<std::string, float>> sortedMetrics(metricTotals.begin(), metricTotals.end());
    // Sort in descending order by value
    std::sort(sortedMetrics.begin(), sortedMetrics.end(),
              [](const std::pair<std::string, float> &a, const std::pair<std::string, float> &b) {
                  return a.second > b.second; // descending order by second
              });

    QPieSeries *series = new QPieSeries();
    for (int i = 0; i < (int)sortedMetrics.size() && i < 3; i++) {
        series->append(QString::fromStdString(sortedMetrics[i].first), sortedMetrics[i].second);
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(series);
    pieChart->setTitle("Health Metric Distribution (Top 3)\nFocus on these metrics as they deviate the most.");

    pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    mainLayout->addWidget(pieChartView);
}

void MainWindow::onBackToMenuClicked() {
    if (currentAction == "register") {
        showInitialButtons();
    } else {
        showMainMenu();
    }

    clearCharts();
}

void MainWindow::onShowRangesClicked() {
    QDialog rangeDialog(this);
    rangeDialog.setWindowTitle("Ryodoraku Healthy Ranges Reference");

    QVBoxLayout layout(&rangeDialog);

    // Providing more realistic ranges without disclaimers:
    QLabel *label = new QLabel("Ryodoraku Ideal Ranges (μA):\n\n"
                               "Head: 80-120\n"
                               "Neck: 80-120\n"
                               "Shoulders: 80-120\n"
                               "Heart: 80-120\n"
                               "Lungs: 80-120\n"
                               "Stomach: 80-120\n"
                               "Kidneys: 80-120\n"
                               "Feet: 80-120\n", &rangeDialog);
    layout.addWidget(label);

    QPushButton *closeBtn = new QPushButton("Close", &rangeDialog);
    connect(closeBtn, &QPushButton::clicked, &rangeDialog, &QDialog::accept);
    layout.addWidget(closeBtn);

    rangeDialog.exec();
}

void MainWindow::onViewHistoryClicked() {
    outputArea->clear();
    outputArea->append("Measurement History:\n");

    std::vector<HealthData> allData = myDevice.getAllUserData();
    if (allData.empty()) {
        outputArea->append("No historical data available for this user.");
        return;
    }

    for (const auto& record : allData) {
        outputArea->append(QString("Timestamp: %1").arg(QString::fromStdString(record.getTimestamp())));
        outputArea->append("-----------------------------------------------------");

        for (const auto& metric : record.getMetrics()) {
            QString name = QString::fromStdString(metric.getName());
            float value = metric.getValue();
            QString status = QString::fromStdString(metric.getStatus());
            QString desc = QString::fromStdString(metric.getDescription());

            outputArea->append(QString("  %1 - Value: %2 μA, Status: %3\n    Description: %4")
                               .arg(name).arg(value).arg(status).arg(desc));
        }
        outputArea->append("\n");
    }

    backButton->setVisible(true);
}

