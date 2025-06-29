// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameserver.h"
#include "gameclient.h"
#include "gamescene.h"
#include "gameview.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QPoint>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_menuWidget(nullptr)
    , m_hostButton(nullptr)
    , m_joinButton(nullptr)
    , m_layout(nullptr)
    , m_gameScene(nullptr)
    , m_gameView(nullptr)
    , m_server(nullptr)
    , m_client(nullptr)
{
    ui->setupUi(this);
    setupMenu();
    setWindowTitle("Pac-Man Multijoueur");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMenu()
{
    m_menuWidget = new QWidget(this);
    m_layout = new QVBoxLayout(m_menuWidget);
    m_hostButton = new QPushButton("Héberger une Partie", m_menuWidget);
    m_joinButton = new QPushButton("Rejoindre une Partie", m_menuWidget);
    m_layout->addWidget(m_hostButton);
    m_layout->addWidget(m_joinButton);
    m_layout->addStretch();
    connect(m_hostButton, &QPushButton::clicked, this, &MainWindow::onHostButtonClicked);
    connect(m_joinButton, &QPushButton::clicked, this, &MainWindow::onJoinButtonClicked);
    setCentralWidget(m_menuWidget);
    resize(300, 200);
}

void MainWindow::switchToGameView()
{
    m_gameScene = new GameScene(this);
    m_gameView = new GameView(m_gameScene, this);

    if (m_client) {
        connect(m_client, &GameClient::gameStateReceived, m_gameScene, &GameScene::updateFromServer);
        connect(m_client, &GameClient::levelClearedOnClient, m_gameScene, &GameScene::levelCleared);
        connect(m_client, &GameClient::pelletRemoved, m_gameScene, &GameScene::onPelletRemoved);
        // La ligne est maintenant à sa place, à l'intérieur du bloc 'if'.
        connect(m_client, &GameClient::resetLevelOnClient, m_gameScene, &GameScene::resetLevelClient);
    }

    connect(m_gameView, &GameView::directionInput, this, [=](int playerID, QPoint direction) {
        if (m_client) {
            m_client->sendPlayerInput(direction);
        }
    });

    setCentralWidget(m_gameView);
    adjustSize();
}

void MainWindow::onHostButtonClicked()
{
    qDebug() << "Bouton Héberger cliqué !";
    m_server = new GameServer(this);
    m_server->startServer(12345);
    m_client = new GameClient(this);
    connect(m_client, &GameClient::connected, this, &MainWindow::switchToGameView);
    m_client->connectToServer("localhost", 12345);
}

void MainWindow::onJoinButtonClicked()
{
    qDebug() << "Bouton Rejoindre cliqué !";
    bool ok;
    QString hostAddress = QInputDialog::getText(this, "Rejoindre une Partie", "Adresse IP du serveur:", QLineEdit::Normal, "localhost", &ok);
    if (ok && !hostAddress.isEmpty()) {
        m_client = new GameClient(this);
        connect(m_client, &GameClient::connected, this, &MainWindow::switchToGameView);
        connect(m_client, &GameClient::disconnected, this, [=](){
            QMessageBox::information(this, "Déconnexion", "Vous avez été déconnecté du serveur.");
            setupMenu();
        });
        m_client->connectToServer(hostAddress, 12345);
    }
}
