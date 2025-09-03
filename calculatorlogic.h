#ifndef CALCULATORLOGIC_H
#define CALCULATORLOGIC_H

#include <QObject>
#include <QString>
#include <QTimer>

class CalculatorLogic : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString expression READ expression NOTIFY expressionChanged)
    Q_PROPERTY(QString result READ result NOTIFY resultChanged)

public:
    explicit CalculatorLogic(QObject* parent = nullptr);

    // Свойства
    QString expression() const { return m_expression; }
    QString result() const { return m_result; }

    // Методы для QML
    Q_INVOKABLE void inputDigit(const QString &d);
    Q_INVOKABLE void inputPoint();
    Q_INVOKABLE void pressOperator(const QString &op); // "+", "-", "*", "/"
    Q_INVOKABLE void pressEquals();
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE void backspace();
    Q_INVOKABLE void toggleSign();
    Q_INVOKABLE void pressPercent();

    Q_INVOKABLE void startLongPress();
    Q_INVOKABLE void endLongPress();
    Q_INVOKABLE void checkSecretCode(const QString &digit);

signals:
    void expressionChanged();
    void resultChanged();
    void showSecretMenu();

private:
    QString m_expression;      // что показываем в верхней строке
    QString m_result;          // что показываем в нижней строке

    QString m_left;            // левый операнд
    QString m_right;           // правый операнд
    QString m_op;              // оператор
    bool m_editingRight = false;

    void updateExpressionFromState();
    void updateResultFromState();
    QString computeCurrentOperation();

    QString addDecimalStrings(const QString &a, const QString &b);
    QString subDecimalStrings(const QString &a, const QString &b);
    int compareDecimalStrings(const QString &a, const QString &b);
    QString normalizeStrip(const QString &s);

    QTimer m_longPressTimer;
    QTimer m_secretCodeTimer;
    bool m_secretModeActive;
    QString m_secretInput;
};

#endif // CALCULATORLOGIC_H
