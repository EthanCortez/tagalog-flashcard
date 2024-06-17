#ifndef FLASHCARDSYSTEM_H
#define FLASHCARDSYSTEM_H

#include "flashcard.h"
#include "json.hpp"

using nlohmann::json;

class FlashcardSystem {
public:

    // Methods for managing flashcards
    static void addFlashcard(std::string question, std::string answer);
    static Flashcard getFlashcard(int index);

    void editFlashcard(int index, const Flashcard& flashcard);
    void deleteFlashcard(int index);
    void reviewFlashcard(int index, bool correct);

private:
    static inline QVector<Flashcard> flashcards;
    QDateTime calculateNextReviewDate(const Flashcard& flashcard, bool correct);
};

#endif // FLASHCARDSYSTEM_H
