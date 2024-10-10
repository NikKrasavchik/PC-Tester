#ifndef QSLIDERBUTTON_H
#define QSLIDERBUTTON_H

#include <QWidget>

#define MANUAL 0
#define AUTO  1

#define ROUND_COLOR "#979797"
#define BG_COLOR    "#E8E8E8"

class QSliderButton : public QWidget
{
	Q_OBJECT
public:
	explicit QSliderButton(QWidget* parent);

	int getStatus();
	void setStatus(int newValue);

private:
	virtual void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);

	int status = 0; // Положение слайдера. 0 - кружок слевва (manual) / 1 - кружок справа (auto)
	int sizeWidth;
	int sizeHeight;

public slots:
	void resizeSlider(int newWidth, int newHeight);

signals:
	void on_sliderSwitchStand_click();
};
#endif QSLIDERBUTTON_H