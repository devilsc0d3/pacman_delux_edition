#ifndef RULESDIALOG_H
#define RULESDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

class RulesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RulesDialog(QWidget *parent = nullptr);

private slots:
    void closeDialog();

private:
    void setupUI();
    QVBoxLayout *mainLayout;
    QScrollArea *scrollArea;
    QLabel *rulesLabel;
    QPushButton *closeButton;
};

#endif // RULESDIALOG_H
