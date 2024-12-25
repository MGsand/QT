#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DragDropListWidget.h"
#include <QMainWindow>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVector>
#include <QMap>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QHBoxLayout>
#include <QDateTime>
#include <QLineEdit>
#include <QDebug>
#include <QRegExp>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QStandardPaths>
#include <QInputDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void showSelectedImage();
    void saveAlbum();
    void loadAlbum();
    void removeImage();
    void updateAlbumData();
    void searchImages();
    void applyStyles();
    void initDatabase();

private:
    void setupUI();

    Ui::MainWindow *ui;
    DragDropListWidget *imageList;
    QLabel *imageLabel;
    QLabel *dateLabel;
    QTextEdit *commentText;
    QPushButton *addButton;
    QPushButton *saveButton;
    QPushButton *loadAlbumButton;
    QPushButton *removeButton;
    QPushButton *searchButton;
    QLineEdit *searchLineEdit;
    QSqlDatabase db;
    struct ImageData {
        QString path;
        QString comment;
        QDateTime saveDate;
    };

    QVector<ImageData> albumData;
};

#endif // MAINWINDOW_H
