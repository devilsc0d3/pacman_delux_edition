#include <QApplication>

// --- Includes combinés ---
// Pour la musique
#include <QMediaPlayer>
#include <QAudioOutput>
// Pour le serveur
#include "lobbyserver.h"
// Pour la nouvelle fenêtre de démarrage
#include "startwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // --- Fonctionnalité de la branche feat/menu : Musique de fond ---
    QMediaPlayer *player = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/music.mp3"));
    player->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(0.5); // Volume à 50%
    player->play();

    // --- Fonctionnalité de la branche dev-merge : Serveur de lobby ---
    // Le serveur est démarré en arrière-plan au lancement de l'application
    LobbyServer server(1234);

    // --- Point d'entrée logique de l'application ---
    // On affiche la nouvelle fenêtre de menu (StartWindow) au lieu de l'ancienne MainWindow
    StartWindow start;
    start.show();

    // L'ancienne logique qui affichait MainWindow et la fenêtre de carte
    // est maintenant obsolète, car gérée par les menus.

    return app.exec();
}