/***********************************************************************************************************************
** Simple Qt DiceRoller Program
** mainwindow.cpp
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
#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include "dicewidget.hpp"

#include <QInputDialog>
#include <QVBoxLayout>
#include <QTextCursor>

class MainWindow::Impl
{
public:
	Impl(MainWindow *top) : top(top)
	{
		ui.setupUi(top);
		diceLayout = dynamic_cast<QVBoxLayout *>(ui.diceAreaLayout);
		if (!diceLayout) throw false;
		historyCursor = QTextCursor(ui.historyEdit->document());
	}

	void connectNewDiceWidget(int index, DiceWidget *dw)
	{
		diceLayout->insertWidget(index, dw);
		QObject::connect(dw, SIGNAL(cloneMe(DiceWidget*)), top, SLOT(cloneDiceWidget(DiceWidget*)));
		QObject::connect(dw, SIGNAL(removeMe(DiceWidget*)), top, SLOT(removeDiceWidget(DiceWidget*)));
	}

	void removeDiceWidget(DiceWidget *dw)
	{
		diceLayout->removeWidget(dw);
		dw->hide();
		dw->deleteLater();
	}

	MainWindow *top;
	Ui::mainWindow ui;
	QVBoxLayout *diceLayout;
	QTextCursor historyCursor;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    im(std::make_unique<MainWindow::Impl>(this))
{
	// No implementation.
}

MainWindow::~MainWindow()
{
	// No implementation.
}

void MainWindow::addDie(unsigned int top)
{
	im->connectNewDiceWidget(-1, new DiceWidget(top, im->ui.diceArea));
}

void MainWindow::addD4()
{
	addDie(4);
}

void MainWindow::addD6()
{
	addDie(6);
}

void MainWindow::addD8()
{
	addDie(8);
}

void MainWindow::addD10()
{
	addDie(10);
}

void MainWindow::addD12()
{
	addDie(12);
}

void MainWindow::addD20()
{
	addDie(20);
}

void MainWindow::addD100()
{
	addDie(100);
}

void MainWindow::addCustomDie()
{
	bool userDidntCancel = true;
	int dieSize = QInputDialog::getInt(this, tr("What size die?"),
	                                   tr("Please enter the number of sides on the new die:"),
	                                   6, 2, 9999, 1, &userDidntCancel);
	if (userDidntCancel)
	{
		addDie(dieSize);
	}
}

void MainWindow::cloneDiceWidget(DiceWidget *dw)
{
	if (dw)
	{
		im->connectNewDiceWidget(im->diceLayout->indexOf(dw) + 1, new DiceWidget(*dw));
	}
}

void MainWindow::removeDiceWidget(DiceWidget *dw)
{
	if (dw)
	{
		im->removeDiceWidget(dw);
	}
}

void MainWindow::rollEm()
{
	int i = 0;
	std::vector<DiceRoll> rolls;
	for (QLayoutItem *item; (item = im->diceLayout->itemAt(i++));)
	{
		if (DiceWidget *dw = dynamic_cast<DiceWidget *>(item->widget()))
		{
			rolls.emplace_back(dw->rollMe());
		}
	}

	QString output = tr("Roll Results:\n");
	int64_t total = 0;
	for (DiceRoll const &roll : rolls)
	{
		QString rollOutput = QString::number(roll.numberOfDice) + tr(" D") +
		                     QString::number(roll.faces) + tr("s results: ");
		int64_t rollSum = 0;
		uint32_t offset = rollOutput.length();
		for (uint8_t die = 0; die < roll.numberOfDice; ++die)
		{
			rollOutput += QString::number(roll.rollAt(die)) + " + " +
			              QString::number(roll.addDie) + " * " +
			              QString::number(roll.multDie) + " = " +
			              QString::number(roll.rollTotal(die)) + "\n";
			if (die)
			{
				rollOutput = rollOutput.rightJustified(rollOutput.length() + offset);
			}

			rollSum += roll.rollTotal(die);
			output.append(rollOutput);
			rollOutput = "";
		}

		QString totalOutput = "Total: ";
		if (totalOutput.length() < offset)
		{
			totalOutput = totalOutput.leftJustified(offset);
		}

		totalOutput += QString::number(rollSum) + " + " +
		               QString::number(roll.addRoll) + " * " +
		               QString::number(roll.multRoll) + " = " +
		               QString::number(roll.total()) + "\n";
		output.append(totalOutput);
		total += roll.total();
	}

	QString totalStr = QString::number(total);
	im->ui.totalLabel->setText(totalStr);
	output += "\nGrand Total: " + totalStr + "\n\n";
	im->historyCursor.insertText(output);
	im->historyCursor.setPosition(0);
}

void MainWindow::clearWidgets()
{
	std::vector<DiceWidget *> dws;
	int i = 0;
	for (QLayoutItem *item; (item = im->ui.diceAreaLayout->itemAt(i++));)
	{
		if (DiceWidget *dw = dynamic_cast<DiceWidget *>(item->widget()))
		{
			dws.push_back(dw);
		}
	}

	for (DiceWidget *dw : dws)
	{
		im->removeDiceWidget(dw);
	}
}

void MainWindow::clearHistory()
{
	im->ui.historyEdit->clear();
}
