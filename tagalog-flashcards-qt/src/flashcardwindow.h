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

    // Extra functions
    void loadFlashcards();
    void addCards();
    void serializer();
    void fillIndexesVector();
    int cardsToStudy();

private slots:

    // Page loaders
    void loadStartPage();
    void loadCardPage();
    void loadGoBackPage();
    void loadSettingsPage();
    void loadConfirmPage();
    void loadResetPage();

    // Button handlers
    void handleStudyButtonClicked();
    void handleYesButtonClicked();
    void handleNoButtonClicked();
    void handleResetButtonClicked();

private:

    Ui::FlashcardWindow *ui;
    FlashcardSystem flashcardSystem;    // Flashcard System class
    QVector<int> indexesToStudy;        // Vector filled with indexes of cards whose review dates have passed
    int randomIndex;                    // Randomly selected index from indexesToStudy

};

#endif // FLASHCARDWINDOW_H
