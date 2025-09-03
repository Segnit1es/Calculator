#ifndef CALCULATORLOGIC_H
#define CALCULATORLOGIC_H

#include <QObject>
#include <QString>
#include <QTimer>

class CalculatorLogic : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString expression READ expression NOTIFY expressionChanged)
    Q_PROPERTY(QString result READ result NOTIFY resultChanged)

public:
    explicit CalculatorLogic(QObject *parent = nullptr);

    QString expression() const { return m_expression; }
    QString result() const { return m_result; }

    Q_INVOKABLE void inputDigit(const QString &digit);
    Q_INVOKABLE void inputDot();
    Q_INVOKABLE void pressOperator(const QString &op);
    Q_INVOKABLE void pressEquals();
    Q_INVOKABLE void pressPercent();
    Q_INVOKABLE void toggleSign();
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE void inputParenthesis(const QString &par);

    Q_INVOKABLE void startLongPress();
    Q_INVOKABLE void endLongPress();
    Q_INVOKABLE void checkSecretCode(const QString &digit);

signals:
    void expressionChanged();
    void resultChanged();
    void showSecretMenu();

private:
    QString m_expression;
    QString m_result;
    bool m_secretModeActive = false;
    QString m_secretInput;
    QTimer m_secretCodeTimer;
    QTimer m_longPressTimer;

    bool m_repeatEquals = false;
    void evaluateExpression();
};

#endif // CALCULATORLOGIC_H
