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
#include <QMessageBox>

#include "addressbar_text.hh"
#include "endtoolbar.hh"

AddressBarText::AddressBarText()
    : addressEntry(new QLineEdit)
{
    this->setMovable(false);
    this->setContextMenuPolicy(Qt::PreventContextMenu);

    addressEntry->setClearButtonEnabled(true);
    connect(addressEntry,&QLineEdit::returnPressed,this,&AddressBarText::onAddrEntryReturnPressed);

    this->addWidget(addressEntry);

    endtoolbar = new EndToolbar;
    this->addWidget(endtoolbar);
}

AddressBarText::~AddressBarText() {
    delete addressEntry;
    delete endtoolbar;
}

void AddressBarText::setBrowserWidget(BrowserWidget *b) {
    bWidget = b;
    addressEntry->setText(bWidget->fsCurrentPath());
    endtoolbar->setBrowserWidget(bWidget);
    connect(bWidget,SIGNAL(dirChanged(QString)),this,SLOT(onDirChanged(QString)));
}

void AddressBarText::onAddrEntryReturnPressed() {
    if (bWidget==nullptr) {
        return;
    }
    QString path = addressEntry->text();
    if (QDir(path).exists()) {
        bWidget->loadDir(path);
    } else {
        QMessageBox msg;
        msg.setWindowTitle("Error");
        msg.setText("Unknown file path");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
        addressEntry->setText(bWidget->fsCurrentPath());
    }
}

void AddressBarText::onDirChanged(QString path) {
    addressEntry->setText(path);
}
