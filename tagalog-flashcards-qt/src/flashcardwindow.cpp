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

    // Load/parse flashcards
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
    clearLayout(ui->centralWidget->layout());

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

    studyButton->setCursor(Qt::PointingHandCursor);
    connect(studyButton, &QPushButton::clicked, this, &FlashcardWindow::handleStudyButtonClicked);

    // Create a button that goes back to study if user desires
    QPushButton *settingsButton = new QPushButton(this);
    settingsButton->setFixedSize(50,50);
    settingsButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background-image: url(:/images/images/settings.png);"
        "background-repeat: no-repeat;"
        "background-position: top right;"
        "}"
        );

    settingsButton->setCursor(Qt::PointingHandCursor);
    connect(settingsButton, &QPushButton::clicked, this, &FlashcardWindow::loadSettingsPage);

    // Create a QHBoxLayout to put the settingsButton on the top right
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addStretch();
    settingsLayout->addWidget(settingsButton);

    // Create a new layout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);

    // Add widgets to the layout
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addLayout(settingsLayout);
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

        addCardsButton->setCursor(Qt::PointingHandCursor);
        connect(addCardsButton, &QPushButton::clicked, this, &FlashcardWindow::addCards);
        mainLayout->addWidget(addCardsButton, 0, Qt::AlignCenter);
    }

    if (count == 0) {
        QLabel *noCardsLabel = new QLabel("No more cards available to add", this);
        QFont noCardsFont("Calibri", 12);
        noCardsLabel->setFont(noCardsFont);
        noCardsLabel->setStyleSheet("color: #333333;");
        mainLayout->addWidget(noCardsLabel, 0, Qt::AlignCenter);
    }

    // Add more widgets
    mainLayout->addSpacing(20);
    mainLayout->addStretch();
    mainLayout->addWidget(studyButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

void FlashcardWindow::loadCardPage()
{
    // Clear the layout of the central widget to replace with new layout
    clearLayout(ui->centralWidget->layout());

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

    yesButton->setCursor(Qt::PointingHandCursor);
    connect(yesButton, &QPushButton::clicked, this, &FlashcardWindow::handleYesButtonClicked);

    // Set style sheet for No Button
    QPushButton *noButton = new QPushButton("No Button", this);
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

    noButton->setCursor(Qt::PointingHandCursor);
    connect(noButton, &QPushButton::clicked, this, &FlashcardWindow::handleNoButtonClicked);

    // Modify font for yes and no buttons
    QFont buttonFont("Calibri", 20);
    yesButton->setFont(buttonFont);
    noButton->setFont(buttonFont);

    // Create a QHBoxLayout for the row containing the two smaller buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(yesButton);
    buttonsLayout->setSpacing(10);
    buttonsLayout->addWidget(noButton);
    buttonsLayout->addStretch();

    // Create a button that goes back to study if user desires
    QPushButton *goBackButton = new QPushButton(this);
    goBackButton->setFixedSize(50,50);
    goBackButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background-image: url(:/images/images/rightgrayarrow.png);"
        "background-repeat: no-repeat;"
        "background-position: top left;"
        "}"
        );

    goBackButton->setCursor(Qt::PointingHandCursor);
    connect(goBackButton, &QPushButton::clicked, this, &FlashcardWindow::loadStartPage);

    // Create a QVBoxLayout to arrange the large button and the QHBoxLayout vertically
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(goBackButton, 0, Qt::AlignLeft);
    mainLayout->addSpacing(10);
    mainLayout->addStretch();
    mainLayout->addWidget(flipButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

void FlashcardWindow::loadGoBackPage()
{
    // Clear the layout of the central widget to replace with new layout
    clearLayout(ui->centralWidget->layout());

    // Display that all cards have been studied using labels
    QLabel *noCardsToStudyLabel = new QLabel("All of the cards have been studied!", this);
    QFont noCardsToStudyFont("Calibri", 20, QFont::Bold);
    noCardsToStudyLabel->setFont(noCardsToStudyFont);

    // Add a button that allows the button to go back to the start page
    QPushButton *goBackButton = new QPushButton("Go back", this);
    QFont goBackButtonFont("Calibri", 16);
    goBackButton->setFont(goBackButtonFont);
    goBackButton->setFixedSize(200, 60);
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

    goBackButton->setCursor(Qt::PointingHandCursor);
    connect(goBackButton, &QPushButton::clicked, this, &FlashcardWindow::loadStartPage);

    // Create a QVBoxLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
    mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(noCardsToStudyLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(goBackButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

void FlashcardWindow::loadSettingsPage()
{
    // Clear the layout of the central widget to replace with new layout
    clearLayout(ui->centralWidget->layout());

    // Create a button that goes back to study if user desires
    QPushButton *goBackButton = new QPushButton(this);
    goBackButton->setFixedSize(50,50);
    goBackButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "background-image: url(:/images/images/rightgrayarrow.png);"
        "background-repeat: no-repeat;"
        "background-position: top left;"
        "}"
    );

    goBackButton->setCursor(Qt::PointingHandCursor);
    connect(goBackButton, &QPushButton::clicked, this, &FlashcardWindow::loadStartPage);

    // Create a QHBoxLayout to put the goBackButton on the top right
    QHBoxLayout *goBackLayout = new QHBoxLayout();
    goBackLayout->addWidget(goBackButton);
    goBackLayout->addStretch();

    // Create label for settings
    QLabel *settingsLabel = new QLabel("Settings", this);

    // Modify fonts and styles
    QFont titleFont("Calibri", 32, QFont::Bold);
    settingsLabel->setFont(titleFont);

    // Create a button that can reset cards
    QPushButton *resetButton = new QPushButton("Reset Cards", this);
    resetButton->setFixedSize(100, 40);
    resetButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #ff7070;"
        "    color: #474747;"
        "    border: 2px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    padding: 5px 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #ff9ea9;"
        "}"
    );

    // Create a QHBoxLayout to push resetButton right a little
    QHBoxLayout *resetLayout = new QHBoxLayout();
    resetLayout->addStretch();
    resetLayout->addWidget(resetButton);
    resetLayout->addSpacing(400);
    resetLayout->addStretch();

    resetButton->setCursor(Qt::PointingHandCursor);
    connect(resetButton, &QPushButton::clicked, this, &FlashcardWindow::loadConfirmPage);

    // Create a QVBoxLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(goBackLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(settingsLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(resetLayout);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

void FlashcardWindow::loadConfirmPage()
{
    // Clear the layout of the central widget to replace with new layout
    clearLayout(ui->centralWidget->layout());

    // Create label for settings
    QLabel *confirmLabel = new QLabel("Are you sure you want to reset your flashcards?", this);

    // Modify fonts and styles
    QFont confirmFont("Calibri", 20, QFont::Bold);
    confirmLabel->setFont(confirmFont);

    // Set style sheet for Yes Button
    QPushButton *yesButton = new QPushButton("Yes", this);
    yesButton->setFixedSize(200, 60);
    yesButton->setStyleSheet("QPushButton {"
                            "background-color: #ff7070;"
                            "color: #3b3b3b;"
                            "border: 2px solid #585c57;"
                            "border-radius: 20px;"
                            "padding: 10px 15px;"
                            "}"
                            "QPushButton:hover {"
                            "    background-color: #ff9ea9;"
                            "}");
    yesButton->setCursor(Qt::PointingHandCursor);
    connect(yesButton, &QPushButton::clicked, this, &FlashcardWindow::handleResetButtonClicked);

    // Set style sheet for No Button
    QPushButton *noButton = new QPushButton("No", this);
    noButton->setStyleSheet("background-color: #ff9ea9; color: black;");
    noButton->setFixedSize(200, 60);
    noButton->setStyleSheet("QPushButton {"
                             "background-color: #a8ff96;"
                             "color: #3b3b3b;"
                             "border: 2px solid #585c57;"
                             "border-radius: 20px;"
                             "padding: 10px 15px;"
                             "}"
                             "QPushButton:hover {"
                             "    background-color: #befcb1;"
                             "}");

    noButton->setCursor(Qt::PointingHandCursor);
    connect(noButton, &QPushButton::clicked, this, &FlashcardWindow::loadSettingsPage);

    // Modify font for yes and no buttons
    QFont buttonFont("Calibri", 20);
    yesButton->setFont(buttonFont);
    noButton->setFont(buttonFont);

    // Create a QHBoxLayout for the row containing the two smaller buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(yesButton);
    buttonsLayout->setSpacing(30);
    buttonsLayout->addWidget(noButton);
    buttonsLayout->addStretch();

    // Create a QVBoxLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(confirmLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    // Set the layout for the central widget
    ui->centralWidget->setLayout(mainLayout);
}

void FlashcardWindow::loadResetPage()
{
    // Clear the layout of the central widget to replace with new layout
    clearLayout(ui->centralWidget->layout());

    // Create label for settings
    QLabel *resetLabel = new QLabel("You have reset the flashcards!", this);

    // Modify fonts and styles
    QFont confirmFont("Calibri", 20, QFont::Bold);
    resetLabel->setFont(confirmFont);

    // Add a button that allows the button to go back to the start page
    QPushButton *goBackButton = new QPushButton("Go back", this);
    QFont goBackButtonFont("Calibri", 16);
    goBackButton->setFont(goBackButtonFont);
    goBackButton->setFixedSize(200, 60);
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

    goBackButton->setCursor(Qt::PointingHandCursor);
    connect(goBackButton, &QPushButton::clicked, this, &FlashcardWindow::loadSettingsPage);

    // Create a QVBoxLayout
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(resetLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(goBackButton, 0, Qt::AlignCenter);
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

    // Check if there are no cards to study
    if (numCardsToStudy == 0)
        loadGoBackPage();
    else
        loadCardPage();
}

void FlashcardWindow::handleYesButtonClicked()
{
    // Modify the cards reviewInterval and reviewDate
    flashcardSystem.getFlashcard(randomIndex).review(true);

    // Delete from indexesToStudy the randomIndex
    auto it = std::find(indexesToStudy.begin(), indexesToStudy.end(), randomIndex);
    if (it != indexesToStudy.end())
        indexesToStudy.erase(it);

    // Check if the last card was the last card to be studied
    if (indexesToStudy.empty())
        loadGoBackPage();
    else
        loadCardPage();
}

void FlashcardWindow::handleNoButtonClicked()
{
    // Modify the cards reviewInterval and reviewDate
    flashcardSystem.getFlashcard(randomIndex).review(false);

    loadCardPage();
}

void FlashcardWindow::handleResetButtonClicked()
{
    // Reset nextReviewDate and reviewInterval
    int index = 0;

    while (index < flashcardSystem.size())
    {
        Flashcard tempCard = flashcardSystem.getFlashcard(index);

        tempCard.setNextReviewDate(QDateTime());
        tempCard.setReviewInterval(1);
        flashcardSystem.editFlashcard(index, tempCard);

        index++;
    }

    // Load reset page
    loadResetPage();
}


void FlashcardWindow::loadFlashcards()
{

    // Read from json to load flashcards
    QFile file("data.json");

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

    loadStartPage();
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
    QFile file("data.json");
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
