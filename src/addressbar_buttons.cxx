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
#include <QDir>
#include <QCursor>

#include "addressbar_buttons.hh"
#include "tabwidget.hh"

AddressBarButtons::AddressBarButtons()
    : subBar(new QToolBar),
      spacer(new QWidget),
      endtoolbar(new EndToolbar)
{
    this->setMovable(false);
    this->setContextMenuPolicy(Qt::PreventContextMenu);
    group = new QButtonGroup;

    spacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    this->addWidget(subBar);
    this->addWidget(spacer);
    this->addWidget(endtoolbar);
}

AddressBarButtons::~AddressBarButtons() {
    delete group;
    delete endtoolbar;
    delete spacer;
    delete subBar;
}

void AddressBarButtons::setBrowserWidget(BrowserWidget *b) {
    bWidget = b;
    endtoolbar->setBrowserWidget(bWidget);
    parsePath(bWidget->fsCurrentPath());
    connect(bWidget,SIGNAL(dirChanged(QString)),this,SLOT(onDirChanged(QString)));
}

void AddressBarButtons::parsePath(QString path) {
    bool buildOnto = false;
    bool lastPathShorter = true;
    QString lastPath = "";
    //If the last path was shorter, then we can just add buttons
    //If it is longer, then we must select the right button
    if (!group->buttons().isEmpty()) {
        lastPath = "/";
        auto list = group->buttons();
        for (int i = 0; i<list.size(); i++) {
            QString txt = list.at(i)->text();
            if (txt!="/") {
                lastPath+=txt+"/";
            }
        }
        if (lastPath.length()<path.length()) {
            if (path.startsWith(lastPath)) {
                buildOnto = true;
            }
        } else {
            if (lastPath.startsWith(path)) {
                buildOnto = true;
                lastPathShorter = false;
            }
        }
    }

    if (!path.endsWith("/")) {
        path+="/";
    }
    if (!lastPath.endsWith("/")) {
        lastPath+="/";
    }

    if (buildOnto==false) {
        parseClean(path);
    } else {
        if (lastPathShorter) {
            QString currentPath = "";
            QString name = "";
            for (int i = 0; i<path.length(); i++) {
                if (currentPath==lastPath) {
                    for (int j = i; j<path.length(); j++) {
                        currentPath+=path.at(j);
                        if (path.at(j)=='/') {
                            if (name!="") {
                                AddrPushButton *btn = new AddrPushButton(currentPath,bWidget);
                                btn->setText(name);
                                subBar->addWidget(btn);
                                group->addButton(btn);
                                btn->setChecked(true);
                                name = "";
                            }
                        } else {
                            name+=path.at(j);
                        }
                    }
                } else {
                    currentPath+=path.at(i);
                }
            }
        } else {
            QString lastName = "";
            QString current = "";
            for (int i = 0; i<path.length(); i++) {
                if (path.at(i)=="/") {
                    lastName = current;
                    current = "";
                } else {
                    current+=path.at(i);
                }
            }

            auto list = group->buttons();
            for (int i = 0; i<list.length(); i++) {
                if (list.at(i)->text()==lastName) {
                    list.at(i)->setChecked(true);
                    break;
                }
            }
        }
    }
}

void AddressBarButtons::parseClean(QString path) {
    subBar->clear();
    group = new QButtonGroup;

    AddrPushButton *bt1 = new AddrPushButton("/",bWidget);
    bt1->setText("/");
    subBar->addWidget(bt1);
    group->addButton(bt1);

    QString name = "";
    QString currentPath = "";
    for (int i = 0; i<path.size(); i++) {
        currentPath+=path.at(i);
        if (path.at(i)=='/') {
            if (name!="") {
                AddrPushButton *btn = new AddrPushButton(currentPath,bWidget);
                btn->setText(name);
                subBar->addWidget(btn);
                group->addButton(btn);
                btn->setChecked(true);
                name = "";
            }
        } else {
            name+=path.at(i);
        }
    }
}

void AddressBarButtons::onDirChanged(QString path) {
    parsePath(path);
}

//AddrPushButton
//This is the button for our button bar

AddrPushButton::AddrPushButton(QString path, BrowserWidget *b) {
    this->setAutoExclusive(true);
    this->setCheckable(true);

    fullpath = path;
    bWidget = b;
    connect(this,&QPushButton::clicked,this,&AddrPushButton::onClicked);
}

void AddrPushButton::mousePressEvent(QMouseEvent *event) {
    if (event->button()==Qt::RightButton) {
        AddrButtonMenu menu(bWidget,fullpath);
        menu.exec(QCursor::pos());
    }
    QPushButton::mousePressEvent(event);
}

void AddrPushButton::onClicked() {
    bWidget->loadDir(fullpath);
}

//AddrButtonMenu
//The context menu displayed when the user right-clicks on the buttons

AddrButtonMenu::AddrButtonMenu(BrowserWidget *b, QString fullpath) {
    bWidget = b;
    path = fullpath;

    open = new QAction("Open",this);
    openTab = new QAction("Open in New Tab",this);

    connect(open,&QAction::triggered,this,&AddrButtonMenu::onOpenClicked);
    connect(openTab,&QAction::triggered,this,&AddrButtonMenu::onOpenTabClicked);

    this->addAction(open);
    this->addAction(openTab);
}

AddrButtonMenu::~AddrButtonMenu() {
    delete open;
    delete openTab;
}

void AddrButtonMenu::onOpenClicked() {
    bWidget->loadDir(path);
}

void AddrButtonMenu::onOpenTabClicked() {
    TabWidget::addNewTab(path);
}
