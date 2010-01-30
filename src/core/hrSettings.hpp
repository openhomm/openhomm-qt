// openhomm - open source clone of Heroes of Might and Magic III
// Copyright (C) 2009-2010 openhomm developers team (see AUTHORS)
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

// List settings from Heroes 3. Delete later

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
QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class hrSettings : public QObject
{
    Q_OBJECT

    /*!
      This property holds whether the application is run in full screen mode.

      By default, this property is false.
    */
    Q_PROPERTY(bool fullscreen READ isFullscreen WRITE setFullscreen)

    /*!
      By default, this property is false
    */
    Q_PROPERTY(bool showmenu READ isShowmenu WRITE setShowmenu)
    
    /*!
      By default, this property is 0
    */
    Q_PROPERTY(qint32 x READ x WRITE setX)
    
    /*!
      By default, this property is 0
    */
    Q_PROPERTY(qint32 y READ y WRITE setY)
    
    /*!
      If no render has been set, the render is \b sdl.
    */
    Q_PROPERTY(QString render READ render WRITE setRender)
    
    /*!
      By default, this property is 2
    */
    Q_PROPERTY(quint8 windowScrollSpeed READ windowScrollSpeed WRITE setWindowScrollSpeed)

    /*!
        This property stores the location of the game.
        By default, this property indicates the current directory.
    */
    Q_PROPERTY(QString gameDir READ gameDir WRITE setGameDir)
public:
    hrSettings(QObject *parent = 0);
    ~hrSettings();

    /*!
      Returns a reference to the application's hrSettings instance.
    */
    static hrSettings& get()
    {
        static hrSettings settings;
        return settings;
    }

    bool isFullscreen() const   { return _isFullscreen; }
    void setFullscreen (bool fullscreen = true);

    bool isShowmenu() const     { return _isShowmenu; }
    void setShowmenu (bool showmenu = true);

    qint32 x() const   { return _x; }
    void setX (qint32 x);

    qint32 y() const { return _y; }
    void setY (qint32 y);

    QString render() const { return _render; }
    void setRender(const QString &render);

    quint8 windowScrollSpeed() const { return _windowScrollSpeed; }
    void setWindowScrollSpeed(quint8 speed);

    QString gameDir() const { return _gameDir; }
    void setGameDir(const QString &gamedir);
private:
    bool _isAnimateSpellBook;    // not used yet
    bool _isAutosave;            // not used yet
    bool _isBlackoutComputer;    // not used yet
    quint8 _lastMusicVolume;     // not used yet
    quint8 _lastSoundVolume;     // not used yet
    bool _isFullscreen;
    bool _isShowmenu;
    qint32 _x, _y;
    QString _render;
    QString _gameDir;
    quint8 _musicVolume;         // not used yet
    bool _isQuickCombat;         // not used yet
    bool _isShowCombatGrid;      // not used yet
    bool _isShowCombatMouseHex;  // not used yet
    bool _isShowIntro;           // not used yet
    bool _isShowRoute;           // not used yet
    bool _isTownOutlines;        // not used yet
    bool _isVideoSubtitles;      // not used yet
    quint8 _walkSpeed;           // not used yet
    quint8 _windowScrollSpeed;
    QSettings * _settings;
};
