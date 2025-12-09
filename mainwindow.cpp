#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QStatusBar>
#include <QDate>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <iomanip>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , db(Database::getInstance())
{
    setupUI();
    setupConnections();
    updateStatusBar();
    
    setWindowTitle("Путевой дневник - CRUD приложение");
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // Создаем центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Основной вертикальный layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Создаем вкладки
    tabWidget = new QTabWidget(this);
    
    // Вкладка "Поездки"
    QWidget *tripsTab = createTripsTab();
    tabWidget->addTab(tripsTab, "Поездки");
    
    // Вкладка "Расходы"
    QWidget *expensesTab = createExpensesTab();
    tabWidget->addTab(expensesTab, "Расходы");
    
    // Вкладка "Сопутешественники"
    QWidget *companionsTab = createCompanionsTab();
    tabWidget->addTab(companionsTab, "Сопутешественники");

    // Вкладка "Страны"
    QWidget *countriesTab = createCountriesTab();
    tabWidget->addTab(countriesTab, "Страны");

    // Вкладка "Города"
    QWidget *citiesTab = createCitiesTab();
    tabWidget->addTab(citiesTab, "Города");

    // Вкладка "Фотографии"
    QWidget *photosTab = createPhotosTab();
    tabWidget->addTab(photosTab, "Фотографии");

    // Вкладка "Участники поездок"
    QWidget *tripCompanionsTab = createTripCompanionsTab();
    tabWidget->addTab(tripCompanionsTab, "Участники поездок");

    // Вкладка "Места посещения"
    QWidget *tripDestinationsTab = createTripDestinationsTab();
    tabWidget->addTab(tripDestinationsTab, "Места посещения");
    
    // Вкладка "Статистика"
    QWidget *statsTab = createStatisticsTab();
    tabWidget->addTab(statsTab, "Статистика");

    QWidget *sqlTab = createSqlTab();
    tabWidget->addTab(sqlTab, "SQL Запросы");
    
    mainLayout->addWidget(tabWidget);
    
    // Панель подключения
    QHBoxLayout *connectionLayout = new QHBoxLayout();
    
    btnConnect = new QPushButton("Подключиться к БД", this);
    btnDisconnect = new QPushButton("Отключиться от БД", this);
    
    connectionLayout->addWidget(btnConnect);
    connectionLayout->addWidget(btnDisconnect);
    connectionLayout->addStretch();
    
    mainLayout->addLayout(connectionLayout);
    
    // Статус бар
    statusBar()->showMessage("Не подключено к базе данных");
}

QWidget* MainWindow::createTripsTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddTrip = new QPushButton("Добавить", tab);
    btnEditTrip = new QPushButton("Редактировать", tab);
    btnEditTrip->setEnabled(false);
    btnDeleteTrip = new QPushButton("Удалить", tab);
    btnDeleteTrip->setEnabled(false);
    btnRefreshTrips = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddTrip);
    buttonLayout->addWidget(btnEditTrip);
    buttonLayout->addWidget(btnDeleteTrip);
    buttonLayout->addWidget(btnRefreshTrips);
    buttonLayout->addStretch();
    
    // Таблица поездок
    tableTrips = new QTableWidget(tab);
    tableTrips->setColumnCount(8);
    tableTrips->setHorizontalHeaderLabels({"ID", "Название", "Дата начала", 
                                          "Дата окончания", "Бюджет", "Факт", 
                                          "Статус", "Заметки"});
    tableTrips->horizontalHeader()->setStretchLastSection(true);
    tableTrips->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addWidget(tableTrips);
    
    return tab;
}

QWidget* MainWindow::createExpensesTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddExpense = new QPushButton("Добавить", tab);
    btnEditExpense = new QPushButton("Редактировать", tab);
    btnEditExpense->setEnabled(false);
    btnDeleteExpense = new QPushButton("Удалить", tab);
    btnDeleteExpense->setEnabled(false);
    btnRefreshExpenses = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddExpense);
    buttonLayout->addWidget(btnEditExpense);
    buttonLayout->addWidget(btnDeleteExpense);
    buttonLayout->addWidget(btnRefreshExpenses);
    buttonLayout->addStretch();
    
    // Таблица расходов
    tableExpenses = new QTableWidget(tab);
    tableExpenses->setColumnCount(7);
    tableExpenses->setHorizontalHeaderLabels({"ID", "Место ID", "Дата", 
                                             "Сумма", "Валюта", "Категория", 
                                             "Описание"});
    tableExpenses->horizontalHeader()->setStretchLastSection(true);
    tableExpenses->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addWidget(tableExpenses);
    
    return tab;
}

QWidget* MainWindow::createCompanionsTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddCompanion = new QPushButton("Добавить", tab);
    btnEditCompanion = new QPushButton("Редактировать", tab);
    btnEditCompanion->setEnabled(false);
    btnDeleteCompanion = new QPushButton("Удалить", tab);
    btnDeleteCompanion->setEnabled(false);
    btnRefreshCompanions = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddCompanion);
    buttonLayout->addWidget(btnEditCompanion);
    buttonLayout->addWidget(btnDeleteCompanion);
    buttonLayout->addWidget(btnRefreshCompanions);
    buttonLayout->addStretch();
    
    // Таблица сопутешественников
    tableCompanions = new QTableWidget(tab);
    tableCompanions->setColumnCount(6);
    tableCompanions->setHorizontalHeaderLabels({"ID", "Имя", "Фамилия", 
                                               "Email", "Телефон", "Отношения"});
    tableCompanions->horizontalHeader()->setStretchLastSection(true);
    tableCompanions->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addWidget(tableCompanions);
    
    return tab;
}

QWidget* MainWindow::createStatisticsTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    btnRefreshStats = new QPushButton("Обновить статистику", tab);
    layout->addWidget(btnRefreshStats);
    
    // Группа статистики
    QGroupBox *statsGroup = new QGroupBox("Общая статистика", tab);
    QGridLayout *gridLayout = new QGridLayout(statsGroup);
    
    gridLayout->addWidget(new QLabel("Количество поездок:", statsGroup), 0, 0);
    labelTripsCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelTripsCount, 0, 1);
    
    gridLayout->addWidget(new QLabel("Количество расходов:", statsGroup), 1, 0);
    labelExpensesCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelExpensesCount, 1, 1);
    
    gridLayout->addWidget(new QLabel("Количество сопутешественников:", statsGroup), 2, 0);
    labelCompanionsCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelCompanionsCount, 2, 1);
    
    gridLayout->addWidget(new QLabel("Общая сумма расходов:", statsGroup), 3, 0);
    labelTotalSpent = new QLabel("0 руб.", statsGroup);
    gridLayout->addWidget(labelTotalSpent, 3, 1);
    
    gridLayout->addWidget(new QLabel("Количество стран:", statsGroup), 4, 0);
    labelCountriesCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelCountriesCount, 4, 1);
    
    gridLayout->addWidget(new QLabel("Количество городов:", statsGroup), 5, 0);
    labelCitiesCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelCitiesCount, 5, 1);

    gridLayout->addWidget(new QLabel("Количество фотографий:", statsGroup), 6, 0);
    labelPhotosCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelPhotosCount, 6, 1);

    gridLayout->addWidget(new QLabel("Количество мест посещ.:", statsGroup), 7, 0);
    labelTripDestinationsCount = new QLabel("0", statsGroup);
    gridLayout->addWidget(labelTripDestinationsCount, 7, 1);
    
    layout->addWidget(statsGroup);
    layout->addStretch();
    
    return tab;
}

void MainWindow::setupConnections()
{
    // Подключение к БД
    connect(btnConnect, &QPushButton::clicked, this, &MainWindow::onConnectClicked);
    connect(btnDisconnect, &QPushButton::clicked, this, &MainWindow::onDisconnectClicked);
    
    // Поездки
    connect(btnAddTrip, &QPushButton::clicked, this, &MainWindow::onAddTripClicked);
    connect(btnEditTrip, &QPushButton::clicked, this, &MainWindow::onEditTripClicked);
    connect(btnDeleteTrip, &QPushButton::clicked, this, &MainWindow::onDeleteTripClicked);
    connect(btnRefreshTrips, &QPushButton::clicked, this, &MainWindow::onRefreshTripsClicked);
    connect(tableTrips, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTripSelectionChanged);
    
    // Расходы
    connect(btnAddExpense, &QPushButton::clicked, this, &MainWindow::onAddExpenseClicked);
    connect(btnEditExpense, &QPushButton::clicked, this, &MainWindow::onEditExpenseClicked);
    connect(btnDeleteExpense, &QPushButton::clicked, this, &MainWindow::onDeleteExpenseClicked);
    connect(btnRefreshExpenses, &QPushButton::clicked, this, &MainWindow::onRefreshExpensesClicked);
    connect(tableExpenses, &QTableWidget::itemSelectionChanged, this, &MainWindow::onExpenseSelectionChanged);
    
    // Сопутешественники
    connect(btnAddCompanion, &QPushButton::clicked, this, &MainWindow::onAddCompanionClicked);
    connect(btnEditCompanion, &QPushButton::clicked, this, &MainWindow::onEditCompanionClicked);
    connect(btnDeleteCompanion, &QPushButton::clicked, this, &MainWindow::onDeleteCompanionClicked);
    connect(btnRefreshCompanions, &QPushButton::clicked, this, &MainWindow::onRefreshCompanionsClicked);
    connect(tableCompanions, &QTableWidget::itemSelectionChanged, this, &MainWindow::onCompanionSelectionChanged);
    
    // Страны
    connect(btnAddCountry, &QPushButton::clicked, this, &MainWindow::onAddCountryClicked);
    connect(btnEditCountry, &QPushButton::clicked, this, &MainWindow::onEditCountryClicked);
    connect(btnDeleteCountry, &QPushButton::clicked, this, &MainWindow::onDeleteCountryClicked);
    connect(btnRefreshCountries, &QPushButton::clicked, this, &MainWindow::onRefreshCountriesClicked);
    connect(btnFilterByContinent, &QPushButton::clicked, this, &MainWindow::onFilterByContinentClicked);
    connect(btnFilterVisa, &QPushButton::clicked, this, &MainWindow::onFilterVisaClicked);
    connect(btnFilterNoVisa, &QPushButton::clicked, this, &MainWindow::onFilterNoVisaClicked);
    connect(btnShowAllCountries, &QPushButton::clicked, this, &MainWindow::onShowAllCountriesClicked);
    connect(tableCountries, &QTableWidget::itemSelectionChanged, this, &MainWindow::onCountrySelectionChanged);

    // Города
    connect(btnAddCity, &QPushButton::clicked, this, &MainWindow::onAddCityClicked);
    connect(btnEditCity, &QPushButton::clicked, this, &MainWindow::onEditCityClicked);
    connect(btnDeleteCity, &QPushButton::clicked, this, &MainWindow::onDeleteCityClicked);
    connect(btnRefreshCities, &QPushButton::clicked, this, &MainWindow::onRefreshCitiesClicked);
    connect(btnFilterByCountry, &QPushButton::clicked, this, &MainWindow::onFilterByCountryClicked);
    connect(btnFilterWithAirport, &QPushButton::clicked, this, &MainWindow::onFilterWithAirportClicked);
    connect(btnShowAllCities, &QPushButton::clicked, this, &MainWindow::onShowAllCitiesClicked);
    connect(tableCities, &QTableWidget::itemSelectionChanged, this, &MainWindow::onCitySelectionChanged);

    // Фотографии
    connect(btnAddPhoto, &QPushButton::clicked, this, &MainWindow::onAddPhotoClicked);
    connect(btnEditPhoto, &QPushButton::clicked, this, &MainWindow::onEditPhotoClicked);
    connect(btnDeletePhoto, &QPushButton::clicked, this, &MainWindow::onDeletePhotoClicked);
    connect(btnRefreshPhotos, &QPushButton::clicked, this, &MainWindow::onRefreshPhotosClicked);
    connect(btnFilterByDestination, &QPushButton::clicked, this, &MainWindow::onFilterByDestinationClicked);
    connect(btnFilterByRating, &QPushButton::clicked, this, &MainWindow::onFilterByRatingClicked);
    connect(btnShowAllPhotos, &QPushButton::clicked, this, &MainWindow::onShowAllPhotosClicked);
    connect(tablePhotos, &QTableWidget::itemSelectionChanged, this, &MainWindow::onPhotoSelectionChanged);

    // Участники поездок
    connect(btnAddTripCompanion, &QPushButton::clicked, this, &MainWindow::onAddTripCompanionClicked);
    connect(btnEditTripCompanion, &QPushButton::clicked, this, &MainWindow::onEditTripCompanionClicked);
    connect(btnDeleteTripCompanion, &QPushButton::clicked, this, &MainWindow::onDeleteTripCompanionClicked);
    connect(btnRefreshTripCompanions, &QPushButton::clicked, this, &MainWindow::onRefreshTripCompanionsClicked);
    connect(btnFilterByTrip, &QPushButton::clicked, this, &MainWindow::onFilterByTripClicked);
    connect(btnFilterByCompanion, &QPushButton::clicked, this, &MainWindow::onFilterByCompanionClicked);
    connect(btnShowAllTripCompanions, &QPushButton::clicked, this, &MainWindow::onShowAllTripCompanionsClicked);
    connect(tableTripCompanions, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTripCompanionSelectionChanged);

    // Места посещения
    connect(btnAddTripDestination, &QPushButton::clicked, this, &MainWindow::onAddTripDestinationClicked);
    connect(btnEditTripDestination, &QPushButton::clicked, this, &MainWindow::onEditTripDestinationClicked);
    connect(btnDeleteTripDestination, &QPushButton::clicked, this, &MainWindow::onDeleteTripDestinationClicked);
    connect(btnRefreshTripDestinations, &QPushButton::clicked, this, &MainWindow::onRefreshTripDestinationsClicked);
    connect(btnFilterByTripForDest, &QPushButton::clicked, this, &MainWindow::onFilterByTripForDestClicked);
    connect(btnFilterByCityForDest, &QPushButton::clicked, this, &MainWindow::onFilterByCityForDestClicked);
    connect(btnFilterByTransport, &QPushButton::clicked, this, &MainWindow::onFilterByTransportClicked);
    connect(btnShowAllTripDestinations, &QPushButton::clicked, this, &MainWindow::onShowAllTripDestinationsClicked);
    connect(tableTripDestinations, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTripDestinationSelectionChanged);

    // Статистика
    connect(btnRefreshStats, &QPushButton::clicked, this, &MainWindow::onRefreshStatsClicked);

    // SQL Запросы
    connect(btnExecuteSql, &QPushButton::clicked, this, &MainWindow::onExecuteSqlClicked);
    connect(btnClearSql, &QPushButton::clicked, this, &MainWindow::onClearSqlClicked);
}

