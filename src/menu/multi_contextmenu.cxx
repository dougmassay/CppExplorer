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
#include "multi_contextmenu.hh"
#include "../clipboard.hh"
#include "../actions.hh"
#include "../trash.hh"
#include "../tabwidget.hh"

MultiContextMenu::MultiContextMenu(BrowserWidget *b) {
    bWidget = b;

    cut = new QAction(QIcon::fromTheme("edit-cut"),"Cut",this);
    copy = new QAction(QIcon::fromTheme("edit-copy"),"Copy",this);
    trash = new QAction(QIcon::fromTheme("user-trash"),"Move to Trash",this);
    restore = new QAction(QIcon::fromTheme("view-refresh"),"Restore",this);
    deleteAll = new QAction(QIcon::fromTheme("edit-delete"),"Delete",this);

    connect(cut,&QAction::triggered,this,&MultiContextMenu::onCutClicked);
    connect(copy,&QAction::triggered,this,&MultiContextMenu::onCopyClicked);
    connect(trash,&QAction::triggered,this,&MultiContextMenu::onTrashClicked);
    connect(restore,&QAction::triggered,this,&MultiContextMenu::onRestoreClicked);
    connect(deleteAll,&QAction::triggered,this,&MultiContextMenu::onDeleteAllClicked);

    this->addAction(cut);
    this->addAction(copy);
    this->addSeparator();
    if (TabWidget::currentWidget()->fsCurrentPath()==Trash::folderPath) {
        this->addAction(restore);
    } else {
        this->addAction(trash);
    }
    this->addAction(deleteAll);
}

MultiContextMenu::~MultiContextMenu() {
    delete cut;
    delete copy;
    delete trash;
    delete restore;
    delete deleteAll;
}

void MultiContextMenu::onCutClicked() {
    clipboard.fileName = bWidget->currentItemNames();
    clipboard.oldPath = bWidget->fsCurrentPath();
    clipboard.action = Clipboard_Actions::CUT;
}

void MultiContextMenu::onCopyClicked() {
    clipboard.fileName = bWidget->currentItemNames();
    clipboard.oldPath = bWidget->fsCurrentPath();
    clipboard.action = Clipboard_Actions::COPY;
}

void MultiContextMenu::onTrashClicked() {
    Actions::trash();
}

void MultiContextMenu::onRestoreClicked() {
    Actions::restore();
}

void MultiContextMenu::onDeleteAllClicked() {
    Actions::deleteFile();
}
