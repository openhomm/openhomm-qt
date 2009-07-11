// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009 openhomm developers team (see AUTHORS)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#pragma once

#include <QApplication>
#include <QObject>
#include <QSettings>

class hrLodEngineHandler;
class hrSndEngineHandler;

//Animate SpellBook=dword:00000001
//Autosave=dword:00000001
//Bink Video=dword:00000000
//Blackout Computer=dword:00000000
//Combat Army Info Level=dword:00000000
//Combat Auto Creatures=dword:00000001
//Combat Auto Spells=dword:00000001
//Combat Ballista=dword:00000001
//Combat Catapult=dword:00000001
//Combat First Aid Tent=dword:00000001
//Combat Shade Level=dword:00000000
//Combat Speed=dword:00000000
//Computer Walk Speed=dword:00000003
//First Time=dword:00000001
//Last Music Volume=dword:00000005
//Last Sound Volume=dword:00000005
//Main Game Full Screen=dword:00000001
//Main Game Show Menu=dword:00000001
//Main Game X=dword:0000000a
//Main Game Y=dword:0000000a
//Move Reminder=dword:00000001
//Music Volume=dword:00000005
//Network Default Name="Игрок"
//Quick Combat=dword:00000000
//Show Combat Grid=dword:00000000
//Show Combat Mouse Hex=dword:00000000
//Show Intro=dword:00000001
//Show Route=dword:00000001
//Sound Volume=dword:00000005
//Test Blit=dword:00000000
//Test Decomp=dword:00000000
//Test Read=dword:00000000
//Town Outlines=dword:00000001
//Unique System ID="LGG"
//Video Subtitles=dword:00000001
//Walk Speed=dword:00000002
//Window Scroll Speed=dword:00000001


struct hrSettings
{
    bool isAnimateSpellBook;    // not used yet
    bool isAutosave;            // not used yet
    bool isBlackoutComputer;    // not used yet
    quint8 lastMusicVolume;     // not used yet
    quint8 lastSoundVolume;     // not used yet
    bool isGameFullscreen;
    bool isGameShowMenu;        // not used yet
    quint32 gameX, gameY;
    quint8 musicVolume;         // not used yet
    bool isQuickCombat;         // not used yet
    bool isShowCombatGrid;      // not used yet
    bool isShowCombatMouseHex;  // not used yet
    bool isShowIntro;           // not used yet
    bool isShowRoute;           // not used yet
    bool isTownOutlines;        // not used yet
    bool isVideoSubtitles;      // not used yet
    quint8 walkSpeed;           // not used yet
    quint8 windowScrollSpeed;   // not used yet

    static hrSettings& get()
    {
        static hrSettings settings;
        return settings;
    }

    void save()
    {
        QSettings settings("settings.ini", QSettings::IniFormat);
        settings.setValue("isGameFullscreen", isGameFullscreen);
        settings.setValue("isGameShowMenu", isGameShowMenu);
        settings.setValue("gameX", gameX);
        settings.setValue("gameY", gameY);
    }

    void load()
    {
        QSettings settings("settings.ini", QSettings::IniFormat);
        isGameFullscreen = settings.value("isGameFullscreen", false).toBool();
        isGameShowMenu = settings.value("isGameShowMenu", true).toBool();
        gameX = settings.value("gameX", 0).toInt();
        gameY = settings.value("gameY", 0).toInt();
    }
};

class hrApplication : public QApplication
{
    Q_OBJECT
public:
    hrApplication(int &argc, char **argv);
    ~hrApplication();
    static QString getMapName()
    {
        return mapName;
    }
private:
    void createFileEngineHandlers();
    void destroyFileEngineHandlers();
    void loadSettings();
// data
private:
    hrLodEngineHandler *lodHandler;
    hrSndEngineHandler *sndHandler;

    static QString mapName;
};
