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

signals:
    void directionInput(int playerID, QPoint direction);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // GAMEVIEW_H
