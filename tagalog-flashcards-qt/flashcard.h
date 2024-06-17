#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QDateTime>

class Flashcard {
public:

    Flashcard(const QString& question, const QString& answer);
    QString getQuestion() const;
    QString getAnswer() const;

    QDateTime getNextReviewDate() const;
    void setNextReviewDate(const QDateTime& nextReviewDate);
    int getReviewInterval() const;
    void setReviewInterval(int reviewInterval);
    void review(bool correct);

private:

    QString question;           // Flashcard question side (tagalog word)
    QString answer;             // Flashcard answer side (english definition)
    QDateTime nextReviewDate;   // Time until it is in study
    int reviewInterval;
};

#endif // FLASHCARD_H
