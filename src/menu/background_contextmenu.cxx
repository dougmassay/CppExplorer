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

#include "background_contextmenu.hh"
#include "../clipboard.hh"
#include "../actions.hh"

BackgroundContextMenu::BackgroundContextMenu(BrowserWidget *b) {
    bWidget = b;

    newFolder = new QAction(QIcon::fromTheme("folder-new"),"New Folder",this);
    newFile = new QAction(QIcon::fromTheme("document-new"),"New File",this);
    paste = new QAction(QIcon::fromTheme("edit-paste"),"Paste",this);

    connect(newFolder,&QAction::triggered,this,&BackgroundContextMenu::onNewFolderClicked);
    connect(newFile,&QAction::triggered,this,&BackgroundContextMenu::onNewFileClicked);
    connect(paste,&QAction::triggered,this,&BackgroundContextMenu::onPasteClicked);

    this->addAction(newFolder);
    this->addAction(newFile);
    this->addSeparator();
    this->addAction(paste);
}

BackgroundContextMenu::~BackgroundContextMenu() {
    delete newFolder;
    delete newFile;
    delete paste;
}

void BackgroundContextMenu::onNewFolderClicked() {
    Actions::newFolder();
}

void BackgroundContextMenu::onNewFileClicked() {
    Actions::newFile();
}

void BackgroundContextMenu::onPasteClicked() {
    clipboard.newPath = bWidget->fsCurrentPath();
    Actions::paste();
}
