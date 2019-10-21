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
#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSize>
#include <QString>
#include <QStringList>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>

#include "trashbar.hh"
#include "searchbar.hh"

class TrashBar;
class SearchBar;
class ListWidget;
class FileSystemWatcher;

class BrowserWidget : public QWidget {
    Q_OBJECT
    friend class ListWidget;
    friend class SearchBar;
public:
    BrowserWidget();
    ~BrowserWidget();
    void setIconView();
    void setListView();
    void loadDir(QString path, bool recordHistory, bool firstLoad);
    void loadDir(QString path, bool recordHistory);
    void loadDir(QString path);
    void loadDir();
    QStringList history();
    void reload();
    QString fsCurrentPath();
    QString currentDirName();
    void startRefresh();
    void stopRefresh();
    QString currentItemName();
    QStringList currentItemNames();
    void selectAll();
private:
    QVBoxLayout *layout;
    QListWidget *listWidget;
    QSize defaultGridSize;
    QString currentPath;
    QStringList historyList;
    FileSystemWatcher *thread;
    QString currentItemTxt;
    TrashBar *trashbar;
    SearchBar *searchbar;
    QStringList searchPatterns;
private slots:
    void onItemDoubleClicked(QListWidgetItem *item);
    void onItemClicked(QListWidgetItem *item);
signals:
    void dirChanged(QString path);
    void historyChanged();
    void selectionState(bool anySelected);
};

class ListWidget : public QListWidget {
    Q_OBJECT
public:
    explicit ListWidget(BrowserWidget *b);
protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    BrowserWidget *bWidget;
};

class FileSystemWatcher : public QTimer {
    Q_OBJECT
public:
    FileSystemWatcher(BrowserWidget *widget);
private:
    BrowserWidget *bWidget;
private slots:
    void onRefresh();
};
