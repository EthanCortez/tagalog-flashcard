#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class FlashcardWindow;
}
QT_END_NAMESPACE

class FlashcardWindow : public QMainWindow
{
    Q_OBJECT

public:
    FlashcardWindow(QWidget *parent = nullptr);
    ~FlashcardWindow();

private:
    Ui::FlashcardWindow *ui;
};
#endif // FLASHCARDWINDOW_H
