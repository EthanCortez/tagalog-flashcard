#include "flashcardwindow.h"
#include "./ui_flashcardwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

FlashcardWindow::FlashcardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlashcardWindow)
{
    ui->setupUi(this);

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
        "    border-radius: 40px; /* Rounded corners */"
        "    padding: 10px 15px; /* Padding */"
        "}"
        "QPushButton:hover {"
        "    background-color: #5c80cc; /* Darker shade of blue on hover */"
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

void FlashcardWindow::handleStudyButtonClicked()
{
    // Clear the layout of the central widget
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

    // Set style sheet for Flashcard Button
    QPushButton *flashcardButton = new QPushButton("Flashcard Button", this);
    QFont flashcardButtonFont("Calibri", 20); // Font family and size for QPushButton text
    flashcardButton->setFont(flashcardButtonFont);
    flashcardButton->setStyleSheet("background-color: #fafafa; color: white;");
    flashcardButton->setMinimumHeight(50); // Set the minimum height of Flashcard Button
    flashcardButton->setMaximumHeight(100); // Set the maximum height of Flashcard Button

    // Set style sheet for Yes Button
    QPushButton *yesButton = new QPushButton("Yes Button", this);
    yesButton->setStyleSheet("background-color: #c6ffc2; color:  white;");
    yesButton->setMinimumHeight(50);
    yesButton->setMaximumHeight(100);

    // Set style sheet for No Button
    QPushButton *noButton = new QPushButton("No Button", this);
    noButton->setStyleSheet("background-color: #ff9ea9; color: white;");
    noButton->setMinimumHeight(50);
    noButton->setMaximumHeight(100);

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
    mainLayout->addWidget(flashcardButton);
    mainLayout->addLayout(buttonsLayout); // Add the QHBoxLayout containing the smaller buttons
    mainLayout->addStretch(); // Add stretch to push the buttons to the top

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

FlashcardWindow::~FlashcardWindow()
{
    delete ui;
}
