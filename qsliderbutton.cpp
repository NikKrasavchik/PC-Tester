#include "qsliderbutton.h"
#include <QPainter>
#include <QMouseEvent>

QSliderButton::QSliderButton(QWidget* parent)
{
	this->setParent(parent);
}

void QSliderButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	QColor roundColor = QColor(ROUND_COLOR); // ���� ������
	QColor bgColor = QColor(BG_COLOR);		 // ���� ����

	painter.setPen(QPen(Qt::black, 3));
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setPen(QPen(QColor("#fff"), 0.1));

	// ������ ���
	painter.setBrush(bgColor);
	painter.drawRoundedRect(0, 0, sizeWidth, sizeHeight, sizeHeight / 2, sizeHeight / 2);

	//������ �����
	if (this->status == MANUAL)
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
			this->status = MANUAL;
		else
			this->status = AUTO;

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
	if (newValue == MANUAL || newValue == AUTO)
		status = newValue;
	repaint();
}

void QSliderButton::resizeSlider(int newWidth, int newHeight)
{
	sizeWidth = newWidth;
	sizeHeight = newHeight;
}