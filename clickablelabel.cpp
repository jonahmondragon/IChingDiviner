#include "clickablelabel.h"
#include "moc_clickablelabel.cpp"

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent) {}

void ClickableLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}
