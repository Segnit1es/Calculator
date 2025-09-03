#include "calculatorlogic.h"
#include <QDebug>

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
    if (m_secretModeActive) {
        checkSecretCode(digit);
        return;
    }
    if (m_op.isEmpty()) {
        m_left += digit;
        m_result = m_left;
    } else {
        m_right += digit;
        m_result = m_right;
    }

    updateExpression();
    emit resultChanged();
}

void CalculatorLogic::inputDot()
{
    QString &target = m_op.isEmpty() ? m_left : m_right;
    if (!target.contains('.'))
        target += '.';
    m_result = target;
    updateExpression();
    emit resultChanged();
}

void CalculatorLogic::pressOperator(const QString &op)
{
    if (!m_left.isEmpty()) {
        if (!m_right.isEmpty()) {
            pressEquals();
        }
        m_op = op;
    }
    updateExpression();
}

void CalculatorLogic::pressEquals()
{
    if (!m_left.isEmpty() && !m_right.isEmpty() && !m_op.isEmpty()) {
        double a = m_left.toDouble();
        double b = m_right.toDouble();
        double res = calculate(a, b, m_op);

        m_expression = m_left + " " + m_op + " " + m_right;
        emit expressionChanged();

        m_result = QString::number(res, 'g', 15);
        emit resultChanged();

        m_left = QString::number(res, 'g', 15);
        m_right.clear();
        m_op.clear();
    }
}

void CalculatorLogic::pressPercent()
{
    if (!m_left.isEmpty()) {
        if (!m_right.isEmpty() && !m_op.isEmpty()) {
            // классическое поведение: "200 + 10%" = 220
            double a = m_left.toDouble();
            double b = m_right.toDouble();
            double percent = a * b / 100.0;
            m_right = QString::number(percent, 'g', 15);
            m_result = m_right;
        } else {
            // если введено только число: "50 %" = 0.5
            double a = m_left.toDouble();
            a = a / 100.0;
            m_left = QString::number(a, 'g', 15);
            m_result = m_left;
        }
        updateExpression();
        updateResult();
    }
}

void CalculatorLogic::toggleSign()
{
    QString &target = m_op.isEmpty() ? m_left : m_right;
    if (!target.isEmpty() && target != "0") {
        if (target.startsWith('-'))
            target.remove(0, 1);
        else
            target.prepend('-');
        m_result = target;
        updateExpression();
        updateResult();
    }
}

void CalculatorLogic::clearAll()
{
    m_left.clear();
    m_right.clear();
    m_op.clear();
    m_expression.clear();
    m_result = "0";
    emit expressionChanged();
    emit resultChanged();
}

void CalculatorLogic::updateExpression()
{
    if (m_op.isEmpty()) {
        m_expression = m_left;
    } else {
        m_expression = m_left + " " + m_op + " " + m_right;
    }
    emit expressionChanged();
}

void CalculatorLogic::updateResult()
{
    emit resultChanged();
}

double CalculatorLogic::calculate(double a, double b, const QString &op) const
{
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : 0;
    return 0;
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
