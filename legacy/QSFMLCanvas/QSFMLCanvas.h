#ifndef _Q_SFML_CANVAS_
#define _Q_SFML_CANVAS_

#include <SFML/Graphics.hpp>
#include <qwidget.h>
#include <qtimer.h>
#include <QFrame>

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
  public :

    explicit QSFMLCanvas(QWidget *parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);
    virtual void showEvent(QShowEvent*);
    virtual QPaintEngine* paintEngine() const;
    virtual void paintEvent(QPaintEvent*);
    virtual ~QSFMLCanvas();
    virtual void OnInit();
    virtual void OnUpdate();
private :

    QTimer myTimer;
    bool myInitialized;
};
#endif
