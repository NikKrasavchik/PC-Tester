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

	painter.setPen(QPen(Qt::black, 3));

	painter.setRenderHint(QPainter::Antialiasing, true);

	painter.setPen(QPen(QColor("#fff"), 0.1));

	QString bgColorTxt = "#ffffff";
	QColor bgColor = QColor(bgColorTxt);

	painter.setBrush(bgColor);

	painter.drawRoundedRect(0, 0, 2000, 50, 10, 10);
	this->resize(200,100);
	//this->setGeometry(0,0,20,20);

	// QString onColor = "#444";
	// QColor mainColorOn = QColor(onColor);
	// QColor subColorOn = QColor(onColor);
	// subColorOn.setHsl(100, 100, 95, 100);

	// //Рисуем шарик
	// if (this->status == ON)
	// {
	// 	QLinearGradient linearGrad(QPointF(32, 2), QPointF(46, 16));
	// 	linearGrad.setColorAt(0, subColorOn);
	// 	linearGrad.setColorAt(1, mainColorOn);

	// 	painter.setBrush(linearGrad);

	// 	painter.drawEllipse(60, 20, 30, 30);
	// }
	// else
	// {
	// 	QLinearGradient linearGrad(QPointF(2, 2), QPointF(16, 16));
	// 	linearGrad.setColorAt(0, subColorOn);
	// 	linearGrad.setColorAt(1, mainColorOn);

	// 	painter.setBrush(linearGrad);

	// 	painter.drawEllipse(10, 20, 20, 20);
	// }
}
QSize QSliderButton::sizeHint() const
{
	return QSize(50, 20);
}
void QSliderButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (this->status)
			this->status = OFF;
		else
			this->status = ON;
		repaint();
	}
}
int QSliderButton::getStatus()
{
	return status;
}

void QSliderButton::setStatus(int value)
{
	status = value;
	repaint();
}