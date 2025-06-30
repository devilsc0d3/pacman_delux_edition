#include "rulesdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPalette>

RulesDialog::RulesDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowTitle("Règles du jeu");
    setModal(true);
    resize(600, 500);

    QString styleSheet = QLatin1String(R"(
        QDialog {
            background-color: black;
        }
        QLabel {
            color: #FFFF00;
            font-family: "Courier New", Courier, monospace;
        }
        QPushButton {
            background-color: #000000;
            color: #FFFF00;
            border: 2px solid #0000FF;
            padding: 10px 20px;
            font-family: "Courier New", Courier, monospace;
            font-size: 16px;
            font-weight: bold;
            text-transform: uppercase;
        }
        QPushButton:hover {
            background-color: #0000FF;
            color: #FFFFFF;
        }
        QScrollArea {
            border: none;
        }
    )");
    this->setStyleSheet(styleSheet);
}

void RulesDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Règles du Jeu", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 36px; font-weight: bold;");

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    rulesLabel = new QLabel(this);
    rulesLabel->setWordWrap(true);
    rulesLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    rulesLabel->setMargin(10);

    QString rulesText =
        "<h1>But du jeu</h1>"
        "<p>Le but du jeu est de manger toutes les Pac-Gommes dans le labyrinthe sans se faire attraper par les fantômes.</p>"

        "<h2>Déplacements</h2>"
        "<p>Utilisez les touches fléchées (Haut, Bas, Gauche, Droite) pour déplacer Pac-Man dans le labyrinthe.</p>"

        "<h2>Les Pac-Gommes</h2>"
        "<p>Mangez toutes les petites gommes pour passer au niveau suivant. Les grosses gommes, appelées Super Pac-Gommes, vous rendent temporairement invincible et vous permettent de manger les fantômes.</p>"

        "<h2>Les Fantômes</h2>"
        "<p>Évitez les fantômes ! S'ils vous touchent, vous perdez une vie. Après avoir mangé une Super Pac-Gomme, les fantômes deviennent bleus et vous pouvez les manger pour gagner des points supplémentaires. Ils réapparaîtront ensuite à leur base.</p>"

        "<h2>Le Score</h2>"
        "<ul>"
        "<li>Pac-Gomme : 10 points</li>"
        "<li>Super Pac-Gomme : 50 points</li>"
        "<li>Manger un fantôme : 200, 400, 800, 1600 points (le score double pour chaque fantôme mangé successivement)</li>"
        "</ul>"

        "<h2>Vies</h2>"
        "<p>Vous commencez avec 3 vies. Vous perdez une vie chaque fois que vous êtes attrapé par un fantôme. Le jeu se termine lorsque vous n'avez plus de vies.</p>";

    rulesLabel->setText(rulesText);
    scrollArea->setWidget(rulesLabel);

    closeButton = new QPushButton("Fermer", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(buttonLayout);

    connect(closeButton, &QPushButton::clicked, this, &RulesDialog::closeDialog);
}

void RulesDialog::closeDialog()
{
    accept();
}
