#include "flashcardwindow.h"
#include "./ui_flashcardwindow.h"

FlashcardWindow::FlashcardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlashcardWindow)
{
    ui->setupUi(this);
    loadFlashcards();
    this->currentCardIndex = 0;

    ui->flashcardButton->setStyleSheet(
        "QPushButton#flashcardButton {"
        "   background-color: #A8CEFF;"
        "   border: none;"
        "   color: black;"
        "   padding: 15px 32px;"
        "   text-align: center;"
        "   text-decoration: none;"
        "   display: inline-block;"
        "   font-size: 30px;"
        "   margin: 4px 2px;"
        "   cursor: pointer;"
        "   border-radius: 10px;"
        "}"
        "QPushButton#flashcardButton:hover {"
        "   background-color: #8ABDFF;"
        "}"
        "QPushButton#flashcardButton:pressed {"
        "   background-color: #A8CEFF;"
        "   box-shadow: 0 5px #666;"
        "   transform: translateY(4px);"
        "}"
    );

    // Create the QPushButton to represent the flashcard
    ui->flashcardButton->setText(flashcards[currentCardIndex].question);

    connect(ui->flashcardButton, &QPushButton::clicked, this, &FlashcardWindow::toggleFlashcard);
}

FlashcardWindow::~FlashcardWindow()
{
    // saveFlashcards();
    delete ui;
}

void FlashcardWindow::loadFlashcards()
{
    // Dummy flashcards
    flashcards.push_back({"What is 2+2?", "4"});
    flashcards.push_back({"What is the capital of France?", "Paris"});
}

void FlashcardWindow::saveFlashcards()
{
    // Save flashcards to storage
    // saveFlashcardsToStorage(flashcards);
}

void FlashcardWindow::toggleFlashcard()
{
    // Toggle between displaying the question and the answer

    if (currentCardIndex < flashcards.size()) {
        // If the button currently displays the question, switch to the answer
        if (ui->flashcardButton->text() == flashcards[currentCardIndex].question) {
            ui->flashcardButton->setText(flashcards[currentCardIndex].answer);
        } else {
            // Otherwise, switch back to the question
            ui->flashcardButton->setText(flashcards[currentCardIndex].question);
        }
    }
}
