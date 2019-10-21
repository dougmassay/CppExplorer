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
#include <QApplication>
#include <QKeySequence>

#include "filemenu.hh"
#include "../actions.hh"
#include "../tabwidget.hh"

FileMenu::FileMenu(Window *p) {
    parent = p;

    this->setTitle("File");

    newTab = new QAction(QIcon::fromTheme("tab-new",QPixmap(":/icons/tab-new.svg")),"New Tab",this);
    closeTab = new QAction(QIcon::fromTheme("tab-close",QPixmap(":/icons/window-close.svg")),"Close Tab",this);
    newFile = new QAction(QIcon::fromTheme("document-new",QPixmap(":/icons/document-new.svg")),"New File",this);
    newFolder = new QAction(QIcon::fromTheme("folder-new",QPixmap(":/icons/folder-new.svg")),"New Folder",this);
    quit = new QAction(QIcon::fromTheme("application-exit",QPixmap(":/icons/application-exit.svg")),"Exit",this);

    newTab->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_T));
    closeTab->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_T));
    newFile->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    newFolder->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_N));
    quit->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));

    connect(newTab,&QAction::triggered,this,&FileMenu::onNewTabClicked);
    connect(closeTab,&QAction::triggered,this,&FileMenu::onCloseTabClicked);
    connect(newFile,&QAction::triggered,this,&FileMenu::onNewFileClicked);
    connect(newFolder,&QAction::triggered,this,&FileMenu::onNewFolderClicked);
    connect(quit,&QAction::triggered,this,&FileMenu::onQuitClicked);

    this->addAction(newTab);
    this->addAction(closeTab);
    this->addSeparator();
    this->addAction(newFile);
    this->addAction(newFolder);
    this->addSeparator();
    this->addAction(quit);
}

FileMenu::~FileMenu() {
    delete newTab;
    delete closeTab;
    delete newFile;
    delete newFolder;
    delete quit;
}

void FileMenu::onNewTabClicked() {
    TabWidget::addNewTab();
}

void FileMenu::onCloseTabClicked() {
    TabWidget::closeCurrentTab();
}

void FileMenu::onNewFileClicked() {
    Actions::newFile();
}

void FileMenu::onNewFolderClicked() {
    Actions::newFolder();
}

void FileMenu::onQuitClicked() {
    parent->closeApp();
    qApp->exit(0);
}
