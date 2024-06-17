#include "flashcardsystem.h"
#include "flashcard.h"

void FlashcardSystem::addFlashcard(std::string question, std::string answer)
{
    QString qStr = QString::fromStdString(question);
    QString aStr = QString::fromStdString(answer);
    Flashcard newCard = Flashcard(qStr, aStr);
    flashcards.push_back(newCard);
    return;
}

Flashcard FlashcardSystem::getFlashcard(int index)
{
    return flashcards[index];
}
