// Copyright 2018 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//	list of conditions and the following disclaimer in the documentation and/or
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may
//	be used to endorse or promote products derived from this software
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <QToolBar>
#include <QButtonGroup>
#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

#include "browserwidget.hh"
#include "endtoolbar.hh"

class AddrPushButton;
class AddrButtonMenu;

class AddressBarButtons : public QToolBar {
    Q_OBJECT
public:
    AddressBarButtons();
    ~AddressBarButtons();
    void setBrowserWidget(BrowserWidget *b);
    void parsePath(QString path);
private:
    BrowserWidget *bWidget;
    QToolBar *subBar;
    QButtonGroup *group;
    QWidget *spacer;
    EndToolbar *endtoolbar;
    void parseClean(QString path);
private slots:
    void onDirChanged(QString path);
};

class AddrPushButton : public QPushButton {
    Q_OBJECT
public:
    explicit AddrPushButton(QString path, BrowserWidget *b);
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    QString fullpath;
    BrowserWidget *bWidget;
private slots:
    void onClicked();
};

class AddrButtonMenu : public QMenu {
    Q_OBJECT
public:
    explicit AddrButtonMenu(BrowserWidget *b, QString fullpath);
    ~AddrButtonMenu();
private:
    BrowserWidget *bWidget;
    QString path;
    QAction *open, *openTab;
private slots:
    void onOpenClicked();
    void onOpenTabClicked();
};
