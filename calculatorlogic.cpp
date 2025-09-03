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
        m_secretInput.clear();
        m_secretCodeTimer.start(5000);
        qDebug() << "Secret mode activated! Enter code within 5 seconds.";
    });

    connect(&m_secretCodeTimer, &QTimer::timeout, this, [this]() {
        if (m_secretModeActive) {
            m_secretModeActive = false;
            m_secretInput.clear();
            qDebug() << "Secret mode deactivated. Timeout.";
        }
    });

    clearAll();
}

void CalculatorLogic::inputDigit(const QString &digit)
{
    if (m_secretModeActive) { checkSecretCode(digit); return; }

    if (m_repeatEquals) {
        m_expression = "";
        m_repeatEquals = false;
    }

    m_expression += digit;
    emit expressionChanged();
}

void CalculatorLogic::inputDot()
{
    if (m_repeatEquals) { m_expression.clear(); m_repeatEquals = false; }
    m_expression += ".";
    emit expressionChanged();
}

void CalculatorLogic::pressOperator(const QString &op)
{
    if (m_repeatEquals) { m_expression = m_result; m_repeatEquals = false; }

    m_expression += op;
    emit expressionChanged();
}

void CalculatorLogic::inputParenthesis(const QString &par)
{
    if (m_repeatEquals) { m_expression.clear(); m_repeatEquals = false; }

    m_expression += par;
    emit expressionChanged();
}

void CalculatorLogic::toggleSign()
{
    if (m_repeatEquals) { m_expression = m_result; m_repeatEquals = false; }

    m_expression = "-(" + m_expression + ")";
    emit expressionChanged();
}

void CalculatorLogic::pressPercent()
{
    QJSEngine engine;

    if (m_repeatEquals) { m_expression = m_result; m_repeatEquals = false; }

    // Ищем последний оператор
    QStringList ops = {"+", "-", "×", "÷"};
    int lastOpIndex = -1;
    QString lastOp;
    for (const QString &op : ops) {
        int idx = m_expression.lastIndexOf(op);
        if (idx > lastOpIndex) {
            lastOpIndex = idx;
            lastOp = op;
        }
    }

    if (lastOpIndex >= 0) {
        QString left = m_expression.left(lastOpIndex);
        QString right = m_expression.mid(lastOpIndex + 1);
        double leftVal = engine.evaluate(left.replace("×", "*").replace("÷", "/")).toNumber();
        double rightVal = engine.evaluate(right.replace("×", "*").replace("÷", "/")).toNumber();
        double percent = leftVal * rightVal / 100.0;
        m_expression = left + lastOp + QString::number(percent, 'g', 15);
    } else {
        double val = engine.evaluate(m_expression.replace("×", "*").replace("÷", "/")).toNumber();
        val /= 100.0;
        m_expression = QString::number(val, 'g', 15);
    }

    emit expressionChanged();
}

void CalculatorLogic::pressEquals()
{
    evaluateExpression();
    m_repeatEquals = true;
}

void CalculatorLogic::evaluateExpression()
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
    m_repeatEquals = false;
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
        m_secretInput.clear();
        qDebug() << "Secret code incorrect. Resetting.";
    }
}
