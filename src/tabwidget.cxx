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
#include <iostream>

#include "tabwidget.hh"
#include "window.hh"

QTabWidget *TabWidget::tabs;
NavBar *TabWidget::navigationBar;

TabWidget::TabWidget(NavBar *navbar)
    : layout(new QVBoxLayout)
{
    navigationBar = navbar;

    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    tabs = new QTabWidget;
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    tabs->setTabBarAutoHide(true);
    layout->addWidget(tabs);

    connect(tabs,&QTabWidget::currentChanged,this,&TabWidget::onTabsChanged);
    connect(tabs,SIGNAL(tabCloseRequested(int)),this,SLOT(onTabClosed(int)));

    addNewTab();
}

TabWidget::~TabWidget() {
    delete tabs;
    delete layout;
}

void TabWidget::addNewTab(QString path) {
    int count = tabs->count();
    BrowserWidget *b = new BrowserWidget;
    b->loadDir(path,true,true);
    QString name = b->currentDirName();
    if (name=="") {
        name+="/";
    }
    tabs->addTab(b,name);
    tabs->setCurrentIndex(count);
    b->startRefresh();
    navigationBar->setBrowserWidget(b);
}

void TabWidget::addNewTab() {
    addNewTab(QDir::homePath());
}

void TabWidget::closeCurrentTab() {
    int index = tabs->currentIndex();
    if (tabs->count()==1) {
        addNewTab();
    }
    BrowserWidget *b = static_cast<BrowserWidget *>(tabs->widget(index));
    b->stopRefresh();
    delete b;
}

BrowserWidget *TabWidget::currentWidget() {
    BrowserWidget *w = static_cast<BrowserWidget *>(tabs->currentWidget());
    return w;
}

void TabWidget::updateTabName() {
    BrowserWidget *w = currentWidget();
    tabs->setTabText(tabs->currentIndex(),w->currentDirName());
}

QVector<BrowserWidget *> TabWidget::allWidgets() {
    QVector<BrowserWidget *> widgets;
    for (int i = 0; i<tabs->count(); i++) {
        BrowserWidget *current = static_cast<BrowserWidget *>(tabs->widget(i));
        widgets.push_back(current);
    }
    return widgets;
}

void TabWidget::onTabsChanged() {
    navigationBar->setBrowserWidget(currentWidget());
    Window::addrTxt->setBrowserWidget(currentWidget());
    Window::addrButtons->setBrowserWidget(currentWidget());
}

void TabWidget::onTabClosed(int index) {
    BrowserWidget *b = static_cast<BrowserWidget *>(tabs->widget(index));
    b->stopRefresh();
    delete b;
}
