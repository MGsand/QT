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
    applyStyles();
    setupUI();
    initDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::initDatabase()
{
    //Определение пути к базе данных
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dbPath.isEmpty()) {
        qDebug() << "Error: Cannot determine writable location.";
        QMessageBox::critical(this, "Error", "Cannot determine writable location.");
         return;
    }
    QDir dir;// создаём объект QDir для работы с каталогами
    if (!dir.exists(dbPath)) {
          if(!dir.mkpath(dbPath))
          {
             qDebug() << "Error: Failed to create database directory";
              QMessageBox::critical(this, "Error", "Failed to create database directory.");
              return;
          }
    }
      dbPath += "/photoalbum.db"; // добавляем имя файла базы данных к пути

    //Создание и настройка объекта QSqlDatabase
    db = QSqlDatabase::addDatabase("QSQLITE"); //драйвер SQLITE
    db.setDatabaseName(dbPath);//Устанавливаем имя файла базы данных

    //Открытие базы данных
    if (!db.open()) {
        qDebug() << "Error: Cannot open database: " << db.lastError().text();
        QMessageBox::critical(this, "Error", "Cannot open database: " + db.lastError().text());
       return;
    }

    // Создание таблицы
     QSqlQuery query(db);
     if (!query.exec("CREATE TABLE IF NOT EXISTS albums (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, photo_count INTEGER, last_saved TEXT)")) {
            qDebug() << "Error: Cannot create table: " << query.lastError().text();
             QMessageBox::critical(this, "Error", "Cannot create table: " + query.lastError().text());
            return;
     }

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

    dateLabel = new QLabel(this);

    addButton = new QPushButton("Добавить", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::loadImage);

    saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveAlbum);

    loadAlbumButton = new QPushButton("Загрузить", this);
    connect(loadAlbumButton, &QPushButton::clicked, this, &MainWindow::loadAlbum);

    removeButton = new QPushButton("Удалить", this); // Создаем кнопку удаления
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeImage);

    searchLineEdit = new QLineEdit(this);
    searchButton = new QPushButton("Найти", this);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchImages);
    searchButton->setToolTip(tr("Поиск по дате или комментарию"));


    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadAlbumButton);
    buttonLayout->addWidget(removeButton);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(imageList);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(imageLabel);
    rightLayout->addWidget(commentText);
    rightLayout->addWidget(dateLabel);
    rightLayout->addLayout(buttonLayout);
    rightLayout->addLayout(searchLayout);

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
            imageData.saveDate = QDateTime::currentDateTime();
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
        dateLabel->setText(imageData.saveDate.toString());
    }
    else {
            dateLabel->setText("");
        }
}

