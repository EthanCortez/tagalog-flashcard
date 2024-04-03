#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QMainWindow>
#include "flashcardsystem.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FlashcardWindow;
}
QT_END_NAMESPACE

// FlashcardWindow class
class FlashcardWindow : public QMainWindow
{
    Q_OBJECT

public:
    FlashcardWindow(QWidget *parent = nullptr);
    ~FlashcardWindow();

private slots:
    void handleStudyButtonClicked();

private:

    Ui::FlashcardWindow *ui;

    FlashcardSystem flashcards; // Flashcard System class
    int currentCardIndex;       // Index of the currently displayed flashcard
};

#endif // FLASHCARDWINDOW_H
