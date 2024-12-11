#include "mainwindow.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialog>
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
#include <QDebug>
#include <QApplication>
#include <algorithm>

static float normalizeValue(float rawValue, float rawMin=0.0f, float rawMax=200.0f, float targetMin=0.0f, float targetMax=200.0f) {
    return ((rawValue - rawMin) / (rawMax - rawMin)) * (targetMax - targetMin) + targetMin;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      myDevice("RadoTechSim"),
      stackedWidget(new QStackedWidget(this)),
      currentMeasurementIndex(0),
      currentMeridianIndex(0)
{
    // Apply a global stylesheet for a modern dark theme
    qApp->setStyleSheet(
        "QWidget { "
        "    background-color: #2c2c2c; "
        "    color: #ffffff; "
        "    font-family: 'Segoe UI'; "
        "    font-size: 14px; "
        "}"
        "QPushButton { "
        "    background-color: #3498db; "
        "    border-radius: 5px; "
        "    padding: 8px 15px; "
        "    color: #ffffff; "
        "    font-weight: bold;"
        "}"
        "QPushButton:hover { "
        "    background-color: #5dade2; "
        "}"
        "QLineEdit, QTextEdit { "
        "    background-color: #3c3c3c; "
        "    border: 1px solid #555; "
        "    border-radius: 3px; "
        "    padding: 4px; "
        "    color: #ffffff;"
        "}"
        "QLabel { "
        "    font-size: 16px; "
        "}"
        "QTabWidget::pane { "
        "    border: 1px solid #555; "
        "}"
        "QTabBar::tab { "
        "    background: #3c3c3c; "
        "    border: 1px solid #555; "
        "    padding: 5px; "
        "}"
        "QTabBar::tab:selected { "
        "    background: #5dade2; "
        "}"
    );

    setupWelcomePage();
    setupLoginPage();
    setupMainMenu();
    setupCreateProfilePage();
    setupPostLoginPage();
    setupMeasurementPage();
    setupResultsPage();
    setupViewMetricsPage();

    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(welcomePage);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupWelcomePage() {
    welcomePage = new QWidget(this);
    welcomePage->setObjectName("welcomePage");

    QVBoxLayout *layout = new QVBoxLayout(welcomePage);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(15);

    QLabel *welcomeLabel = new QLabel("Welcome to RadoTech Simulation!", welcomePage);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = welcomeLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    welcomeLabel->setFont(titleFont);

    QPushButton *welcomeCreateProfileButton = new QPushButton("Create Profile", welcomePage);
    QPushButton *welcomeLoginButton = new QPushButton("Login", welcomePage);

    layout->addWidget(welcomeLabel);
    layout->addStretch();
    layout->addWidget(welcomeCreateProfileButton);
    layout->addWidget(welcomeLoginButton);
    layout->addStretch();

    connect(welcomeCreateProfileButton, &QPushButton::clicked, this, &MainWindow::onWelcomeCreateProfileClicked);
    connect(welcomeLoginButton, &QPushButton::clicked, this, &MainWindow::onWelcomeLoginClicked);

    stackedWidget->addWidget(welcomePage);
}

void MainWindow::setupLoginPage() {
    loginPage = new QWidget(this);
    loginPage->setObjectName("loginPage");

    QVBoxLayout *layout = new QVBoxLayout(loginPage);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(15);

    QLabel *loginLabel = new QLabel("Login", loginPage);
    loginLabel->setAlignment(Qt::AlignCenter);
    QFont loginFont = loginLabel->font();
    loginFont.setPointSize(18);
    loginFont.setBold(true);
    loginLabel->setFont(loginFont);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Username");
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    QPushButton *loginBtn = new QPushButton("Login", loginPage);
    QPushButton *backBtn = new QPushButton("Back to Welcome", loginPage);

    layout->addWidget(loginLabel);
    layout->addSpacing(10);
    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addSpacing(10);
    layout->addWidget(loginBtn);
    layout->addWidget(backBtn);

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);

    stackedWidget->addWidget(loginPage);
}

void MainWindow::setupMainMenu() {
    mainMenuPage = new QWidget(this);
    mainMenuPage->setObjectName("mainMenu");

    QVBoxLayout *mainLayout = new QVBoxLayout(mainMenuPage);
    mainLayout->setContentsMargins(20,20,20,20);
    mainLayout->setSpacing(15);

    QLabel *menuLabel = new QLabel("Main Menu", mainMenuPage);
    QFont menuFont = menuLabel->font();
    menuFont.setPointSize(18);
    menuFont.setBold(true);
    menuLabel->setFont(menuFont);
    menuLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(menuLabel);
    mainLayout->addSpacing(10);

    startMeasurementButton = new QPushButton("Go to Measurement Page", mainMenuPage);
    viewMetricsButton = new QPushButton("View Metrics", mainMenuPage);
    historyButton = new QPushButton("View History", mainMenuPage);
    logoutButton = new QPushButton("Logout", mainMenuPage);

    updateButton = new QPushButton("Update Profile", mainMenuPage);
    deleteButton = new QPushButton("Delete Profile", mainMenuPage);
    displayButton = new QPushButton("Display Profiles", mainMenuPage);
    showRangesButton = new QPushButton("Show Healthy Ranges", mainMenuPage);

    mainLayout->addWidget(startMeasurementButton);
    mainLayout->addWidget(viewMetricsButton);
    mainLayout->addWidget(historyButton);
    mainLayout->addWidget(updateButton);
    mainLayout->addWidget(deleteButton);
    mainLayout->addWidget(displayButton);
    mainLayout->addWidget(showRangesButton);
    mainLayout->addWidget(logoutButton);
    mainLayout->addStretch();

    connect(startMeasurementButton, &QPushButton::clicked, this, &MainWindow::onStartMeasurementClicked);
    connect(viewMetricsButton, &QPushButton::clicked, this, &MainWindow::onViewMetricsClicked);
    connect(historyButton, &QPushButton::clicked, this, &MainWindow::onViewHistoryClicked);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateProfileClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteProfileClicked);
    connect(showRangesButton, &QPushButton::clicked, this, &MainWindow::onShowRangesClicked);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);
    connect(displayButton, &QPushButton::clicked, this, &MainWindow::onDisplayProfilesClicked);

    stackedWidget->addWidget(mainMenuPage);
}

