 #ifndef PACGUM_H
#define PACGUM_H

#include <QObject>

class PacGum : public QObject
{
    Q_OBJECT

public:
    explicit PacGum(int value = 10, QObject* parent = nullptr);

    int points() const;
    virtual void consume();

signals:
    void consumed(int points);

protected:
    int m_value;
    bool m_consumed;
};

#endif // PACGUM_H
