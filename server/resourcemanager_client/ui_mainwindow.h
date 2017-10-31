/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTextBrowser *textBrowser;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *lineEdit_localname;
    QLabel *label_3;
    QLineEdit *lineEdit_ipfilter;
    QCheckBox *checkBox_autosendlocalip;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_resource;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::WindowModal);
        MainWindow->resize(648, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        gridLayout->addWidget(textBrowser, 3, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(164, 27));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(14);
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        lineEdit_localname = new QLineEdit(centralWidget);
        lineEdit_localname->setObjectName(QStringLiteral("lineEdit_localname"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setWeight(50);
        lineEdit_localname->setFont(font1);

        horizontalLayout_2->addWidget(lineEdit_localname);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(164, 27));
        label_3->setFont(font);

        horizontalLayout_2->addWidget(label_3);

        lineEdit_ipfilter = new QLineEdit(centralWidget);
        lineEdit_ipfilter->setObjectName(QStringLiteral("lineEdit_ipfilter"));
        lineEdit_ipfilter->setFont(font1);

        horizontalLayout_2->addWidget(lineEdit_ipfilter);

        checkBox_autosendlocalip = new QCheckBox(centralWidget);
        checkBox_autosendlocalip->setObjectName(QStringLiteral("checkBox_autosendlocalip"));

        horizontalLayout_2->addWidget(checkBox_autosendlocalip);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(164, 27));
        label->setFont(font);

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy1);
        comboBox->setMaximumSize(QSize(261, 27));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font2.setPointSize(14);
        comboBox->setFont(font2);
        comboBox->setEditable(true);

        horizontalLayout->addWidget(comboBox);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMaximumSize(QSize(260, 27));
        pushButton->setFont(font);
        pushButton->setStyleSheet(QLatin1String("QPushButton {\n"
"    /* Let's make the size of the button 1,5 times of font size. */\n"
"    min-height: 1.5em;\n"
"    /* Font size just 1.*/\n"
"    font: 1em;\n"
"    /* Margins so that we get a little space on the left and right. */\n"
"    margin: 0 1px 0 1px;\n"
"    /* The font color */ \n"
"    color: white;\n"
"    /* Here's the background gradient with start point, end point, \n"
"       stop \"percentage\" and color, stop percentage and color. */\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #2198c0, stop: 1 #0d5ca6);\n"
"    /* We'll round the borders. */\n"
"    border-style: outset;\n"
"    /* Round radius will be 3px */\n"
"    border-radius: 3px;\n"
"    /* Border is only one pixel */\n"
"    border-width: 1px;\n"
"    /* Border color is now set */\n"
"    border-color: #0c457e;\n"
"}\n"
" \n"
"/* This is style when button is pressed */\n"
"QPushButton:pressed {\n"
"    /* We'll just invert the gradient by changing the c"
                        "olors around. */\n"
"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                      stop: 0 #0d5ca6, stop: 1 #2198c0);\n"
"}\n"
" \n"
"QPushButton:flat {\n"
"    border: none;\n"
"}\n"
"QPushButton:disabled {\n"
"	color: grey\n"
"}"));

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_resource = new QVBoxLayout();
        verticalLayout_resource->setSpacing(6);
        verticalLayout_resource->setObjectName(QStringLiteral("verticalLayout_resource"));

        verticalLayout->addLayout(verticalLayout_resource);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 648, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\347\216\257\345\242\203\345\256\242\346\210\267\347\253\257", 0));
        label_2->setText(QApplication::translate("MainWindow", "\346\234\254\345\234\260\345\220\215\347\247\260", 0));
        lineEdit_localname->setText(QApplication::translate("MainWindow", "S3-2", 0));
        label_3->setText(QApplication::translate("MainWindow", "\345\234\260\345\235\200\350\277\207\346\273\244", 0));
        lineEdit_ipfilter->setText(QString());
        checkBox_autosendlocalip->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\217\221\351\200\201\346\234\254\346\234\272IP", 0));
        label->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250\350\216\267\345\217\226\350\265\204\346\272\220", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