void MainWindow::saveAlbum()
{
    int selectedIndex = imageList->currentRow();
      QString albumName = QInputDialog::getText(this, "Имя альбома", "Введите имя альбома:", QLineEdit::Normal, "Мой альбом");
    if (selectedIndex >= 0)
    {
        albumData[selectedIndex].comment = commentText->toPlainText();
        albumData[selectedIndex].saveDate = QDateTime::currentDateTime();
         dateLabel->setText(albumData[selectedIndex].saveDate.toString());
    }

    QFileDialog fileDialog(this);
    QString savePath = fileDialog.getSaveFileName(this, "Сохранить альбом", "", "JSON (*.json)");
    if (!savePath.isEmpty()) {
        QJsonArray jsonArray;
        for (const auto& imageData : albumData) {
            QJsonObject jsonObject;
            jsonObject["path"] = imageData.path;
            jsonObject["comment"] = imageData.comment;
             jsonObject["saveDate"] = imageData.saveDate.toString(Qt::ISODate); // Сохраняем дату в формате ISO
            jsonArray.append(jsonObject);
        }
        QJsonDocument jsonDoc(jsonArray);

        QFile saveFile(savePath);
        if (saveFile.open(QIODevice::WriteOnly)) {
           saveFile.write(jsonDoc.toJson());
           saveFile.close();
            QSqlQuery query(db);
            query.prepare("INSERT INTO albums (name, photo_count, last_saved) VALUES (:name, :photo_count, :last_saved)");
            query.bindValue(":name", albumName);
            query.bindValue(":photo_count", albumData.count());
            query.bindValue(":last_saved", QDateTime::currentDateTime().toString(Qt::ISODate));
           if (!query.exec()) {
                 qDebug() << "Error: Cannot insert into table: " << query.lastError().text();
                QMessageBox::critical(this, "Ошибка", "Ошибка при сохранении данных в БД: " + query.lastError().text());
            } else
            {
               QMessageBox::information(this, "Сохранено", "Альбом успешно сохранен, и данные занесены в БД.");
            }
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
                    imageData.saveDate = QDateTime::fromString(jsonObject["saveDate"].toString(), Qt::ISODate); // Восстанавливаем дату из строки
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
             dateLabel->clear();
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


void MainWindow::searchImages() {
    QString searchText = searchLineEdit->text();
    QString filter = searchText.toLower();

    QVector<ImageData> filteredImages;
    for (const auto& imageData : albumData) {
        if (imageData.comment.toLower().contains(filter) ||
            imageData.saveDate.toString().toLower().contains(filter)) {
            filteredImages.append(imageData);
        }
    }

    imageList->clear(); // без этого вылетает
    albumData = filteredImages; // обновляет дату
    for (const auto& imageData : albumData) {
        imageList->addItem(new QListWidgetItem(imageData.path.split('/').last()));
    }

    if(albumData.isEmpty()) {
        QMessageBox::information(this, "Поиск", "Ничего не найдено.");
    }

    // показывает 1 картинку или очищает при фейле
     if (!filteredImages.isEmpty()){
        imageList->setCurrentRow(0);
        showSelectedImage();
    } else {
        imageLabel->clear();
        commentText->clear();
        dateLabel->clear();
    }
}

void MainWindow::applyStyles() {

    QString styleSheet =  "QPushButton {"
                       "   background-color: #3498db; /* Темно-голубой */"
                       "   color: white;"
                       "   border: none;"
                       "   padding: 10px 20px;"
                       "   border-radius: 5px;"
                       "   font-size: 14px;"
                       "}"
                      "QPushButton:hover {"
                       " background-color: #2980b9; /* Темно-синий при наведении */"
                       "}"
                       "QPushButton:pressed {"
                        "background-color: #1f618d; /* Еще темнее при нажатии */"
                        "}"
                        "QLineEdit {"
                        "   background-color: #ecf0f1; /* Светло-серый */"
                        "   border: 1px solid #bdc3c7; /* Граница */"
                        "   padding: 8px;"
                        "   border-radius: 3px;"
                        "   font-size: 14px;"
                        "}"
                         "QLineEdit:focus {"
                        "  border-color: #3498db; /* Голубая граница при фокусе */"
                       "}"
                      "QTextEdit {"
                       "  background-color: #ecf0f1; /* Светло-серый */"
                        "   border: 1px solid #bdc3c7; /* Граница */"
                       "    padding: 8px;"
                       "   border-radius: 3px;"
                        "   font-size: 14px;"
                        "}"
                         "QTextEdit:focus {"
                        "  border-color: #3498db; /* Голубая граница при фокусе */"
                       "}"
                        "QListWidget {"
                        "  background-color: #f0f0f0; /* Светло-серый */"
                        " border: 1px solid #d0d0d0;"
                        "}"
                        "QListWidget::item {"
                        "  padding: 8px;"
                       "  border-bottom: 1px solid #e0e0e0;"
                       "}"
                       "QListWidget::item:selected {"
                       "  background-color: #f7f7f7;"
                        "  color: #333;"
                        "}"
                        "QLabel { "
                         "font-size: 14px;"
                         "}";

    this->setStyleSheet(styleSheet);


}
