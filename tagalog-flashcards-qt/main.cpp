#include <QApplication>
#include "flashcardwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set the application
    QIcon appIcon(":/images/images/flag.png");
    app.setWindowIcon(appIcon);

    // Create the main window
    FlashcardWindow w;

    // Set the window
    w.setWindowTitle("Tagalog Flashcards");
    w.setWindowIcon(appIcon);

    w.show();
    return app.exec();
}
