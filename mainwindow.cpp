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
    
    // Вкладка "Статистика"
    QWidget *statsTab = createStatisticsTab();
    tabWidget->addTab(statsTab, "Статистика");
    
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
    
    // Статистика
    connect(btnRefreshStats, &QPushButton::clicked, this, &MainWindow::onRefreshStatsClicked);
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
        //tableCompanions->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(companion.relationship)));
    }
    
    tableCompanions->resizeColumnsToContents();
}

void MainWindow::onAddCompanionClicked()
{
    if (!db.isConnected()) {
        showError("Сначала подключитесь к базе данных!");
        return;
    }
    
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить сопутешественника");
    dialog.setFixedSize(400, 300);
    
    QVBoxLayout layout(&dialog);
    
    QLabel lblFirstName("Имя:");
    QLineEdit editFirstName;
    
    QLabel lblLastName("Фамилия:");
    QLineEdit editLastName;
    
    QLabel lblEmail("Email:");
    QLineEdit editEmail;
    
    QLabel lblPhone("Телефон:");
    QLineEdit editPhone;
    
    QLabel lblRelationship("Отношения:");
    QComboBox comboRelationship;
    comboRelationship.addItems({"friend", "family", "colleague", "partner", "other"});
    
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
    layout.addWidget(&lblRelationship);
    layout.addWidget(&comboRelationship);
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
        //companion.relationship = comboRelationship.currentText().toStdString();
        
        if (db.createCompanion(companion)) {
            showInfo("Сопутешественник успешно добавлен!");
            loadCompanions();
        } else {
            showError("Ошибка добавления сопутешественника: " + db.getLastError());
        }
    }
}

void MainWindow::onEditCompanionClicked()
{
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
    dialog.setFixedSize(400, 300);
    
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
    
    QLabel lblRelationship("Отношения:");
    QComboBox comboRelationship;
    comboRelationship.addItems({"friend", "family", "colleague", "partner", "other"});
    //comboRelationship.setCurrentText(QString::fromStdString(companion.relationship));
    
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
    layout.addWidget(&lblRelationship);
    layout.addWidget(&comboRelationship);
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
        //companion.relationship = comboRelationship.currentText().toStdString();
        
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

// === Статистика ===
void MainWindow::loadStatistics()
{
    if (!db.isConnected()) return;
    
    int tripsCount = db.getTripsCount();
    int expensesCount = db.getExpensesCount();
    int companionsCount = db.getCompanionsCount();
    double totalSpent = db.getTotalSpent();
    
    labelTripsCount->setText(QString::number(tripsCount));
    labelExpensesCount->setText(QString::number(expensesCount));
    labelCompanionsCount->setText(QString::number(companionsCount));
    
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