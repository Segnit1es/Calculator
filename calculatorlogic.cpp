#include "calculatorlogic.h"
#include <QDebug>
#include <QJSEngine>

CalculatorLogic::CalculatorLogic(QObject *parent)
    : QObject(parent)
{
    m_longPressTimer.setSingleShot(true);
    m_secretCodeTimer.setSingleShot(true);

    connect(&m_longPressTimer, &QTimer::timeout, this, [this]() {
        m_secretModeActive = true;
        m_secretInput = "";
        m_secretCodeTimer.start(5000);
        qDebug() << "Secret mode activated! Enter code within 5 seconds.";
    });

    connect(&m_secretCodeTimer, &QTimer::timeout, this, [this]() {
        if (m_secretModeActive) {
            m_secretModeActive = false;
            m_secretInput = "";
            qDebug() << "Secret mode deactivated. Timeout.";
        }
    });

    clearAll();
}

void CalculatorLogic::inputDigit(const QString &digit)
{
    if (m_secretModeActive) { checkSecretCode(digit); return; }
    m_expression += digit;
    emit expressionChanged();
}

void CalculatorLogic::inputDot()
{
    m_expression += ".";
    emit expressionChanged();
}

void CalculatorLogic::pressOperator(const QString &op)
{
    m_expression += op;
    emit expressionChanged();
}

void CalculatorLogic::inputParenthesis(const QString &par)
{
    m_expression += par;
    emit expressionChanged();
}

void CalculatorLogic::pressPercent()
{
    m_expression += "%";
    emit expressionChanged();
}

void CalculatorLogic::toggleSign()
{
    m_expression = "-(" + m_expression + ")";
    emit expressionChanged();
}

void CalculatorLogic::pressEquals()
{
    QJSEngine engine;
    QString expr = m_expression;
    expr.replace("×", "*").replace("÷", "/").replace("%", "/100");
    QJSValue result = engine.evaluate(expr);

    if (result.isError())
        m_result = "Error";
    else
        m_result = result.toString();

    emit resultChanged();
}

void CalculatorLogic::clearAll()
{
    m_expression.clear();
    m_result = "0";
    emit expressionChanged();
    emit resultChanged();
}

void CalculatorLogic::startLongPress()
{
    m_longPressTimer.start(4000);
}

void CalculatorLogic::endLongPress()
{
    if (m_longPressTimer.isActive()) {
        m_longPressTimer.stop();
        pressEquals();
    }
}

void CalculatorLogic::checkSecretCode(const QString &digit)
{
    if (!m_secretModeActive) return;

    m_secretInput += digit;
    qDebug() << "Secret input:" << m_secretInput;

    if (m_secretInput == "123") {
        m_secretModeActive = false;
        m_secretCodeTimer.stop();
        emit showSecretMenu();
        qDebug() << "Secret code correct! Showing secret menu.";
    } else if (m_secretInput.length() >= 3) {
        m_secretModeActive = false;
        m_secretCodeTimer.stop();
        m_secretInput = "";
        qDebug() << "Secret code incorrect. Resetting.";
    }
}
