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
#include <QVariant>
#include <QIcon>
#include <QApplication>
#include <QMenuBar>
#ifdef _WIN32
    //Windows: Use the registry
#else
#include "settings.hh"
#endif

#include "window.hh"
#include "actions.hh"
#include "clipboard.hh"

#ifndef _WIN32
using namespace CppLib;
#endif

AddressBarText *Window::addrTxt;
AddressBarButtons *Window::addrButtons;

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("CppExplorer");
    this->setWindowIcon(QIcon::fromTheme("system-file-manager",QPixmap(":/icons/system-file-manager.svg")));
    this->menuBar()->setContextMenuPolicy(Qt::PreventContextMenu);

#ifdef _WIN32
    //Windows: Use the registry
    int winX = 900;
    int winY = 500;
#else
    int winX = QVariant(Settings::getSetting("window/x","700")).toInt();
    int winY = QVariant(Settings::getSetting("window/y","500")).toInt();
#endif
    this->resize(winX,winY);

    filemenu = new FileMenu(this);
    editmenu = new EditMenu;
    viewmenu = new ViewMenu;
    helpmenu = new HelpMenu;

    this->menuBar()->addMenu(filemenu);
    this->menuBar()->addMenu(editmenu);
    this->menuBar()->addMenu(viewmenu);
    this->menuBar()->addMenu(helpmenu);

    navbar = new NavBar;
    this->addToolBar(navbar);

    addrTxt = new AddressBarText;
    addrTxt->hide();
    this->addToolBar(addrTxt);

    addrButtons = new AddressBarButtons;
    this->addToolBar(addrButtons);

    tabPane = new TabWidget(navbar);
    this->setCentralWidget(tabPane);

    sidebar = new SideBar;
    this->addDockWidget(Qt::LeftDockWidgetArea,sidebar);
}

Window::~Window() {
    delete filemenu;
    delete editmenu;
    delete viewmenu;
    delete helpmenu;
    delete navbar;
    delete addrTxt;
    delete addrButtons;
    delete tabPane;
    delete sidebar;
}

void Window::closeApp() {
#ifdef _WIN32
    //Windows: Use the registry
#else
    Settings::writeSetting("window/x",QVariant(this->width()).toString());
    Settings::writeSetting("window/y",QVariant(this->height()).toString());
#endif
}

void Window::closeEvent(QCloseEvent *event) {
    closeApp();
    event->accept();
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers()==Qt::ControlModifier) {
        switch (event->key()) {
        case Qt::Key_Q: {
            closeApp();
            qApp->exit(0);
        } break;
        case Qt::Key_T: TabWidget::addNewTab(); break;
        case Qt::Key_N: Actions::newFile(); break;
        case Qt::Key_X: {
            clipboard.fileName = TabWidget::currentWidget()->currentItemNames();
            clipboard.oldPath = TabWidget::currentWidget()->fsCurrentPath();
            clipboard.action = Clipboard_Actions::CUT;
        } break;
        case Qt::Key_C: {
            clipboard.fileName = TabWidget::currentWidget()->currentItemNames();
            clipboard.oldPath = TabWidget::currentWidget()->fsCurrentPath();
            clipboard.action = Clipboard_Actions::COPY;
        } break;
        case Qt::Key_V: {
            clipboard.newPath = TabWidget::currentWidget()->fsCurrentPath();
            Actions::paste();
        } break;
        }
    } else if (event->modifiers()==(Qt::ControlModifier | Qt::ShiftModifier)) {
        switch (event->key()) {
        case Qt::Key_N: Actions::newFolder(); break;
        case Qt::Key_T: TabWidget::closeCurrentTab(); break;
        }
    }
    QMainWindow::keyPressEvent(event);
}
