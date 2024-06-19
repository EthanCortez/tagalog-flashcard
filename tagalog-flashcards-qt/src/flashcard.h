#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class Flashcard {
public:

    Flashcard(const QString& question, const QString& answer, const QDateTime& nextReviewDate, int reviewInterval);
    QString getQuestion() const;
    QString getAnswer() const;

    QDateTime getNextReviewDate() const;
    int getReviewInterval() const;

    void setNextReviewDate(const QDateTime& nextReviewDate);
    void setReviewInterval(int reviewInterval);
    void review(bool correct);

    QJsonObject toJson() const;

private:

    QString question;           // Flashcard question side (tagalog word)
    QString answer;             // Flashcard answer side (english definition)
    QDateTime nextReviewDate;   // Time until it is in study
    int reviewInterval;         // Used to calculate nextReviewDate
};

#endif // FLASHCARD_H
