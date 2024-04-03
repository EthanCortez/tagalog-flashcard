#include "flashcard.h"

QString Flashcard::getQuestion() const {
    return question;
}

QString Flashcard::getAnswer() const {
    return answer;
}

QDateTime Flashcard::getNextReviewDate() const {
    return nextReviewDate;
}

int Flashcard::getReviewInterval() const {
    return reviewInterval;
}

void Flashcard::setNextReviewDate(const QDateTime& nextReviewDate) {
    this->nextReviewDate = nextReviewDate;
}

void Flashcard::setReviewInterval(int reviewInterval) {
    reviewInterval = reviewInterval;
}

void Flashcard::review(bool correct) {
    if (correct) {
        // Increase the review interval (e.g., using a spaced repetition algorithm)
        reviewInterval *= 2;
    } else {
        // Reset the review interval (e.g., review again soon)
        reviewInterval = 1;
    }
    // Update the next review date based on the new review interval
    nextReviewDate = QDateTime::currentDateTime().addDays(reviewInterval);
}
