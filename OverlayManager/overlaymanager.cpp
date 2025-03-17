#include "overlaymanager.h"
#include <QApplication>
#include <QPainter>
#include <QScreen>
#include <windows.h>



OverlayManager::OverlayManager(QWidget *parent)
    : QWidget(parent), displayText("")
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);

    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        resize(screenGeometry.size());
    } else {
        resize(800, 600);
    }

    fadeAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    fadeAnimation->setDuration(3000);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        qDebug()<<"here";
        displayText.clear();
        close();
    });
}

void OverlayManager::showOverlay()
{
    HWND hDota2 = FindWindowA(NULL, "Dota 2");
    HWND hForeground = GetForegroundWindow();
    if (hForeground != hDota2) {
        qDebug() << "The Dota 2 window does not have a focus, overlay is not shown.";
        return;
    }

    setWindowOpacity(1.0);
    showFullScreen();
    fadeAnimation->start();

    hDota2 = FindWindowA(NULL, "Dota 2");
    if (hDota2 != NULL) {
        if (!SetForegroundWindow(hDota2)) {
            qDebug() << "Couldn't set focus on dota 2";
        }
    }
}


void OverlayManager::setOverlayText(const QString &newText)
{
    displayText = newText;
    update();
}

void OverlayManager::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 48, QFont::Bold));

    QRect rect = this->rect();
    painter.drawText(rect, Qt::AlignCenter, displayText);
}
