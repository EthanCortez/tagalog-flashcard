#include "flashcard.h"

Flashcard::Flashcard(const QString& question, const QString& answer, const QDateTime& nextReviewDate, int reviewInterval)
    : question(question), answer(answer), nextReviewDate(nextReviewDate), reviewInterval(reviewInterval){
}

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
    this->reviewInterval = reviewInterval;
}

void Flashcard::review(bool correct) {

    if (correct) {
        // Increase the review interval (e.g., using a spaced repetition algorithm)
        reviewInterval *= 2;
        nextReviewDate = QDateTime::currentDateTime().addSecs(reviewInterval * 30);
    } else {
        // Reset the review interval (e.g., review again soon)
        reviewInterval = 1;
    }
}

QJsonObject Flashcard::toJson() const {

    QJsonObject json;
    json["question"] = question;
    json["answer"] = answer;
    json["nextReviewDate"] = nextReviewDate.toString(Qt::ISODate);
    json["reviewInterval"] = reviewInterval;
    return json;
}
