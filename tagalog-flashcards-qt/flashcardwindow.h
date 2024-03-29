#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QMainWindow>

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

private:

    Ui::FlashcardWindow *ui;

    // Data structure to store flashcards
    struct Flashcard {
        QString question;
        QString answer;
    };

    QVector<Flashcard> flashcards; // Vector to store flashcards
    int currentCardIndex;          // Index of the currently displayed flashcard

    void loadFlashcards(); // Method to load flashcards from storage
    void saveFlashcards(); // Method to save flashcards to storage
    void toggleFlashcard();
};

#endif // FLASHCARDWINDOW_H
