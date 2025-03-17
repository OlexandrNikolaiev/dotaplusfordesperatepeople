#pragma once

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class OverlayManager : public QWidget
{
    Q_OBJECT
public:
    explicit OverlayManager(QWidget *parent = nullptr);

    void showOverlay();
    void setOverlayText(const QString &newText);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QString displayText;
    QGraphicsOpacityEffect* opacityEffect;
    QPropertyAnimation* fadeAnimation;
};
