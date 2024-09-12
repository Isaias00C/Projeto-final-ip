#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QInputDialog>
#include <QList>
#include <QMessageBox>
#include <QDir>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QLabel *in;
    QString fileName;
    QLabel *out;
    QString outName;

public slots:
    void OpenIMG();
    void Filters();
    void CloseIMG();
    void SwapIMGs();
    void Cancel();

};
#endif // WIDGET_H
