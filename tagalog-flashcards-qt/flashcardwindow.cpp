#include "flashcardwindow.h"
#include "./ui_flashcardwindow.h"

FlashcardWindow::FlashcardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlashcardWindow)
{
    ui->setupUi(this);
}

FlashcardWindow::~FlashcardWindow()
{
    delete ui;
}
