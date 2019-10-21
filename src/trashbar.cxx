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
#include "trashbar.hh"
#include "actions.hh"
#include "trash.hh"

TrashBar::TrashBar()
    : layout(new QHBoxLayout),
      subLayout(new QHBoxLayout),
      subWidget(new QWidget),
      restore(new QPushButton("Restore")),
      deleteItem(new QPushButton("Delete")),
      empty(new QPushButton("Empty Trash"))
{
    this->setLayout(layout);

    restore->setEnabled(false);
    deleteItem->setEnabled(false);

    connect(restore,&QPushButton::clicked,this,&TrashBar::onRestoreClicked);
    connect(deleteItem,&QPushButton::clicked,this,&TrashBar::onDeleteClicked);
    connect(empty,&QPushButton::clicked,this,&TrashBar::onEmptyClicked);

    subLayout->setContentsMargins(0,0,0,0);
    subWidget->setLayout(subLayout);
    layout->addWidget(subWidget,0,Qt::AlignLeft);

    subLayout->addWidget(restore);
    subLayout->addWidget(deleteItem);
    subLayout->addWidget(empty);
}

TrashBar::~TrashBar() {
    delete layout;
    delete restore;
    delete deleteItem;
    delete empty;
}

void TrashBar::setBrowserWidget(BrowserWidget *b) {
    bWidget = b;
    connect(bWidget,SIGNAL(selectionState(bool)),this,SLOT(onSelectionStateChanged(bool)));
}

void TrashBar::onSelectionStateChanged(bool state) {
    restore->setEnabled(state);
    deleteItem->setEnabled(state);
}

void TrashBar::onRestoreClicked() {
    Actions::restore();
}

void TrashBar::onDeleteClicked() {
    Trash::deleteCurrentFile();
}

void TrashBar::onEmptyClicked() {
    Trash::emptyTrash();
}
