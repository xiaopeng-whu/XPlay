#pragma once

#include <QtWidgets/QWidget>
#include "ui_XPlay2.h"

class XPlay2 : public QWidget
{
	Q_OBJECT

public:
	XPlay2(QWidget *parent = Q_NULLPTR);
	~XPlay2();

	//定时器 滑动条显示
	void timerEvent(QTimerEvent* e);

	//窗口尺寸变化
	void resizeEvent(QResizeEvent* e);

	//双击全屏
	void mouseDoubleClickEvent(QMouseEvent* e);
	void SetPause(bool isPause);
public	slots:
	void OpenFile();
	void PlayOrPause();
	void SliderPress();
	void SliderRelease();
private:
	bool isSliderPress = false;
	Ui::XPlay2Class ui;
};