void MainWindow::updateStatusBar()
{
    QString status;
    if (db.isConnected()) {
        status = "Подключено к базе данных";
    } else {
        status = "Не подключено к базе данных";
    }
    statusBar()->showMessage(status);
}

// === Подключение к БД ===
void MainWindow::onConnectClicked()
{
    bool ok;
    QString host = QInputDialog::getText(this, "Подключение", "Хост:", QLineEdit::Normal, "localhost", &ok);
    if (!ok) return;
    
    QString port = QInputDialog::getText(this, "Подключение", "Порт:", QLineEdit::Normal, "5432", &ok);
    if (!ok) return;
    
    QString dbname = QInputDialog::getText(this, "Подключение", "База данных:", QLineEdit::Normal, "my_travels", &ok);
    if (!ok) return;
    
    QString user = QInputDialog::getText(this, "Подключение", "Пользователь:", QLineEdit::Normal, "postgres", &ok);
    if (!ok) return;
    
    QString password = QInputDialog::getText(this, "Подключение", "Пароль:", QLineEdit::Password, "", &ok);
    if (!ok) return;
    
    if (db.connect(host.toStdString(), port.toStdString(), 
                   dbname.toStdString(), user.toStdString(), 
                   password.toStdString())) {
        showInfo("Успешно подключено к базе данных!");
        loadTrips();
        loadExpenses();
        loadCompanions();
        loadCountries();
        loadCities();
        loadPhotos();
        loadTripCompanions(); 
        loadTripDestinations();
        loadStatistics();
    } else {
        showError("Ошибка подключения: " + db.getLastError());
    }
    
    updateStatusBar();
}

void MainWindow::onDisconnectClicked()
{
    db.disconnect();
    showInfo("Отключено от базы данных");
    
    // Очищаем таблицы
    tableTrips->setRowCount(0);
    tableExpenses->setRowCount(0);
    tableCompanions->setRowCount(0);
    
    updateStatusBar();
}

// === Поездки ===
void MainWindow::loadTrips()
{
    if (!db.isConnected()) return;
    
    std::vector<Trip> trips = db.readTrips();
    tableTrips->setRowCount(trips.size());
    
    for (size_t i = 0; i < trips.size(); ++i) {
        const Trip& trip = trips[i];
        
        tableTrips->setItem(i, 0, new QTableWidgetItem(QString::number(trip.id)));
        tableTrips->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(trip.name)));
        tableTrips->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(trip.start_date)));
        tableTrips->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(trip.end_date)));
        
        std::ostringstream budgetStream;
        budgetStream << std::fixed << std::setprecision(2) << trip.total_budget;
        tableTrips->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(budgetStream.str() + " руб.")));
        
        std::ostringstream spentStream;
        spentStream << std::fixed << std::setprecision(2) << trip.actual_spent;
        tableTrips->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(spentStream.str() + " руб.")));
        
        tableTrips->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(trip.status)));
        tableTrips->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(trip.notes)));
    }
    
    tableTrips->resizeColumnsToContents();
}

void MainWindow::onAddTripClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    // Создаем диалоговое окно
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить поездку");
    dialog.setFixedSize(400, 400);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblName("Название:");
    QLineEdit editName;
    
    QLabel lblStartDate("Дата начала (ГГГГ-ММ-ДД):");
    QLineEdit editStartDate;
    editStartDate.setText(QDate::currentDate().toString("yyyy-MM-dd"));
    
    QLabel lblEndDate("Дата окончания (ГГГГ-ММ-ДД):");
    QLineEdit editEndDate;
    editEndDate.setText(QDate::currentDate().addDays(7).toString("yyyy-MM-dd"));
    
    QLabel lblBudget("Бюджет (руб.):");
    QDoubleSpinBox spinBudget;
    spinBudget.setRange(0, 10000000);
    spinBudget.setValue(10000);
    spinBudget.setSuffix(" руб.");
    
    QLabel lblStatus("Статус:");
    QComboBox comboStatus;
    comboStatus.addItems({"planned", "in_progress", "completed", "cancelled"});
    
    QLabel lblNotes("Заметки:");
    QTextEdit editNotes;
    editNotes.setMaximumHeight(100);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblStartDate);
    layout.addWidget(&editStartDate);
    layout.addWidget(&lblEndDate);
    layout.addWidget(&editEndDate);
    layout.addWidget(&lblBudget);
    layout.addWidget(&spinBudget);
    layout.addWidget(&lblStatus);
    layout.addWidget(&comboStatus);
    layout.addWidget(&lblNotes);
    layout.addWidget(&editNotes);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название поездки не может быть пустым!");
            return;
        }
        
        Trip trip;
        trip.name = editName.text().toStdString();
        trip.start_date = editStartDate.text().toStdString();
        trip.end_date = editEndDate.text().toStdString();
        trip.total_budget = spinBudget.value();
        trip.status = comboStatus.currentText().toStdString();
        trip.notes = editNotes.toPlainText().toStdString();
        
        if (db.createTrip(trip)) {
            showInfo("Поездка успешно добавлена!");
            loadTrips();
        } else {
            showError("Ошибка добавления поездки: " + db.getLastError());
        }
    }
}

void MainWindow::onEditTripClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedTripId == -1) {
        showError("Выберите поездку для редактирования!");
        return;
    }
    
    Trip trip = db.readTrip(selectedTripId);
    if (trip.id == -1) {
        showError("Поездка не найдена!");
        return;
    }
    
    // Создаем диалоговое окно
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать поездку");
    dialog.setFixedSize(400, 400);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblName("Название:");
    QLineEdit editName;
    editName.setText(QString::fromStdString(trip.name));
    
    QLabel lblStartDate("Дата начала (ГГГГ-ММ-ДД):");
    QLineEdit editStartDate;
    editStartDate.setText(QString::fromStdString(trip.start_date));
    
    QLabel lblEndDate("Дата окончания (ГГГГ-ММ-ДД):");
    QLineEdit editEndDate;
    editEndDate.setText(QString::fromStdString(trip.end_date));
    
    QLabel lblBudget("Бюджет (руб.):");
    QDoubleSpinBox spinBudget;
    spinBudget.setRange(0, 10000000);
    spinBudget.setValue(trip.total_budget);
    spinBudget.setSuffix(" руб.");
    
    QLabel lblStatus("Статус:");
    QComboBox comboStatus;
    comboStatus.addItems({"planned", "in_progress", "completed", "cancelled"});
    comboStatus.setCurrentText(QString::fromStdString(trip.status));
    
    QLabel lblNotes("Заметки:");
    QTextEdit editNotes;
    editNotes.setText(QString::fromStdString(trip.notes));
    editNotes.setMaximumHeight(100);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblStartDate);
    layout.addWidget(&editStartDate);
    layout.addWidget(&lblEndDate);
    layout.addWidget(&editEndDate);
    layout.addWidget(&lblBudget);
    layout.addWidget(&spinBudget);
    layout.addWidget(&lblStatus);
    layout.addWidget(&comboStatus);
    layout.addWidget(&lblNotes);
    layout.addWidget(&editNotes);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название поездки не может быть пустым!");
            return;
        }
        
        trip.name = editName.text().toStdString();
        trip.start_date = editStartDate.text().toStdString();
        trip.end_date = editEndDate.text().toStdString();
        trip.total_budget = spinBudget.value();
        trip.status = comboStatus.currentText().toStdString();
        trip.notes = editNotes.toPlainText().toStdString();
        
        if (db.updateTrip(trip)) {
            showInfo("Поездка успешно обновлена!");
            loadTrips();
        } else {
            showError("Ошибка обновления поездки: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteTripClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedTripId == -1) {
        showError("Выберите поездку для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить выбранную поездку?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteTrip(selectedTripId)) {
            showInfo("Поездка успешно удалена!");
            selectedTripId = -1;
            loadTrips();
        } else {
            showError("Ошибка удаления поездки: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshTripsClicked()
{
    loadTrips();
}

void MainWindow::onTripSelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableTrips->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tableTrips->item(row, 0);
        if (idItem) {
            selectedTripId = idItem->text().toInt();
            btnEditTrip->setEnabled(true);
            btnDeleteTrip->setEnabled(true);
        }
    } else {
        selectedTripId = -1;
        btnEditTrip->setEnabled(false);
        btnDeleteTrip->setEnabled(false);
    }
}

// === Расходы ===
void MainWindow::loadExpenses()
{
    if (!db.isConnected()) return;
    
    std::vector<Expense> expenses = db.readExpenses();
    tableExpenses->setRowCount(expenses.size());
    
    for (size_t i = 0; i < expenses.size(); ++i) {
        const Expense& expense = expenses[i];
        
        tableExpenses->setItem(i, 0, new QTableWidgetItem(QString::number(expense.id)));
        tableExpenses->setItem(i, 1, new QTableWidgetItem(QString::number(expense.trip_destination_id)));
        tableExpenses->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(expense.expense_date)));
        
        std::ostringstream amountStream;
        amountStream << std::fixed << std::setprecision(2) << expense.amount;
        tableExpenses->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(amountStream.str())));
        
        tableExpenses->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(expense.currency)));
        tableExpenses->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(expense.category)));
        tableExpenses->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(expense.description)));
    }
    
    tableExpenses->resizeColumnsToContents();
}

void MainWindow::onAddExpenseClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить расход");
    dialog.setFixedSize(400, 350);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblTripDestId("ID места посещения:");
    QSpinBox spinTripDestId;
    spinTripDestId.setRange(1, 10000);
    spinTripDestId.setValue(1);
    
    QLabel lblDate("Дата расхода (ГГГГ-ММ-ДД):");
    QLineEdit editDate;
    editDate.setText(QDate::currentDate().toString("yyyy-MM-dd"));
    
    QLabel lblAmount("Сумма:");
    QDoubleSpinBox spinAmount;
    spinAmount.setRange(0, 1000000);
    spinAmount.setValue(1000);
    
    QLabel lblCurrency("Валюта:");
    QComboBox comboCurrency;
    comboCurrency.addItems({"RUB", "USD", "EUR"});
    
    QLabel lblCategory("Категория:");
    QComboBox comboCategory;
    comboCategory.addItems({"transport", "accommodation", "food", "entertainment", 
                           "shopping", "sightseeing", "other"});
    
    QLabel lblDescription("Описание:");
    QLineEdit editDescription;
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblTripDestId);
    layout.addWidget(&spinTripDestId);
    layout.addWidget(&lblDate);
    layout.addWidget(&editDate);
    layout.addWidget(&lblAmount);
    layout.addWidget(&spinAmount);
    layout.addWidget(&lblCurrency);
    layout.addWidget(&comboCurrency);
    layout.addWidget(&lblCategory);
    layout.addWidget(&comboCategory);
    layout.addWidget(&lblDescription);
    layout.addWidget(&editDescription);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editDescription.text().isEmpty()) {
            showError("Описание не может быть пустым!");
            return;
        }
        
        Expense expense;
        expense.trip_destination_id = spinTripDestId.value();
        expense.expense_date = editDate.text().toStdString();
        expense.amount = spinAmount.value();
        expense.currency = comboCurrency.currentText().toStdString();
        expense.category = comboCategory.currentText().toStdString();
        expense.description = editDescription.text().toStdString();
        
        if (db.createExpense(expense)) {
            showInfo("Расход успешно добавлен!");
            loadExpenses();
        } else {
            showError("Ошибка добавления расхода: " + db.getLastError());
        }
    }
}