void MainWindow::setupCreateProfilePage() {
    createProfilePage = new QWidget(this);
    createProfilePage->setObjectName("createProfilePage");

    QVBoxLayout *layout = new QVBoxLayout(createProfilePage);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(15);

    QLabel *titleLabel = new QLabel("Create Profile", createProfilePage);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    layout->addWidget(titleLabel);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    QLabel *nameLabel = new QLabel("Name:", this);
    nameInput = new QLineEdit(this);
    QLabel *heightLabel = new QLabel("Height (cm):", this);
    heightInput = new QLineEdit(this);
    QLabel *weightLabel = new QLabel("Weight (kg):", this);
    weightInput = new QLineEdit(this);
    QLabel *dobLabel = new QLabel("DOB (YYYY-MM-DD):", this);
    dobInput = new QLineEdit(this);
    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameInput = new QLineEdit(this);
    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);

    inputLayout->addWidget(nameLabel);
    inputLayout->addWidget(nameInput);
    inputLayout->addWidget(heightLabel);
    inputLayout->addWidget(heightInput);
    inputLayout->addWidget(weightLabel);
    inputLayout->addWidget(weightInput);
    inputLayout->addWidget(dobLabel);
    inputLayout->addWidget(dobInput);
    inputLayout->addWidget(usernameLabel);
    inputLayout->addWidget(usernameInput);
    inputLayout->addWidget(passwordLabel);
    inputLayout->addWidget(passwordInput);

    layout->addLayout(inputLayout);

    confirmButton = new QPushButton("Create Profile", createProfilePage);
    QPushButton *backBtn = new QPushButton("Back to Welcome", createProfilePage);

    layout->addWidget(confirmButton);
    layout->addWidget(backBtn);

    connect(confirmButton, &QPushButton::clicked, this, &MainWindow::onCreateProfileClicked);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);

    stackedWidget->addWidget(createProfilePage);
}

