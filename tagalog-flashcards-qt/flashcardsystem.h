#ifndef FLASHCARDSYSTEM_H
#define FLASHCARDSYSTEM_H

#include "flashcard.h"

class FlashcardSystem {
public:

    // Methods for managing flashcards
    void addFlashcard(const Flashcard& flashcard);
    void editFlashcard(int index, const Flashcard& flashcard);
    void deleteFlashcard(int index);
    Flashcard getNextFlashcard();
    void reviewFlashcard(int index, bool correct);

private:
    QVector<Flashcard> flashcards;
    QDateTime calculateNextReviewDate(const Flashcard& flashcard, bool correct);
};

#endif // FLASHCARDSYSTEM_H