void MainWindow::onEditExpenseClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedExpenseId == -1) {
        showError("Выберите расход для редактирования!");
        return;
    }
    
    Expense expense = db.readExpense(selectedExpenseId);
    if (expense.id == -1) {
        showError("Расход не найден!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать расход");
    dialog.setFixedSize(400, 350);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblTripDestId("ID места посещения:");
    QSpinBox spinTripDestId;
    spinTripDestId.setRange(1, 10000);
    spinTripDestId.setValue(expense.trip_destination_id);
    
    QLabel lblDate("Дата расхода (ГГГГ-ММ-ДД):");
    QLineEdit editDate;
    editDate.setText(QString::fromStdString(expense.expense_date));
    
    QLabel lblAmount("Сумма:");
    QDoubleSpinBox spinAmount;
    spinAmount.setRange(0, 1000000);
    spinAmount.setValue(expense.amount);
    
    QLabel lblCurrency("Валюта:");
    QComboBox comboCurrency;
    comboCurrency.addItems({"RUB", "USD", "EUR"});
    comboCurrency.setCurrentText(QString::fromStdString(expense.currency));
    
    QLabel lblCategory("Категория:");
    QComboBox comboCategory;
    comboCategory.addItems({"transport", "accommodation", "food", "entertainment", 
                           "shopping", "sightseeing", "other"});
    comboCategory.setCurrentText(QString::fromStdString(expense.category));
    
    QLabel lblDescription("Описание:");
    QLineEdit editDescription;
    editDescription.setText(QString::fromStdString(expense.description));
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblTripDestId);
    layout.addWidget(&spinTripDestId);
    layout.addWidget(&lblDate);
    layout.addWidget(&editDate);
    layout.addWidget(&lblAmount);
    layout.addWidget(&spinAmount);
    layout.addWidget(&lblCurrency);
    layout.addWidget(&comboCurrency);
    layout.addWidget(&lblCategory);
    layout.addWidget(&comboCategory);
    layout.addWidget(&lblDescription);
    layout.addWidget(&editDescription);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editDescription.text().isEmpty()) {
            showError("Описание не может быть пустым!");
            return;
        }
        
        expense.trip_destination_id = spinTripDestId.value();
        expense.expense_date = editDate.text().toStdString();
        expense.amount = spinAmount.value();
        expense.currency = comboCurrency.currentText().toStdString();
        expense.category = comboCategory.currentText().toStdString();
        expense.description = editDescription.text().toStdString();
        
        if (db.updateExpense(expense)) {
            showInfo("Расход успешно обновлен!");
            loadExpenses();
        } else {
            showError("Ошибка обновления расхода: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteExpenseClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedExpenseId == -1) {
        showError("Выберите расход для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить выбранный расход?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteExpense(selectedExpenseId)) {
            showInfo("Расход успешно удален!");
            selectedExpenseId = -1;
            loadExpenses();
        } else {
            showError("Ошибка удаления расхода: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshExpensesClicked()
{
    loadExpenses();
}

void MainWindow::onExpenseSelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableExpenses->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tableExpenses->item(row, 0);
        if (idItem) {
            selectedExpenseId = idItem->text().toInt();
            btnEditExpense->setEnabled(true);
            btnDeleteExpense->setEnabled(true);
        }
    } else {
        selectedExpenseId = -1;
        btnEditExpense->setEnabled(false);
        btnDeleteExpense->setEnabled(false);
    }
}

// === Сопутешественники ===
void MainWindow::loadCompanions()
{
    if (!db.isConnected()) return;
    
    std::vector<TravelCompanion> companions = db.readCompanions();
    tableCompanions->setRowCount(companions.size());
    
    for (size_t i = 0; i < companions.size(); ++i) {
        const TravelCompanion& companion = companions[i];
        
        tableCompanions->setItem(i, 0, new QTableWidgetItem(QString::number(companion.id)));
        tableCompanions->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(companion.first_name)));
        tableCompanions->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(companion.last_name)));
        tableCompanions->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(companion.email)));
        tableCompanions->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(companion.phone)));
        // Убрали setItem для поля relationship
    }
    
    tableCompanions->resizeColumnsToContents();
}

void MainWindow::onAddCompanionClicked() {
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить сопутешественника");
    dialog.setFixedSize(400, 250);  // Уменьшили высоту, убрав поле relationship
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblFirstName("Имя:");
    QLineEdit editFirstName;
    
    QLabel lblLastName("Фамилия:");
    QLineEdit editLastName;
    
    QLabel lblEmail("Email:");
    QLineEdit editEmail;
    
    QLabel lblPhone("Телефон:");
    QLineEdit editPhone;
    
    // Убрали поле relationship
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblFirstName);
    layout.addWidget(&editFirstName);
    layout.addWidget(&lblLastName);
    layout.addWidget(&editLastName);
    layout.addWidget(&lblEmail);
    layout.addWidget(&editEmail);
    layout.addWidget(&lblPhone);
    layout.addWidget(&editPhone);
    // Убрали layout.addWidget(&lblRelationship);
    // Убрали layout.addWidget(&comboRelationship);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editFirstName.text().isEmpty() || editLastName.text().isEmpty()) {
            showError("Имя и фамилия не могут быть пустыми!");
            return;
        }
        
        TravelCompanion companion;
        companion.first_name = editFirstName.text().toStdString();
        companion.last_name = editLastName.text().toStdString();
        companion.email = editEmail.text().toStdString();
        companion.phone = editPhone.text().toStdString();
        // Убрали companion.relationship
        
        if (db.createCompanion(companion)) {
            showInfo("Сопутешественник успешно добавлен!");
            loadCompanions();
        } else {
            showError("Ошибка добавления сопутешественника: " + db.getLastError());
        }
    }
}

void MainWindow::onEditCompanionClicked() {
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedCompanionId == -1) {
        showError("Выберите сопутешественника для редактирования!");
        return;
    }
    
    TravelCompanion companion = db.readCompanion(selectedCompanionId);
    if (companion.id == -1) {
        showError("Сопутешественник не найден!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать сопутешественника");
    dialog.setFixedSize(400, 250);  // Уменьшили высоту
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblFirstName("Имя:");
    QLineEdit editFirstName;
    editFirstName.setText(QString::fromStdString(companion.first_name));
    
    QLabel lblLastName("Фамилия:");
    QLineEdit editLastName;
    editLastName.setText(QString::fromStdString(companion.last_name));
    
    QLabel lblEmail("Email:");
    QLineEdit editEmail;
    editEmail.setText(QString::fromStdString(companion.email));
    
    QLabel lblPhone("Телефон:");
    QLineEdit editPhone;
    editPhone.setText(QString::fromStdString(companion.phone));
    
    // Убрали поле relationship
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblFirstName);
    layout.addWidget(&editFirstName);
    layout.addWidget(&lblLastName);
    layout.addWidget(&editLastName);
    layout.addWidget(&lblEmail);
    layout.addWidget(&editEmail);
    layout.addWidget(&lblPhone);
    layout.addWidget(&editPhone);
    // Убрали layout.addWidget(&lblRelationship);
    // Убрали layout.addWidget(&comboRelationship);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editFirstName.text().isEmpty() || editLastName.text().isEmpty()) {
            showError("Имя и фамилия не могут быть пустыми!");
            return;
        }
        
        companion.first_name = editFirstName.text().toStdString();
        companion.last_name = editLastName.text().toStdString();
        companion.email = editEmail.text().toStdString();
        companion.phone = editPhone.text().toStdString();
        // Убрали companion.relationship
        
        if (db.updateCompanion(companion)) {
            showInfo("Сопутешественник успешно обновлен!");
            loadCompanions();
        } else {
            showError("Ошибка обновления сопутешественника: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteCompanionClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedCompanionId == -1) {
        showError("Выберите сопутешественника для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить выбранного сопутешественника?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteCompanion(selectedCompanionId)) {
            showInfo("Сопутешественник успешно удален!");
            selectedCompanionId = -1;
            loadCompanions();
        } else {
            showError("Ошибка удаления сопутешественника: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshCompanionsClicked()
{
    loadCompanions();
}

void MainWindow::onCompanionSelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableCompanions->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tableCompanions->item(row, 0);
        if (idItem) {
            selectedCompanionId = idItem->text().toInt();
            btnEditCompanion->setEnabled(true);
            btnDeleteCompanion->setEnabled(true);
        }
    } else {
        selectedCompanionId = -1;
        btnEditCompanion->setEnabled(false);
        btnDeleteCompanion->setEnabled(false);
    }
}

// === Страны ===

// Новый метод createCountriesTab
QWidget* MainWindow::createCountriesTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок основная
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddCountry = new QPushButton("Добавить", tab);
    btnEditCountry = new QPushButton("Редактировать", tab);
    btnEditCountry->setEnabled(false);
    btnDeleteCountry = new QPushButton("Удалить", tab);
    btnDeleteCountry->setEnabled(false);
    btnRefreshCountries = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddCountry);
    buttonLayout->addWidget(btnEditCountry);
    buttonLayout->addWidget(btnDeleteCountry);
    buttonLayout->addWidget(btnRefreshCountries);
    buttonLayout->addStretch();
    
    // Панель фильтров
    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    btnFilterByContinent = new QPushButton("По континенту", tab);
    btnFilterVisa = new QPushButton("Требуют визу", tab);
    btnFilterNoVisa = new QPushButton("Без визы", tab);
    btnShowAllCountries = new QPushButton("Все страны", tab);
    
    filterLayout->addWidget(btnFilterByContinent);
    filterLayout->addWidget(btnFilterVisa);
    filterLayout->addWidget(btnFilterNoVisa);
    filterLayout->addWidget(btnShowAllCountries);
    filterLayout->addStretch();
    
    // Таблица стран
    tableCountries = new QTableWidget(tab);
    tableCountries->setColumnCount(6);
    tableCountries->setHorizontalHeaderLabels({"ID", "Название", "Код ISO", 
                                              "Континент", "Визовый режим", 
                                              "Создано"});
    tableCountries->horizontalHeader()->setStretchLastSection(true);
    tableCountries->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(tableCountries);
    
    return tab;
}

// Метод загрузки стран
void MainWindow::loadCountries()
{
    if (!db.isConnected()) return;
    
    std::vector<Country> countries = db.readCountries();
    tableCountries->setRowCount(countries.size());
    
    for (size_t i = 0; i < countries.size(); ++i) {
        const Country& country = countries[i];
        
        tableCountries->setItem(i, 0, new QTableWidgetItem(QString::number(country.id)));
        tableCountries->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(country.name)));
        tableCountries->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(country.iso_code)));
        tableCountries->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(country.continent)));
        tableCountries->setItem(i, 4, new QTableWidgetItem(country.visa_required ? "Требуется" : "Не требуется"));
        tableCountries->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(country.created_at)));
    }
    
    tableCountries->resizeColumnsToContents();
}

// Добавить после других CRUD методов
void MainWindow::onAddCountryClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить страну");
    dialog.setFixedSize(400, 300);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblName("Название страны:");
    QLineEdit editName;
    
    QLabel lblIsoCode("Код ISO (3 символа):");
    QLineEdit editIsoCode;
    editIsoCode.setMaxLength(3);
    
    QLabel lblContinent("Континент:");
    QComboBox comboContinent;
    comboContinent.addItems({"Азия", "Европа", "Африка", "Северная Америка", 
                            "Южная Америка", "Австралия", "Антарктида"});
    
    QLabel lblVisaRequired("Визовый режим:");
    QCheckBox checkVisa("Требуется виза");
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblIsoCode);
    layout.addWidget(&editIsoCode);
    layout.addWidget(&lblContinent);
    layout.addWidget(&comboContinent);
    layout.addWidget(&lblVisaRequired);
    layout.addWidget(&checkVisa);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название страны не может быть пустым!");
            return;
        }
        
        if (editIsoCode.text().length() != 3) {
            showError("Код ISO должен содержать 3 символа!");
            return;
        }
        
        Country country;
        country.name = editName.text().toStdString();
        country.iso_code = editIsoCode.text().toUpper().toStdString();
        country.continent = comboContinent.currentText().toStdString();
        country.visa_required = checkVisa.isChecked();
        
        if (db.createCountry(country)) {
            showInfo("Страна успешно добавлена!");
            loadCountries();
        } else {
            showError("Ошибка добавления страны: " + db.getLastError());
        }
    }
}

void MainWindow::onEditCountryClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedCountryId == -1) {
        showError("Выберите страну для редактирования!");
        return;
    }
    
    Country country = db.readCountry(selectedCountryId);
    if (country.id == -1) {
        showError("Страна не найдена!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать страну");
    dialog.setFixedSize(400, 300);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblName("Название страны:");
    QLineEdit editName;
    editName.setText(QString::fromStdString(country.name));
    
    QLabel lblIsoCode("Код ISO (3 символа):");
    QLineEdit editIsoCode;
    editIsoCode.setText(QString::fromStdString(country.iso_code));
    editIsoCode.setMaxLength(3);
    
    QLabel lblContinent("Континент:");
    QComboBox comboContinent;
    comboContinent.addItems({"Азия", "Европа", "Африка", "Северная Америка", 
                            "Южная Америка", "Австралия", "Антарктида"});
    comboContinent.setCurrentText(QString::fromStdString(country.continent));
    
    QLabel lblVisaRequired("Визовый режим:");
    QCheckBox checkVisa("Требуется виза");
    checkVisa.setChecked(country.visa_required);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblIsoCode);
    layout.addWidget(&editIsoCode);
    layout.addWidget(&lblContinent);
    layout.addWidget(&comboContinent);
    layout.addWidget(&lblVisaRequired);
    layout.addWidget(&checkVisa);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название страны не может быть пустым!");
            return;
        }
        
        if (editIsoCode.text().length() != 3) {
            showError("Код ISO должен содержать 3 символа!");
            return;
        }
        
        country.name = editName.text().toStdString();
        country.iso_code = editIsoCode.text().toUpper().toStdString();
        country.continent = comboContinent.currentText().toStdString();
        country.visa_required = checkVisa.isChecked();
        
        if (db.updateCountry(country)) {
            showInfo("Страна успешно обновлена!");
            loadCountries();
        } else {
            showError("Ошибка обновления страны: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteCountryClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedCountryId == -1) {
        showError("Выберите страну для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить выбранную страну?\n"
                                  "Все связанные города также будут удалены!",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteCountry(selectedCountryId)) {
            showInfo("Страна успешно удалена!");
            selectedCountryId = -1;
            loadCountries();
        } else {
            showError("Ошибка удаления страны: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshCountriesClicked()
{
    loadCountries();
}

void MainWindow::onFilterByContinentClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QStringList continents = {"Азия", "Европа", "Африка", "Северная Америка", 
                             "Южная Америка", "Австралия", "Антарктида"};
    
    bool ok;
    QString continent = QInputDialog::getItem(this, "Фильтр по континенту", 
                                             "Выберите континент:", 
                                             continents, 0, false, &ok);
    if (!ok) return;
    
    std::vector<Country> countries = db.getCountriesByContinent(continent.toStdString());
    tableCountries->setRowCount(countries.size());
    
    for (size_t i = 0; i < countries.size(); ++i) {
        const Country& country = countries[i];
        
        tableCountries->setItem(i, 0, new QTableWidgetItem(QString::number(country.id)));
        tableCountries->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(country.name)));
        tableCountries->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(country.iso_code)));
        tableCountries->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(country.continent)));
        tableCountries->setItem(i, 4, new QTableWidgetItem(country.visa_required ? "Требуется" : "Не требуется"));
        tableCountries->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(country.created_at)));
    }
    
    tableCountries->resizeColumnsToContents();
}

void MainWindow::onFilterVisaClicked()
{
    if (!db.isConnected()) return;
    
    std::vector<Country> countries = db.getCountriesRequiringVisa();
    tableCountries->setRowCount(countries.size());
    
    for (size_t i = 0; i < countries.size(); ++i) {
        const Country& country = countries[i];
        
        tableCountries->setItem(i, 0, new QTableWidgetItem(QString::number(country.id)));
        tableCountries->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(country.name)));
        tableCountries->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(country.iso_code)));
        tableCountries->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(country.continent)));
        tableCountries->setItem(i, 4, new QTableWidgetItem("Требуется"));
        tableCountries->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(country.created_at)));
    }
    
    tableCountries->resizeColumnsToContents();
}

