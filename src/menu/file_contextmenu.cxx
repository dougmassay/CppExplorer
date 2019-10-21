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

#include "file_contextmenu.hh"
#include "../clipboard.hh"
#include "../actions.hh"
#include "../tabwidget.hh"
#include "../trash.hh"
#include "../dialogs/appchooserdialog.hh"

FileContextMenu::FileContextMenu(BrowserWidget *b) {
    bWidget = b;

    open = new QAction(QIcon::fromTheme("document-open"),"Open",this);
    openWith = new QAction("Open With",this);
    cut = new QAction(QIcon::fromTheme("edit-cut"),"Cut",this);
    copy = new QAction(QIcon::fromTheme("edit-copy"),"Copy",this);
    rename = new QAction(QIcon::fromTheme("edit-rename"),"Rename",this);
    trash = new QAction(QIcon::fromTheme("user-trash"),"Move to Trash",this);
    restore = new QAction(QIcon::fromTheme("view-refresh"),"Restore",this);
    deleteFile = new QAction(QIcon::fromTheme("edit-delete"),"Delete",this);

    connect(open,&QAction::triggered,this,&FileContextMenu::onOpenClicked);
    connect(openWith,&QAction::triggered,this,&FileContextMenu::onOpenWithClicked);
    connect(cut,&QAction::triggered,this,&FileContextMenu::onCutClicked);
    connect(copy,&QAction::triggered,this,&FileContextMenu::onCopyClicked);
    connect(rename,&QAction::triggered,this,&FileContextMenu::onRenameClicked);
    connect(trash,&QAction::triggered,this,&FileContextMenu::onTrashClicked);
    connect(restore,&QAction::triggered,this,&FileContextMenu::onRestoreClicked);
    connect(deleteFile,&QAction::triggered,this,&FileContextMenu::onDeleteClicked);

    this->addAction(open);
    this->addAction(openWith);
    this->addSeparator();
    this->addAction(cut);
    this->addAction(copy);
    this->addSeparator();
    this->addAction(rename);
    this->addSeparator();
    if (TabWidget::currentWidget()->fsCurrentPath()==Trash::folderPath) {
        this->addAction(restore);
    } else {
        this->addAction(trash);
    }
    this->addAction(deleteFile);
}

FileContextMenu::~FileContextMenu() {
    delete open;
    delete openWith;
    delete cut;
    delete copy;
    delete rename;
    delete trash;
    delete restore;
    delete deleteFile;
}

void FileContextMenu::onOpenClicked() {
    Actions::openCurrentFile();
}

void FileContextMenu::onOpenWithClicked() {
    AppChooserDialog dialog(bWidget->currentItemName());
    dialog.exec();
}

void FileContextMenu::onCutClicked() {
    clipboard.fileName = bWidget->currentItemNames();
    clipboard.oldPath = bWidget->fsCurrentPath();
    clipboard.action = Clipboard_Actions::CUT;
}

void FileContextMenu::onCopyClicked() {
    clipboard.fileName = bWidget->currentItemNames();
    clipboard.oldPath = bWidget->fsCurrentPath();
    clipboard.action = Clipboard_Actions::COPY;
}

void FileContextMenu::onRenameClicked() {
    Actions::rename();
}

void FileContextMenu::onTrashClicked() {
    Actions::trash();
}

void FileContextMenu::onRestoreClicked() {
    Actions::restore();
}

void FileContextMenu::onDeleteClicked() {
    if (TabWidget::currentWidget()->fsCurrentPath()==Trash::folderPath) {
        Trash::deleteCurrentFile();
    } else {
        Actions::deleteFolder();
    }
}
