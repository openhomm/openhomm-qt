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
//#include "precompiled.hpp"
#pragma once

#include "hrScene.hpp"
#include "hrCache.hpp"

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#endif

#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#endif

class hrRender : public QGLWidget
{
    Q_OBJECT

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public:
    hrRender(QWidget *parent, hrScene *scene);
    ~hrRender();

    void setScene(hrScene* scene);

private:
    hrScene* scene;
    GLuint target;

    void drawSprite(GLuint tx, QRect r, bool horizontal = false, bool vertical = false);
    void checkExtensions();

private slots:
    void mouseMoveEvent(QMouseEvent * event);
    void leaveEvent(QEvent * event);

    void onSceneChanged();
    void onCursorChanged(int type);
};