void MainWindow::onFilterNoVisaClicked()
{
    if (!db.isConnected()) return;
    
    std::vector<Country> countries = db.getCountriesNotRequiringVisa();
    tableCountries->setRowCount(countries.size());
    
    for (size_t i = 0; i < countries.size(); ++i) {
        const Country& country = countries[i];
        
        tableCountries->setItem(i, 0, new QTableWidgetItem(QString::number(country.id)));
        tableCountries->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(country.name)));
        tableCountries->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(country.iso_code)));
        tableCountries->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(country.continent)));
        tableCountries->setItem(i, 4, new QTableWidgetItem("Не требуется"));
        tableCountries->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(country.created_at)));
    }
    
    tableCountries->resizeColumnsToContents();
}

void MainWindow::onShowAllCountriesClicked()
{
    loadCountries();
}

void MainWindow::onCountrySelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableCountries->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tableCountries->item(row, 0);
        if (idItem) {
            selectedCountryId = idItem->text().toInt();
            btnEditCountry->setEnabled(true);
            btnDeleteCountry->setEnabled(true);
        }
    } else {
        selectedCountryId = -1;
        btnEditCountry->setEnabled(false);
        btnDeleteCountry->setEnabled(false);
    }
}

// === Города ===

QWidget* MainWindow::createCitiesTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок основная
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddCity = new QPushButton("Добавить", tab);
    btnEditCity = new QPushButton("Редактировать", tab);
    btnEditCity->setEnabled(false);
    btnDeleteCity = new QPushButton("Удалить", tab);
    btnDeleteCity->setEnabled(false);
    btnRefreshCities = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddCity);
    buttonLayout->addWidget(btnEditCity);
    buttonLayout->addWidget(btnDeleteCity);
    buttonLayout->addWidget(btnRefreshCities);
    buttonLayout->addStretch();
    
    // Панель фильтров
    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    btnFilterByCountry = new QPushButton("По стране", tab);
    btnFilterWithAirport = new QPushButton("С аэропортом", tab);
    btnShowAllCities = new QPushButton("Все города", tab);
    
    filterLayout->addWidget(btnFilterByCountry);
    filterLayout->addWidget(btnFilterWithAirport);
    filterLayout->addWidget(btnShowAllCities);
    filterLayout->addStretch();
    
    // Таблица городов
    tableCities = new QTableWidget(tab);
    tableCities->setColumnCount(8);
    tableCities->setHorizontalHeaderLabels({"ID", "Название", "ID страны", 
                                           "Страна", "Население", "Часовой пояс", 
                                           "Аэропорт", "Создано"});
    tableCities->horizontalHeader()->setStretchLastSection(true);
    tableCities->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(tableCities);
    
    return tab;
}

// Метод загрузки городов
void MainWindow::loadCities()
{
    if (!db.isConnected()) return;
    
    std::vector<City> cities = db.readCities();
    tableCities->setRowCount(cities.size());
    
    for (size_t i = 0; i < cities.size(); ++i) {
        const City& city = cities[i];
        
        tableCities->setItem(i, 0, new QTableWidgetItem(QString::number(city.id)));
        tableCities->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(city.name)));
        tableCities->setItem(i, 2, new QTableWidgetItem(QString::number(city.country_id)));
        
        // Получаем название страны (нужно добавить метод в Database)
        Country country = db.readCountry(city.country_id);
        QString countryName = (country.id != -1) ? QString::fromStdString(country.name) : "Неизвестно";
        tableCities->setItem(i, 3, new QTableWidgetItem(countryName));
        
        tableCities->setItem(i, 4, new QTableWidgetItem(
            city.population > 0 ? QString::number(city.population) : ""));
        tableCities->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(city.time_zone)));
        tableCities->setItem(i, 6, new QTableWidgetItem(city.has_airport ? "Есть" : "Нет"));
        tableCities->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(city.created_at)));
    }
    
    tableCities->resizeColumnsToContents();
}

// Добавить после методов для стран
void MainWindow::onAddCityClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить город");
    dialog.setFixedSize(400, 400);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblName("Название города:");
    QLineEdit editName;
    
    QLabel lblCountryId("ID страны:");
    QSpinBox spinCountryId;
    spinCountryId.setRange(1, 10000);
    spinCountryId.setValue(1);
    
    QLabel lblPopulation("Население (опционально):");
    QSpinBox spinPopulation;
    spinPopulation.setRange(0, 100000000);
    spinPopulation.setValue(0);
    spinPopulation.setSpecialValueText("Не указано");
    
    QLabel lblTimeZone("Часовой пояс (опционально):");
    QLineEdit editTimeZone;
    editTimeZone.setPlaceholderText("Например: UTC+3");
    
    QLabel lblHasAirport("Инфраструктура:");
    QCheckBox *checkAirport = new QCheckBox("Есть аэропорт");
    checkAirport->setChecked(false);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblCountryId);
    layout.addWidget(&spinCountryId);
    layout.addWidget(&lblPopulation);
    layout.addWidget(&spinPopulation);
    layout.addWidget(&lblTimeZone);
    layout.addWidget(&editTimeZone);
    layout.addWidget(&lblHasAirport);
    layout.addWidget(checkAirport);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название города не может быть пустым!");
            return;
        }
        
        City city;
        city.name = editName.text().toStdString();
        city.country_id = spinCountryId.value();
        city.population = spinPopulation.value();
        city.time_zone = editTimeZone.text().toStdString();
        city.has_airport = checkAirport->isChecked();
        
        if (db.createCity(city)) {
            showInfo("Город успешно добавлен!");
            loadCities();
        } else {
            showError("Ошибка добавления города: " + db.getLastError());
        }
    }
}

void MainWindow::onEditCityClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedCityId == -1) {
        showError("Выберите город для редактирования!");
        return;
    }
    
    City city = db.readCity(selectedCityId);
    if (city.id == -1) {
        showError("Город не найден!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать город");
    dialog.setFixedSize(400, 400);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblName("Название города:");
    QLineEdit editName;
    editName.setText(QString::fromStdString(city.name));
    
    QLabel lblCountryId("ID страны:");
    QSpinBox spinCountryId;
    spinCountryId.setRange(1, 10000);
    spinCountryId.setValue(city.country_id);
    
    QLabel lblPopulation("Население (опционально):");
    QSpinBox spinPopulation;
    spinPopulation.setRange(0, 100000000);
    spinPopulation.setValue(city.population);
    spinPopulation.setSpecialValueText("Не указано");
    
    QLabel lblTimeZone("Часовой пояс (опционально):");
    QLineEdit editTimeZone;
    editTimeZone.setText(QString::fromStdString(city.time_zone));
    
    QLabel lblHasAirport("Инфраструктура:");
    QCheckBox *checkAirport = new QCheckBox("Есть аэропорт");
    checkAirport->setChecked(city.has_airport);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblCountryId);
    layout.addWidget(&spinCountryId);
    layout.addWidget(&lblPopulation);
    layout.addWidget(&spinPopulation);
    layout.addWidget(&lblTimeZone);
    layout.addWidget(&editTimeZone);
    layout.addWidget(&lblHasAirport);
    layout.addWidget(checkAirport);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название города не может быть пустым!");
            return;
        }
        
        city.name = editName.text().toStdString();
        city.country_id = spinCountryId.value();
        city.population = spinPopulation.value();
        city.time_zone = editTimeZone.text().toStdString();
        city.has_airport = checkAirport->isChecked();
        
        if (db.updateCity(city)) {
            showInfo("Город успешно обновлен!");
            loadCities();
        } else {
            showError("Ошибка обновления города: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteCityClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedCityId == -1) {
        showError("Выберите город для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить выбранный город?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteCity(selectedCityId)) {
            showInfo("Город успешно удален!");
            selectedCityId = -1;
            loadCities();
        } else {
            showError("Ошибка удаления города: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshCitiesClicked()
{
    loadCities();
}

void MainWindow::onFilterByCountryClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int countryId = QInputDialog::getInt(this, "Фильтр по стране", 
                                        "Введите ID страны:", 
                                        1, 1, 10000, 1, &ok);
    if (!ok) return;
    
    std::vector<City> cities = db.getCitiesByCountry(countryId);
    tableCities->setRowCount(cities.size());
    
    for (size_t i = 0; i < cities.size(); ++i) {
        const City& city = cities[i];
        
        tableCities->setItem(i, 0, new QTableWidgetItem(QString::number(city.id)));
        tableCities->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(city.name)));
        tableCities->setItem(i, 2, new QTableWidgetItem(QString::number(city.country_id)));
        
        Country country = db.readCountry(city.country_id);
        QString countryName = (country.id != -1) ? QString::fromStdString(country.name) : "Неизвестно";
        tableCities->setItem(i, 3, new QTableWidgetItem(countryName));
        
        tableCities->setItem(i, 4, new QTableWidgetItem(
            city.population > 0 ? QString::number(city.population) : ""));
        tableCities->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(city.time_zone)));
        tableCities->setItem(i, 6, new QTableWidgetItem(city.has_airport ? "Есть" : "Нет"));
        tableCities->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(city.created_at)));
    }
    
    tableCities->resizeColumnsToContents();
}

void MainWindow::onFilterWithAirportClicked()
{
    if (!db.isConnected()) return;
    
    std::vector<City> cities = db.getCitiesWithAirport();
    tableCities->setRowCount(cities.size());
    
    for (size_t i = 0; i < cities.size(); ++i) {
        const City& city = cities[i];
        
        tableCities->setItem(i, 0, new QTableWidgetItem(QString::number(city.id)));
        tableCities->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(city.name)));
        tableCities->setItem(i, 2, new QTableWidgetItem(QString::number(city.country_id)));
        
        Country country = db.readCountry(city.country_id);
        QString countryName = (country.id != -1) ? QString::fromStdString(country.name) : "Неизвестно";
        tableCities->setItem(i, 3, new QTableWidgetItem(countryName));
        
        tableCities->setItem(i, 4, new QTableWidgetItem(
            city.population > 0 ? QString::number(city.population) : ""));
        tableCities->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(city.time_zone)));
        tableCities->setItem(i, 6, new QTableWidgetItem("Есть"));
        tableCities->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(city.created_at)));
    }
    
    tableCities->resizeColumnsToContents();
}

void MainWindow::onShowAllCitiesClicked()
{
    loadCities();
}

void MainWindow::onCitySelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableCities->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tableCities->item(row, 0);
        if (idItem) {
            selectedCityId = idItem->text().toInt();
            btnEditCity->setEnabled(true);
            btnDeleteCity->setEnabled(true);
        }
    } else {
        selectedCityId = -1;
        btnEditCity->setEnabled(false);
        btnDeleteCity->setEnabled(false);
    }
}

// === Фотографии ===

QWidget* MainWindow::createPhotosTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок основная
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddPhoto = new QPushButton("Добавить", tab);
    btnEditPhoto = new QPushButton("Редактировать", tab);
    btnEditPhoto->setEnabled(false);
    btnDeletePhoto = new QPushButton("Удалить", tab);
    btnDeletePhoto->setEnabled(false);
    btnRefreshPhotos = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddPhoto);
    buttonLayout->addWidget(btnEditPhoto);
    buttonLayout->addWidget(btnDeletePhoto);
    buttonLayout->addWidget(btnRefreshPhotos);
    buttonLayout->addStretch();
    
    // Панель фильтров
    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    btnFilterByDestination = new QPushButton("По месту", tab);
    btnFilterByRating = new QPushButton("По рейтингу", tab);
    btnShowAllPhotos = new QPushButton("Все фото", tab);
    
    filterLayout->addWidget(btnFilterByDestination);
    filterLayout->addWidget(btnFilterByRating);
    filterLayout->addWidget(btnShowAllPhotos);
    filterLayout->addStretch();
    
    // Таблица фотографий
    tablePhotos = new QTableWidget(tab);
    tablePhotos->setColumnCount(9);
    tablePhotos->setHorizontalHeaderLabels({"ID", "ID места", "Название", 
                                           "Файл", "Дата съемки", "Описание", 
                                           "Рейтинг", "Теги", "Создано"});
    tablePhotos->horizontalHeader()->setStretchLastSection(true);
    tablePhotos->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(tablePhotos);
    
    return tab;
}

// Метод загрузки фотографий
void MainWindow::loadPhotos()
{
    if (!db.isConnected()) return;
    
    std::vector<Photo> photos = db.readPhotos();
    tablePhotos->setRowCount(photos.size());
    
    for (size_t i = 0; i < photos.size(); ++i) {
        const Photo& photo = photos[i];
        
        tablePhotos->setItem(i, 0, new QTableWidgetItem(QString::number(photo.id)));
        tablePhotos->setItem(i, 1, new QTableWidgetItem(QString::number(photo.trip_destination_id)));
        tablePhotos->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(photo.name)));
        
        // Сокращаем путь к файлу для отображения
        QString filePath = QString::fromStdString(photo.file_path);
        if (filePath.length() > 30) {
            filePath = "..." + filePath.right(27);
        }
        tablePhotos->setItem(i, 3, new QTableWidgetItem(filePath));
        
        tablePhotos->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(photo.taken_date)));
        
        // Сокращаем описание для отображения
        QString description = QString::fromStdString(photo.description);
        if (description.length() > 50) {
            description = description.left(47) + "...";
        }
        tablePhotos->setItem(i, 5, new QTableWidgetItem(description));
        
        tablePhotos->setItem(i, 6, new QTableWidgetItem(
            photo.rating > 0 ? QString::number(photo.rating) + "★" : ""));
        tablePhotos->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(photo.tags)));
        tablePhotos->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(photo.created_at)));
    }
    
    tablePhotos->resizeColumnsToContents();
}