void MainWindow::setupPostLoginPage() {
    postLoginPage = new QWidget(this);
    postLoginPage->setObjectName("postLoginPage");

    QVBoxLayout *layout = new QVBoxLayout(postLoginPage);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(15);

    QLabel *postLoginLabel = new QLabel("Post Login Page", postLoginPage);
    postLoginLabel->setAlignment(Qt::AlignCenter);
    QFont plFont = postLoginLabel->font();
    plFont.setPointSize(18);
    plFont.setBold(true);
    postLoginLabel->setFont(plFont);

    layout->addWidget(postLoginLabel);

    QPushButton *backToMenuButton = new QPushButton("Back to Main Menu", postLoginPage);
    layout->addWidget(backToMenuButton);

    connect(backToMenuButton, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);

    stackedWidget->addWidget(postLoginPage);
}

void MainWindow::setupMeasurementPage() {
    measurementPage = new QWidget(this);
    measurementPage->setObjectName("measurementPage");

    QVBoxLayout *mainLayout = new QVBoxLayout(measurementPage);
    mainLayout->setContentsMargins(20,20,20,20);
    mainLayout->setSpacing(15);

    QHBoxLayout *topLayout = new QHBoxLayout();
    skinContactStatusLabel = new QLabel("Skin Contact: Off", measurementPage);
    skinContactStatusLabel->setAlignment(Qt::AlignRight);
    QFont statusFont = skinContactStatusLabel->font();
    statusFont.setBold(true);
    skinContactStatusLabel->setFont(statusFont);
    topLayout->addWidget(skinContactStatusLabel);
    topLayout->addStretch();

    mainLayout->addLayout(topLayout);

    QLabel *measurementLabel = new QLabel("Measurement Process", measurementPage);
    measurementLabel->setAlignment(Qt::AlignCenter);
    QFont measureFont = measurementLabel->font();
    measureFont.setPointSize(18);
    measureFont.setBold(true);
    measurementLabel->setFont(measureFont);

    measurementOutput = new QTextEdit(measurementPage);
    measurementOutput->setReadOnly(true);

    QHBoxLayout *skinLayout = new QHBoxLayout();
    skinOnButton = new QPushButton("Skin On", measurementPage);
    skinOffButton = new QPushButton("Skin Off", measurementPage);
    skinLayout->addWidget(skinOnButton);
    skinLayout->addWidget(skinOffButton);

    QPushButton *measureNowButton = new QPushButton("Take Measurement Now", measurementPage);
    QPushButton *backBtn = new QPushButton("Back to Main Menu", measurementPage);

    measurementProgressBar = new QProgressBar(measurementPage);
    measurementProgressBar->setRange(0,24);
    measurementProgressBar->setValue(0);

    mainLayout->addWidget(measurementLabel);
    mainLayout->addWidget(measurementOutput);
    mainLayout->addWidget(new QLabel("Device Controls:", measurementPage));
    mainLayout->addLayout(skinLayout);
    mainLayout->addWidget(measureNowButton);
    mainLayout->addWidget(measurementProgressBar);
    mainLayout->addWidget(backBtn);

    connect(measureNowButton, &QPushButton::clicked, this, &MainWindow::onMeasurementNowClicked);
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    connect(skinOnButton, &QPushButton::clicked, this, &MainWindow::onSkinOnClicked);
    connect(skinOffButton, &QPushButton::clicked, this, &MainWindow::onSkinOffClicked);

    stackedWidget->addWidget(measurementPage);
}

void MainWindow::setupResultsPage() {
    resultsPage = new QWidget(this);
    resultsPage->setObjectName("resultsPage");

    QVBoxLayout *layout = new QVBoxLayout(resultsPage);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(15);

    QLabel *resultsLabel = new QLabel("Health Metrics Results", resultsPage);
    resultsLabel->setAlignment(Qt::AlignCenter);
    QFont resultsFont = resultsLabel->font();
    resultsFont.setPointSize(18);
    resultsFont.setBold(true);
    resultsLabel->setFont(resultsFont);

    resultsOutput = new QTextEdit(resultsPage);
    resultsOutput->setReadOnly(true);

    backButton = new QPushButton("Back to Main Menu", resultsPage);
    backButton->setVisible(true);
    connect(backButton, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);

    layout->addWidget(resultsLabel);
    layout->addWidget(resultsOutput);
    layout->addWidget(backButton);

    stackedWidget->addWidget(resultsPage);
}

