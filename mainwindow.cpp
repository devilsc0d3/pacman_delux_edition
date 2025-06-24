#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Includes combinés des deux branches
#include <QMessageBox>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include "rulesdialog.h" // Ajout de la branche feat/map


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Code ajouté par la branche feat/Server
    connect(ui->buttonCreate, &QPushButton::clicked, this, &MainWindow::onCreateLobby);
    connect(ui->buttonJoin, &QPushButton::clicked, this, &MainWindow::onJoinLobby);
    connect(ui->buttonLeave, &QPushButton::clicked, this, &MainWindow::onLeaveLobby);
    connect(ui->buttonRefreshLobbies, &QPushButton::clicked, this, &MainWindow::onRefreshLobbies);

    connect(ui->listWidgetLobbies, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem* item){
        ui->lineEditLobbyId->setText(item->text());
        qDebug() << "Lobby sélectionné depuis la liste :" << item->text();
    });


    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::onRefreshLobbies);
    refreshTimer->start(10000); // 10s

    qDebug() << "UI initialisée.";
}

MainWindow::~MainWindow()
{
    if (client) {
        qDebug() << "Fermeture : envoi de leave au serveur.";
        client->sendLeave();
        delete client;
    }
    delete ui;
    qDebug() << "MainWindow détruit.";
}

// ---- Fonctions ajoutées par la branche feat/Server ----

void MainWindow::startClient()
{
    if (client) {
        delete client;
        qDebug() << "Ancien client détruit.";
    }

    client = new LobbyClient(QUrl(QStringLiteral("ws://localhost:1234")), this);

    connect(client, &LobbyClient::connectionSuccess, this, &MainWindow::onConnectionSuccess);
    connect(client, &LobbyClient::connectionFailed, this, &MainWindow::onConnectionFailed);
    connect(client, &LobbyClient::lobbyUpdated, this, &MainWindow::onLobbyUpdated);
    connect(client, &LobbyClient::lobbyListReceived, this, &MainWindow::onLobbyListReceived);

    qDebug() << "Client WebSocket initialisé.";
}

void MainWindow::onCreateLobby()
{
    QString name = ui->lineEditName->text();
    QString lobbyId = ui->lineEditLobbyId->text();

    if (name.isEmpty() || lobbyId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez un pseudo ET un ID de lobby !");
        return;
    }

    qDebug() << "Création du lobby" << lobbyId << "par" << name;
    startClient();
    client->sendCreate(lobbyId, name);
}

void MainWindow::onJoinLobby()
{
    QString name = ui->lineEditName->text();
    QString lobbyId = ui->lineEditLobbyId->text();

    if (name.isEmpty() || lobbyId.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez un pseudo ET un ID de lobby !");
        return;
    }

    qDebug() << "Rejoindre le lobby" << lobbyId << "avec pseudo" << name;
    startClient();
    client->sendJoin(lobbyId, name);
}

void MainWindow::onLeaveLobby()
{
    if (client) {
        qDebug() << "Quitter le lobby actuel.";
        client->sendLeave();
        ui->listWidgetLobby->clear();
    }
}

void MainWindow::onRefreshLobbies()
{
    // Note : le code original avait un `if (client)` qui empêchait de rafraîchir
    // avant une première connexion. On l'adapte pour pouvoir se connecter et rafraîchir.
    if (!client || client->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Client non connecté, tentative de connexion pour rafraîchir.";
        startClient();
    }
    client->requestLobbyList();
}

void MainWindow::onConnectionSuccess()
{
    qDebug() << "Connexion WebSocket réussie.";
    // On peut demander la liste des lobbies dès la connexion réussie.
    client->requestLobbyList();
}

void MainWindow::onConnectionFailed(QString reason)
{
    qDebug() << "Erreur de connexion : " << reason;
    QMessageBox::critical(this, "Erreur de connexion", reason);

    if (client) {
        delete client;
        client = nullptr;
    }
}

void MainWindow::onLobbyUpdated(QString lobbyId, QStringList players)
{
    ui->listWidgetLobby->clear();
    for (const QString &p : players) {
        ui->listWidgetLobby->addItem(p);
        qDebug() << " - joueur : " << p;
    }

    ui->lineEditLobbyId->setText(lobbyId);
    qDebug() << "Lobby mis à jour : " << lobbyId << "avec" << players.size() << "joueurs.";
}

void MainWindow::onLobbyListReceived(QStringList lobbies)
{
    ui->listWidgetLobbies->clear();
    for (const QString &lobby : lobbies) {
        ui->listWidgetLobbies->addItem(lobby);
    }

    qDebug() << "Liste des lobbies reçue : " << lobbies;
}


// ---- Fonction ajoutée par la branche feat/map ----

void MainWindow::on_pushButton_clicked()
{
    RulesDialog rulesDialog(this);
    rulesDialog.exec();
}