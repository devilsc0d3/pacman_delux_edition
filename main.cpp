// #include "mainwindow.h"

// #include <QApplication>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }
#include <QApplication>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QUrl>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("PacMan Neon");

    // 🎵 Création d’un lecteur de musique
    QMediaPlayer *player = new QMediaPlayer(&window);

    // (Facultatif) Lire en boucle :
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl::fromLocalFile("pacman_theme.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);

    player->setVolume(50);
    player->play();

    window.resize(800, 600);
    window.show();

    return app.exec();
}

