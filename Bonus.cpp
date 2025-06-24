#include "Bonus.h"

Bonus::Bonus(int value, int durationMs, QObject* parent)
    : PacGum(value, parent), m_active(false)
{
    m_timer.setSingleShot(true);
    m_timer.setInterval(durationMs);
    connect(&m_timer, &QTimer::timeout, this, &Bonus::onTimeout);
}

void Bonus::consume()
{
    if (!m_consumed) {
        m_consumed = true;
        m_active = true;
        emit consumed(m_value);
        emit activated();
        m_timer.start();
    }
}

bool Bonus::isActive() const
{
    return m_active;
}

void Bonus::onTimeout()
{
    m_active = false;
    emit expired();
}
