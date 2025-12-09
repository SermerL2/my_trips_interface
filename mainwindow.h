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
#include <QInputDialog>
#include <QSpinBox>    
#include <QCheckBox>
#include "database.h"
#include <QApplication>
#include <QTextCursor>
#include <QTextCharFormat>

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

    // Страны
    QWidget* createCountriesTab();
    void loadCountries();
    void onAddCountryClicked();
    void onEditCountryClicked();
    void onDeleteCountryClicked();
    void onRefreshCountriesClicked();
    void onCountrySelectionChanged();
    void onFilterByContinentClicked();
    void onFilterVisaClicked();
    void onFilterNoVisaClicked();
    void onShowAllCountriesClicked();
    
    // Города
    QWidget* createCitiesTab();
    void loadCities();
    void onAddCityClicked();
    void onEditCityClicked();
    void onDeleteCityClicked();
    void onRefreshCitiesClicked();
    void onCitySelectionChanged();
    void onFilterByCountryClicked();
    void onFilterWithAirportClicked();
    void onShowAllCitiesClicked();

    // Фото
    QWidget* createPhotosTab();
    void loadPhotos();
    void onAddPhotoClicked();
    void onEditPhotoClicked();
    void onDeletePhotoClicked();
    void onRefreshPhotosClicked();
    void onPhotoSelectionChanged();
    void onFilterByDestinationClicked();
    void onFilterByRatingClicked();
    void onShowAllPhotosClicked();
    
    // связь trip-companion
    QWidget* createTripCompanionsTab();
    void loadTripCompanions();
    void onAddTripCompanionClicked();
    void onEditTripCompanionClicked();
    void onDeleteTripCompanionClicked();
    void onRefreshTripCompanionsClicked();
    void onTripCompanionSelectionChanged();
    void onFilterByTripClicked();
    void onFilterByCompanionClicked();
    void onShowAllTripCompanionsClicked();

    // Путешествия
    QWidget* createTripDestinationsTab();
    void loadTripDestinations();
    void onAddTripDestinationClicked();
    void onEditTripDestinationClicked();
    void onDeleteTripDestinationClicked();
    void onRefreshTripDestinationsClicked();
    void onTripDestinationSelectionChanged();
    void onFilterByTripForDestClicked();
    void onFilterByCityForDestClicked();
    void onFilterByTransportClicked();
    void onShowAllTripDestinationsClicked();

    // Статистика
    void onRefreshStatsClicked();

    QWidget* createSqlTab();
    void onExecuteSqlClicked();
    void onClearSqlClicked();
    void displaySqlResult(PGresult* result);
    void displaySqlError(const QString& error);
    void highlightSqlKeywords();

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

    // Страны
    QTableWidget *tableCountries;
    QPushButton *btnAddCountry;
    QPushButton *btnEditCountry;
    QPushButton *btnDeleteCountry;
    QPushButton *btnRefreshCountries;
    QPushButton *btnFilterByContinent;
    QPushButton *btnFilterVisa;
    QPushButton *btnFilterNoVisa;
    QPushButton *btnShowAllCountries;

    // Города
    QTableWidget *tableCities;
    QPushButton *btnAddCity;
    QPushButton *btnEditCity;
    QPushButton *btnDeleteCity;
    QPushButton *btnRefreshCities;
    QPushButton *btnFilterByCountry;
    QPushButton *btnFilterWithAirport;
    QPushButton *btnShowAllCities;

    // Фотографии
    QTableWidget *tablePhotos;
    QPushButton *btnAddPhoto;
    QPushButton *btnEditPhoto;
    QPushButton *btnDeletePhoto;
    QPushButton *btnRefreshPhotos;
    QPushButton *btnFilterByDestination;
    QPushButton *btnFilterByRating;
    QPushButton *btnShowAllPhotos;

    // Участники поездок
    QTableWidget *tableTripCompanions;
    QPushButton *btnAddTripCompanion;
    QPushButton *btnEditTripCompanion;
    QPushButton *btnDeleteTripCompanion;
    QPushButton *btnRefreshTripCompanions;
    QPushButton *btnFilterByTrip;
    QPushButton *btnFilterByCompanion;
    QPushButton *btnShowAllTripCompanions;

    // Места посещения
    QTableWidget *tableTripDestinations;
    QPushButton *btnAddTripDestination;
    QPushButton *btnEditTripDestination;
    QPushButton *btnDeleteTripDestination;
    QPushButton *btnRefreshTripDestinations;
    QPushButton *btnFilterByTripForDest;
    QPushButton *btnFilterByCityForDest;
    QPushButton *btnFilterByTransport;
    QPushButton *btnShowAllTripDestinations;
    
    // Статистика
    QPushButton *btnRefreshStats;
    QLabel *labelTripsCount;
    QLabel *labelExpensesCount;
    QLabel *labelCompanionsCount;
    QLabel *labelTotalSpent;
    QLabel *labelCountriesCount;
    QLabel *labelCitiesCount;
    QLabel *labelPhotosCount;
    QLabel *labelTripDestinationsCount;

    // SQL Запросы
    QTextEdit *sqlInputEdit;
    QPushButton *btnExecuteSql;
    QPushButton *btnClearSql;
    QTableWidget *sqlResultTable;
    QTextEdit *sqlStatusEdit;

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
    int selectedCountryId = -1;
    int selectedCityId = -1;
    int selectedPhotoId = -1;
    int selectedTripIdForCompanion = -1;
    int selectedCompanionIdForTrip = -1;
    int selectedTripDestinationId = -1;
};

#endif // MAINWINDOW_H