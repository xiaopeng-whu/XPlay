#pragma once

#include <QtWidgets/QWidget>
#include "ui_TestQtOpengl.h"

class TestQtOpengl : public QWidget
{
	Q_OBJECT

public:
	TestQtOpengl(QWidget *parent = Q_NULLPTR);

private:
	Ui::TestQtOpenglClass ui;
};
