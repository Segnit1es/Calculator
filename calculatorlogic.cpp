#include "calculatorlogic.h"
#include <QDebug>
#include <algorithm>

static void splitParts(const QString &s, QString &intPart, QString &fracPart) {
    int dot = s.indexOf('.');
    if (dot >= 0) {
        intPart = s.left(dot);
        fracPart = s.mid(dot + 1);
    } else {
        intPart = s;
        fracPart = "";
    }
    if (intPart.isEmpty()) intPart = "0";
}

CalculatorLogic::CalculatorLogic(QObject* parent)
    : QObject(parent),
    m_expression(""),
    m_result("0"),
    m_left(""),
    m_right(""),
    m_op(""),
    m_editingRight(false),
    m_secretModeActive(false),
    m_secretInput("")
{
    m_longPressTimer.setSingleShot(true);
    m_secretCodeTimer.setSingleShot(true);

    connect(&m_longPressTimer, &QTimer::timeout, this, [this]() {
        m_secretModeActive = true;
        m_secretInput = "";
        m_secretCodeTimer.start(5000); // 5 секунд на ввод кода
        qDebug() << "Secret mode activated! Enter code within 5 seconds.";
    });

    connect(&m_secretCodeTimer, &QTimer::timeout, this, [this]() {
        if (m_secretModeActive) {
            m_secretModeActive = false;
            m_secretInput = "";
            qDebug() << "Secret mode deactivated. Timeout.";
        }
    });
}

void CalculatorLogic::inputDigit(const QString &d) {

    if (m_secretModeActive) {
        checkSecretCode(d);
        return;
    }
    if (d.isEmpty()) return;
    QChar ch = d[0];
    if (!ch.isDigit()) return;

    QString &target = m_editingRight ? m_right : m_left;

    if (target == "0") target = QString(ch);
    else target += ch;

    QString digitsOnly = target;
    digitsOnly.remove('-').remove('.');
    if (digitsOnly.length() > 25) {
        target.chop(1);
    }


    updateExpressionFromState();
    updateResultFromState();
}

void CalculatorLogic::inputPoint() {
    QString &target = m_editingRight ? m_right : m_left;
    if (!target.contains('.')) {
        if (target.isEmpty()) target = "0.";
        else target += '.';
    }
    updateExpressionFromState();
    updateResultFromState();
}

void CalculatorLogic::pressOperator(const QString &op) {
    if (op.isEmpty()) return;
    if (!(op == "+" || op == "-" || op == "*" || op == "/")) return;
    if (!m_op.isEmpty() && !m_right.isEmpty()) {
        QString computed = computeCurrentOperation();
        m_left = computed;
        m_right.clear();
        m_editingRight = true;
        m_op = op;
        updateExpressionFromState();
        updateResultFromState();
        return;
    }

    if (m_left.isEmpty()) {
        if (!m_result.isEmpty() && m_result != "Err") {
            m_left = m_result;
        } else {
            m_left = "0";
        }
    }

    m_op = op;
    m_editingRight = true;
    m_right.clear();

    updateExpressionFromState();
    updateResultFromState();
}

void CalculatorLogic::pressEquals() {
    if (m_op.isEmpty() || m_left.isEmpty()) {
        updateExpressionFromState();
        updateResultFromState();
        return;
    }

    if (m_right.isEmpty()) {
        m_right = m_left;
    }

    QString computed = computeCurrentOperation();
    m_expression = normalizeStrip(m_left) + " " + m_op + " " + normalizeStrip(m_right);
    emit expressionChanged();

    m_result = computed;
    emit resultChanged();

    m_left = (m_result == "Err") ? QString() : m_result;
    m_right.clear();
    m_op.clear();
    m_editingRight = false;
}

void CalculatorLogic::clearAll() {
    m_expression.clear();
    m_result = "0";
    m_left.clear();
    m_right.clear();
    m_op.clear();
    m_editingRight = false;
    emit expressionChanged();
    emit resultChanged();
}

