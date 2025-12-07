#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QStatusBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QInputDialog>  // <-- Добавить эту строку
#include <QSpinBox>      // <-- Добавить эту строку
#include "database.h"

QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectClicked();
    void onDisconnectClicked();
    
    // Поездки
    void onAddTripClicked();
    void onEditTripClicked();
    void onDeleteTripClicked();
    void onRefreshTripsClicked();
    void onTripSelectionChanged();
    
    // Расходы
    void onAddExpenseClicked();
    void onEditExpenseClicked();
    void onDeleteExpenseClicked();
    void onRefreshExpensesClicked();
    void onExpenseSelectionChanged();
    
    // Сопутешественники
    void onAddCompanionClicked();
    void onEditCompanionClicked();
    void onDeleteCompanionClicked();
    void onRefreshCompanionsClicked();
    void onCompanionSelectionChanged();
    
    // Статистика
    void onRefreshStatsClicked();

private:
    // Ui::MainWindow *ui; // Убрали, так как UI создаем программно
    Database& db;
    
    // Указатели на виджеты
    QTabWidget *tabWidget;
    
    // Поездки
    QTableWidget *tableTrips;
    QPushButton *btnAddTrip;
    QPushButton *btnEditTrip;
    QPushButton *btnDeleteTrip;
    QPushButton *btnRefreshTrips;
    
    // Расходы
    QTableWidget *tableExpenses;
    QPushButton *btnAddExpense;
    QPushButton *btnEditExpense;
    QPushButton *btnDeleteExpense;
    QPushButton *btnRefreshExpenses;
    
    // Сопутешественники
    QTableWidget *tableCompanions;
    QPushButton *btnAddCompanion;
    QPushButton *btnEditCompanion;
    QPushButton *btnDeleteCompanion;
    QPushButton *btnRefreshCompanions;
    
    // Статистика
    QPushButton *btnRefreshStats;
    QLabel *labelTripsCount;
    QLabel *labelExpensesCount;
    QLabel *labelCompanionsCount;
    QLabel *labelTotalSpent;
    
    // Подключение
    QPushButton *btnConnect;
    QPushButton *btnDisconnect;
    
    void setupUI();
    QWidget* createTripsTab();
    QWidget* createExpensesTab();
    QWidget* createCompanionsTab();
    QWidget* createStatisticsTab();
    void setupConnections();
    void updateStatusBar();
    
    // Методы загрузки данных
    void loadTrips();
    void loadExpenses();
    void loadCompanions();
    void loadStatistics();
    
    // Вспомогательные методы
    void showError(const QString& message);
    void showInfo(const QString& message);
    
    // Текущие выбранные ID
    int selectedTripId = -1;
    int selectedExpenseId = -1;
    int selectedCompanionId = -1;
};
#endif // MAINWINDOW_H