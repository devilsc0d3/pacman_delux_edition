#include "pacGum.h"

PacGum::PacGum(int value, QObject* parent)
    : QObject(parent), m_value(value), m_consumed(false)
{
}

int PacGum::points() const
{
    return m_value;
}

void PacGum::consume()
{
    if (!m_consumed) {
        m_consumed = true;
        emit consumed(m_value);
    }
}