void MainWindow::onAddPhotoClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить фотографию");
    dialog.setFixedSize(500, 450);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblDestinationId("ID места посещения:");
    QSpinBox spinDestinationId;
    spinDestinationId.setRange(1, 10000);
    spinDestinationId.setValue(1);
    
    QLabel lblName("Название фото:");
    QLineEdit editName;
    
    QLabel lblFilePath("Путь к файлу:");
    QLineEdit editFilePath;
    editFilePath.setPlaceholderText("/path/to/photo.jpg");
    
    QLabel lblTakenDate("Дата съемки (ГГГГ-ММ-ДД):");
    QLineEdit editTakenDate;
    editTakenDate.setText(QDate::currentDate().toString("yyyy-MM-dd"));
    
    QLabel lblDescription("Описание:");
    QTextEdit editDescription;
    editDescription.setMaximumHeight(80);
    
    QLabel lblRating("Рейтинг (1-5):");
    QSpinBox spinRating;
    spinRating.setRange(1, 5);
    spinRating.setValue(3);
    
    QLabel lblTags("Теги (JSON или текст):");
    QLineEdit editTags;
    editTags.setPlaceholderText("{\"tags\": [\"nature\", \"city\"]}");
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblDestinationId);
    layout.addWidget(&spinDestinationId);
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblFilePath);
    layout.addWidget(&editFilePath);
    layout.addWidget(&lblTakenDate);
    layout.addWidget(&editTakenDate);
    layout.addWidget(&lblDescription);
    layout.addWidget(&editDescription);
    layout.addWidget(&lblRating);
    layout.addWidget(&spinRating);
    layout.addWidget(&lblTags);
    layout.addWidget(&editTags);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название фото не может быть пустым!");
            return;
        }
        
        if (editFilePath.text().isEmpty()) {
            showError("Путь к файлу не может быть пустым!");
            return;
        }
        
        Photo photo;
        photo.trip_destination_id = spinDestinationId.value();
        photo.name = editName.text().toStdString();
        photo.file_path = editFilePath.text().toStdString();
        photo.taken_date = editTakenDate.text().toStdString();
        photo.description = editDescription.toPlainText().toStdString();
        photo.rating = spinRating.value();
        photo.tags = editTags.text().toStdString();
        
        if (db.createPhoto(photo)) {
            showInfo("Фотография успешно добавлена!");
            loadPhotos();
        } else {
            showError("Ошибка добавления фотографии: " + db.getLastError());
        }
    }
}

void MainWindow::onEditPhotoClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedPhotoId == -1) {
        showError("Выберите фотографию для редактирования!");
        return;
    }
    
    Photo photo = db.readPhoto(selectedPhotoId);
    if (photo.id == -1) {
        showError("Фотография не найдена!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать фотографию");
    dialog.setFixedSize(500, 450);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblDestinationId("ID места посещения:");
    QSpinBox spinDestinationId;
    spinDestinationId.setRange(1, 10000);
    spinDestinationId.setValue(photo.trip_destination_id);
    
    QLabel lblName("Название фото:");
    QLineEdit editName;
    editName.setText(QString::fromStdString(photo.name));
    
    QLabel lblFilePath("Путь к файлу:");
    QLineEdit editFilePath;
    editFilePath.setText(QString::fromStdString(photo.file_path));
    
    QLabel lblTakenDate("Дата съемки (ГГГГ-ММ-ДД):");
    QLineEdit editTakenDate;
    editTakenDate.setText(QString::fromStdString(photo.taken_date));
    
    QLabel lblDescription("Описание:");
    QTextEdit editDescription;
    editDescription.setText(QString::fromStdString(photo.description));
    editDescription.setMaximumHeight(80);
    
    QLabel lblRating("Рейтинг (1-5):");
    QSpinBox spinRating;
    spinRating.setRange(1, 5);
    spinRating.setValue(photo.rating);
    
    QLabel lblTags("Теги (JSON или текст):");
    QLineEdit editTags;
    editTags.setText(QString::fromStdString(photo.tags));
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblDestinationId);
    layout.addWidget(&spinDestinationId);
    layout.addWidget(&lblName);
    layout.addWidget(&editName);
    layout.addWidget(&lblFilePath);
    layout.addWidget(&editFilePath);
    layout.addWidget(&lblTakenDate);
    layout.addWidget(&editTakenDate);
    layout.addWidget(&lblDescription);
    layout.addWidget(&editDescription);
    layout.addWidget(&lblRating);
    layout.addWidget(&spinRating);
    layout.addWidget(&lblTags);
    layout.addWidget(&editTags);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        if (editName.text().isEmpty()) {
            showError("Название фото не может быть пустым!");
            return;
        }
        
        if (editFilePath.text().isEmpty()) {
            showError("Путь к файлу не может быть пустым!");
            return;
        }
        
        photo.trip_destination_id = spinDestinationId.value();
        photo.name = editName.text().toStdString();
        photo.file_path = editFilePath.text().toStdString();
        photo.taken_date = editTakenDate.text().toStdString();
        photo.description = editDescription.toPlainText().toStdString();
        photo.rating = spinRating.value();
        photo.tags = editTags.text().toStdString();
        
        if (db.updatePhoto(photo)) {
            showInfo("Фотография успешно обновлена!");
            loadPhotos();
        } else {
            showError("Ошибка обновления фотографии: " + db.getLastError());
        }
    }
}

void MainWindow::onDeletePhotoClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedPhotoId == -1) {
        showError("Выберите фотографию для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить выбранную фотографию?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deletePhoto(selectedPhotoId)) {
            showInfo("Фотография успешно удалена!");
            selectedPhotoId = -1;
            loadPhotos();
        } else {
            showError("Ошибка удаления фотографии: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshPhotosClicked()
{
    loadPhotos();
}

void MainWindow::onFilterByDestinationClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int destinationId = QInputDialog::getInt(this, "Фильтр по месту", 
                                            "Введите ID места посещения:", 
                                            1, 1, 10000, 1, &ok);
    if (!ok) return;
    
    std::vector<Photo> photos = db.getPhotosByDestination(destinationId);
    tablePhotos->setRowCount(photos.size());
    
    for (size_t i = 0; i < photos.size(); ++i) {
        const Photo& photo = photos[i];
        
        tablePhotos->setItem(i, 0, new QTableWidgetItem(QString::number(photo.id)));
        tablePhotos->setItem(i, 1, new QTableWidgetItem(QString::number(photo.trip_destination_id)));
        tablePhotos->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(photo.name)));
        
        QString filePath = QString::fromStdString(photo.file_path);
        if (filePath.length() > 30) {
            filePath = "..." + filePath.right(27);
        }
        tablePhotos->setItem(i, 3, new QTableWidgetItem(filePath));
        
        tablePhotos->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(photo.taken_date)));
        
        QString description = QString::fromStdString(photo.description);
        if (description.length() > 50) {
            description = description.left(47) + "...";
        }
        tablePhotos->setItem(i, 5, new QTableWidgetItem(description));
        
        tablePhotos->setItem(i, 6, new QTableWidgetItem(
            photo.rating > 0 ? QString::number(photo.rating) + "★" : ""));
        tablePhotos->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(photo.tags)));
        tablePhotos->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(photo.created_at)));
    }
    
    tablePhotos->resizeColumnsToContents();
}

void MainWindow::onFilterByRatingClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int minRating = QInputDialog::getInt(this, "Фильтр по рейтингу", 
                                        "Минимальный рейтинг (1-5):", 
                                        3, 1, 5, 1, &ok);
    if (!ok) return;
    
    std::vector<Photo> photos = db.getPhotosByRating(minRating);
    tablePhotos->setRowCount(photos.size());
    
    for (size_t i = 0; i < photos.size(); ++i) {
        const Photo& photo = photos[i];
        
        tablePhotos->setItem(i, 0, new QTableWidgetItem(QString::number(photo.id)));
        tablePhotos->setItem(i, 1, new QTableWidgetItem(QString::number(photo.trip_destination_id)));
        tablePhotos->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(photo.name)));
        
        QString filePath = QString::fromStdString(photo.file_path);
        if (filePath.length() > 30) {
            filePath = "..." + filePath.right(27);
        }
        tablePhotos->setItem(i, 3, new QTableWidgetItem(filePath));
        
        tablePhotos->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(photo.taken_date)));
        
        QString description = QString::fromStdString(photo.description);
        if (description.length() > 50) {
            description = description.left(47) + "...";
        }
        tablePhotos->setItem(i, 5, new QTableWidgetItem(description));
        
        tablePhotos->setItem(i, 6, new QTableWidgetItem(
            photo.rating > 0 ? QString::number(photo.rating) + "★" : ""));
        tablePhotos->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(photo.tags)));
        tablePhotos->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(photo.created_at)));
    }
    
    tablePhotos->resizeColumnsToContents();
}

void MainWindow::onShowAllPhotosClicked()
{
    loadPhotos();
}

void MainWindow::onPhotoSelectionChanged()
{
    QList<QTableWidgetItem*> selected = tablePhotos->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tablePhotos->item(row, 0);
        if (idItem) {
            selectedPhotoId = idItem->text().toInt();
            btnEditPhoto->setEnabled(true);
            btnDeletePhoto->setEnabled(true);
        }
    } else {
        selectedPhotoId = -1;
        btnEditPhoto->setEnabled(false);
        btnDeletePhoto->setEnabled(false);
    }
}

// === связь trip - companions
// Новый метод createTripCompanionsTab
QWidget* MainWindow::createTripCompanionsTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок основная
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddTripCompanion = new QPushButton("Добавить", tab);
    btnEditTripCompanion = new QPushButton("Редактировать", tab);
    btnEditTripCompanion->setEnabled(false);
    btnDeleteTripCompanion = new QPushButton("Удалить", tab);
    btnDeleteTripCompanion->setEnabled(false);
    btnRefreshTripCompanions = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddTripCompanion);
    buttonLayout->addWidget(btnEditTripCompanion);
    buttonLayout->addWidget(btnDeleteTripCompanion);
    buttonLayout->addWidget(btnRefreshTripCompanions);
    buttonLayout->addStretch();
    
    // Панель фильтров
    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    btnFilterByTrip = new QPushButton("По поездке", tab);
    btnFilterByCompanion = new QPushButton("По участнику", tab);
    btnShowAllTripCompanions = new QPushButton("Все записи", tab);
    
    filterLayout->addWidget(btnFilterByTrip);
    filterLayout->addWidget(btnFilterByCompanion);
    filterLayout->addWidget(btnShowAllTripCompanions);
    filterLayout->addStretch();
    
    // Таблица участников поездок
    tableTripCompanions = new QTableWidget(tab);
    tableTripCompanions->setColumnCount(9);
    tableTripCompanions->setHorizontalHeaderLabels({"Поездка ID", "Участник ID", 
                                                   "Роль", "Дата присоединения", 
                                                   "Дата выхода", "Делит расходы", 
                                                   "Создано", "Поездка", "Участник"});
    tableTripCompanions->horizontalHeader()->setStretchLastSection(true);
    tableTripCompanions->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(tableTripCompanions);
    
    return tab;
}

// Метод загрузки участников поездок
void MainWindow::loadTripCompanions()
{
    if (!db.isConnected()) return;
    
    std::vector<TripCompanion> tripCompanions = db.readTripCompanions();
    tableTripCompanions->setRowCount(tripCompanions.size());
    
    for (size_t i = 0; i < tripCompanions.size(); ++i) {
        const TripCompanion& tc = tripCompanions[i];
        
        tableTripCompanions->setItem(i, 0, new QTableWidgetItem(QString::number(tc.trip_id)));
        tableTripCompanions->setItem(i, 1, new QTableWidgetItem(QString::number(tc.companion_id)));
        tableTripCompanions->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tc.role)));
        tableTripCompanions->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(tc.joined_date)));
        tableTripCompanions->setItem(i, 4, new QTableWidgetItem(
            tc.left_date.empty() ? "" : QString::fromStdString(tc.left_date)));
        tableTripCompanions->setItem(i, 5, new QTableWidgetItem(tc.share_expenses ? "Да" : "Нет"));
        tableTripCompanions->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(tc.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(tc.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripCompanions->setItem(i, 7, new QTableWidgetItem(tripName));
        
        // Получаем имя участника
        TravelCompanion companion = db.readCompanion(tc.companion_id);
        QString companionName = (companion.id != -1) ? 
            QString::fromStdString(companion.first_name + " " + companion.last_name) : "Неизвестно";
        tableTripCompanions->setItem(i, 8, new QTableWidgetItem(companionName));
    }
    
    tableTripCompanions->resizeColumnsToContents();
}

// Добавить после методов для фотографий
void MainWindow::onAddTripCompanionClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить участника к поездке");
    dialog.setFixedSize(400, 350);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblTripId("ID поездки:");
    QSpinBox spinTripId;
    spinTripId.setRange(1, 10000);
    spinTripId.setValue(1);
    
    QLabel lblCompanionId("ID сопутчика:");
    QSpinBox spinCompanionId;
    spinCompanionId.setRange(1, 10000);
    spinCompanionId.setValue(1);
    
    QLabel lblRole("Роль:");
    QComboBox comboRole;
    comboRole.addItems({"organizer", "companion", "guide"});
    
    QLabel lblJoinedDate("Дата присоединения (ГГГГ-ММ-ДД):");
    QLineEdit editJoinedDate;
    editJoinedDate.setText(QDate::currentDate().toString("yyyy-MM-dd"));
    
    QLabel lblLeftDate("Дата выхода (ГГГГ-ММ-ДД, опционально):");
    QLineEdit editLeftDate;
    editLeftDate.setPlaceholderText("Оставьте пустым если еще в поездке");
    
    QLabel lblShareExpenses("Делит расходы:");
    QCheckBox *checkShareExpenses = new QCheckBox("Да");
    checkShareExpenses->setChecked(true);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblTripId);
    layout.addWidget(&spinTripId);
    layout.addWidget(&lblCompanionId);
    layout.addWidget(&spinCompanionId);
    layout.addWidget(&lblRole);
    layout.addWidget(&comboRole);
    layout.addWidget(&lblJoinedDate);
    layout.addWidget(&editJoinedDate);
    layout.addWidget(&lblLeftDate);
    layout.addWidget(&editLeftDate);
    layout.addWidget(&lblShareExpenses);
    layout.addWidget(checkShareExpenses);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Проверяем существование поездки
        Trip trip = db.readTrip(spinTripId.value());
        if (trip.id == -1) {
            showError("Поездка с указанным ID не найдена!");
            return;
        }
        
        // Проверяем существование участника
        TravelCompanion companion = db.readCompanion(spinCompanionId.value());
        if (companion.id == -1) {
            showError("Сопутчик с указанным ID не найден!");
            return;
        }
        
        TripCompanion tc;
        tc.trip_id = spinTripId.value();
        tc.companion_id = spinCompanionId.value();
        tc.role = comboRole.currentText().toStdString();
        tc.joined_date = editJoinedDate.text().toStdString();
        tc.left_date = editLeftDate.text().toStdString();
        tc.share_expenses = checkShareExpenses->isChecked();
        
        if (db.createTripCompanion(tc)) {
            showInfo("Участник успешно добавлен к поездке!");
            loadTripCompanions();
        } else {
            showError("Ошибка добавления участника: " + db.getLastError());
        }
    }
}

