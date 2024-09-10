#include "widget.h"
#include "photoshop.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Photoshop do Lucas");
    resize(854, 480);
    showMaximized();

//MENU
    QMenu *options = new QMenu(this);

    QAction *open = new QAction("Abrir arquivo", this);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(OpenIMG()));
    options->addAction(open);

    QAction *filters = new QAction("Filtros", this);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(Filters()));
    options->addAction(filters);

    QAction *close = new QAction("Fechar arquivo", this);
    connect(open, SIGNAL(triggered(bool)), this, SLOT(CloseIMG()));
    options->addAction(close);

    QAction *act = new QAction(this);
    act->setText("Opções");
    act->setMenu(options);

    /*QMenu *options2 = new QMenu(this);
    options->addAction("Mediana");
    options->addAction("Gauss");
    options->addAction("Inverter");
    options->addAction("Rotacionar");
    options->addAction("Equalizar");*/

    QMenuBar *menu = new QMenuBar(this);
    menu->addAction(act);

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


    label = new QLabel(this);
    label->setText("Teste");
    label->setFont(QFont("Times New Roman", 15));




    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(menu);
    vbox->addLayout(grid);
    vbox->addWidget(label);

}

Widget::~Widget() {}

void Widget::OpenIMG()
{
    label->setText("Deu bom");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "C:/", tr("Image Files (*.pgm)"));
    if (!fileName.isEmpty()){
        in->setPixmap(QPixmap(fileName));
        //photoshop(fileName.toStdString().c_str());
    }
}

void Widget::Filters()
{

}

void Widget::CloseIMG()
{

}




