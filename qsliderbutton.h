#ifndef QSLIDERBUTTON_H
#define QSLIDERBUTTON_H

#include <QWidget>

#define OFF 0
#define ON  1

class QSliderButton : public QWidget
{
	Q_OBJECT
public:
	explicit QSliderButton(QWidget* parent);

	int getStatus();
	void setStatus(int value);

private:
	virtual void paintEvent(QPaintEvent* event);
	virtual QSize sizeHint() const;
	virtual void mousePressEvent(QMouseEvent* event);

	int status = 0; // ��������� ��������. 0 - ������ ������ / 1 - ������ ������
};
#endif QSLIDERBUTTON_H