void MainWindow::setupViewMetricsPage() {
    viewMetricsPage = new QWidget(this);
    viewMetricsPage->setObjectName("viewMetricsPage");

    QVBoxLayout *layout = new QVBoxLayout(viewMetricsPage);
    layout->setContentsMargins(20,20,20,20);
    layout->setSpacing(15);

    QLabel *titleLabel = new QLabel("View Metrics", viewMetricsPage);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont vmFont = titleLabel->font();
    vmFont.setPointSize(18);
    vmFont.setBold(true);
    titleLabel->setFont(vmFont);
    layout->addWidget(titleLabel);

    // QTabWidget to hold multiple measurements
    metricsTabWidget = new QTabWidget(viewMetricsPage);
    layout->addWidget(metricsTabWidget);

    QPushButton *backToMenuBtn = new QPushButton("Back to Main Menu", viewMetricsPage);
    connect(backToMenuBtn, &QPushButton::clicked, this, &MainWindow::onBackToMenuClicked);
    layout->addWidget(backToMenuBtn);

    stackedWidget->addWidget(viewMetricsPage);
}

void MainWindow::onWelcomeCreateProfileClicked() {
    stackedWidget->setCurrentWidget(createProfilePage);
}

void MainWindow::onCreateProfileClicked() {
    QString name = nameInput->text();
    QString heightStr = heightInput->text();
    QString weightStr = weightInput->text();
    QString dob = dobInput->text();
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (name.isEmpty() || heightStr.isEmpty() || weightStr.isEmpty() || dob.isEmpty() || username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Incomplete Information", "Please fill in all fields to create a profile.");
        return;
    }

    bool heightOk, weightOk;
    float heightValue = heightStr.toFloat(&heightOk);
    float weightValue = weightStr.toFloat(&weightOk);

    if (!heightOk || !weightOk || heightValue <= 0 || weightValue <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid height and weight.");
        return;
    }

    if (myDevice.createUserProfile(name.toStdString(), heightValue, weightValue, dob.toStdString(), username.toStdString(), password.toStdString())) {
        QMessageBox::information(this, "Profile Created", "Profile created successfully.");
        currentUser = username;
        myDevice.authenticate(username.toStdString(), password.toStdString());
        stackedWidget->setCurrentWidget(mainMenuPage);
    } else {
        QMessageBox::warning(this, "Profile Creation Failed", "Unable to create profile. Max may be reached or username exists.");
    }
}

void MainWindow::onWelcomeLoginClicked() {
    stackedWidget->setCurrentWidget(loginPage);
}

void MainWindow::onLoginClicked() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    bool isAuthenticated = myDevice.authenticate(username.toStdString(), password.toStdString());

    if (isAuthenticated) {
        currentUser = username;
        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");
        logoutButton->setVisible(true);
        stackedWidget->setCurrentWidget(mainMenuPage);
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
        stackedWidget->setCurrentWidget(loginPage);
    }
}

void MainWindow::onLogoutClicked() {
    currentUser.clear();
    myDevice.logout();
    QMessageBox::information(this, "Logout", "You have logged out.");
    stackedWidget->setCurrentWidget(welcomePage);
}

void MainWindow::onBackToMenuClicked() {
    if (currentUser.isEmpty()) {
        stackedWidget->setCurrentWidget(welcomePage);
    } else {
        stackedWidget->setCurrentWidget(mainMenuPage);
    }
}

void MainWindow::onDisplayProfilesClicked() {
    resultsOutput->clear();
    std::vector<User> profiles = myDevice.getAllProfiles();

    if (profiles.empty()) {
        resultsOutput->append("No profiles available.");
    } else {
        resultsOutput->append("Existing Profiles:\n");
        for (const auto &profile : profiles) {
            resultsOutput->append(QString("Name: %1\nHeight: %2 cm\nWeight: %3 kg\nDOB: %4\nUsername: %5")
                                   .arg(QString::fromStdString(profile.getName()))
                                   .arg(profile.getHeight())
                                   .arg(profile.getWeight())
                                   .arg(QString::fromStdString(profile.getDob()))
                                   .arg(QString::fromStdString(profile.getUsername())));
            resultsOutput->append("--------------------------------------------\n");
        }
    }

    backButton->setVisible(true);
    stackedWidget->setCurrentWidget(resultsPage);
}

