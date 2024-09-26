#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "square.h"
#include <QPixmap>

int check;

/*float square_real(double a,double b, double h, int check){
    switch(check){
        case 0:
            return a*a;
        case 1:
            return a*b;
        case 2:
            return a*b;
        case 3:
            return 0.5*a*h;
        case 4:
            return (a+b)/2*h;
        case 5:
            return 3.14*a*a;
        case 6:
            return (3.14*a*a)/360*b;
        case 7:
            return 0.5*a*h;
    }
}*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->textEdit_3->QTextEdit::clear();
    float a=1,b=1,h=2;
    float result = 0.0;
    QString textNET = ui->textEdit->toPlainText();
    a = textNET.toDouble();
    textNET = ui->textEdit_2->toPlainText();
    b = textNET.toDouble();
    textNET = ui->textEdit_4->toPlainText();
    h = textNET.toDouble();

    switch(check){
        case 0:
            result = square::square_real(a);
            break;
        case 1:
            result = square::priam(a,b);
            break;
        case 2:
            result = square::parall(a,b);
            break;
        case 3:
            result = square::romb(a,b);
            break;
        case 4:
            result = square::trapecia(a,b,h);
            break;
        case 5:
            result = square::Krug(a);
            break;
        case 6:
            result = square::sector(a,b);
            break;
        case 7:
            result = square::treug(a,b);
            break;
    }

   // result = square_real(a,b,h,check);

    textNET = QString::number(result);

    ui->textEdit_3->insertPlainText(textNET);
}

//отображение картинок под каждую кнокпку
void MainWindow::on_pushButton_2_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/square.jpeg");
    ui->label_5->setPixmap(pix);
     check = 0;
     ui->label_2->setText("Длина");
     ui->label_3->setText("Ширина");
     ui->textEdit_2->setReadOnly(true);
     ui->textEdit_4->setReadOnly(true);
}


void MainWindow::on_pushButton_3_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/priamougol.jpeg");
    ui->label_5->setPixmap(pix);
     check = 1;
     ui->label_2->setText("Длина");
     ui->label_3->setText("Ширина");
     ui->textEdit_2->setReadOnly(false);
     ui->textEdit_4->setReadOnly(true);
}



void MainWindow::on_pushButton_4_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/parall.png");
    ui->label_5->setPixmap(pix);
     check = 2;
     ui->label_2->setText("Длина");
     ui->label_3->setText("Ширина");
     ui->textEdit_2->setReadOnly(false);
     ui->textEdit_4->setReadOnly(true);
}

void MainWindow::on_pushButton_5_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/romb.jpg");
    ui->label_5->setPixmap(pix);
     check = 3;
     ui->label_2->setText("Диагональ 1");
     ui->label_3->setText("Диагональ 2");
     ui->textEdit_2->setReadOnly(false);
     ui->textEdit_4->setReadOnly(true);
}

void MainWindow::on_pushButton_6_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/trapecia.jpg");
    ui->label_5->setPixmap(pix);
     check = 4;
     ui->label_2->setText("Длина");
     ui->label_3->setText("Ширина");
     ui->textEdit_2->setReadOnly(false);
     ui->textEdit_4->setReadOnly(false);
}

void MainWindow::on_pushButton_7_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/krug_i_sector.jpg");
    ui->label_5->setPixmap(pix);
     check = 5;
     ui->label_2->setText("Радиус");
     ui->textEdit_2->setReadOnly(true);
     ui->textEdit_4->setReadOnly(true);
     ui->label_3->setText("Ширина");
}

void MainWindow::on_pushButton_8_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/krug_i_sector.jpg");
    ui->label_5->setPixmap(pix);
     check = 6;
     ui->label_2->setText("Радиус");
     ui->label_3->setText("Сектор");
     ui->textEdit_2->setReadOnly(false);
     ui->textEdit_4->setReadOnly(true);
}

void MainWindow::on_pushButton_9_clicked()
{
    QPixmap pix("E:/_/3_kurs/1_aba/treug.jpeg");
    ui->label_5->setPixmap(pix);
     check = 7;
    ui->textEdit_4->setReadOnly(true);
    ui->label_3->setText("Ширина");
}




void MainWindow::on_action_4_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_3_triggered()
{
    ui->statusbar->showMessage("Помогите");
}

void MainWindow::on_action_2_triggered()
{
    ui->textEdit->QTextEdit::clear();
    ui->textEdit_2->QTextEdit::clear();
    ui->textEdit_3->QTextEdit::clear();
    ui->textEdit_4->QTextEdit::clear();
}
