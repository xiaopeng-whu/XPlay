#include "XSlider.h"
void XSlider::mousePressEvent(QMouseEvent* e)
{
	double pos = (double)e->pos().x() / (double)width();
	setValue(pos * this->maximum());
	//原有事件处理
	QSlider::mousePressEvent(e);
	//QSlider::sliderReleased();  //解决了最后部分的点击seek，但导致无法拖动seek
}

XSlider::XSlider(QWidget *parent)
	: QSlider(parent)
{
}

XSlider::~XSlider()
{
}
