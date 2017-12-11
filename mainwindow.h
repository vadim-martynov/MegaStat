#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTimer>
#include <QDateTime>

#include "cidentry.h"
#include "xmllibrary.h"

#define TITLE_MAIN  "MegaStat"


namespace Ui {
class MainWindow;
}

enum EIntClass
{
    C_PHOHE = 0,
    C_RADIO,
    C_RECORD,
    C_TERMWS,
    C_NONE = 10,
};

enum EHostClass
{
    C_HOST_D = 0,
    C_HOST_E,
    C_HOST_I,
    C_HOST_W,
    C_HOST_M2,
};

struct CIntClass
{
    QMap<quint16, CIdEntry> idMap;
    QString name;
};

struct CTypeClass
{
    quint16 type;
    quint8 intClass;
    quint16 count;
    QString name;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void menu_open_click();
    void menu_save_click();
    void starter_tick();
    void status_change(QString text);

private slots:
    void on_chAll_toggled(bool checked);


private:
    Ui::MainWindow *ui;

    QTimer *starter;
    QString confPath;
    QList<CIntClass> intClass;
    QMap<quint16, CTypeClass> intTypes;
    QList<quint16> hostClass;

    void RunSet();
    void ClearScreen();
    void InitTypeClasses();
    void InitIntClasses();
    void FindAbonents();
    void TypeCalc();
    void ShowInterfaces();
    QString GetRevision();
    QString GetSection(const QString &name, quint8 i);


};

#endif // MAINWINDOW_H
