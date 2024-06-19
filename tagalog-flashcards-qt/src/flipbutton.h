#ifndef FLIPBUTTON_H
#define FLIPBUTTON_H

#include <QPushButton>

class FlipButton : public QPushButton {
    Q_OBJECT

public:

    FlipButton();
    FlipButton(const QString& question, const QString& answer, QWidget* parent);
    void setQuestion(const QString& question);
    void setAnswer(const QString& answer);

protected:
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private slots:
    void toggleText();

private:
    QString question;
    QString answer;
};


#endif // FLIPBUTTON_H
