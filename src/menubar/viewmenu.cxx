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
#include <QKeySequence>
#ifdef _WIN32
    //Windows: Use the registry
#else
#include "../settings.hh"
#endif

#include "viewmenu.hh"
#include "../tabwidget.hh"

#ifndef _WIN32
using namespace CppLib;
#endif

ViewMenu::ViewMenu() {
    this->setTitle("View");

    reload = new QAction(QIcon::fromTheme("view-refresh",QPixmap(":/icons/view-refresh.svg")),"Reload",this);
    hidden = new QAction("View Hidden Files",this);

    hidden->setCheckable(true);
#ifdef _WIN32
    hidden->setChecked(false);
#else
    hidden->setChecked(QVariant(Settings::getSetting("view/hidden","false")).toBool());
#endif

    reload->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
    hidden->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_H));

    connect(reload,&QAction::triggered,this,&ViewMenu::onReloadClicked);
    connect(hidden,&QAction::triggered,this,&ViewMenu::onHiddenClicked);

    this->addAction(reload);
    this->addAction(hidden);
}

ViewMenu::~ViewMenu() {
    delete reload;
    delete hidden;
}

void ViewMenu::onReloadClicked() {
    TabWidget::currentWidget()->reload();
}

void ViewMenu::onHiddenClicked() {
#ifdef _WIN32
    //Windows: Use the registry
#else
    Settings::writeSetting("view/hidden",QVariant(hidden->isChecked()).toString());
#endif
    TabWidget::currentWidget()->reload();
}
