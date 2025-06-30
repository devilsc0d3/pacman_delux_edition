// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gameserver.h"
#include "gameclient.h"
#include "gamescene.h"
#include "gameview.h"
#include "rulesdialog.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QPoint>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_menuWidget(nullptr)
    , m_hostButton(nullptr)
    , m_joinButton(nullptr)
    , m_rulesButton(nullptr)
    , m_layout(nullptr)
    , m_gameScene(nullptr)
    , m_gameView(nullptr)
    , m_server(nullptr)
    , m_client(nullptr)
    , m_rulesDialog(nullptr)
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
    m_menuWidget->setObjectName("menuWidget");

    QString styleSheet = QLatin1String(R"(
        QWidget#menuWidget {
            background-color: black;
        }
        QPushButton {
            background-color: #000000;
            color: #FFFF00;
            border: 2px solid #0000FF;
            padding: 10px 20px;
            font-family: "Courier New", Courier, monospace;
            font-size: 16px;
            font-weight: bold;
            border-radius: 0px;
            text-transform: uppercase;
        }
        QPushButton:hover {
            background-color: #0000FF;
            color: #FFFFFF;
        }
    )");
    m_menuWidget->setStyleSheet(styleSheet);

    m_layout = new QVBoxLayout(m_menuWidget);

    QLabel* titleLabel = new QLabel("PAC-MAN", m_menuWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #FFFF00; font-size: 48px; font-weight: bold; font-family: 'Courier New', Courier, monospace;");
    m_layout->addWidget(titleLabel);
    m_layout->addSpacing(20);
    m_layout->addStretch(1);

    m_hostButton = new QPushButton("Héberger une Partie", m_menuWidget);
    m_joinButton = new QPushButton("Rejoindre une Partie", m_menuWidget);
    m_rulesButton = new QPushButton("Règles du jeu", m_menuWidget);
    m_layout->addWidget(m_hostButton);
    m_layout->addWidget(m_joinButton);
    m_layout->addWidget(m_rulesButton);
    m_layout->addStretch(1);

    connect(m_hostButton, &QPushButton::clicked, this, &MainWindow::onHostButtonClicked);
    connect(m_joinButton, &QPushButton::clicked, this, &MainWindow::onJoinButtonClicked);
    connect(m_rulesButton, &QPushButton::clicked, this, &MainWindow::onRulesButtonClicked);
    setCentralWidget(m_menuWidget);
    resize(480, 400);
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

void MainWindow::onRulesButtonClicked()
{
    if (!m_rulesDialog) {
        m_rulesDialog = new RulesDialog(this);
    }
    m_rulesDialog->exec();
}
