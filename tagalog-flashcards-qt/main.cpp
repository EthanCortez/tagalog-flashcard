#include <QApplication>
#include "flashcardwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    FlashcardWindow w;
    w.show();
    return app.exec();
}
