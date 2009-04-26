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
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "precompiled.hpp"
#include <QGLWidget>

#include "hrScene.hpp"
#include "hrCoord.hpp"

class hrGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    hrGLWidget(QWidget *parent, hrScene *scene);
    ~hrGLWidget();
    void startAnimate(int delay);
    void stopAnimate();

public slots:
    void animate();
    void scroll();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //void paintEvent(QPaintEvent *event);
private:
    hrScene *scene;

    QTimer scrollTimer;
    QTimer animateTimer;
    QRect viewport;
    int dx;
    int dy;

    bool isAnimate;

    QList<hrSceneTile> tiles;
    int oldTileId;
    void drawAtlasTiles();

    QList<hrSceneObject> objects;
    void animateObjects() const;
    void drawObjects();

    void Begin();
    void End();

    void ImageToPOT(hrGraphicsItem *item, const QImage& im) const;
    qint32 NearestGLTextureSize(qint32 v) const;

    void checkExtensions();
    bool textureRects;
    bool generateMipmap;
    int textureTarget;

    int textureMaxDim;

    class GLTexture
    {
        GLuint id;
    public:
        GLTexture(GLuint id) : id(id) {}
        ~GLTexture()
        {
            glDeleteTextures(1, &id);
        }
        GLuint getId() const
        {
            return id;
        }
    };
    QCache<quint64, GLTexture> texs;

    GLuint bindImage(const QImage &im, GLuint target);
    void drawImage(const QPoint &point, const QImage &im);
    void drawAtlasImage(const QPoint &point
                       , const QRect &src
                       , int dim
                       , bool horizontal = false
                       , bool vertical = false
                       );

private slots:
    void mouseMoveEvent(QMouseEvent * event);
    void leaveEvent(QEvent * event);
};

#endif // GLWIDGET_H

