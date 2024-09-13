#include "widget.h"
#include "photoshop.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Photoshop do Lucas");
    resize(854, 480);
    showMaximized();

//MENU

    QMenu *file = new QMenu(this);
    file->setTitle("Arquivo");

    QAction *open = new QAction("Abrir arquivo", this);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(OpenIMG()));

    QAction *filters = new QAction("Filtros", this);
    connect(filters, SIGNAL(triggered(bool)), this, SLOT(Filters()));

    QAction *close = new QAction("Fechar arquivo", this);
    connect(close, SIGNAL(triggered(bool)), this, SLOT(CloseIMG()));

    QMenuBar *menu = new QMenuBar(this);
    file->addAction(open);
    file->addAction(close);
    menu->addMenu(file);
    menu->addAction(filters);

//IMAGES
    QLabel *inLabel = new QLabel("Antes", this);
    inLabel->setFont(QFont("Segoe UI", 20));
    in = new QLabel();
    in->setGeometry(0, 0, 512, 240);

    QLabel *outLabel = new QLabel("Depois", this);
    outLabel->setFont(QFont("Segoe UI", 20));
    out = new QLabel();
    out->setGeometry(0, 0, 512, 240);

    QPushButton *okButton = new QPushButton("Ok", this);
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(SwapIMGs()));

    QPushButton *cancelButton = new QPushButton("Cancelar", this);
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(Cancel()));

    QHBoxLayout *titles = new QHBoxLayout();
    titles->addWidget(inLabel, 0, Qt::AlignHCenter);
    titles->addWidget(outLabel, 0, Qt::AlignHCenter);

    QHBoxLayout *images = new QHBoxLayout();
    images->addWidget(in, 0, Qt::AlignHCenter);
    images->addWidget(out, 0, Qt::AlignHCenter);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addWidget(okButton, 0, Qt::AlignRight);
    buttons->addWidget(cancelButton, 0, Qt::AlignRight);

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(menu);
    vbox->addLayout(titles, 1);
    vbox->addLayout(images, 10);
    vbox->addLayout(buttons, 1);

}

Widget::~Widget() {}

void Widget::OpenIMG()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Abrir imagem"), "C:/", tr("Image Files (*.pgm)"));
    if (!fileName.isEmpty()) {
        in->setPixmap(QPixmap(fileName));
        out->setPixmap(QPixmap());
        outName = "";
    }
}

void Widget::Filters()
{
    if (!fileName.isEmpty()){
        QList<QString> filters = {"Selecione o filtro", "Mediana", "Gauss", "Inverter", "Rotacionar", "Equalizar"};
        QString filter = QInputDialog::getItem(this, "Seletor de filtros", "Filtro:", filters, 0, false);
        outName = fileName;
        outName.chop(4);
        int filterEnum = -1;
        if (filter == "Mediana") {
            outName.append("_median.pgm");
            filterEnum = 1;
        }
        else if (filter == "Gauss") {
            outName.append("_gauss.pgm");
            filterEnum = 2;
        }
        else if (filter == "Inverter") {
            outName.append("_inverted.pgm");
            filterEnum = 3;
        }
        else if (filter == "Rotacionar") {
            outName.append("_rotated.pgm");
            filterEnum = 4;
        }
        else if (filter == "Equalizar") {
            outName.append("_equalized.pgm");
            filterEnum = 5;
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
    in->setPixmap(QPixmap());
    fileName = "";
    out->setPixmap(QPixmap());
    outName = "";

}

void Widget::SwapIMGs()
{
    if (!outName.isEmpty()){
        fileName = outName;
        outName = "";
        in->setPixmap(QPixmap(fileName));
        out->setPixmap(QPixmap());
    }
}

void Widget::Cancel()
{
    if (!outName.isEmpty()){
        out->setPixmap(QPixmap());
        QDir *aux = new QDir();
        aux->remove(outName);
        outName = "";
    }
}




