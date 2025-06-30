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
        "<p>Ce mode de jeu se joue en deux équipes : l'équipe des Pac-Man et l'équipe des Fantômes.</p>"

        "<h2>Équipe Pac-Man</h2>"
        "<p><b>Objectif :</b> Manger toutes les Pac-Gommes du labyrinthe pour marquer un maximum de points et gagner la partie.</p>"
        "<p><b>Déplacements :</b> Utilisez les touches fléchées pour vous déplacer.</p>"
        "<p><b>Pac-Gommes :</b> Les petites gommes rapportent des points. Les grosses Super Pac-Gommes vous rendent temporairement invincibles et vous permettent de manger les fantômes pour des points bonus.</p>"
        "<p><b>Vies :</b> Chaque Pac-Man a un nombre de vies limité. Perdre toutes vos vies vous élimine de la partie.</p>"

        "<h2>Équipe Fantômes</h2>"
        "<p><b>Objectif :</b> Attraper les Pac-Man pour les empêcher de vider le labyrinthe et de marquer des points. La victoire est à vous si vous empêchez les Pac-Man de finir le niveau.</p>"
        "<p><b>Déplacements :</b> Utilisez les touches fléchées pour vous déplacer.</p>"
        "<p><b>Comportement :</b> Si un Pac-Man mange une Super Pac-Gomme, vous devenez vulnérable. Si vous êtes mangé, vous réapparaissez à votre base après un court instant.</p>"

        "<h2>Score</h2>"
        "<ul>"
        "<li>Pac-Gomme : 10 points</li>"
        "<li>Super Pac-Gomme : 50 points</li>"
        "<li>Manger un fantôme : 200, 400, 800, 1600 points</li>"
        "</ul>"

        "<h2>Fin de la Partie</h2>"
        "<p>La partie se termine si l'équipe Pac-Man mange toutes les Pac-Gommes, ou si tous les Pac-Man ont perdu toutes leurs vies.</p>";

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
