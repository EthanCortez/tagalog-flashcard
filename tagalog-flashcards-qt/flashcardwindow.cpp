#include "flashcardwindow.h"
#include "flipbutton.h"
#include "random_selector.h"
#include "clearLayout.h"
#include "./ui_flashcardwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QIODevice>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

FlashcardWindow::FlashcardWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlashcardWindow)
{
    ui->setupUi(this);

    // Load flashcards
    loadFlashcards();

    // Set QMainWindow style
    resize(800,600);
    this->setStyleSheet("QMainWindow {background-color: white;}");

    // Load start page of app
    loadStartPage();

    // Serialize information when user closes app
    connect(qApp, &QCoreApplication::aboutToQuit, this, &FlashcardWindow::serializer);
}

void FlashcardWindow::loadStartPage()
{
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

    // Create label for title
    QLabel *titleLabel = new QLabel("Tagalog Flashcards", this);

    // Create label that displays how many flashcards ready to study
    QString qNumCards = QString::fromStdString(std::to_string(cardsToStudy()));
    QLabel *numStudyLabel = new QLabel("Flashcards ready for studying: " + qNumCards, this);

    // Modify fonts and styles
    QFont titleFont("Calibri", 48, QFont::Bold); // Font family, size, and weight
    QFont numStudyFont("Calibri", 16);
    titleLabel->setFont(titleFont);
    numStudyLabel->setFont(numStudyFont);
    numStudyLabel->setStyleSheet("background-color: #c6ffc2;"
                                 "color: #2c3e50;"
                                 "padding: 10px 20px;"
                                 "border-radius: 15px;");


    // Create button that will go to handleStudyButtonClicked
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

    // Create a new layout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);

    // Add widgets to the layout
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(numStudyLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);

    // Check how many cards are left to add
    int index = 0;
    int count = 0;
    while (index < flashcardSystem.size() && count < 5)
    {
        Flashcard tempCard = flashcardSystem.getFlashcard(index);
        if (tempCard.getNextReviewDate().isNull())
        {
            count++;
        }
        index++;
    }

    // If there are no cards to study and there are cards left to add, a button that add new cards to study will appear
    if (cardsToStudy() == 0 && count >= 5) {

        QPushButton *addCardsButton = new QPushButton("Add +5 Cards", this);
        QFont addCardsButtonFont("Calibri", 16);
        addCardsButton->setFont(addCardsButtonFont);
        addCardsButton->setFixedSize(180, 40);
        addCardsButton->setStyleSheet("QPushButton {"
                                      "    background-color: #d9fff4;"
                                      "    color: #474747;"
                                      "    border: 2px solid #bdc3c7;"
                                      "    border-radius: 5px;"
                                      "    padding: 5px 5px;"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: #ebfff9;"
                                      "}");


        connect(addCardsButton, &QPushButton::clicked, this, &FlashcardWindow::addCards);
        mainLayout->addWidget(addCardsButton, 0, Qt::AlignCenter);
    }

    // Add more widgets
    mainLayout->addSpacing(20);
    mainLayout->addStretch();
    mainLayout->addWidget(studyButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}





void FlashcardWindow::handleStudyButtonClicked()
{
    // Fills a vector that contains indexes of all cards that need to be studied.
    fillIndexesVector();

    // Initialize variables
    int numCardsToStudy = cardsToStudy();
    randomIndex = 0;

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

    // Check if there are no cards to study
    if (numCardsToStudy == 0)
    {

        // Display that all cards have been studied using labels
        QLabel *noCardsToStudyLabel = new QLabel("All of the cards have been studied!", this);
        QFont noCardsToStudyFont("Calibri", 36);
        noCardsToStudyLabel->setFont(noCardsToStudyFont);

        // Add a button that allows the button to go back to the start page
        QPushButton *goBackButton = new QPushButton("Go back", this);
        QFont goBackButtonFont("Calibri", 16);
        goBackButton->setFont(goBackButtonFont);
        goBackButton->setFixedSize(250, 80);
        goBackButton->setStyleSheet("QPushButton {"
                                   "    background-color: #2c3e50;"
                                   "    color: white;"
                                   "    border: 2px solid #bdc3c7;"
                                   "    border-radius: 10px;"
                                   "    padding: 10px 15px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "    background-color: #5c80cc;"
                                   "}");

        connect(goBackButton, &QPushButton::clicked, this, &FlashcardWindow::loadStartPage);

        // Create a QVBoxLayout
        QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
        mainLayout->setAlignment(Qt::AlignHCenter);
        mainLayout->addStretch();
        mainLayout->addWidget(noCardsToStudyLabel, 0, Qt::AlignCenter);
        mainLayout->addSpacing(20);
        mainLayout->addWidget(goBackButton, 0, Qt::AlignCenter);
        mainLayout->addSpacing(20);
        mainLayout->addStretch();

        // Set the layout for the central widget
        ui->centralWidget->setLayout(mainLayout);
    }
    else
    {
        // Get a random card that needs to be studied
        randomIndex = *select_randomly(indexesToStudy.begin(), indexesToStudy.end());
        QString question = flashcardSystem.getFlashcard(randomIndex).getQuestion();
        QString answer = flashcardSystem.getFlashcard(randomIndex).getAnswer();

        // Button used to display question and answer
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

        connect(yesButton, &QPushButton::clicked, this, &FlashcardWindow::handleYesButtonClicked);

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

        connect(noButton, &QPushButton::clicked, this, &FlashcardWindow::handleNoButtonClicked);

        // Modify font for yes and no buttons
        QFont buttonFont("Calibri", 20);
        yesButton->setFont(buttonFont);
        noButton->setFont(buttonFont);

        // Create a QHBoxLayout for the row containing the two smaller buttons
        QHBoxLayout *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget(yesButton);
        buttonsLayout->addWidget(noButton);
        buttonsLayout->setSpacing(10);

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
}

void FlashcardWindow::handleYesButtonClicked()
{
    // Modify the cards reviewInterval and reviewDate
    flashcardSystem.getFlashcard(randomIndex).review(true);

    // Delete from indexesToStudy the randomIndex
    auto it = std::find(indexesToStudy.begin(), indexesToStudy.end(), randomIndex);
    if (it != indexesToStudy.end())
        indexesToStudy.erase(it);

    // Clear the layout of the central widget to replace with new layout
    QLayout *layout = ui->centralWidget->layout();
    clearLayout(layout);

    // Check if the last card was the last card to be studied
    if (indexesToStudy.empty())
    {

        // Display that all cards have been studied using labels
        QLabel *noCardsToStudyLabel = new QLabel("All of the cards have been studied!", this);
        QFont noCardsToStudyFont("Calibri", 36);
        noCardsToStudyLabel->setFont(noCardsToStudyFont);

        // Add a button that allows the button to go back to the start page
        QPushButton *goBackButton = new QPushButton("Go back", this);
        QFont goBackButtonFont("Calibri", 16);
        goBackButton->setFont(goBackButtonFont);
        goBackButton->setFixedSize(250, 80);
        goBackButton->setStyleSheet("QPushButton {"
                                    "    background-color: #2c3e50;"
                                    "    color: white;"
                                    "    border: 2px solid #bdc3c7;"
                                    "    border-radius: 10px;"
                                    "    padding: 10px 15px;"
                                    "}"
                                    "QPushButton:hover {"
                                    "    background-color: #5c80cc;"
                                    "}");

        connect(goBackButton, &QPushButton::clicked, this, &FlashcardWindow::loadStartPage);

        // Create a QVBoxLayout
        QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
        mainLayout->setAlignment(Qt::AlignHCenter);
        mainLayout->addStretch();
        mainLayout->addWidget(noCardsToStudyLabel, 0, Qt::AlignCenter);
        mainLayout->addSpacing(20);
        mainLayout->addWidget(goBackButton, 0, Qt::AlignCenter);
        mainLayout->addSpacing(20);
        mainLayout->addStretch();

        // Set the layout for the central widget
        ui->centralWidget->setLayout(mainLayout);
    }
    else
    {
        // Get a random card that needs to be studied
        randomIndex = *select_randomly(indexesToStudy.begin(), indexesToStudy.end());
        QString question = flashcardSystem.getFlashcard(randomIndex).getQuestion();
        QString answer = flashcardSystem.getFlashcard(randomIndex).getAnswer();

        // Button used to display question and answer
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

        connect(yesButton, &QPushButton::clicked, this, &FlashcardWindow::handleYesButtonClicked);

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

        connect(noButton, &QPushButton::clicked, this, &FlashcardWindow::handleNoButtonClicked);

        // Modify font for yes and no buttons
        QFont buttonFont("Calibri", 20);
        yesButton->setFont(buttonFont);
        noButton->setFont(buttonFont);

        // Create a QHBoxLayout for the row containing the two smaller buttons
        QHBoxLayout *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget(yesButton);
        buttonsLayout->addWidget(noButton);
        buttonsLayout->setSpacing(10);

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

}

void FlashcardWindow::handleNoButtonClicked()
{
    // Modify the cards reviewInterval and reviewDate
    flashcardSystem.getFlashcard(randomIndex).review(false);

    // Clear the layout of the central widget to replace with new layout
    QLayout *layout = ui->centralWidget->layout();
    clearLayout(layout);

    // Get a random card that needs to be studied
    randomIndex = *select_randomly(indexesToStudy.begin(), indexesToStudy.end());
    QString question = flashcardSystem.getFlashcard(randomIndex).getQuestion();
    QString answer = flashcardSystem.getFlashcard(randomIndex).getAnswer();

    // Button used to display question and answer
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

    connect(yesButton, &QPushButton::clicked, this, &FlashcardWindow::handleYesButtonClicked);

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

    connect(noButton, &QPushButton::clicked, this, &FlashcardWindow::handleNoButtonClicked);

    // Modify font for yes and no buttons
    QFont buttonFont("Calibri", 20);
    yesButton->setFont(buttonFont);
    noButton->setFont(buttonFont);

    // Create a QHBoxLayout for the row containing the two smaller buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(yesButton);
    buttonsLayout->addWidget(noButton);
    buttonsLayout->setSpacing(10);

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

void FlashcardWindow::loadFlashcards()
{

    // Read from json to load flashcards
    QFile file("../tagalog-flashcards-qt/data/data.json");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open JSON file:" << file.errorString();
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Failed to parse JSON:" << parseError.errorString();
        return;
    }

    if (!jsonDoc.isArray()) {
        qDebug() << "JSON document is not an array";
        return;
    }

    QJsonArray cardArray = jsonDoc.array();
    for (int i = 0; i < cardArray.size(); ++i) {
        QJsonObject cardObject = cardArray[i].toObject();

        QString question = cardObject["question"].toString();
        QString answer = cardObject["answer"].toString();
        QString nextReviewDate = cardObject["nextReviewDate"].toString();
        int reviewInterval = cardObject["reviewInterval"].toInt();

        flashcardSystem.addFlashcard(question.toStdString(), answer.toStdString(), nextReviewDate.toStdString(), reviewInterval);
    }
}

void FlashcardWindow::addCards()
{
    // Check if nextReviewDate of card is empty, if so the card has not been added yet
    // Add it by setting nextReviewDate
    int index = 0;
    int count = 0;
    while (index < flashcardSystem.size() && count < 5)
    {
        Flashcard tempCard = flashcardSystem.getFlashcard(index);
        if (tempCard.getNextReviewDate().isNull())
        {
            tempCard.setNextReviewDate(QDateTime::currentDateTime());
            flashcardSystem.editFlashcard(index, tempCard);
            count++;
        }
        index++;
    }

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


    QLabel *titleLabel = new QLabel("Tagalog Flashcards", this);

    QString qNumCards = QString::fromStdString(std::to_string(count));
    QLabel *numStudyLabel = new QLabel("Flashcards ready for studying: " + qNumCards, this);

    QFont titleFont("Calibri", 48, QFont::Bold); // Font family, size, and weight
    QFont numStudyFont("Calibri", 16);
    titleLabel->setFont(titleFont);
    numStudyLabel->setFont(numStudyFont);
    numStudyLabel->setStyleSheet("background-color: #c6ffc2;"
                                 "color: #2c3e50;"
                                 "padding: 10px 20px;"
                                 "border-radius: 15px;");


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

    // Create a new layout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);

    // Add widgets to the layout
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(numStudyLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addSpacing(20);
    mainLayout->addStretch();
    mainLayout->addWidget(studyButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

void FlashcardWindow::serializer()
{
    QJsonArray cardArray;

    // Serialize each flashcard in flashcardSystem
    for (int index = 0; index < flashcardSystem.size(); ++index) {
        QJsonObject cardObject = flashcardSystem.getFlashcard(index).toJson();
        cardArray.append(cardObject);
    }

    // Convert QJsonArray to QJsonDocument
    QJsonDocument jsonDoc(cardArray);

    // Write JSON data to file using QFile
    QFile file("../tagalog-flashcards-qt/data/data.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open JSON file for writing:" << file.errorString();
        return;
    }

    file.write(jsonDoc.toJson());
    file.close();
}

int FlashcardWindow::cardsToStudy()
{
    int index = 0;
    int count = 0;

    while (index < flashcardSystem.size())
    {
        Flashcard tempCard = flashcardSystem.getFlashcard(index);
        if (!tempCard.getNextReviewDate().isNull() && tempCard.getNextReviewDate() < QDateTime::currentDateTime())
        {
            count++;
        }
        index++;
    }

    return count;
}


void FlashcardWindow::fillIndexesVector()
{
    // Clear vector
    indexesToStudy.clear();

    int index = 0;

    while (index < flashcardSystem.size())
    {
        Flashcard tempCard = flashcardSystem.getFlashcard(index);
        if (!tempCard.getNextReviewDate().isNull() && tempCard.getNextReviewDate() < QDateTime::currentDateTime())
        {
            indexesToStudy.push_back(index);
        }
        index++;
    }
}


FlashcardWindow::~FlashcardWindow()
{
    delete ui;
}