void MainWindow::onStartMeasurementClicked() {
    stackedWidget->setCurrentWidget(measurementPage);
}

void MainWindow::onViewHistoricalDataClicked() {
    resultsOutput->clear();

    std::vector<HealthData> historicalData = myDevice.getAllUserData();

    if (historicalData.empty()) {
        QMessageBox::information(this, "Historical Data", "No historical health data available for this user.");
        return;
    }

    resultsOutput->append("User Historical Data:\n");

    for (const auto &record : historicalData) {
        QString timestamp = QString::fromStdString(record.getTimestamp());
        resultsOutput->append(QString("Timestamp: %1").arg(timestamp));
        resultsOutput->append("-----------------------------------------------------");

        for (const auto &metric : record.getMetrics()) {
            QString metricName = QString::fromStdString(metric.getName());
            float value = metric.getValue();
            QString status = QString::fromStdString(metric.getStatus());
            QString description = QString::fromStdString(metric.getDescription());

            resultsOutput->append(QString("  %1 - Value: %2 μA, Status: %3\n    Description: %4")
                                  .arg(metricName).arg(value).arg(status).arg(description));
        }
        resultsOutput->append("\n");
    }

    resultsOutput->append("End of historical data.");
    stackedWidget->setCurrentWidget(resultsPage);
}

void MainWindow::onViewResultsClicked() {
    stackedWidget->setCurrentWidget(resultsPage);
}

void MainWindow::onViewMetricsClicked() {
    // Clear old tabs
    while (metricsTabWidget->count() > 0) {
        QWidget *tab = metricsTabWidget->widget(0);
        metricsTabWidget->removeTab(0);
        delete tab;
    }

    std::vector<HealthData> allData = myDevice.getAllUserData();
    if (allData.empty()) {
        QWidget *noDataWidget = new QWidget();
        QVBoxLayout *noDataLayout = new QVBoxLayout(noDataWidget);
        QLabel *noDataLabel = new QLabel("No health metrics yet.");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLayout->addWidget(noDataLabel);
        metricsTabWidget->addTab(noDataWidget, "No Data");
    } else {
        int measurementNumber = 1;
        for (const auto &record : allData) {
            QWidget *measurementWidget = new QWidget();
            QVBoxLayout *mLayout = new QVBoxLayout(measurementWidget);

            QTextEdit *metricsText = new QTextEdit(measurementWidget);
            metricsText->setReadOnly(true);

            QString timestamp = QString::fromStdString(record.getTimestamp());
            metricsText->append(QString("Timestamp: %1").arg(timestamp));
            metricsText->append("-----------------------------------------------------");

            const auto &metrics = record.getMetrics();
            if (metrics.empty()) {
                metricsText->append("No metrics recorded for this timestamp.\n");
            } else {
                // Display metrics
                for (const auto &metric : metrics) {
                    QString name = QString::fromStdString(metric.getName());
                    float value = metric.getValue();
                    QString status = QString::fromStdString(metric.getStatus());
                    QString desc = QString::fromStdString(metric.getDescription());

                    metricsText->append(QString("%1 - Value: %2 μA, Status: %3\nDescription: %4\n")
                                         .arg(name).arg(value).arg(status).arg(desc));
                }
            }

            // Data Processing and Recommendations
            QString recommendations = processMetricsForRecommendations(metrics);
            metricsText->append("Summary & Recommendations:\n");
            metricsText->append(recommendations);
            metricsText->append("End of measurement data.\n");

            mLayout->addWidget(metricsText);

            // Add charts for this record
            QChartView *bodyChartView = createBodyChartForRecord(record);
            if (bodyChartView) mLayout->addWidget(bodyChartView);

            QChartView *pieChartView = createPieChartForRecord(record);
            if (pieChartView) mLayout->addWidget(pieChartView);

            QString tabLabel = QString("Measurement %1").arg(measurementNumber++);
            metricsTabWidget->addTab(measurementWidget, tabLabel);
        }
    }

    stackedWidget->setCurrentWidget(viewMetricsPage);
}