void MainWindow::onEditTripCompanionClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedTripIdForCompanion == -1 || selectedCompanionIdForTrip == -1) {
        showError("Выберите запись для редактирования!");
        return;
    }
    
    TripCompanion tc = db.readTripCompanion(selectedTripIdForCompanion, selectedCompanionIdForTrip);
    if (tc.trip_id == -1) {
        showError("Запись не найдена!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать участника поездки");
    dialog.setFixedSize(400, 350);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblTripId("ID поездки:");
    QLabel labelTripId(QString::number(tc.trip_id));
    
    QLabel lblCompanionId("ID сопутчика:");
    QLabel labelCompanionId(QString::number(tc.companion_id));
    
    QLabel lblRole("Роль:");
    QComboBox comboRole;
    comboRole.addItems({"organizer", "companion", "guide"});
    comboRole.setCurrentText(QString::fromStdString(tc.role));
    
    QLabel lblJoinedDate("Дата присоединения (ГГГГ-ММ-ДД):");
    QLineEdit editJoinedDate;
    editJoinedDate.setText(QString::fromStdString(tc.joined_date));
    
    QLabel lblLeftDate("Дата выхода (ГГГГ-ММ-ДД, опционально):");
    QLineEdit editLeftDate;
    editLeftDate.setText(QString::fromStdString(tc.left_date));
    
    QLabel lblShareExpenses("Делит расходы:");
    QCheckBox *checkShareExpenses = new QCheckBox("Да");
    checkShareExpenses->setChecked(tc.share_expenses);
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblTripId);
    layout.addWidget(&labelTripId);
    layout.addWidget(&lblCompanionId);
    layout.addWidget(&labelCompanionId);
    layout.addWidget(&lblRole);
    layout.addWidget(&comboRole);
    layout.addWidget(&lblJoinedDate);
    layout.addWidget(&editJoinedDate);
    layout.addWidget(&lblLeftDate);
    layout.addWidget(&editLeftDate);
    layout.addWidget(&lblShareExpenses);
    layout.addWidget(checkShareExpenses);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        tc.role = comboRole.currentText().toStdString();
        tc.joined_date = editJoinedDate.text().toStdString();
        tc.left_date = editLeftDate.text().toStdString();
        tc.share_expenses = checkShareExpenses->isChecked();
        
        if (db.updateTripCompanion(tc)) {
            showInfo("Запись успешно обновлена!");
            loadTripCompanions();
        } else {
            showError("Ошибка обновления записи: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteTripCompanionClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedTripIdForCompanion == -1 || selectedCompanionIdForTrip == -1) {
        showError("Выберите запись для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить эту запись?\n"
                                  "Участник будет откреплен от поездки.",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteTripCompanion(selectedTripIdForCompanion, selectedCompanionIdForTrip)) {
            showInfo("Запись успешно удалена!");
            selectedTripIdForCompanion = -1;
            selectedCompanionIdForTrip = -1;
            loadTripCompanions();
        } else {
            showError("Ошибка удаления записи: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshTripCompanionsClicked()
{
    loadTripCompanions();
}

void MainWindow::onFilterByTripClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int tripId = QInputDialog::getInt(this, "Фильтр по поездке", 
                                     "Введите ID поездки:", 
                                     1, 1, 10000, 1, &ok);
    if (!ok) return;
    
    std::vector<TripCompanion> tripCompanions = db.getCompanionsByTrip(tripId);
    tableTripCompanions->setRowCount(tripCompanions.size());
    
    for (size_t i = 0; i < tripCompanions.size(); ++i) {
        const TripCompanion& tc = tripCompanions[i];
        
        tableTripCompanions->setItem(i, 0, new QTableWidgetItem(QString::number(tc.trip_id)));
        tableTripCompanions->setItem(i, 1, new QTableWidgetItem(QString::number(tc.companion_id)));
        tableTripCompanions->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tc.role)));
        tableTripCompanions->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(tc.joined_date)));
        tableTripCompanions->setItem(i, 4, new QTableWidgetItem(
            tc.left_date.empty() ? "" : QString::fromStdString(tc.left_date)));
        tableTripCompanions->setItem(i, 5, new QTableWidgetItem(tc.share_expenses ? "Да" : "Нет"));
        tableTripCompanions->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(tc.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(tc.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripCompanions->setItem(i, 7, new QTableWidgetItem(tripName));
        
        // Получаем имя участника
        TravelCompanion companion = db.readCompanion(tc.companion_id);
        QString companionName = (companion.id != -1) ? 
            QString::fromStdString(companion.first_name + " " + companion.last_name) : "Неизвестно";
        tableTripCompanions->setItem(i, 8, new QTableWidgetItem(companionName));
    }
    
    tableTripCompanions->resizeColumnsToContents();
}

void MainWindow::onFilterByCompanionClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int companionId = QInputDialog::getInt(this, "Фильтр по участнику", 
                                          "Введите ID участника:", 
                                          1, 1, 10000, 1, &ok);
    if (!ok) return;
    
    std::vector<TripCompanion> tripCompanions = db.getTripsByCompanion(companionId);
    tableTripCompanions->setRowCount(tripCompanions.size());
    
    for (size_t i = 0; i < tripCompanions.size(); ++i) {
        const TripCompanion& tc = tripCompanions[i];
        
        tableTripCompanions->setItem(i, 0, new QTableWidgetItem(QString::number(tc.trip_id)));
        tableTripCompanions->setItem(i, 1, new QTableWidgetItem(QString::number(tc.companion_id)));
        tableTripCompanions->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(tc.role)));
        tableTripCompanions->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(tc.joined_date)));
        tableTripCompanions->setItem(i, 4, new QTableWidgetItem(
            tc.left_date.empty() ? "" : QString::fromStdString(tc.left_date)));
        tableTripCompanions->setItem(i, 5, new QTableWidgetItem(tc.share_expenses ? "Да" : "Нет"));
        tableTripCompanions->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(tc.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(tc.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripCompanions->setItem(i, 7, new QTableWidgetItem(tripName));
        
        // Получаем имя участника
        TravelCompanion companion = db.readCompanion(tc.companion_id);
        QString companionName = (companion.id != -1) ? 
            QString::fromStdString(companion.first_name + " " + companion.last_name) : "Неизвестно";
        tableTripCompanions->setItem(i, 8, new QTableWidgetItem(companionName));
    }
    
    tableTripCompanions->resizeColumnsToContents();
}

void MainWindow::onShowAllTripCompanionsClicked()
{
    loadTripCompanions();
}

void MainWindow::onTripCompanionSelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableTripCompanions->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* tripIdItem = tableTripCompanions->item(row, 0);
        QTableWidgetItem* companionIdItem = tableTripCompanions->item(row, 1);
        
        if (tripIdItem && companionIdItem) {
            selectedTripIdForCompanion = tripIdItem->text().toInt();
            selectedCompanionIdForTrip = companionIdItem->text().toInt();
            btnEditTripCompanion->setEnabled(true);
            btnDeleteTripCompanion->setEnabled(true);
        }
    } else {
        selectedTripIdForCompanion = -1;
        selectedCompanionIdForTrip = -1;
        btnEditTripCompanion->setEnabled(false);
        btnDeleteTripCompanion->setEnabled(false);
    }
}

// === Места помещения ===

QWidget* MainWindow::createTripDestinationsTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    
    // Панель кнопок основная
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnAddTripDestination = new QPushButton("Добавить", tab);
    btnEditTripDestination = new QPushButton("Редактировать", tab);
    btnEditTripDestination->setEnabled(false);
    btnDeleteTripDestination = new QPushButton("Удалить", tab);
    btnDeleteTripDestination->setEnabled(false);
    btnRefreshTripDestinations = new QPushButton("Обновить", tab);
    
    buttonLayout->addWidget(btnAddTripDestination);
    buttonLayout->addWidget(btnEditTripDestination);
    buttonLayout->addWidget(btnDeleteTripDestination);
    buttonLayout->addWidget(btnRefreshTripDestinations);
    buttonLayout->addStretch();
    
    // Панель фильтров
    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    btnFilterByTripForDest = new QPushButton("По поездке", tab);
    btnFilterByCityForDest = new QPushButton("По городу", tab);
    btnFilterByTransport = new QPushButton("По транспорту", tab);
    btnShowAllTripDestinations = new QPushButton("Все места", tab);
    
    filterLayout->addWidget(btnFilterByTripForDest);
    filterLayout->addWidget(btnFilterByCityForDest);
    filterLayout->addWidget(btnFilterByTransport);
    filterLayout->addWidget(btnShowAllTripDestinations);
    filterLayout->addStretch();
    
    // Таблица мест посещения
    tableTripDestinations = new QTableWidget(tab);
    tableTripDestinations->setColumnCount(11);
    tableTripDestinations->setHorizontalHeaderLabels({"ID", "Поездка ID", "Город ID", 
                                                     "Порядок", "Прибытие", "Выезд", 
                                                     "Проживание", "Транспорт", "Создано",
                                                     "Поездка", "Город/Страна"});
    tableTripDestinations->horizontalHeader()->setStretchLastSection(true);
    tableTripDestinations->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    layout->addLayout(buttonLayout);
    layout->addLayout(filterLayout);
    layout->addWidget(tableTripDestinations);
    
    return tab;
}

// Метод загрузки мест посещения
void MainWindow::loadTripDestinations()
{
    if (!db.isConnected()) return;
    
    std::vector<TripDestination> destinations = db.readTripDestinations();
    tableTripDestinations->setRowCount(destinations.size());
    
    for (size_t i = 0; i < destinations.size(); ++i) {
        const TripDestination& td = destinations[i];
        
        tableTripDestinations->setItem(i, 0, new QTableWidgetItem(QString::number(td.id)));
        tableTripDestinations->setItem(i, 1, new QTableWidgetItem(QString::number(td.trip_id)));
        tableTripDestinations->setItem(i, 2, new QTableWidgetItem(QString::number(td.city_id)));
        tableTripDestinations->setItem(i, 3, new QTableWidgetItem(QString::number(td.visit_order)));
        tableTripDestinations->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(td.arrival_date)));
        tableTripDestinations->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(td.departure_date)));
        tableTripDestinations->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(td.accommodation)));
        tableTripDestinations->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(td.transport_type)));
        tableTripDestinations->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(td.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(td.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripDestinations->setItem(i, 9, new QTableWidgetItem(tripName));
        
        // Получаем город и страну
        City city = db.readCity(td.city_id);
        QString cityInfo = "Неизвестно";
        if (city.id != -1) {
            Country country = db.readCountry(city.country_id);
            cityInfo = QString::fromStdString(city.name) + 
                      (country.id != -1 ? ", " + QString::fromStdString(country.name) : "");
        }
        tableTripDestinations->setItem(i, 10, new QTableWidgetItem(cityInfo));
    }
    
    tableTripDestinations->resizeColumnsToContents();
}

// Добавить после методов для участников поездок
void MainWindow::onAddTripDestinationClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить место посещения");
    dialog.setFixedSize(500, 450);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblTripId("ID поездки:");
    QSpinBox spinTripId;
    spinTripId.setRange(1, 10000);
    spinTripId.setValue(1);
    
    QLabel lblCityId("ID города:");
    QSpinBox spinCityId;
    spinCityId.setRange(1, 10000);
    spinCityId.setValue(1);
    
    QLabel lblVisitOrder("Порядок посещения:");
    QSpinBox spinVisitOrder;
    spinVisitOrder.setRange(1, 100);
    spinVisitOrder.setValue(1);
    
    QLabel lblArrivalDate("Дата прибытия (ГГГГ-ММ-ДД):");
    QLineEdit editArrivalDate;
    editArrivalDate.setText(QDate::currentDate().toString("yyyy-MM-dd"));
    
    QLabel lblDepartureDate("Дата выезда (ГГГГ-ММ-ДД):");
    QLineEdit editDepartureDate;
    editDepartureDate.setText(QDate::currentDate().addDays(2).toString("yyyy-MM-dd"));
    
    QLabel lblAccommodation("Проживание (опционально):");
    QLineEdit editAccommodation;
    editAccommodation.setPlaceholderText("Название отеля или адрес");
    
    QLabel lblTransportType("Тип транспорта:");
    QComboBox comboTransport;
    comboTransport.addItems({"plane", "train", "bus", "car", "ship", "other"});
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Добавить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblTripId);
    layout.addWidget(&spinTripId);
    layout.addWidget(&lblCityId);
    layout.addWidget(&spinCityId);
    layout.addWidget(&lblVisitOrder);
    layout.addWidget(&spinVisitOrder);
    layout.addWidget(&lblArrivalDate);
    layout.addWidget(&editArrivalDate);
    layout.addWidget(&lblDepartureDate);
    layout.addWidget(&editDepartureDate);
    layout.addWidget(&lblAccommodation);
    layout.addWidget(&editAccommodation);
    layout.addWidget(&lblTransportType);
    layout.addWidget(&comboTransport);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Проверяем существование поездки
        Trip trip = db.readTrip(spinTripId.value());
        if (trip.id == -1) {
            showError("Поездка с указанным ID не найдена!");
            return;
        }
        
        // Проверяем существование города
        City city = db.readCity(spinCityId.value());
        if (city.id == -1) {
            showError("Город с указанным ID не найден!");
            return;
        }
        
        // Проверяем даты
        QDate arrivalDate = QDate::fromString(editArrivalDate.text(), "yyyy-MM-dd");
        QDate departureDate = QDate::fromString(editDepartureDate.text(), "yyyy-MM-dd");
        if (departureDate < arrivalDate) {
            showError("Дата выезда не может быть раньше даты прибытия!");
            return;
        }
        
        TripDestination td;
        td.trip_id = spinTripId.value();
        td.city_id = spinCityId.value();
        td.visit_order = spinVisitOrder.value();
        td.arrival_date = editArrivalDate.text().toStdString();
        td.departure_date = editDepartureDate.text().toStdString();
        td.accommodation = editAccommodation.text().toStdString();
        td.transport_type = comboTransport.currentText().toStdString();
        
        if (db.createTripDestination(td)) {
            showInfo("Место посещения успешно добавлено!");
            loadTripDestinations();
        } else {
            showError("Ошибка добавления места посещения: " + db.getLastError());
        }
    }
}

