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
    void loadStartPage();
    void handleStudyButtonClicked();
    void handleYesButtonClicked();
    void handleNoButtonClicked();
    void loadFlashcards();
    void addCards();
    void serializer();
    int cardsToStudy();
    void fillIndexesVector();

private:

    Ui::FlashcardWindow *ui;
    FlashcardSystem flashcardSystem;    // Flashcard System class
    QVector<int> indexesToStudy;        // Vector filled with indexes of cards whose review dates have passed
    int randomIndex;                    // Randomly selected index from indexesToStudy

};

#endif // FLASHCARDWINDOW_H
