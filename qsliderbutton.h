#ifndef QSLIDERBUTTON_H
#define QSLIDERBUTTON_H

#include <QWidget>
#include "Components.h"

class QSliderButton : public QWidget
{
	Q_OBJECT

public:
	explicit QSliderButton(bool isHorizontal, QWidget* parent);

	// ------------------------------------
	// Name: getStatus
	//		Возврат текущего статуса
	// Return: 
	//			TypeStand: Переменная, хранящая в себе информацию о выбранном стенде. Возвращаемые данные:
 //    		MANUAL
 //    		AUTO
	// ------------------------------------
	TypeStand getStatus();

	// ------------------------------------
	// Name: setStatus
	//		Установка текущего статуса
	// Variables: 
	//			TypeStand newValue: Переменная, хранящая в себе информация о выбранном стенде. Обрабатываемые аргументы:
 //    				MANUAL
 //    				AUTO
	// ------------------------------------
	void setStatus(TypeStand newValue);

	// ------------------------------------
	// Name: setStyleSheet
	//		Установка стиля
	// Variables: 
	//			QString roundColor: Переменная, хранящая в себе информацию о стиле грани
	//			QString bgColor: Переменная, хранящая в себе информацию о стиле заднего фона
	// ------------------------------------
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