/********************************************************************************
** Form generated from reading UI file 'ProyectoFinal.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROYECTOFINAL_H
#define UI_PROYECTOFINAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QtImageManager.h"

QT_BEGIN_NAMESPACE

class Ui_ProyectoFinalClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    CQtImageManager *imagenmostrar;
    CQtImageManager *imagenmodificada;
    QHBoxLayout *horizontalLayout;
    QPushButton *buttonCapturarImagen;
    QPushButton *buttonSegmentar;
    QPushButton *buttonGuardarImagen;
    QPushButton *pushButton_2;
    QTextEdit *image_info;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ProyectoFinalClass)
    {
        if (ProyectoFinalClass->objectName().isEmpty())
            ProyectoFinalClass->setObjectName(QStringLiteral("ProyectoFinalClass"));
        ProyectoFinalClass->resize(835, 572);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProyectoFinalClass->sizePolicy().hasHeightForWidth());
        ProyectoFinalClass->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(ProyectoFinalClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        centralWidget->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        imagenmostrar = new CQtImageManager(centralWidget);
        imagenmostrar->setObjectName(QStringLiteral("imagenmostrar"));

        horizontalLayout_4->addWidget(imagenmostrar);

        imagenmodificada = new CQtImageManager(centralWidget);
        imagenmodificada->setObjectName(QStringLiteral("imagenmodificada"));

        horizontalLayout_4->addWidget(imagenmodificada);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonCapturarImagen = new QPushButton(centralWidget);
        buttonCapturarImagen->setObjectName(QStringLiteral("buttonCapturarImagen"));
        buttonCapturarImagen->setCheckable(true);

        horizontalLayout->addWidget(buttonCapturarImagen);

        buttonSegmentar = new QPushButton(centralWidget);
        buttonSegmentar->setObjectName(QStringLiteral("buttonSegmentar"));

        horizontalLayout->addWidget(buttonSegmentar);

        buttonGuardarImagen = new QPushButton(centralWidget);
        buttonGuardarImagen->setObjectName(QStringLiteral("buttonGuardarImagen"));

        horizontalLayout->addWidget(buttonGuardarImagen);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);

        image_info = new QTextEdit(centralWidget);
        image_info->setObjectName(QStringLiteral("image_info"));
        image_info->setMinimumSize(QSize(736, 0));
        image_info->setMaximumSize(QSize(16777215, 71));
        image_info->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);

        verticalLayout->addWidget(image_info);

        ProyectoFinalClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ProyectoFinalClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 835, 21));
        ProyectoFinalClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ProyectoFinalClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ProyectoFinalClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ProyectoFinalClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ProyectoFinalClass->setStatusBar(statusBar);

        retranslateUi(ProyectoFinalClass);

        QMetaObject::connectSlotsByName(ProyectoFinalClass);
    } // setupUi

    void retranslateUi(QMainWindow *ProyectoFinalClass)
    {
        ProyectoFinalClass->setWindowTitle(QApplication::translate("ProyectoFinalClass", "ProyectoFinal", nullptr));
        buttonCapturarImagen->setText(QApplication::translate("ProyectoFinalClass", "Capturar", nullptr));
        buttonSegmentar->setText(QApplication::translate("ProyectoFinalClass", "Segmentar", nullptr));
        buttonGuardarImagen->setText(QApplication::translate("ProyectoFinalClass", "Guardar", nullptr));
        pushButton_2->setText(QApplication::translate("ProyectoFinalClass", "Abrir Archivo", nullptr));
        image_info->setHtml(QApplication::translate("ProyectoFinalClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProyectoFinalClass: public Ui_ProyectoFinalClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROYECTOFINAL_H
