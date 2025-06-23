#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}

void MainWindow::on_buttonConnect_clicked()
{
    QString name = ui->lineEditName->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Entrez un pseudo !");
        return;
    }

    if (client)
        delete client;

    // Connexion au serveur local sur le port 1234
    client = new LobbyClient(QUrl(QStringLiteral("ws://localhost:1234")), this);

    connect(client, &LobbyClient::connectionSuccess, this, &MainWindow::onConnectionSuccess);
    connect(client, &LobbyClient::connectionFailed, this, &MainWindow::onConnectionFailed);
    connect(client, &LobbyClient::lobbyUpdated, this, &MainWindow::updateLobby);
}

void MainWindow::onConnectionSuccess()
{
    client->sendJoinMessage(ui->lineEditName->text());
}

void MainWindow::onConnectionFailed(QString reason)
{
    QMessageBox::critical(this, "Connexion échouée", reason);
}
void MainWindow::updateLobby(QStringList players)
{
    ui->listWidgetLobby->clear();
    for (const QString &p : players) {
        ui->listWidgetLobby->addItem(p);
    }
}
