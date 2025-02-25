#ifndef QSLIDERBUTTON_H
#define QSLIDERBUTTON_H

#include <QWidget>
#include "Components.h"

#define ROUND_COLOR "#979797"
#define BG_COLOR    "#E8E8E8"

class QSliderButton : public QWidget
{
	Q_OBJECT
public:
	explicit QSliderButton(bool isHorizontal, QWidget* parent);

	TypeStand getStatus();
	void setStatus(TypeStand newValue);
	void setStyleSheet(QString roundColor, QString bgColor);

private:
	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

	TypeStand status; // Положение слайдера. 0 - кружок слева (manual) / 1 - кружок справа (auto)

	int sizeWidth;
	int sizeHeight;

	QColor roundColor;
	QColor bgColor;

	bool isHorizontal;

public slots:
	void resizeSlider(int newWidth, int newHeight);

signals:
	void on_sliderSwitchStand_click();
};

#endif QSLIDERBUTTON_H