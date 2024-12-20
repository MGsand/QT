#ifndef DRAGDROPLISTWIDGET_H
#define DRAGDROPLISTWIDGET_H


#include <QListWidget>
#include <QDropEvent>

class DragDropListWidget : public QListWidget
{
    Q_OBJECT
public:
    DragDropListWidget(QWidget *parent = nullptr);

signals:
    void rowsDropped();


protected:
    void dropEvent(QDropEvent *event) override;
};

#endif // DRAGDROPLISTWIDGET_H
