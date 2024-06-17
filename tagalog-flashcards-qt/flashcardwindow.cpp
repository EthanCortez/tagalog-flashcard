#include "flashcardwindow.h"
#include "flipbutton.h"
#include "./ui_flashcardwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <fstream>

FlashcardWindow::FlashcardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlashcardWindow)
{
    ui->setupUi(this);
    loadFlashcards();
    loadStartPage();
}

void FlashcardWindow::loadStartPage()
{
    // Set QMainWindow style
    resize(800,600);
    this->setStyleSheet("QMainWindow {background-color: white;}");

    QLabel *titleLabel = new QLabel("Tagalog Flashcards", this);
    QLabel *numStudyLabel = new QLabel("Flashcards ready for studying: ", this);

    QFont titleFont("Calibri", 48, QFont::Bold); // Font family, size, and weight
    QFont numStudyFont("Calibri", 16);
    titleLabel->setFont(titleFont);
    numStudyLabel->setFont(numStudyFont);
    numStudyLabel->setStyleSheet("background-color: #c6ffc2;"
                                 "color: #2c3e50;"
                                 "padding: 10px 20px;" // Adjust padding as needed
                                 "border-radius: 15px;"); // Adjust border-radius to make edges rounded


    QPushButton *studyButton = new QPushButton("Study", this);
    QFont studyButtonFont("Calibri", 16);
    studyButton->setFont(studyButtonFont);
    studyButton->setFixedSize(250, 80);
    studyButton->setStyleSheet("QPushButton {"
                               "    background-color: #2c3e50;"
                               "    color: white;"
                               "    border: 2px solid #bdc3c7;"
                               "    border-radius: 40px;"
                               "    padding: 10px 15px;"
                               "}"
                               "QPushButton:hover {"
                               "    background-color: #5c80cc;"
                               "}");

    connect(studyButton, &QPushButton::clicked, this, &FlashcardWindow::handleStudyButtonClicked);

    // Create a layout
    QVBoxLayout *layout = new QVBoxLayout(ui->centralWidget);

    // Add widgets to the layout
    layout->setAlignment(Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(titleLabel, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addWidget(numStudyLabel, 0, Qt::AlignCenter);
    layout->addSpacing(20);
    layout->addStretch();
    layout->addWidget(studyButton, 0, Qt::AlignCenter);
    layout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(layout);
}

void FlashcardWindow::loadFlashcards()
{
    std::ifstream inFile("test.json");
    json cardarr;
    inFile >> cardarr;

    for (json::iterator it = cardarr.begin(); it != cardarr.end(); ++it)
    {
        std::string question = (*it)["question"].get<std::string>();
        std::string answer = (*it)["answer"].get<std::string>();
        flashcardSystem.addFlashcard(question, answer);
    }

    inFile.close();
}



void FlashcardWindow::handleStudyButtonClicked()
{

    // Check if new flashcards need to be added or study previously added cards

    // Clear the layout of the central widget to replace with new layout
    QLayout *layout = ui->centralWidget->layout();
    if (layout)
    {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }

        delete layout;
    }

    Flashcard temp = flashcardSystem.getFlashcard(0);
    QString question = temp.getQuestion();
    QString answer = temp.getAnswer();

    FlipButton *flipButton = new FlipButton(question, answer, nullptr);
    QFont flipButtonFont("Calibri", 20);
    flipButton->setFont(flipButtonFont);
    flipButton->setStyleSheet("background-color: #fafafa; color: black;");
    flipButton->setFixedSize(500, 300);

    // Set style sheet for Yes Button
    QPushButton *yesButton = new QPushButton("Yes Button", this);
    yesButton->setFixedSize(245, 80);
    yesButton->setStyleSheet("QPushButton {"
                             "background-color: #a8ff96;"
                             "color: #3b3b3b;"
                             "border: 2px solid #585c57;"
                             "border-radius: 10px;"
                             "padding: 10px 15px;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: #befcb1;"
                             "}");

    // Set style sheet for No Button
    QPushButton *noButton = new QPushButton("No Button", this);
    noButton->setStyleSheet("background-color: #ff9ea9; color: black;");
    noButton->setFixedSize(245, 80);
    noButton->setStyleSheet("QPushButton {"
                             "background-color: #ff7070;"
                             "color: #3b3b3b;"
                             "border: 2px solid #585c57;"
                             "border-radius: 10px;"
                             "padding: 10px 15px;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: #ff9ea9;"
                             "}");

    QFont buttonFont("Calibri", 20);
    yesButton->setFont(buttonFont);
    noButton->setFont(buttonFont);

    // Create a QHBoxLayout for the row containing the two smaller buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(yesButton);
    buttonsLayout->addWidget(noButton);
    buttonsLayout->setSpacing(10); // Set spacing between the buttons

    // Create a QVBoxLayout to arrange the large button and the QHBoxLayout vertically
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(flipButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

FlashcardWindow::~FlashcardWindow()
{
    delete ui;
}
