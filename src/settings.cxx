// Copyright 2017 Patrick Flynn
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
#include <QFile>
#include <QTextStream>
#include <iostream>

#include "settings.hh"
#include "tinyxml2.h"

using namespace CppLib;
using namespace tinyxml2;

QString Settings::appName = "org.test.app";
QString Settings::defaultSettingsPath = "";
QString Settings::settingsPath = "";

void Settings::registerApp(QString app) {
    appName = app;
}

#ifdef _WIN32
//Windows path:
//%USERPROFILE%\AppData\Local\<app name>\settings.xml
void Settings::initPaths() {
    settingsPath = QDir::homePath();
    if (!settingsPath.endsWith("\\")) {
        settingsPath+="\\";
    }
    settingsPath+="AppData\\Local\\"+appName;
    if (!QDir(settingsPath).exists()) {
        QDir().mkpath(settingsPath);
    }
    settingsPath+="\\settings.xml";
    if (!QFile(settingsPath).exists()) {
        writeSettingsFile();
    }
}
#elif HAIKU_OS
//Haikus OS path:
//$HOME/config/settings/<app name>/settings.xml
void Settings::initPaths() {
    settingsPath = QDir::homePath();
    if (!settingsPath.endsWith("/")) {
        settingsPath+="/";
    }
    settingsPath+="config/settings/"+appName;
    if (!QDir(settingsPath).exists()) {
        QDir().mkpath(settingsPath);
    }
    settingsPath+="/settings.xml";
    if (!QFile(settingsPath).exists()) {
        writeSettingsFile();
    }
}
#else
//UNIX path:
//$HOME/.<app name>/settings.xml
void Settings::initPaths() {
    settingsPath = QDir::homePath();
    if (!settingsPath.endsWith("/")) {
        settingsPath+="/";
    }
    settingsPath+="."+appName;
    if (!QDir(settingsPath).exists()) {
        QDir().mkpath(settingsPath);
    }
    settingsPath+="/settings.xml";
    if (!QFile(settingsPath).exists()) {
        writeSettingsFile();
    }
}
#endif

void Settings::setDefaultSettingsFile(QString dSettings) {
    defaultSettingsPath = dSettings;
}

QString Settings::getSetting(QString id, QString defaultSetting) {
    return getAttributeSetting(id,nullptr,defaultSetting);
}

QString Settings::getAttributeSetting(QString id, QString attrID, QString defaultSetting) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(settingsPath.toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("settings");
    if (root==nullptr) {
        return defaultSetting;
    }
    XMLElement *target = getElement(root,id);
    if (target==nullptr) {
        return defaultSetting;
    }
    QString data = defaultSetting;
    if (attrID==nullptr) {
        data = QString(target->GetText());
        if (data==nullptr) {
            data = defaultSetting;
        }
    } else {
        data = QString(target->Attribute(attrID.toStdString().c_str()));
        if (data==nullptr) {
            data = defaultSetting;
        }
    }
    return data;
}

void Settings::writeSetting(QString id, QString attr, QString attrValue, QString text) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(settingsPath.toStdString().c_str());
    XMLElement *root = doc->FirstChildElement("settings");
    if (root==nullptr) {
        return;
    }
    XMLElement *target = getElement(root,id);
    if (target==nullptr) {
        return;
    }
    target->SetText(text.toStdString().c_str());
    if (attr!=nullptr) {
        target->SetAttribute(attr.toStdString().c_str(),attrValue.toStdString().c_str());
    }
    doc->SaveFile(settingsPath.toStdString().c_str());
}

void Settings::writeSetting(QString id, QString text) {
    Settings::writeSetting(id,nullptr, nullptr,text);
}

XMLElement *Settings::getElement(XMLElement *root, QString path) {
    XMLElement *element = nullptr;
    bool isFirst = true;
    QString currentItem = "";
    QString lastItem = "";
    for (int i = 0; i<path.length(); i++) {
        if (path.at(i)=='/') {
            lastItem = currentItem;
            currentItem = "";
            if (isFirst) {
                isFirst = false;
                element = root->FirstChildElement(lastItem.toStdString().c_str());
                if (element==nullptr) {
                    return nullptr;
                }
            } else {
                element = element->FirstChildElement(lastItem.toStdString().c_str());
                if (element==nullptr) {
                    return nullptr;
                }
            }
        } else {
            currentItem+=path.at(i);
        }
    }
    if (isFirst) {
        element = root->FirstChildElement(currentItem.toStdString().c_str());
    } else {
        element = element->FirstChildElement(currentItem.toStdString().c_str());
    }
    return element;
}

void Settings::writeSettingsFile() {
    QFile file(defaultSettingsPath);
    QString content = "";
    if (file.open(QFile::ReadOnly)) {
        QTextStream reader(&file);
        while (!reader.atEnd()) {
            content+=reader.readLine();
            file.close();
        }
    }

    QFile file2(settingsPath);
    if (file2.open(QFile::ReadWrite)) {
        QTextStream writer(&file2);
        writer << content;
        file2.close();
    }
}
