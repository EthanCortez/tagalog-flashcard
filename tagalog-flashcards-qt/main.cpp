#include <QApplication>
#include "flashcardwindow.h"

int main(int argc, char *argv[])
{
    /*
    std::ifstream inFile("test.json");
    json cardarr;
    inFile >> cardarr;
    std::cout << cardarr[0].dump(4) << std::endl;

    for (json::iterator it = cardarr.begin(); it != cardarr.end(); ++it)
    {
        std::string obj = (*it)["question"].get<std::string>();
    }

    json newCard = {
        {"question", "hey"},
        {"answer", "yo"}
    };

    cardarr.push_back(newCard);

    std::ofstream outFile("test.json");
    outFile << cardarr.dump(4);

*/



    QApplication app(argc, argv);
    FlashcardWindow w;
    w.show();
    return app.exec();
}
