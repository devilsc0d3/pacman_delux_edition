#include "mainwindow.h"

#include <QApplication>
#include <QMediaPlayer>
#include <QAudioOutput>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5); // 50% de volume

    player->setSource(QUrl("qrc:/music.mp3"));
    player->setLoops(QMediaPlayer::Infinite); // Lecture en boucle

    player->play();
    MainWindow w;
    w.show();
    return a.exec();
}
