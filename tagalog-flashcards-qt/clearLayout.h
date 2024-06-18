#ifndef CLEARLAYOUT_H
#define CLEARLAYOUT_H

#include "qwidget.h"
#include <QLayout>
void clearLayout(QLayout *layout) {
    if (layout)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr)
        {
            if (item->layout()) {
                clearLayout(item->layout());
            }
            if (item->widget()) {
                delete item->widget();
            }
        }
        delete layout;
    }
}

#endif // CLEARLAYOUT_H
