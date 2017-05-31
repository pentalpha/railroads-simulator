#include "setupdialog.h"
#include "ui_setupdialog.h"
#include "global.h"

SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    this->setModal(true);
    ui->setupUi(this);
}

SetupDialog::~SetupDialog()
{
    delete ui;
}

void SetupDialog::close(){
    if(accepted){
         std::cout << "IP " << ip << std::endl;
         std::cout << "Port " << port << std::endl;
         std::cout << "Graph Path " << graphPath << std::endl;
         this->setVisible(false);
         Global::startServer(this, port, ip, graphPath);
    }else{
        this->setVisible(false);
        std::cout << "Not accepted" << std::endl;
    }
}

void SetupDialog::on_buttonBox_accepted()
{
    this->graphPath = ui->lineEdit_3->text().toStdString();
    this->ip = ui->ipInput->text().toStdString();
    std::string portStr = ui->portField->text().toStdString();
    try{
        this->port = std::stoi(portStr);
        this->accepted = true;
        close();
    }catch(...){
        std::cout << "Could not parse '" << portStr << "' to 'int';" << std::endl;
    }
}

void SetupDialog::on_buttonBox_rejected()
{
    close();
}


