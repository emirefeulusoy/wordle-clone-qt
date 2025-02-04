#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QFile>
#include <QStringList>

class InputHandler : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString inputText READ getInputText NOTIFY inputTextChanged)
    Q_PROPERTY(int currentRow READ getCurrentRow NOTIFY currentRowChanged)
    Q_PROPERTY(QVariantList currentGuessColors READ getCurrentGuessColors NOTIFY currentGuessColorsChanged)
    Q_PROPERTY(QVariantList guesses READ getGuesses NOTIFY guessesChanged)
    Q_PROPERTY(bool gameOver READ gameOver NOTIFY gameOverChanged)
    Q_PROPERTY(QString gameResult READ getGameResult NOTIFY gameResultChanged)
    Q_PROPERTY(QString solution READ getSolution NOTIFY solutionChanged)

public:
    explicit InputHandler(QObject *parent = nullptr);
    Q_INVOKABLE void handleKeyPress(int key, QString text);
    QString getInputText() const { return inputText; }
    void submitQuess(QString guess, QString solution);
    void checkLetterExists(QString guessedWord, QString solution);
    int getCurrentRow() const { return m_currentRow; }
    QVariantList getCurrentGuessColors() const { return m_currentGuessColors; }
    void setCurrentGuessColors(const QVariantList &colors) {
        if (m_currentGuessColors != colors) {
            m_currentGuessColors = colors;
            emit currentGuessColorsChanged();
        }
    }
    QVariantList getGuesses() const { return m_guesses; }
    QString getSolution() const { return m_solution; }
    void setSolution(const QString &solution) {
        if(m_solution != solution) {
            m_solution = solution;
            emit solutionChanged();
        }
    }
    bool gameOver() const { return m_gameOver; }
    QString getGameResult() const { return m_gameResult; }
    void setGameResult(const QString &result) {
        if (m_gameResult != result) {
            m_gameResult = result;
            emit gameResultChanged();
        }
    }
   void gameStarted();
   void fillDictionary(QStringList &dictionary, const QString &file);
   Q_INVOKABLE void showError(const QString &message) {
         emit errorMessageChanged(message);
     }



signals:
    void inputTextChanged(QString newText);
    void currentRowChanged();
    void currentGuessColorsChanged();
    void guessesChanged();
    void gameOverChanged();
    void gameResultChanged();
    void solutionChanged();
    void errorMessageChanged(const QString &message);


public slots:

private:
    QString inputText;
    QString m_solution;
    int m_currentRow = 0;
    QVariantList m_currentGuessColors; // Color information in the active row ( initially #272729 )
    QVariantList m_guesses;
    bool m_gameOver = false;
    QString m_gameResult = "";
    QStringList m_dictionary;
};

#endif // INPUTHANDLER_H
