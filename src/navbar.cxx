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
#include <QIcon>
#include <QDir>

#include "navbar.hh"

NavBar::NavBar()
    : goBack(new QToolButton),
      goUp(new QToolButton),
      refresh(new QToolButton),
      home(new QToolButton)
{
    this->setMovable(false);
    this->setContextMenuPolicy(Qt::PreventContextMenu);

    goBack->setIcon(QIcon::fromTheme("go-previous",QPixmap(":/icons/go-previous.svg")));
    goUp->setIcon(QIcon::fromTheme("go-up",QPixmap(":/icons/go-up.svg")));
    refresh->setIcon(QIcon::fromTheme("view-refresh",QPixmap(":/icons/view-refresh.svg")));
    home->setIcon(QIcon::fromTheme("go-home",QPixmap(":/icons/user-home.svg")));

    goBack->setEnabled(false);
    goUp->setEnabled(false);

    connect(goBack,&QToolButton::clicked,this,&NavBar::onGoBackClicked);
    connect(goUp,&QToolButton::clicked,this,&NavBar::onGoUpClicked);
    connect(refresh,&QToolButton::clicked,this,&NavBar::onRefreshClicked);
    connect(home,&QToolButton::clicked,this,&NavBar::onHomeClicked);

    this->addWidget(goBack);
    this->addWidget(goUp);
    this->addWidget(refresh);
    this->addWidget(home);
}

NavBar::~NavBar() {
    delete goBack;
    delete goUp;
    delete refresh;
    delete home;
}

void NavBar::setBrowserWidget(BrowserWidget *b) {
    bWidget = b;
    if (bWidget->fsCurrentPath()=="/") {
        goUp->setEnabled(false);
    } else {
        goUp->setEnabled(true);
    }
    if (b->history().isEmpty()) {
        goBack->setEnabled(false);
    } else {
        goBack->setEnabled(true);
    }
    connect(bWidget,SIGNAL(dirChanged(QString)),this,SLOT(onDirChanged(QString)));
    connect(bWidget,&BrowserWidget::historyChanged,this,&NavBar::onHistoryChanged);
}

void NavBar::onGoUpClicked() {
    if (bWidget==nullptr) {
        return;
    }
    QString currentPath = bWidget->fsCurrentPath();
    QDir dir(currentPath);
    dir.cdUp();
    QString newPath = dir.path();
    bWidget->loadDir(newPath);
}

void NavBar::onGoBackClicked() {
    QString path = bWidget->history().first();
    bWidget->loadDir(path);
}

void NavBar::onRefreshClicked() {
    bWidget->reload();
}

void NavBar::onHomeClicked() {
    bWidget->loadDir();
}

void NavBar::onDirChanged(QString path) {
    if (path=="/") {
        goUp->setEnabled(false);
    } else {
        goUp->setEnabled(true);
    }
}

void NavBar::onHistoryChanged() {
    if (bWidget->history().isEmpty()) {
        goBack->setEnabled(false);
    } else {
        goBack->setEnabled(true);
    }
}
