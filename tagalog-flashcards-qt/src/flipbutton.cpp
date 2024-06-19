#include "flipbutton.h"

FlipButton::FlipButton()
    : QPushButton("", nullptr), question(""), answer("")
{
    connect(this, &QPushButton::clicked, this, &FlipButton::toggleText);
}

FlipButton::FlipButton(const QString& question, const QString& answer, QWidget* parent = nullptr)
    : QPushButton(question, parent), question(question), answer(answer)
{
    connect(this, &QPushButton::clicked, this, &FlipButton::toggleText);
}

void FlipButton::toggleText()
{
    if (this->text() == question) {
        this->setText(answer);
    } else {
        this->setText(question);
    }
}

void FlipButton::setQuestion(const QString& question)
{
    this->question = question;
}

void FlipButton::setAnswer(const QString& answer)
{
    this->answer = answer;
}

void FlipButton::enterEvent(QEnterEvent* event)
{
    QPushButton::enterEvent(event); // Call base class implementation
    setCursor(Qt::PointingHandCursor); // Change cursor to arrow when entering button
}

void FlipButton::leaveEvent(QEvent* event)
{
    QPushButton::leaveEvent(event); // Call base class implementation
    setCursor(Qt::ArrowCursor); // Change cursor back to pointing hand when leaving button
}