void CalculatorLogic::backspace() {
    QString &target = m_editingRight ? m_right : m_left;
    if (!target.isEmpty()) {
        target.chop(1);
        if (target.isEmpty()) target = "0";
    } else {
    }
    updateExpressionFromState();
    updateResultFromState();
}

void CalculatorLogic::updateExpressionFromState() {
    if (m_editingRight) {
        if (!m_op.isEmpty()) {
            m_expression = normalizeStrip(m_left) + " " + m_op + " " + (m_right.isEmpty() ? "" : normalizeStrip(m_right));
        } else {
            m_expression = normalizeStrip(m_left);
        }
    } else {
        m_expression = normalizeStrip(m_left);
    }
    emit expressionChanged();
}

void CalculatorLogic::updateResultFromState() {
    QString display;
    if (m_editingRight) {
        display = m_right.isEmpty() ? "0" : normalizeStrip(m_right);
    } else {
        display = m_left.isEmpty() ? m_result : normalizeStrip(m_left);
    }
    if (display != m_result) {
        m_result = display;
        emit resultChanged();
    } else {
        if (m_result.isEmpty()) {
            m_result = "0";
            emit resultChanged();
        }
    }
}

QString CalculatorLogic::computeCurrentOperation() {
    if (m_left.isEmpty() || m_right.isEmpty() || m_op.isEmpty())
        return QString("0");

    QString result;
    if (m_op == "+") {
        result = addDecimalStrings(m_left, m_right);
    } else if (m_op == "-") {
        int cmp = compareDecimalStrings(m_left, m_right);
        if (cmp >= 0) result = subDecimalStrings(m_left, m_right);
        else result = "-" + subDecimalStrings(m_right, m_left);
    } else if (m_op == "*") {
        QString a = m_left; QString b = m_right;
        bool an = a.startsWith('-'); bool bn = b.startsWith('-');
        if (an) a.remove(0,1);
        if (bn) b.remove(0,1);
        QString ai, af, bi, bf;
        splitParts(a, ai, af);
        splitParts(b, bi, bf);
        int frac = af.length() + bf.length();
        QString A = ai + af;
        QString B = bi + bf;
        while (A.length() > 1 && A.startsWith('0')) A.remove(0,1);
        while (B.length() > 1 && B.startsWith('0')) B.remove(0,1);
        int n = A.length(), m = B.length();
        QVector<int> prod(n+m, 0);
        for (int i = n-1; i>=0; --i) {
            int da = A[i].digitValue();
            for (int j = m-1; j>=0; --j) {
                int db = B[j].digitValue();
                prod[i+j+1] += da * db;
            }
        }
        for (int k = n+m-1; k>0; --k) {
            int carry = prod[k]/10;
            prod[k] %= 10;
            prod[k-1] += carry;
        }
        QString R;
        for (int k = 0; k < prod.size(); ++k) R.append(QChar('0' + prod[k]));
        while (R.length() > 1 && R[0] == '0') R.remove(0,1);
        if (frac > 0) {
            int pos = R.length() - frac;
            if (pos <= 0) {
                while (pos <= 0) { R.prepend('0'); pos++; }
            }
            R.insert(pos, '.');
        }
        if (an ^ bn && R != "0") R.prepend('-');
        result = normalizeStrip(R);
    } else if (m_op == "/") {
        QString a = m_left; QString b = m_right;
        bool an = a.startsWith('-'); bool bn = b.startsWith('-');
        if (an) a.remove(0,1);
        if (bn) b.remove(0,1);
        if (b == "0" || b == "0.0" || b.isEmpty()) return QString("Err");
        QString ai, af, bi, bf;
        splitParts(a, ai, af);
        splitParts(b, bi, bf);
        int scale = qMax(af.length(), bf.length());
        while (af.length() < scale) af += '0';
        while (bf.length() < scale) bf += '0';
        QString A = ai + af;
        QString B = bi + bf;
        while (A.length() > 1 && A.startsWith('0')) A.remove(0,1);
        while (B.length() > 1 && B.startsWith('0')) B.remove(0,1);
        QString quotient;
        QString remainder = "";
        int idx = 0;
        QString dividend = A;
        QString cur = "";
        while (idx < dividend.length()) {
            cur.append(dividend[idx++]);
            while (cur.length() > 1 && cur.startsWith('0')) cur.remove(0,1);
            int q = 0;
            while (compareDecimalStrings(cur, B) >= 0) {
                cur = subDecimalStrings(cur, B);
                q++;
            }
            quotient.append(QChar('0' + q));
            if (cur == "0") cur = "";
        }
        if (quotient.isEmpty()) quotient = "0";
        QString fracPart;
        QString intDigits = quotient;
        intDigits.remove('-').remove('.');
        int intLen = intDigits.length();
        int maxFrac = qMax(0, 25 - intLen);
        remainder = cur.isEmpty() ? "0" : cur;
        for (int i = 0; i < maxFrac; ++i) {
            if (remainder == "0") remainder = "0";
            remainder += "0";
            int q = 0;
            while (compareDecimalStrings(remainder, B) >= 0) {
                remainder = subDecimalStrings(remainder, B);
                q++;
            }
            fracPart.append(QChar('0' + q));
        }
        QString R = quotient;
        if (!fracPart.isEmpty()) {
            R += "." + fracPart;
        }
        if (an ^ bn && R != "0") R.prepend('-');
        result = normalizeStrip(R);
    } else {
        result = QString("0");
    }
    QString digitsOnly = result;
    digitsOnly.remove('-').remove('.');
    if (digitsOnly.length() > 25) {
        int dot = result.indexOf('.');
        if (dot >= 0) {
            int keep = 25 - (result.startsWith('-') ? 0 : 0) - dot;
            if (keep < 0) {
                result = "Err";
            } else {
                result = result.left(dot + 1 + keep);
                result = normalizeStrip(result);
            }
        } else {
            result = "Err";
        }
    }

    return result;
}

