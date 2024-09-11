#include "widget.h"
#include "photoshop.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Photoshop do Lucas");
    resize(854, 480);
    showMaximized();

//MENU
    //QMenu *options = new QMenu(this);

    QAction *open = new QAction("Abrir arquivo", this);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(OpenIMG()));

    QAction *filters = new QAction("Filtros", this);
    connect(filters, SIGNAL(triggered(bool)), this, SLOT(Filters()));

    QAction *close = new QAction("Fechar arquivo", this);
    connect(close, SIGNAL(triggered(bool)), this, SLOT(CloseIMG()));

    QMenuBar *menu = new QMenuBar(this);
    menu->addAction(open);
    menu->addAction(filters);
    menu->addAction(close);

//IMAGES
    QLabel *inLabel = new QLabel("Antes", this);
    inLabel->setFont(QFont("Times New Roman", 15));
    in = new QLabel();
    QLabel *outLabel = new QLabel("Depois", this);
    outLabel->setFont(QFont("Times New Roman", 15));
    out = new QLabel();

    QGridLayout *grid = new QGridLayout();
    grid->addWidget(inLabel, 0, 0);
    grid->addWidget(outLabel, 0, 1);
    grid->addWidget(in, 1, 0);
    grid->addWidget(out, 1, 1);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(menu);
    vbox->addLayout(grid);

}

Widget::~Widget() {}

void Widget::OpenIMG()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Abrir imagem"), "C:/", tr("Image Files (*.pgm)"));
    if (!fileName.isEmpty()) in->setPixmap(QPixmap(fileName));
}

void Widget::Filters()
{
    if (!fileName.isEmpty()){
        QList<QString> filters = {"Selecione o filtro", "Mediana", "Gauss 3x3", "Gauss 5x5", "Gauss 7x7", "Inverter", "Rotacionar", "Equalizar"};
        QString filter = QInputDialog::getItem(this, "Seletor de filtros", "Filtro:", filters, 0, false);
        QString outName = fileName;
        outName.chop(4);
        int filterEnum = -1;
        if (filter == "Mediana") {
            outName.append("_median.pgm");
            filterEnum = 1;
        }
        else if (filter == "Gauss 3x3") {
            outName.append("_gauss3.pgm");
            filterEnum = 2;
        }
        else if (filter == "Gauss 5x5") {
            outName.append("_gauss5.pgm");
            filterEnum = 3;
        }
        else if (filter == "Gauss 7x7") {
            outName.append("_gauss7.pgm");
            filterEnum = 4;
        }
        else if (filter == "Inverter") {
            outName.append("_inverted.pgm");
            filterEnum = 7;
        }
        else if (filter == "Rotacionar") {
            outName.append("_rotated.pgm");
            filterEnum = 8;
        }
        else if (filter == "Equalizar") {
            outName.append("_equalized.pgm");
            filterEnum = 9;
        }

        if (filterEnum != -1) {
            photoshop(fileName.toStdString().c_str(), filterEnum);
            out->setPixmap(QPixmap(outName));
        }
    }
    else{
        QMessageBox error;
        error.setText("ERRO: Selecione uma imagem");
        error.exec();
    }
}

void Widget::CloseIMG()
{
    in = NULL;
    fileName = "";
    out = NULL;
    outName = "";

}




