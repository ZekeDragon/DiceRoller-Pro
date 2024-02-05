/***********************************************************************************************************************
** Simple Qt DiceRoller Program
** dicewidget.cpp
** Copyright (C) 2024 Ezekiel Oruven
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
** documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
** rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
** Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
** WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
** COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
** OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************************************/
#include "dicewidget.hpp"
#include "ui_dicewidget.h"

class DiceWidget::Impl
{
public:
	uint16_t type, amount;
	Ui::DiceWidget ui;
};

DiceWidget::DiceWidget(unsigned int dieType, QWidget *parent) :
    QWidget(parent),
    im(std::make_unique<DiceWidget::Impl>())
{
	im->type = dieType;
	im->amount = 1;
	im->ui.setupUi(this);
	im->ui.diceTypeLabel->setText("d" + QString::number(dieType));
}

DiceWidget::DiceWidget(const DiceWidget &other) :
    QWidget(other.parentWidget()),
    im(std::make_unique<DiceWidget::Impl>())
{
	im->type = other.im->type;
	im->amount = other.im->amount;
	Ui::DiceWidget &myUi = im->ui,
	               &otherUi = other.im->ui;
	myUi.setupUi(this);
	myUi.diceTypeLabel->setText("d" + QString::number(im->type));
	myUi.numDiceLineEdit->setText(otherUi.numDiceLineEdit->text());
	myUi.perDieAddSpinBox->setValue(otherUi.perDieAddSpinBox->value());
	myUi.perDieMultSpinBox->setValue(otherUi.perDieMultSpinBox->value());
	myUi.totalAddSpinBox->setValue(otherUi.totalAddSpinBox->value());
	myUi.totalMultSpinBox->setValue(otherUi.totalMultSpinBox->value());
}

DiceWidget::~DiceWidget()
{
	// No implementation.
}

DiceRoll DiceWidget::rollMe()
{
	int32_t addDie = im->ui.perDieAddSpinBox->value(),
	        multDie = im->ui.perDieMultSpinBox->value(),
	        addTotal = im->ui.totalAddSpinBox->value(),
	        multTotal = im->ui.totalMultSpinBox->value();
	return { uint8_t(im->amount), im->type, addDie, multDie, addTotal, multTotal };
}

void DiceWidget::remove()
{
	emit removeMe(this);
}

void DiceWidget::clone()
{
	emit cloneMe(this);
}

void DiceWidget::updateAmount(QString amount)
{
	bool good = true;
	if (uint16_t am = amount.toUShort(&good); good)
	{
		im->amount = am;
	}
}
