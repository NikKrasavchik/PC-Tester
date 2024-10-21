#include "qsliderbutton.h"
#include <QPainter>
#include <QMouseEvent>

QSliderButton::QSliderButton(QWidget* parent)
{
	this->setParent(parent);
	status = AUTO_STAND;
}

void QSliderButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	QColor roundColor = QColor(ROUND_COLOR); // ÷вет кружка
	QColor bgColor = QColor(BG_COLOR);		 // ÷вет фона

	painter.setPen(QPen(Qt::black, 3));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(QColor("#fff"), 0.1));

	// –исуем фон
	painter.setBrush(bgColor);
	painter.drawRoundedRect(0, 0, sizeWidth, sizeHeight, sizeHeight / 2, sizeHeight / 2);

	//–исуем шарик
	if (this->status == MANUAL_STAND)
	{
		painter.setBrush(roundColor);

		painter.drawEllipse(0, 0, sizeHeight, sizeHeight);
	}
	else
	{
		painter.setBrush(roundColor);
		painter.drawEllipse(sizeWidth - sizeHeight, 0, sizeHeight, sizeHeight);
	}
}

void QSliderButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (this->status)
			this->status = MANUAL_STAND;
		else
			this->status = AUTO_STAND;

		on_sliderSwitchStand_click();

		repaint();
	}
}
int QSliderButton::getStatus()
{
	return status;
}

void QSliderButton::setStatus(int newValue)
{
	if (newValue == MANUAL_STAND || newValue == AUTO_STAND)
		status = newValue;
	repaint();
}

void QSliderButton::resizeSlider(int newWidth, int newHeight)
{
	sizeWidth = newWidth;
	sizeHeight = newHeight;
}