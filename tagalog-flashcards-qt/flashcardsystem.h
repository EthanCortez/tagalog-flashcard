#ifndef FLASHCARDSYSTEM_H
#define FLASHCARDSYSTEM_H

#include <QDateTime>
#include "flashcard.h"

class FlashcardSystem {
public:

    // Methods for managing flashcards
    void addFlashcard(std::string question, std::string answer, std::string nextReviewDate, int reviewInterval);
    Flashcard& getFlashcard(int index);
    void editFlashcard(int index, const Flashcard& flashcard);
    int size();

private:
    static inline QVector<Flashcard> flashcards;
    QDateTime calculateNextReviewDate(const Flashcard& flashcard, bool correct);
};

#endif // FLASHCARDSYSTEM_H