void MainWindow::onEditTripDestinationClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedTripDestinationId == -1) {
        showError("Выберите место посещения для редактирования!");
        return;
    }
    
    TripDestination td = db.readTripDestination(selectedTripDestinationId);
    if (td.id == -1) {
        showError("Место посещения не найдено!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать место посещения");
    dialog.setFixedSize(500, 450);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblTripId("ID поездки:");
    QSpinBox spinTripId;
    spinTripId.setRange(1, 10000);
    spinTripId.setValue(td.trip_id);
    
    QLabel lblCityId("ID города:");
    QSpinBox spinCityId;
    spinCityId.setRange(1, 10000);
    spinCityId.setValue(td.city_id);
    
    QLabel lblVisitOrder("Порядок посещения:");
    QSpinBox spinVisitOrder;
    spinVisitOrder.setRange(1, 100);
    spinVisitOrder.setValue(td.visit_order);
    
    QLabel lblArrivalDate("Дата прибытия (ГГГГ-ММ-ДД):");
    QLineEdit editArrivalDate;
    editArrivalDate.setText(QString::fromStdString(td.arrival_date));
    
    QLabel lblDepartureDate("Дата выезда (ГГГГ-ММ-ДД):");
    QLineEdit editDepartureDate;
    editDepartureDate.setText(QString::fromStdString(td.departure_date));
    
    QLabel lblAccommodation("Проживание (опционально):");
    QLineEdit editAccommodation;
    editAccommodation.setText(QString::fromStdString(td.accommodation));
    
    QLabel lblTransportType("Тип транспорта:");
    QComboBox comboTransport;
    comboTransport.addItems({"plane", "train", "bus", "car", "ship", "other"});
    comboTransport.setCurrentText(QString::fromStdString(td.transport_type));
    
    QHBoxLayout buttonLayout;
    QPushButton btnOk("Сохранить");
    QPushButton btnCancel("Отмена");
    buttonLayout.addWidget(&btnOk);
    buttonLayout.addWidget(&btnCancel);
    
    layout.addWidget(&lblTripId);
    layout.addWidget(&spinTripId);
    layout.addWidget(&lblCityId);
    layout.addWidget(&spinCityId);
    layout.addWidget(&lblVisitOrder);
    layout.addWidget(&spinVisitOrder);
    layout.addWidget(&lblArrivalDate);
    layout.addWidget(&editArrivalDate);
    layout.addWidget(&lblDepartureDate);
    layout.addWidget(&editDepartureDate);
    layout.addWidget(&lblAccommodation);
    layout.addWidget(&editAccommodation);
    layout.addWidget(&lblTransportType);
    layout.addWidget(&comboTransport);
    layout.addLayout(&buttonLayout);
    
    connect(&btnOk, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        // Проверяем существование поездки
        Trip trip = db.readTrip(spinTripId.value());
        if (trip.id == -1) {
            showError("Поездка с указанным ID не найдена!");
            return;
        }
        
        // Проверяем существование города
        City city = db.readCity(spinCityId.value());
        if (city.id == -1) {
            showError("Город с указанным ID не найден!");
            return;
        }
        
        // Проверяем даты
        QDate arrivalDate = QDate::fromString(editArrivalDate.text(), "yyyy-MM-dd");
        QDate departureDate = QDate::fromString(editDepartureDate.text(), "yyyy-MM-dd");
        if (departureDate < arrivalDate) {
            showError("Дата выезда не может быть раньше даты прибытия!");
            return;
        }
        
        td.trip_id = spinTripId.value();
        td.city_id = spinCityId.value();
        td.visit_order = spinVisitOrder.value();
        td.arrival_date = editArrivalDate.text().toStdString();
        td.departure_date = editDepartureDate.text().toStdString();
        td.accommodation = editAccommodation.text().toStdString();
        td.transport_type = comboTransport.currentText().toStdString();
        
        if (db.updateTripDestination(td)) {
            showInfo("Место посещения успешно обновлено!");
            loadTripDestinations();
        } else {
            showError("Ошибка обновления места посещения: " + db.getLastError());
        }
    }
}

void MainWindow::onDeleteTripDestinationClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    if (selectedTripDestinationId == -1) {
        showError("Выберите место посещения для удаления!");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", 
                                  "Вы уверены, что хотите удалить это место посещения?\n"
                                  "Все связанные расходы и фотографии также будут удалены!",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (db.deleteTripDestination(selectedTripDestinationId)) {
            showInfo("Место посещения успешно удалено!");
            selectedTripDestinationId = -1;
            loadTripDestinations();
        } else {
            showError("Ошибка удаления места посещения: " + db.getLastError());
        }
    }
}

void MainWindow::onRefreshTripDestinationsClicked()
{
    loadTripDestinations();
}

void MainWindow::onFilterByTripForDestClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int tripId = QInputDialog::getInt(this, "Фильтр по поездке", 
                                     "Введите ID поездки:", 
                                     1, 1, 10000, 1, &ok);
    if (!ok) return;
    
    std::vector<TripDestination> destinations = db.getDestinationsByTrip(tripId);
    tableTripDestinations->setRowCount(destinations.size());
    
    for (size_t i = 0; i < destinations.size(); ++i) {
        const TripDestination& td = destinations[i];
        
        tableTripDestinations->setItem(i, 0, new QTableWidgetItem(QString::number(td.id)));
        tableTripDestinations->setItem(i, 1, new QTableWidgetItem(QString::number(td.trip_id)));
        tableTripDestinations->setItem(i, 2, new QTableWidgetItem(QString::number(td.city_id)));
        tableTripDestinations->setItem(i, 3, new QTableWidgetItem(QString::number(td.visit_order)));
        tableTripDestinations->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(td.arrival_date)));
        tableTripDestinations->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(td.departure_date)));
        tableTripDestinations->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(td.accommodation)));
        tableTripDestinations->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(td.transport_type)));
        tableTripDestinations->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(td.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(td.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripDestinations->setItem(i, 9, new QTableWidgetItem(tripName));
        
        // Получаем город и страну
        City city = db.readCity(td.city_id);
        QString cityInfo = "Неизвестно";
        if (city.id != -1) {
            Country country = db.readCountry(city.country_id);
            cityInfo = QString::fromStdString(city.name) + 
                      (country.id != -1 ? ", " + QString::fromStdString(country.name) : "");
        }
        tableTripDestinations->setItem(i, 10, new QTableWidgetItem(cityInfo));
    }
    
    tableTripDestinations->resizeColumnsToContents();
}

void MainWindow::onFilterByCityForDestClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    bool ok;
    int cityId = QInputDialog::getInt(this, "Фильтр по городу", 
                                     "Введите ID города:", 
                                     1, 1, 10000, 1, &ok);
    if (!ok) return;
    
    std::vector<TripDestination> destinations = db.getDestinationsByCity(cityId);
    tableTripDestinations->setRowCount(destinations.size());
    
    for (size_t i = 0; i < destinations.size(); ++i) {
        const TripDestination& td = destinations[i];
        
        tableTripDestinations->setItem(i, 0, new QTableWidgetItem(QString::number(td.id)));
        tableTripDestinations->setItem(i, 1, new QTableWidgetItem(QString::number(td.trip_id)));
        tableTripDestinations->setItem(i, 2, new QTableWidgetItem(QString::number(td.city_id)));
        tableTripDestinations->setItem(i, 3, new QTableWidgetItem(QString::number(td.visit_order)));
        tableTripDestinations->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(td.arrival_date)));
        tableTripDestinations->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(td.departure_date)));
        tableTripDestinations->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(td.accommodation)));
        tableTripDestinations->setItem(i, 7, new QTableWidgetItem(QString::fromStdString(td.transport_type)));
        tableTripDestinations->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(td.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(td.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripDestinations->setItem(i, 9, new QTableWidgetItem(tripName));
        
        // Получаем город и страну
        City city = db.readCity(td.city_id);
        QString cityInfo = "Неизвестно";
        if (city.id != -1) {
            Country country = db.readCountry(city.country_id);
            cityInfo = QString::fromStdString(city.name) + 
                      (country.id != -1 ? ", " + QString::fromStdString(country.name) : "");
        }
        tableTripDestinations->setItem(i, 10, new QTableWidgetItem(cityInfo));
    }
    
    tableTripDestinations->resizeColumnsToContents();
}

void MainWindow::onFilterByTransportClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QStringList transports = {"plane", "train", "bus", "car", "ship", "other"};
    
    bool ok;
    QString transport = QInputDialog::getItem(this, "Фильтр по транспорту", 
                                             "Выберите тип транспорта:", 
                                             transports, 0, false, &ok);
    if (!ok) return;
    
    std::vector<TripDestination> destinations = db.getDestinationsByTransport(transport.toStdString());
    tableTripDestinations->setRowCount(destinations.size());
    
    for (size_t i = 0; i < destinations.size(); ++i) {
        const TripDestination& td = destinations[i];
        
        tableTripDestinations->setItem(i, 0, new QTableWidgetItem(QString::number(td.id)));
        tableTripDestinations->setItem(i, 1, new QTableWidgetItem(QString::number(td.trip_id)));
        tableTripDestinations->setItem(i, 2, new QTableWidgetItem(QString::number(td.city_id)));
        tableTripDestinations->setItem(i, 3, new QTableWidgetItem(QString::number(td.visit_order)));
        tableTripDestinations->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(td.arrival_date)));
        tableTripDestinations->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(td.departure_date)));
        tableTripDestinations->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(td.accommodation)));
        tableTripDestinations->setItem(i, 7, new QTableWidgetItem(transport));
        tableTripDestinations->setItem(i, 8, new QTableWidgetItem(QString::fromStdString(td.created_at)));
        
        // Получаем название поездки
        Trip trip = db.readTrip(td.trip_id);
        QString tripName = (trip.id != -1) ? QString::fromStdString(trip.name) : "Неизвестно";
        tableTripDestinations->setItem(i, 9, new QTableWidgetItem(tripName));
        
        // Получаем город и страну
        City city = db.readCity(td.city_id);
        QString cityInfo = "Неизвестно";
        if (city.id != -1) {
            Country country = db.readCountry(city.country_id);
            cityInfo = QString::fromStdString(city.name) + 
                      (country.id != -1 ? ", " + QString::fromStdString(country.name) : "");
        }
        tableTripDestinations->setItem(i, 10, new QTableWidgetItem(cityInfo));
    }
    
    tableTripDestinations->resizeColumnsToContents();
}

void MainWindow::onShowAllTripDestinationsClicked()
{
    loadTripDestinations();
}

void MainWindow::onTripDestinationSelectionChanged()
{
    QList<QTableWidgetItem*> selected = tableTripDestinations->selectedItems();
    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        QTableWidgetItem* idItem = tableTripDestinations->item(row, 0);
        if (idItem) {
            selectedTripDestinationId = idItem->text().toInt();
            btnEditTripDestination->setEnabled(true);
            btnDeleteTripDestination->setEnabled(true);
        }
    } else {
        selectedTripDestinationId = -1;
        btnEditTripDestination->setEnabled(false);
        btnDeleteTripDestination->setEnabled(false);
    }
}

// === Статистика ===
void MainWindow::loadStatistics()
{
    if (!db.isConnected()) return;
    
    int tripsCount = db.getTripsCount();
    int expensesCount = db.getExpensesCount();
    int companionsCount = db.getCompanionsCount();
    double totalSpent = db.getTotalSpent();
    int countriesCount = db.getCountriesCount();  // Добавить эту строку
    int citiesCount = db.getCitiesCount();  // Эта строка должна быть
    int photosCount = db.getPhotosCount();
    int tripDestinationsCount = db.getTripDestinationsCount();
    
    labelTripsCount->setText(QString::number(tripsCount));
    labelExpensesCount->setText(QString::number(expensesCount));
    labelCompanionsCount->setText(QString::number(companionsCount));
    labelCountriesCount->setText(QString::number(countriesCount));  // Добавить эту строку
    labelCitiesCount->setText(QString::number(citiesCount));  // Добавить
    labelPhotosCount->setText(QString::number(photosCount)); 
    labelTripDestinationsCount->setText(QString::number(tripDestinationsCount));
    
    std::ostringstream spentStream;
    spentStream << std::fixed << std::setprecision(2) << totalSpent;
    labelTotalSpent->setText(QString::fromStdString(spentStream.str() + " руб."));
}

void MainWindow::onRefreshStatsClicked()
{
    loadStatistics();
}

// === Вспомогательные методы ===
void MainWindow::showError(const QString& message)
{
    QMessageBox::critical(this, "Ошибка", message);
}

void MainWindow::showInfo(const QString& message)
{
    QMessageBox::information(this, "Информация", message);
}

