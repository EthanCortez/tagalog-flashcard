#include "flashcardsystem.h"

void FlashcardSystem::addFlashcard(std::string question, std::string answer, std::string nextReviewDate, int reviewInterval)
{
    QString qStr = QString::fromStdString(question);
    QString aStr = QString::fromStdString(answer);

    QString nStr = QString::fromStdString(nextReviewDate);
    QDateTime rDate = QDateTime::fromString(nStr, Qt::ISODate);

    Flashcard newCard = Flashcard(qStr, aStr, rDate, reviewInterval);
    flashcards.push_back(newCard);
    return;
}

Flashcard& FlashcardSystem::getFlashcard(int index)
{
    return flashcards[index];
}

int FlashcardSystem::size()
{
    return flashcards.size();
}

void FlashcardSystem::editFlashcard(int index, const Flashcard& flashcard)
{
    flashcards[index] = flashcard;
}
