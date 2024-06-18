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