void MainWindow::onUpdateProfileClicked() {
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "Update Profile", "No user logged in.");
        return;
    }

    const User* userProfile = myDevice.getUserProfile(currentUser.toStdString());
    if (!userProfile) {
        QMessageBox::warning(this, "Update Profile", "No profile found for current user.");
        return;
    }

    showUpdateProfileDialog();
}

void MainWindow::showUpdateProfileDialog() {
    const User* userProfile = myDevice.getUserProfile(currentUser.toStdString());
    if (!userProfile) {
        QMessageBox::warning(this, "Update Profile", "No profile found.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Update Profile");
    dialog.setModal(true);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *infoLabel = new QLabel("Update your profile details:", &dialog);
    layout->addWidget(infoLabel);

    QFormLayout *formLayout = new QFormLayout();

    QLineEdit *nameEdit = new QLineEdit(QString::fromStdString(userProfile->getName()), &dialog);
    QLineEdit *heightEdit = new QLineEdit(QString::number(userProfile->getHeight()), &dialog);
    QLineEdit *weightEdit = new QLineEdit(QString::number(userProfile->getWeight()), &dialog);
    QLineEdit *dobEdit = new QLineEdit(QString::fromStdString(userProfile->getDob()), &dialog);

    QLineEdit *usernameEdit = new QLineEdit(QString::fromStdString(userProfile->getUsername()), &dialog);
    QLineEdit *passwordEdit = new QLineEdit(&dialog);
    passwordEdit->setPlaceholderText("Enter new password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Height (cm):", heightEdit);
    formLayout->addRow("Weight (kg):", weightEdit);
    formLayout->addRow("DOB (YYYY-MM-DD):", dobEdit);
    formLayout->addRow("Username:", usernameEdit);
    formLayout->addRow("Password:", passwordEdit);

    layout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *confirmBtn = new QPushButton("Confirm", &dialog);
    QPushButton *cancelBtn = new QPushButton("Cancel", &dialog);
    buttonLayout->addWidget(confirmBtn);
    buttonLayout->addWidget(cancelBtn);

    layout->addLayout(buttonLayout);

    connect(confirmBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString name = nameEdit->text();
        QString heightStr = heightEdit->text();
        QString weightStr = weightEdit->text();
        QString dob = dobEdit->text();
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();

        if (name.isEmpty() || heightStr.isEmpty() || weightStr.isEmpty() || dob.isEmpty() || username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "All fields must be filled.");
            return;
        }

        bool heightOk, weightOk;
        float newHeight = heightStr.toFloat(&heightOk);
        float newWeight = weightStr.toFloat(&weightOk);

        if (!heightOk || !weightOk || newHeight <= 0 || newWeight <= 0) {
            QMessageBox::warning(this, "Invalid Input", "Please enter valid height and weight.");
            return;
        }

        const User* profile = myDevice.getUserProfile(currentUser.toStdString());
        if (!profile) {
            QMessageBox::warning(this, "Update Profile", "No profile found for current user.");
            return;
        }

        int userID = profile->getUserID();
        myDevice.updateUserProfile(userID, name.toStdString(), newHeight, newWeight, dob.toStdString(), username.toStdString(), password.toStdString());

        QMessageBox::information(this, "Profile Updated", "Your profile has been updated successfully.");
        currentUser = username; // If username changed, update currentUser
    }
}

void MainWindow::onDeleteProfileClicked() {
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "Delete", "No user logged in.");
        return;
    }
    const User* userProfile = myDevice.getUserProfile(currentUser.toStdString());
    if (!userProfile) {
        QMessageBox::warning(this, "Delete", "No profile found for current user.");
        return;
    }
    int userID = userProfile->getUserID();
    myDevice.deleteUserProfile(userID);
    QMessageBox::information(this, "Delete", "Profile deleted successfully.");
    currentUser.clear();
    stackedWidget->setCurrentWidget(welcomePage);
}

void MainWindow::onTakeMeasurementClicked() {
    QMessageBox::information(this, "Measurement", "Proceeding to measurement page...");
    stackedWidget->setCurrentWidget(measurementPage);
}

