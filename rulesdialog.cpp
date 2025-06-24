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
    setWindowTitle("Règles du Jeu Pacman");
    setModal(true);
    resize(500, 400);
}

void RulesDialog::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("Règles du Jeu Pacman", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    rulesLabel = new QLabel(this);
    rulesLabel->setWordWrap(true);
    rulesLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    rulesLabel->setMargin(10);

    QString rulesText =
        "<h1>Lorem ipsum dolo</h1>"
        "<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</p>"

        "<h2>Lorem ipsum dolo</h2>"
        "<p>Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.</p>"

        "<h2>Lorem ipsum dolo</h2>"
        "<p>Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo.</p>"

        "<h2>Lorem ipsum dolo</h2>"
        "<p>Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt.</p>"

        "<h2>Lorem ipsum dolo</h2>"
        "<p>Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem.</p>"

        "<h2>Lorem ipsum dolo</h2>"
        "<p>Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur.</p>"

        "<h2>Lorem ipsum dolo</h2>"
        "<p>At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident.</p>";

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
