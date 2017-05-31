/********************************************************************************
** Form generated from reading UI file 'setupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUPDIALOG_H
#define UI_SETUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *portLabel;
    QLineEdit *portField;
    QLabel *ipLabel;
    QLineEdit *ipInput;
    QLabel *graphPathLabel;
    QLineEdit *lineEdit_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *SetupDialog)
    {
        if (SetupDialog->objectName().isEmpty())
            SetupDialog->setObjectName(QStringLiteral("SetupDialog"));
        SetupDialog->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SetupDialog->sizePolicy().hasHeightForWidth());
        SetupDialog->setSizePolicy(sizePolicy);
        SetupDialog->setMinimumSize(QSize(400, 300));
        SetupDialog->setMaximumSize(QSize(400, 300));
        SetupDialog->setModal(true);
        buttonBox = new QDialogButtonBox(SetupDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(290, 20, 101, 271));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        verticalLayoutWidget = new QWidget(SetupDialog);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(9, 20, 271, 211));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        portLabel = new QLabel(verticalLayoutWidget);
        portLabel->setObjectName(QStringLiteral("portLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        portLabel->setFont(font);

        verticalLayout->addWidget(portLabel);

        portField = new QLineEdit(verticalLayoutWidget);
        portField->setObjectName(QStringLiteral("portField"));
        QFont font1;
        font1.setFamily(QStringLiteral("Droid Sans"));
        portField->setFont(font1);

        verticalLayout->addWidget(portField);

        ipLabel = new QLabel(verticalLayoutWidget);
        ipLabel->setObjectName(QStringLiteral("ipLabel"));
        ipLabel->setFont(font);

        verticalLayout->addWidget(ipLabel);

        ipInput = new QLineEdit(verticalLayoutWidget);
        ipInput->setObjectName(QStringLiteral("ipInput"));
        ipInput->setFont(font1);

        verticalLayout->addWidget(ipInput);

        graphPathLabel = new QLabel(verticalLayoutWidget);
        graphPathLabel->setObjectName(QStringLiteral("graphPathLabel"));
        graphPathLabel->setFont(font);

        verticalLayout->addWidget(graphPathLabel);

        lineEdit_3 = new QLineEdit(verticalLayoutWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        QFont font2;
        font2.setFamily(QStringLiteral("Droid Sans"));
        font2.setPointSize(8);
        font2.setItalic(true);
        font2.setStrikeOut(false);
        font2.setKerning(true);
        font2.setStyleStrategy(QFont::PreferAntialias);
        lineEdit_3->setFont(font2);

        verticalLayout->addWidget(lineEdit_3);

        verticalSpacer = new QSpacerItem(258, 278, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(SetupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SetupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SetupDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetupDialog);
    } // setupUi

    void retranslateUi(QDialog *SetupDialog)
    {
        SetupDialog->setWindowTitle(QApplication::translate("SetupDialog", "Server Setup", Q_NULLPTR));
        portLabel->setText(QApplication::translate("SetupDialog", "Port:", Q_NULLPTR));
        portField->setText(QApplication::translate("SetupDialog", "50002", Q_NULLPTR));
        ipLabel->setText(QApplication::translate("SetupDialog", "Hosting IPv4:", Q_NULLPTR));
        ipInput->setText(QApplication::translate("SetupDialog", "127.0.0.1", Q_NULLPTR));
        graphPathLabel->setText(QApplication::translate("SetupDialog", "Railroad Graph File:", Q_NULLPTR));
        lineEdit_3->setText(QApplication::translate("SetupDialog", "/home/pentalpha/railroads/min.graph", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SetupDialog: public Ui_SetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUPDIALOG_H