void MainWindow::onShowRangesClicked() {
    QDialog rangeDialog(this);
    rangeDialog.setWindowTitle("Ryodoraku Healthy Ranges Reference");

    QVBoxLayout layout(&rangeDialog);

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
    resultsOutput->clear();
    resultsOutput->append("Measurement History:\n");

    std::vector<HealthData> allData = myDevice.getAllUserData();

    if (allData.empty()) {
        resultsOutput->append("No historical data available for this user.");
        backButton->setVisible(true);
        stackedWidget->setCurrentWidget(resultsPage);
        return;
    }

    for (const auto &record : allData) {
        QString timestamp = QString::fromStdString(record.getTimestamp());
        resultsOutput->append(QString("Timestamp: %1").arg(timestamp));
        resultsOutput->append("-----------------------------------------------------");

        for (const auto &metric : record.getMetrics()) {
            QString name = QString::fromStdString(metric.getName());
            float value = metric.getValue();
            QString status = QString::fromStdString(metric.getStatus());
            QString desc = QString::fromStdString(metric.getDescription());

            resultsOutput->append(QString("  %1 - Value: %2 μA, Status: %3\n    Description: %4")
                                   .arg(name).arg(value).arg(status).arg(desc));
        }
        resultsOutput->append("\n");
    }

    resultsOutput->append("End of history.");
    backButton->setVisible(true);
    stackedWidget->setCurrentWidget(resultsPage);
}

void MainWindow::onMeasurementNowClicked() {
    if (currentUser.isEmpty()) {
        QMessageBox::warning(this, "No User Logged In", "Please log in before measurement.");
        return;
    }

    if (myDevice.getBatteryLevel() <= 0.0f) {
        QMessageBox::warning(this, "Low Battery", "Battery is empty! Please recharge the device before taking a measurement.");
        return;
    }

    if (!myDevice.checkSkinContact()) {
        QMessageBox::warning(this, "Measurement", "Skin contact must be on before taking a measurement.");
        return;
    }

    measurementOutput->clear();
    measurementOutput->append(QString("Measuring meridian %1 of 24...").arg(currentMeridianIndex+1));

    std::vector<float> rawData = myDevice.collectMeasurementData();
    auto labels = myDevice.getMeasurementLabels();

    if (rawData.size() < 24) {
        QMessageBox::warning(this, "Error", "Insufficient data collected.");
        return;
    }

    float val = rawData[currentMeridianIndex];
    std::string name = labels[currentMeridianIndex];
    std::string status = "Normal";
    if (val < 80.0f) status = "Low";
    else if (val > 120.0f) status = "High";

    Metric m(name, val, status, "Ryodoraku metric point", 80.0f, 120.0f);
    allMeridianMetrics.push_back(m);

    measurementOutput->append(QString("%1: %2 μA (%3)")
                              .arg(QString::fromStdString(m.getName()))
                              .arg(m.getValue())
                              .arg(QString::fromStdString(m.getStatus())));

    measurementOutput->append("Please lift off skin (Skin Off) to continue.");
    QMessageBox::information(this, "Measurement", "Meridian measurement completed.\nPlease click 'Skin Off' to proceed to the next meridian.");

    measurementOutput->clear();

    // Update progress
    measurementProgressBar->setValue(currentMeridianIndex + 1);

    currentMeridianIndex++;
    if (currentMeridianIndex == 24) {
        myDevice.storeProcessedData(allMeridianMetrics);
        QMessageBox::information(this, "All Measurements Complete", "All 24 meridians have been measured and stored.");
        allMeridianMetrics.clear();
        currentMeridianIndex = 0;
        measurementProgressBar->setValue(0);
    }
}

void MainWindow::onSkinOnClicked() {
    myDevice.applyToSkin();
    skinContactStatusLabel->setText("Skin Contact: On");
    QMessageBox::information(this, "Skin Contact", "Device applied to skin.");
}

void MainWindow::onSkinOffClicked() {
    myDevice.liftOffSkin();
    skinContactStatusLabel->setText("Skin Contact: Off");
    QMessageBox::information(this, "Skin Contact", "Device lifted off skin.");
}

