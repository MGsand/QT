#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DragDropListWidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonParseError>
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Фотоальбом");
    setGeometry(100, 100, 800, 600);

    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::setupUI()
{
    imageList = new DragDropListWidget(this);
    imageList->setMaximumWidth(150);
    imageList->setDragDropMode(QAbstractItemView::InternalMove);
    imageList->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(imageList, &QListWidget::itemClicked, this, &MainWindow::showSelectedImage);
    connect(imageList, SIGNAL(rowsDropped()), this, SLOT(updateAlbumData())); //обновление данных после перетаскивания

    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);

    commentText = new QTextEdit(this);

    addButton = new QPushButton("Добавить", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::loadImage);

    saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveAlbum);

    loadAlbumButton = new QPushButton("Загрузить", this);
    connect(loadAlbumButton, &QPushButton::clicked, this, &MainWindow::loadAlbum);

    removeButton = new QPushButton("Удалить", this); // Создаем кнопку удаления
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeImage);


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadAlbumButton);
    buttonLayout->addWidget(removeButton);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(imageList);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(imageLabel);
    rightLayout->addWidget(commentText);
    rightLayout->addLayout(buttonLayout);

    mainLayout->addLayout(rightLayout);


    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::loadImage()
{
    QFileDialog fileDialog(this);
    QString imagePath = fileDialog.getOpenFileName(this, "Выберите изображение", "", "Изображения (*.png *.jpg *.jpeg)");
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            // Add to list
            QListWidgetItem *item = new QListWidgetItem(imagePath.split('/').last());
            imageList->addItem(item);
            imageList->setCurrentItem(item);
            // Add to album data
            ImageData imageData;
            imageData.path = imagePath;
            imageData.comment = "";
            albumData.push_back(imageData);
            showSelectedImage();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось загрузить изображение");
        }
    }
}

void MainWindow::showSelectedImage()
{
    int selectedIndex = imageList->currentRow();
    if (selectedIndex >= 0) {
        ImageData imageData = albumData[selectedIndex];
        QPixmap pixmap(imageData.path);
        imageLabel->setPixmap(pixmap);
        commentText->setText(imageData.comment);
    }
}

void MainWindow::saveAlbum()
{
    int selectedIndex = imageList->currentRow();
    if (selectedIndex >= 0) {
        albumData[selectedIndex].comment = commentText->toPlainText();
    }

    QFileDialog fileDialog(this);
    QString savePath = fileDialog.getSaveFileName(this, "Сохранить альбом", "", "JSON (*.json)");
    if (!savePath.isEmpty()) {
        QJsonArray jsonArray;
        for (const auto& imageData : albumData) {
            QJsonObject jsonObject;
            jsonObject["path"] = imageData.path;
            jsonObject["comment"] = imageData.comment;
            jsonArray.append(jsonObject);
        }
        QJsonDocument jsonDoc(jsonArray);

        QFile saveFile(savePath);
        if (saveFile.open(QIODevice::WriteOnly)) {
           saveFile.write(jsonDoc.toJson());
           saveFile.close();
           QMessageBox::information(this, "Сохранено", "Альбом успешно сохранен.");
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить альбом.");
        }
    }
}

void MainWindow::loadAlbum()
{
    QFileDialog fileDialog(this);
    QString loadPath = fileDialog.getOpenFileName(this, "Загрузить альбом", "", "JSON (*.json)");
    if (!loadPath.isEmpty()) {
        QFile loadFile(loadPath);
        if (loadFile.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = loadFile.readAll();
            loadFile.close();

            QJsonParseError error;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
            if (error.error == QJsonParseError::NoError && jsonDoc.isArray()) {
                albumData.clear();
                imageList->clear();

                QJsonArray jsonArray = jsonDoc.array();
                for (const auto& value : jsonArray) {
                    QJsonObject jsonObject = value.toObject();
                    ImageData imageData;
                    imageData.path = jsonObject["path"].toString();
                    imageData.comment = jsonObject["comment"].toString();
                    albumData.push_back(imageData);
                    imageList->addItem(imageData.path.split('/').last());
                }
                if(!albumData.isEmpty())
                {
                    imageList->setCurrentRow(0);
                    showSelectedImage();
                }

                 QMessageBox::information(this, "Загружено", "Альбом успешно загружен.");
            } else
            {
                QMessageBox::critical(this, "Ошибка", "Ошибка при загрузке альбома.");
            }

        } else {
              QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл альбома.");
        }

    }

}


void MainWindow::removeImage()
{
   int selectedIndex = imageList->currentRow();
    if(selectedIndex >= 0)
    {
        imageList->takeItem(selectedIndex); // Удаляем элемент из списка
        albumData.remove(selectedIndex); // Удаляем элемент из данных

        if(!albumData.isEmpty())
        {
            // Обновляем отображение (если остались элементы)
            if (selectedIndex < albumData.size()) {
                imageList->setCurrentRow(selectedIndex);
            } else {
               imageList->setCurrentRow(albumData.size() - 1);
            }

            showSelectedImage();
        } else
        {
             // Очищаем отображение (если элементов не осталось)
             imageLabel->clear();
             commentText->clear();

        }
    }
}

void MainWindow::updateAlbumData()
{
   int selectedIndex = imageList->currentRow();
   QVector<ImageData> newAlbumData;
   for (int i = 0; i < imageList->count(); ++i) {
         QListWidgetItem* item = imageList->item(i);
          for (const auto& imageData : albumData) {
              if(item->text() == imageData.path.split('/').last())
              {
                   newAlbumData.push_back(imageData);
                   break;
              }

         }
     }
    albumData = newAlbumData;
     if(selectedIndex >= 0)
         imageList->setCurrentRow(selectedIndex);
}
