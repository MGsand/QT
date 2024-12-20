#include "dragdroplistwidget.h"

DragDropListWidget::DragDropListWidget(QWidget *parent) : QListWidget(parent)
{

}

void DragDropListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
    emit rowsDropped();
}