QString CalculatorLogic::normalizeStrip(const QString &s) {
    QString a = s;
    bool neg = a.startsWith('-');
    if (neg) a.remove(0,1);
    QString intP, fracP;
    splitParts(a, intP, fracP);
    while (intP.size() > 1 && intP.startsWith('0')) intP.remove(0,1);
    while (!fracP.isEmpty() && fracP.endsWith('0')) fracP.chop(1);
    QString res = intP;
    if (!fracP.isEmpty()) res += "." + fracP;
    if (res.isEmpty()) res = "0";
    if (neg && res != "0") res = "-" + res;
    return res;
}

int CalculatorLogic::compareDecimalStrings(const QString &aOrig, const QString &bOrig) {
    QString a = aOrig; QString b = bOrig;
    bool an = a.startsWith('-'); bool bn = b.startsWith('-');
    if (an) a.remove(0,1);
    if (bn) b.remove(0,1);
    QString ai, af, bi, bf;
    splitParts(a, ai, af);
    splitParts(b, bi, bf);
    if (ai.length() != bi.length()) {
        int cmp = (ai.length() > bi.length()) ? 1 : -1;
        if (an ^ bn) return an ? -cmp : cmp;
        return an ? -cmp : cmp;
    }
    int cmp = ai.compare(bi, Qt::CaseInsensitive);
    if (cmp != 0) {
        if (an ^ bn) return an ? -1 : 1;
        return an ? - (cmp>0?1:-1) : (cmp>0?1:-1);
    }
    int maxf = qMax(af.length(), bf.length());
    for (int i=0;i<maxf;i++) {
        QChar ac = (i < af.length()) ? af[i] : '0';
        QChar bc = (i < bf.length()) ? bf[i] : '0';
        if (ac != bc) {
            int rc = (ac > bc) ? 1 : -1;
            if (an ^ bn) return an ? -rc : rc;
            return an ? -rc : rc;
        }
    }
    return 0;
}

