#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Includes nécessaires pour la fenêtre du LOBBY
#include <QMessageBox>
#include <QUrl>
#include <QTimer>
#include <QDebug>
#include "rulesdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // On conserve TOUTE la logique du lobby venant de la branche dev-merge
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

    qDebug() << "UI du Lobby initialisée.";
}

MainWindow::~MainWindow()
{
    if (client) {
        qDebug() << "Fermeture : envoi de leave au serveur.";
        client->sendLeave();
        delete client;
    }
    delete ui;
    qDebug() << "MainWindow (Lobby) détruit.";
}

// Toutes les fonctions du lobby sont conservées
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
    if (!client) {
        startClient();
    }
    client->requestLobbyList();
}

void MainWindow::onConnectionSuccess()
{
    qDebug() << "Connexion WebSocket réussie.";
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
    }
    ui->lineEditLobbyId->setText(lobbyId);
}

void MainWindow::onLobbyListReceived(QStringList lobbies)
{
    ui->listWidgetLobbies->clear();
    for (const QString &lobby : lobbies) {
        ui->listWidgetLobbies->addItem(lobby);
    }
    qDebug() << "Liste des lobbies reçue : " << lobbies;
}

void MainWindow::on_pushButton_clicked()
{
    RulesDialog rulesDialog(this);
    rulesDialog.exec();
}