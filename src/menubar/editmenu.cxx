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

#include "editmenu.hh"
#include "../clipboard.hh"
#include "../tabwidget.hh"
#include "../actions.hh"

EditMenu::EditMenu() {
    this->setTitle("Edit");

    selectAll = new QAction(QIcon::fromTheme("edit-select-all",QPixmap(":/icons/edit-select-all.svg")),"Select All",this);
    cut = new QAction(QIcon::fromTheme("edit-cut",QPixmap(":/icons/edit-cut.svg")),"Cut",this);
    copy = new QAction(QIcon::fromTheme("edit-copy",QPixmap(":/icons/edit-copy.svg")),"Copy",this);
    paste = new QAction(QIcon::fromTheme("edit-paste",QPixmap(":/icons/edit-paste.svg")),"Paste",this);
    trash = new QAction(QIcon::fromTheme("user-trash",QPixmap(":/icons/user-trash.svg")),"Trash",this);
    deleteFile = new QAction(QIcon::fromTheme("edit-delete",QPixmap(":/icons/edit-delete.svg")),"Delete",this);

    selectAll->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
    cut->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));
    copy->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
    paste->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_V));
    trash->setShortcut(QKeySequence(Qt::Key_Delete));
    deleteFile->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_D));

    connect(selectAll,&QAction::triggered,this,&EditMenu::onSelectAllClicked);
    connect(cut,&QAction::triggered,this,&EditMenu::onCutClicked);
    connect(copy,&QAction::triggered,this,&EditMenu::onCopyClicked);
    connect(paste,&QAction::triggered,this,&EditMenu::onPasteClicked);
    connect(trash,&QAction::triggered,this,&EditMenu::onTrashClicked);
    connect(deleteFile,&QAction::triggered,this,&EditMenu::onDeleteFileClicked);

    this->addAction(selectAll);
    this->addSeparator();
    this->addAction(cut);
    this->addAction(copy);
    this->addAction(paste);
    this->addSeparator();
    this->addAction(trash);
    this->addAction(deleteFile);
}

EditMenu::~EditMenu() {
    delete selectAll;
    delete cut;
    delete copy;
    delete paste;
    delete trash;
    delete deleteFile;
}

void EditMenu::onSelectAllClicked() {
    TabWidget::currentWidget()->selectAll();
}

void EditMenu::onCutClicked() {
    clipboard.fileName = TabWidget::currentWidget()->currentItemNames();
    clipboard.oldPath = TabWidget::currentWidget()->fsCurrentPath();
    clipboard.action = Clipboard_Actions::CUT;
}

void EditMenu::onCopyClicked() {
    clipboard.fileName = TabWidget::currentWidget()->currentItemNames();
    clipboard.oldPath = TabWidget::currentWidget()->fsCurrentPath();
    clipboard.action = Clipboard_Actions::COPY;
}

void EditMenu::onPasteClicked() {
    clipboard.newPath = TabWidget::currentWidget()->fsCurrentPath();
    Actions::paste();
}

void EditMenu::onTrashClicked() {
    Actions::trash();
}

void EditMenu::onDeleteFileClicked() {
    Actions::deleteFile();
}
