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
#include <QStandardPaths>
#include <QIcon>
#include <QDir>
#include <QTreeWidgetItem>
#include <QStorageInfo>
#include <iostream>

#include "sidebar.hh"
#include "tabwidget.hh"
#include "trash.hh"

SideBar::SideBar()
    : mainWidget(new QWidget),
      layout(new QVBoxLayout),
      placeslist(new PlacesList),
      deviceslist(new DeviceList)
{
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    layout->setContentsMargins(0,0,0,0);
    mainWidget->setLayout(layout);
    this->setWidget(mainWidget);

    layout->addWidget(placeslist);
    layout->addWidget(deviceslist);
}

SideBar::~SideBar() {
    delete layout;
    delete placeslist;
    delete deviceslist;
}

//PlacesList
//This holds the list of default places- things like Documents, Pictures,
//the Desktop, Downloads, Music, etc
//We use a tree widget because we can add a header item (a title)

PlacesList::PlacesList() {
    this->setHeaderLabel("Places");
    this->setColumnCount(1);

    //When we create a new item, we specify the text for two columns:
    //Column 1 (0) contains the display text
    //Column 2 (1) contains the ID text
    //To avoid confusion, keep the ID text all-lowercase
    home = new QTreeWidgetItem(this);
    home->setText(0,"Home");
    home->setText(1,"home");
    home->setIcon(0,QIcon::fromTheme("user-home"));

    docs = new QTreeWidgetItem(this);
    docs->setText(0,"Documents");
    docs->setText(1,"documents");
    docs->setIcon(0,QIcon::fromTheme("folder-documents"));

    pics = new QTreeWidgetItem(this);
    pics->setText(0,"Pictures");
    pics->setText(1,"pictures");
    pics->setIcon(0,QIcon::fromTheme("folder-pictures"));

    downloads = new QTreeWidgetItem(this);
    downloads->setText(0,"Downloads");
    downloads->setText(1,"downloads");
    downloads->setIcon(0,QIcon::fromTheme("folder-download"));

    music = new QTreeWidgetItem(this);
    music->setText(0,"Music");
    music->setText(1,"music");
    music->setIcon(0,QIcon::fromTheme("folder-music"));

    videos = new QTreeWidgetItem(this);
    videos->setText(0,"Videos");
    videos->setText(1,"videos");
    videos->setIcon(0,QIcon::fromTheme("folder-videos"));

    templates = new QTreeWidgetItem(this);
    templates->setText(0,"Templates");
    templates->setText(1,"templates");
    templates->setIcon(0,QIcon::fromTheme("folder-templates"));

    trash = new QTreeWidgetItem(this);
    trash->setText(0,"Trash");
    trash->setText(1,"trash");
    trash->setIcon(0,QIcon::fromTheme("user-trash"));

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(onItemClicked(QTreeWidgetItem*)));
}

PlacesList::~PlacesList() {
    delete home;
    delete docs;
    delete pics;
    delete downloads;
    delete music;
    delete videos;
    delete templates;
    delete trash;
}

void PlacesList::onItemClicked(QTreeWidgetItem *item) {
    QString title = item->text(1);
    if (title=="home") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    } else if (title=="documents") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    } else if (title=="pictures") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    } else if (title=="downloads") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    } else if (title=="music") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
    } else if (title=="videos") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
#ifndef _WIN32
    } else if (title=="templates") {
        TabWidget::currentWidget()->loadDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/Templates");
#endif
    } else if (title=="trash") {
        TabWidget::currentWidget()->loadDir(Trash::folderPath);
    }
}

//DevicesList
//This displays removable devices on the user's computer

DeviceList::DeviceList() {
    this->setHeaderLabel("Devices");
    this->setColumnCount(1);

    loadDrives();

    timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&DeviceList::loadDrives);
    timer->start(3000);

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(onItemClicked(QTreeWidgetItem*)));
}

DeviceList::~DeviceList() {
    timer->stop();
    delete timer;
}

void DeviceList::loadDrives() {
    this->clear();
    auto list = QStorageInfo::mountedVolumes();
    foreach (QStorageInfo d, list) {
        if ((!d.rootPath().startsWith("/run"))&&(!d.rootPath().startsWith("/boot"))) {
            QTreeWidgetItem *item = new QTreeWidgetItem(this);
            item->setText(0,d.displayName());
            item->setText(1,d.rootPath());
            item->setToolTip(0,d.device());
            item->setIcon(0,QIcon::fromTheme("drive-removable-media"));
        }
    }
}

void DeviceList::onItemClicked(QTreeWidgetItem *item) {
    QString text = item->text(1);
    TabWidget::currentWidget()->loadDir(text);
}
