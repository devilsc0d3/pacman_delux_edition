#ifndef BONUS_H
#define BONUS_H

#include "pacGum.h"
#include <QTimer>

class Bonus : public PacGum
{
    Q_OBJECT

public:
    explicit Bonus(int value = 50, int durationMs = 8000, QObject* parent = nullptr);

    void consume() override;
    bool isActive() const;

signals:
    void activated();
    void expired();

private slots:
    void onTimeout();

private:
    QTimer m_timer;
    bool m_active;
};

#endif // BONUS_H
