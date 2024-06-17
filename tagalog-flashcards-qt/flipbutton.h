#ifndef FLIPBUTTON_H
#define FLIPBUTTON_H

#include <QPushButton>

class FlipButton : public QPushButton {
    Q_OBJECT

public:
    FlipButton(const QString& question, const QString& answer, QWidget* parent);

private slots:
    void toggleText();

private:
    QString question;
    QString answer;
};


#endif // FLIPBUTTON_H
