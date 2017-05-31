#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <string>
#include <iostream>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog();
    std::string graphPath, ip;
    int port;
    bool accepted = false;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SetupDialog *ui;
    void close();
};

#endif // SETUPDIALOG_H
