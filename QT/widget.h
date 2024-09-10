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

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QLabel *label;
    QLabel *in;
    QLabel *out;

public slots:
    void OpenIMG();
    void Filters();
    void CloseIMG();

};
#endif // WIDGET_H