void MainWindow::clearCharts() {
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(resultsPage->layout());
    if (!layout) return;

    for (int i = layout->count() - 1; i >= 0; --i) {
        QLayoutItem *item = layout->itemAt(i);
        if (!item) continue;
        QWidget *w = item->widget();
        if (w && w->inherits("QChartView")) {
            layout->removeWidget(w);
            w->deleteLater();
        }
    }
}

void MainWindow::createBodyChart() {
    // Not used in this code
}

void MainWindow::createPieChart(const std::vector<HealthData> &allData) {
    // Not used in this code
}

QChartView* MainWindow::createBodyChartForRecord(const HealthData &record) {
    QChart *bodyChart = new QChart();
    bodyChart->setTitle("Measurement Conductivity by Region");

    std::vector<std::pair<QString,float>> bodyRegions;
    for (auto &m : record.getMetrics()) {
        QString n = QString::fromStdString(m.getName());
        float v = m.getValue();
        bodyRegions.push_back({n,v});
    }

    if (bodyRegions.empty()) return nullptr;

    QBarSet *set = new QBarSet("Conductivity (μA)");
    QStringList categories;
    for (auto &region : bodyRegions) {
        *set << region.second;
        categories << region.first;
    }

    QBarSeries *series = new QBarSeries();
    series->append(set);
    bodyChart->addSeries(series);

    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(categories);
    xAxis->setTitleText("Regions");

    QValueAxis *yAxis = new QValueAxis();
    yAxis->setTitleText("Conductivity (μA)");
    yAxis->setRange(0,200);

    bodyChart->addAxis(xAxis, Qt::AlignBottom);
    bodyChart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);

    QChartView *chartView = new QChartView(bodyChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}

QChartView* MainWindow::createPieChartForRecord(const HealthData &record) {
    std::map<std::string,float> metricTotals;
    for (const auto &metric : record.getMetrics()) {
        if (metric.getValue()>0)
            metricTotals[metric.getName()] += metric.getValue();
    }

    if (metricTotals.empty()) return nullptr;

    std::vector<std::pair<std::string,float>> sortedMetrics(metricTotals.begin(), metricTotals.end());
    std::sort(sortedMetrics.begin(), sortedMetrics.end(),
              [](const std::pair<std::string,float> &a, const std::pair<std::string,float> &b){
                  return a.second > b.second;
              });

    QPieSeries *series = new QPieSeries();
    for (int i=0; i<(int)sortedMetrics.size() && i<3; i++){
        series->append(QString::fromStdString(sortedMetrics[i].first), sortedMetrics[i].second);
    }

    QChart *pieChart = new QChart();
    pieChart->addSeries(series);
    pieChart->setTitle("Top 3 Metrics Distribution");

    QChartView *pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    return pieChartView;
}

QString MainWindow::processMetricsForRecommendations(const std::vector<Metric> &metrics) {
    QMap<QString, float> meridianValues;
    for (const auto &m : metrics) {
        QString name = QString::fromStdString(m.getName());
        QString meridianCode = name.split(" ").first();
        float val = m.getValue();
        meridianValues[meridianCode] = val;
    }

    QString result;
    for (auto it = meridianValues.begin(); it != meridianValues.end(); ++it) {
        QString code = it.key();
        float val = it.value();
        bool isHigh = (val > 120.0f);
        bool isLow = (val < 80.0f);

        if (code.startsWith("H1")) {
            if (isHigh) result += "H1 (Lung) High: Consider stress reduction, check for respiratory issues.\n";
            if (isLow) result += "H1 (Lung) Low: Consider breathing exercises, improve lung capacity.\n";
        } else if (code.startsWith("F2")) {
            if (isHigh) result += "F2 (Liver) High: Could relate to insomnia, irritability. Consider relaxation, dietary adjustments.\n";
            if (isLow) result += "F2 (Liver) Low: Possible weakness in liver function, consider consulting about diet and rest.\n";
        }

        if (!isHigh && !isLow) {
            result += code + ": Balanced range.\n";
        }
    }

    result += "\nGeneral Recommendation (Placeholder):\n"
              "Based on Ryodoraku principles, regulate lifestyle, manage stress, consider acupuncture or electrical stimulation therapy. Consult a specialist if symptoms persist.\n";

    return result;
}