QString CalculatorLogic::addDecimalStrings(const QString &aOrig, const QString &bOrig) {
    QString a = aOrig; QString b = bOrig;
    bool an = a.startsWith('-'); bool bn = b.startsWith('-');
    if (an) a.remove(0,1);
    if (bn) b.remove(0,1);
    QString ai, af, bi, bf;
    splitParts(a, ai, af);
    splitParts(b, bi, bf);
    int maxFrac = qMax(af.length(), bf.length());
    while (af.length() < maxFrac) af += '0';
    while (bf.length() < maxFrac) bf += '0';
    QString A = ai + af;
    QString B = bi + bf;
    int carry = 0;
    QString R;
    int ia = A.length()-1, ib = B.length()-1;
    while (ia>=0 || ib>=0 || carry) {
        int da = (ia>=0)?(A[ia].digitValue()):0;
        int db = (ib>=0)?(B[ib].digitValue()):0;
        int s = da + db + carry;
        carry = s/10;
        R.prepend(QChar('0' + (s%10)));
        ia--; ib--;
    }
    if (maxFrac > 0) {
        int pos = R.length() - maxFrac;
        if (pos <= 0) {
            while (pos <= 0) { R.prepend('0'); pos++; }
        }
        R.insert(pos, '.');
    }
    return normalizeStrip(R);
}

QString CalculatorLogic::subDecimalStrings(const QString &aOrig, const QString &bOrig) {
    QString a = aOrig; QString b = bOrig;
    QString ai, af, bi, bf;
    splitParts(a, ai, af);
    splitParts(b, bi, bf);
    int maxFrac = qMax(af.length(), bf.length());
    while (af.length() < maxFrac) af += '0';
    while (bf.length() < maxFrac) bf += '0';
    QString A = ai + af;
    QString B = bi + bf;
    while (A.length() > B.length()) B.prepend('0');
    while (B.length() > A.length()) A.prepend('0');

    int borrow = 0;
    QString R;
    for (int i=A.length()-1;i>=0;i--) {
        int da = A[i].digitValue() - borrow;
        int db = B[i].digitValue();
        if (da < db) {
            da += 10;
            borrow = 1;
        } else borrow = 0;
        int d = da - db;
        R.prepend(QChar('0' + d));
    }
    if (maxFrac > 0) {
        int pos = R.length() - maxFrac;
        R.insert(pos, '.');
    }
    return normalizeStrip(R);
}

void CalculatorLogic::toggleSign() {
    if (m_editingRight) {
        if (m_right.startsWith("-"))
            m_right.remove(0, 1);
        else if (!m_right.isEmpty() && m_right != "0")
            m_right.prepend("-");
        m_result = m_right;
    } else {
        if (m_left.startsWith("-"))
            m_left.remove(0, 1);
        else if (!m_left.isEmpty() && m_left != "0")
            m_left.prepend("-");
        m_result = m_left;
    }
    emit resultChanged();
}

void CalculatorLogic::pressPercent()
{

    if (m_left.isEmpty() && m_right.isEmpty()) return;


    if (!m_op.isEmpty() && !m_left.isEmpty() && !m_right.isEmpty()) {

        QString origLeft = m_left;
        QString origRight = m_right;
        QString origOp = m_op;
        m_op = "*";
        QString prod = computeCurrentOperation();
        m_left = prod;
        m_right = "100";
        m_op = "/";
        QString percent = computeCurrentOperation();
        m_left = origLeft;
        m_right = percent;
        m_op = origOp;
        updateExpressionFromState();
        updateResultFromState();
        return;
    }

    QString &target = m_editingRight ? m_right : m_left;
    if (target.isEmpty()) return;
    QString savedLeft = m_left;
    QString savedRight = m_right;
    QString savedOp = m_op;
    m_left = target;
    m_right = "100";
    m_op = "/";
    QString divided = computeCurrentOperation();
    if (m_editingRight) {
        m_left = savedLeft;
        m_right = divided;
        m_op = savedOp;
    } else {
        m_left = divided;
        m_right = savedRight;
        m_op = savedOp;
    }

    updateExpressionFromState();
    updateResultFromState();
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

