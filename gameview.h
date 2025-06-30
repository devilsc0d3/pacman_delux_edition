// gameview.h
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>

class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameView(QGraphicsScene *scene, QWidget *parent = nullptr);
    void setPlayerId(int id);

signals:
    void directionInput(int playerID, QPoint direction);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    int m_playerId = 0;
};

#endif // GAMEVIEW_H
