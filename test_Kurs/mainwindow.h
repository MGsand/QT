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

private:
    void setupUI();

    Ui::MainWindow *ui;
    DragDropListWidget *imageList;
    QLabel *imageLabel;
    QTextEdit *commentText;
    QPushButton *addButton;
    QPushButton *saveButton;
    QPushButton *loadAlbumButton;
    QPushButton *removeButton;

    struct ImageData {
        QString path;
        QString comment;
    };

    QVector<ImageData> albumData;
};

#endif // MAINWINDOW_H