QWidget* MainWindow::createSqlTab()
{
    QWidget *tab = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(tab);
    
    // Заголовок - черный
    QLabel *titleLabel = new QLabel("SQL Запросы к базе данных", tab);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: black;");
    mainLayout->addWidget(titleLabel);
    
    // Поле ввода SQL
    QLabel *inputLabel = new QLabel("Введите SQL запрос:", tab);
    inputLabel->setStyleSheet("color: black;");
    mainLayout->addWidget(inputLabel);
    
    sqlInputEdit = new QTextEdit(tab);
    sqlInputEdit->setPlaceholderText("Например: SELECT * FROM trips WHERE trip_status = 'completed'");
    sqlInputEdit->setMinimumHeight(100);
    sqlInputEdit->setMaximumHeight(150);
    sqlInputEdit->setFont(QFont("Monospace", 10));
    sqlInputEdit->setStyleSheet("color: black;");
    mainLayout->addWidget(sqlInputEdit);
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    btnExecuteSql = new QPushButton("Выполнить запрос", tab);
    btnExecuteSql->setStyleSheet("QPushButton { background-color: #27ae60; color: white; padding: 8px; font-weight: bold; }");
    
    btnClearSql = new QPushButton("Очистить", tab);
    btnClearSql->setStyleSheet("QPushButton { background-color: #7f8c8d; color: white; padding: 8px; }");
    
    buttonLayout->addWidget(btnExecuteSql);    // Исправлено: ->
    buttonLayout->addWidget(btnClearSql);      // Исправлено: ->
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Результаты запроса - черный заголовок
    QLabel *resultLabel = new QLabel("Результат запроса:", tab);
    resultLabel->setStyleSheet("color: black;");
    mainLayout->addWidget(resultLabel);
    
    sqlResultTable = new QTableWidget(tab);
    sqlResultTable->setAlternatingRowColors(true);
    sqlResultTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Устанавливаем черный цвет для заголовков столбцов и номеров строк
    sqlResultTable->setStyleSheet(
        "QTableWidget {"
        "    color: black;"
        "}"
        "QHeaderView::section {"
        "    background-color: #f0f0f0;"
        "    color: black;"
        "    padding: 5px;"
        "    border: 1px solid #d0d0d0;"
        "    font-weight: bold;"
        "}"
        "QTableCornerButton::section {"
        "    background-color: #f0f0f0;"
        "    border: 1px solid #d0d0d0;"
        "}"
        "QTableWidget::item {"
        "    color: black;"
        "    padding: 3px;"
        "}"
        "QTableWidget QTableCornerButton::section {"
        "    background-color: #f0f0f0;"
        "    color: black;"
        "    font-weight: bold;"
        "}"
    );
    
    mainLayout->addWidget(sqlResultTable, 1);
    
    // Статус запроса - черный
    QLabel *statusLabel = new QLabel("Статус запроса:", tab);
    statusLabel->setStyleSheet("color: black;");
    mainLayout->addWidget(statusLabel);
    
    sqlStatusEdit = new QTextEdit(tab);
    sqlStatusEdit->setMaximumHeight(80);
    sqlStatusEdit->setReadOnly(true);
    sqlStatusEdit->setStyleSheet(
        "QTextEdit {"
        "    background-color: #f8f9fa;"
        "    border: 1px solid #dee2e6;"
        "    color: black;"
        "    padding: 5px;"
        "}"
    );
    mainLayout->addWidget(sqlStatusEdit);
    
    // Подсказки - черный текст
    QGroupBox *tipsGroup = new QGroupBox("Подсказки", tab);
    tipsGroup->setStyleSheet(
        "QGroupBox {"
        "    color: black;"
        "    font-weight: bold;"
        "    border: 2px solid #3498db;"
        "    border-radius: 5px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px 0 5px;"
        "}"
    );
    
    QVBoxLayout *tipsLayout = new QVBoxLayout();
    
    QLabel *tip1 = new QLabel("• Используйте SELECT для запросов на чтение");
    QLabel *tip2 = new QLabel("• Используйте INSERT, UPDATE, DELETE для изменения данных");
    QLabel *tip3 = new QLabel("• Будьте осторожны с операциями DELETE и DROP");
    QLabel *tip4 = new QLabel("• Для транзакций используйте BEGIN, COMMIT, ROLLBACK");
    
    tip1->setStyleSheet("color: black;");
    tip2->setStyleSheet("color: black;");
    tip3->setStyleSheet("color: black;");
    tip4->setStyleSheet("color: black;");
    
    tipsLayout->addWidget(tip1);
    tipsLayout->addWidget(tip2);
    tipsLayout->addWidget(tip3);
    tipsLayout->addWidget(tip4);
    tipsGroup->setLayout(tipsLayout);
    
    mainLayout->addWidget(tipsGroup);
    
    // Примеры запросов - черный текст
    QGroupBox *examplesGroup = new QGroupBox("Примеры запросов", tab);
    examplesGroup->setStyleSheet(
        "QGroupBox {"
        "    color: black;"
        "    font-weight: bold;"
        "    border: 2px solid #2ecc71;"
        "    border-radius: 5px;"
        "    margin-top: 10px;"
        "    padding-top: 10px;"
        "}"
        "QGroupBox::title {"
        "    subcontrol-origin: margin;"
        "    left: 10px;"
        "    padding: 0 5px 0 5px;"
        "}"
    );
    
    QVBoxLayout *examplesLayout = new QVBoxLayout();
    
    QStringList examples = {
        "SELECT * FROM trips",
        "SELECT * FROM expenses LIMIT 10",
        "SELECT trip_name, start_date FROM trips WHERE trip_status = 'completed'",
        "SELECT country_name, COUNT(*) as city_count FROM countries JOIN cities ON countries.country_id = cities.country_id GROUP BY country_name"
    };
    
    foreach (const QString &example, examples) {
        QPushButton *exampleBtn = new QPushButton(example, tab);
        exampleBtn->setStyleSheet(
            "QPushButton {"
            "    text-align: left;"
            "    padding: 5px;"
            "    color: black;"
            "    background-color: #f8f9fa;"
            "    border: 1px solid #dee2e6;"
            "    border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e9ecef;"
            "}"
        );
        exampleBtn->setMaximumWidth(700);
        
        connect(exampleBtn, &QPushButton::clicked, [this, example]() {
            sqlInputEdit->setText(example);
        });
        
        examplesLayout->addWidget(exampleBtn);
    }
    
    examplesGroup->setLayout(examplesLayout);
    mainLayout->addWidget(examplesGroup);
    
    return tab;
}

// Можно добавить простую подсветку ключевых слов SQL
void MainWindow::highlightSqlKeywords()
{
    QString text = sqlInputEdit->toPlainText();
    
    // Простая подсветка ключевых слов
    QStringList keywords = {
        "SELECT", "FROM", "WHERE", "INSERT", "INTO", "VALUES",
        "UPDATE", "SET", "DELETE", "CREATE", "DROP", "ALTER",
        "TABLE", "JOIN", "LEFT", "RIGHT", "INNER", "OUTER",
        "GROUP BY", "ORDER BY", "HAVING", "LIMIT", "OFFSET",
        "AND", "OR", "NOT", "NULL", "IS", "IN", "BETWEEN",
        "LIKE", "DISTINCT", "COUNT", "SUM", "AVG", "MIN", "MAX"
    };
    
    // Сохраняем позицию курсора
    int cursorPos = sqlInputEdit->textCursor().position();
    
    // Применяем форматирование
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#2980b9"));
    keywordFormat.setFontWeight(QFont::Bold);
    
    QTextCharFormat defaultFormat;
    defaultFormat.setForeground(Qt::black);
    defaultFormat.setFontWeight(QFont::Normal);
    
    // Применяем форматирование ко всему тексту
    QTextCursor cursor(sqlInputEdit->document());
    cursor.select(QTextCursor::Document);
    cursor.setCharFormat(defaultFormat);
    
    // Выделяем ключевые слова
    foreach (const QString &keyword, keywords) {
        QTextCursor highlightCursor(sqlInputEdit->document());
        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = sqlInputEdit->document()->find(keyword, highlightCursor, 
                                                           QTextDocument::FindCaseSensitively);
            if (!highlightCursor.isNull()) {
                highlightCursor.mergeCharFormat(keywordFormat);
            }
        }
    }
    
    // Восстанавливаем позицию курсора
    QTextCursor restoreCursor(sqlInputEdit->document());
    restoreCursor.setPosition(cursorPos);
    sqlInputEdit->setTextCursor(restoreCursor);
}

// Добавить после других методов
void MainWindow::onExecuteSqlClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QString sqlQuery = sqlInputEdit->toPlainText().trimmed();
    
    if (sqlQuery.isEmpty()) {
        showError("Введите SQL запрос!");
        return;
    }
    
    // Очищаем предыдущие результаты
    sqlResultTable->clear();
    sqlResultTable->setRowCount(0);
    sqlResultTable->setColumnCount(0);
    
    // Обновляем статус
    sqlStatusEdit->clear();
    sqlStatusEdit->append("Выполнение запроса...");
    sqlStatusEdit->append("Запрос: " + sqlQuery.left(100) + (sqlQuery.length() > 100 ? "..." : ""));
    
    // Выполняем запрос
    QApplication::setOverrideCursor(Qt::WaitCursor);
    
    try {
        // Выполняем запрос напрямую через libpq
        PGresult* result = db.executeRawQuery(sqlQuery.toStdString());
        
        if (result) {
            int status = PQresultStatus(result);
            
            if (status == PGRES_TUPLES_OK || status == PGRES_COMMAND_OK) {
                displaySqlResult(result);
            } else {
                const char* error = PQresultErrorMessage(result);
                displaySqlError(QString("Ошибка выполнения запроса: %1").arg(error ? error : "Неизвестная ошибка"));
            }
            
            PQclear(result);
        } else {
            displaySqlError("Ошибка выполнения запроса: " + db.getLastError());
        }
        
    } catch (const std::exception& e) {
        displaySqlError("Исключение при выполнении запроса: " + QString(e.what()));
    } catch (...) {
        displaySqlError("Неизвестная ошибка при выполнении запроса");
    }
    
    QApplication::restoreOverrideCursor();
}

void MainWindow::displaySqlResult(PGresult* result)
{
    if (!result) {
        displaySqlError("Нет результата от запроса");
        return;
    }
    
    int status = PQresultStatus(result);
    
    if (status == PGRES_TUPLES_OK) {
        // Запрос вернул данные
        int rows = PQntuples(result);
        int cols = PQnfields(result);
        
        sqlStatusEdit->append(QString("✓ Запрос выполнен успешно"));
        sqlStatusEdit->append(QString("Найдено строк: %1, колонок: %2").arg(rows).arg(cols));
        
        // Настраиваем таблицу
        sqlResultTable->setColumnCount(cols);
        sqlResultTable->setRowCount(rows);
        
        // Устанавливаем заголовки
        QStringList headers;
        for (int i = 0; i < cols; ++i) {
            headers << PQfname(result, i);
        }
        sqlResultTable->setHorizontalHeaderLabels(headers);
        
        // Заполняем данными
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                const char* value = PQgetvalue(result, row, col);
                QString cellValue = value ? QString::fromUtf8(value) : "NULL";
                QTableWidgetItem *item = new QTableWidgetItem(cellValue);
                item->setForeground(Qt::black); // Черный текст в ячейках
                sqlResultTable->setItem(row, col, item);
            }
        }
        
        sqlResultTable->resizeColumnsToContents();
        
    } else if (status == PGRES_COMMAND_OK) {
        // Команда выполнена успешно (INSERT, UPDATE, DELETE и т.д.)
        const char* cmdStatus = PQcmdStatus(result);
        const char* tuples = PQcmdTuples(result);
        
        sqlStatusEdit->append("✓ Команда выполнена успешно");
        sqlStatusEdit->append(QString("Статус: %1").arg(cmdStatus ? cmdStatus : ""));
        if (tuples && strlen(tuples) > 0) {
            sqlStatusEdit->append(QString("Обработано строк: %1").arg(tuples));
        }
        
        // Показываем информацию в таблице
        sqlResultTable->setColumnCount(2);
        sqlResultTable->setRowCount(2);
        sqlResultTable->setHorizontalHeaderLabels({"Параметр", "Значение"});
        
        QTableWidgetItem *item1 = new QTableWidgetItem("Статус");
        item1->setForeground(Qt::black);
        QTableWidgetItem *item2 = new QTableWidgetItem(cmdStatus ? cmdStatus : "");
        item2->setForeground(Qt::black);
        QTableWidgetItem *item3 = new QTableWidgetItem("Строк обработано");
        item3->setForeground(Qt::black);
        QTableWidgetItem *item4 = new QTableWidgetItem(tuples ? tuples : "N/A");
        item4->setForeground(Qt::black);
        
        sqlResultTable->setItem(0, 0, item1);
        sqlResultTable->setItem(0, 1, item2);
        sqlResultTable->setItem(1, 0, item3);
        sqlResultTable->setItem(1, 1, item4);
        
        sqlResultTable->resizeColumnsToContents();
        
    } else {
        // Ошибка
        const char* error = PQresultErrorMessage(result);
        displaySqlError(QString("Ошибка выполнения запроса: %1").arg(error ? error : "Неизвестная ошибка"));
    }
}

void MainWindow::onClearSqlClicked()
{
    sqlInputEdit->clear();
    sqlResultTable->clear();
    sqlResultTable->setRowCount(0);
    sqlResultTable->setColumnCount(0);
    sqlStatusEdit->clear();
}

void MainWindow::displaySqlError(const QString& error)
{
    sqlStatusEdit->append("✗ Ошибка!");
    sqlStatusEdit->append(error);
    
    // Показываем ошибку в таблице
    sqlResultTable->setColumnCount(1);
    sqlResultTable->setRowCount(1);
    sqlResultTable->setHorizontalHeaderLabels({"Ошибка"});
    
    QTableWidgetItem *errorItem = new QTableWidgetItem(error);
    errorItem->setForeground(Qt::black);
    sqlResultTable->setItem(0, 0, errorItem);
    
    sqlResultTable->resizeColumnsToContents();
}