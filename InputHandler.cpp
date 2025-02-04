#include "InputHandler.h"
#include <QDebug>
#include <QKeyEvent>
#include <QString>
#include <QDebug>
#include <QQmlContext>
#include <cstdlib>
#include <ctime>
#include <QFile>
#include <QStringList>

InputHandler::InputHandler(QObject *parent)
    : QObject(parent)
{
    // Initial Color
    m_currentGuessColors = QVariantList()
        << "#272729" << "#272729" << "#272729" << "#272729" << "#272729";
}

    // function that fills the word list
void InputHandler::fillDictionary(QStringList &dictionary, const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Dictionary file could not be opened:" << filePath;
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            dictionary.append(line.toUpper());
        }
    }
    file.close();
}



void InputHandler::gameStarted() {
    qsrand(static_cast<uint>(time(nullptr)));
    QStringList dictionary;
    fillDictionary(m_dictionary, ":/txt/wordlist.txt");
    if (m_dictionary.isEmpty()) {
        qWarning() << "Dictionary is empty or could not be read!";
        return;
    }
    int index = qrand() % m_dictionary.length();
    QString chosenWord = m_dictionary.at(index);
    setSolution(chosenWord);
    qDebug() << "Solution:" << chosenWord;
}

    // keyboard input function
void InputHandler::handleKeyPress(int key, QString text) {
    if (m_gameOver) {
        qDebug() << "Game over. Input ignored.";
        return;
    }

    if (key == Qt::Key_Backspace) {
        if (!inputText.isEmpty()) {
            inputText.chop(1);
            emit inputTextChanged(inputText);
        }
        qDebug() << "Backspace Pressed:" << inputText;
    } else if (key == Qt::Key_Enter || key == Qt::Key_Return) {
        if (inputText.length() == 5) {
            qDebug() << "Enter Pressed";
            submitQuess(inputText, m_solution);
        }
        else{
            emit showError("Not Enough Letter");
        }
    } else if (key >= Qt::Key_A && key <= Qt::Key_Z) {
        if (inputText.length() < 5) {
            inputText += text.toUpper();
            emit inputTextChanged(inputText);
        }
        qDebug() << "Letter Pressed:" << inputText;
    } else {
        qDebug() << "Ignored Key Pressed";
    }
}

void InputHandler::submitQuess(QString guess, QString solution) {
    if (!m_dictionary.contains(guess)) {
           qDebug() << "Invalid word guessed:" << guess;
           emit showError("Not In Word List");
           return;
       }

    if (guess == solution) {
        // If the answer is correct, color all the boxes green.
        QVariantList winningColors;
        for (int i = 0; i < 5; i++) {
            winningColors << "#6aaa64";
        }
        setCurrentGuessColors(winningColors);
        qDebug() << "You won!";
        QVariantMap guessEntry;
        guessEntry["text"] = guess;
        guessEntry["colors"] = m_currentGuessColors;
        m_guesses.append(guessEntry);
        emit guessesChanged();
        m_gameOver = true;
        setGameResult("win");
        emit gameOverChanged();
        inputText = "";
        emit inputTextChanged(inputText);
        return;
    } else {
        qDebug() << "Incorrect guess:" << guess;
        checkLetterExists(guess, solution);
    }

    QVariantMap guessEntry;
    guessEntry["text"] = guess;
    guessEntry["colors"] = m_currentGuessColors;
    m_guesses.append(guessEntry);
    emit guessesChanged();

    if (m_currentRow < 5) {
        m_currentRow++;
        emit currentRowChanged();
    } else {
        // All lines have been used, the right to guess is over. Game lost
        m_gameOver = true;
        setGameResult("lose");
        emit gameOverChanged();
    }

    inputText = "";
    emit inputTextChanged(inputText);

    QVariantList defaultColors = QVariantList()
        << "#272729" << "#272729" << "#272729" << "#272729" << "#272729";
    setCurrentGuessColors(defaultColors);
}

void InputHandler::checkLetterExists(QString guessedWord, QString solution) {
    if (guessedWord.length() < 5 || solution.length() < 5) {
        qWarning() << "guessedWord or solution less than 5 characters:" << guessedWord << ", " << solution;
        return;
    }

    QVariantList newColors;
    for (int i = 0; i < 5; i++) {
        QChar guessedLetter = guessedWord.at(i);
        if (solution.contains(guessedLetter)) {
            if (guessedLetter == solution.at(i)) {
                newColors << "#6aaa64";  // Green
                qDebug() << "Letter" << guessedLetter << "is in the correct position (green)";
            } else {
                newColors << "#c9b458";  // Yellow
                qDebug() << "Letter" << guessedLetter << "exists but in wrong position (yellow)";
            }
        } else {
            newColors << "#787c7e";      // Gray
            qDebug() << "Letter" << guessedLetter << "does not exist in the solution (gray)";
        }
    }
    setCurrentGuessColors(newColors);
}
