#include "stdafx.h"
#include "ChipDataWindow.h"
#include "ui_ChipDataWindow.h"
#include "GetChipWindow.h"

ChipDataWindow::ChipDataWindow(QWidget* parent, Qt::WindowFlags f):
	QDialog(parent,f),
	ui(new Ui::ChipDataWindow),
	getChipWindow(new GetChipWindow(this)),
	tableModel_(new ChipTableModel(this))
{
	ui->setupUi(this);
	connect();
	getChipWindow->init();
	this->ui->tableView->setModel(this->tableModel_);
}

ChipDataWindow::~ChipDataWindow()
{
}

void ChipDataWindow::recvChipJsonObject(const QJsonObject& object)
{
	auto squads = object["squad_with_user_info"].toObject();
	std::map<int, int> squadID;
	for(const auto& it : squads)
	{
		auto obj = it.toObject();
		squadID[obj["id"].toInt(0)] = obj["squad_id"].toInt(0);
	}

	chips_ = getChips(object["chip_with_user_info"].toObject());
	for(auto i = 0;i < chips_.size();++i)
	{
		auto& chip = chips_[i];
		chip.no = i + 1;
		chip.squad = squadID[chip.squad];
	}
	this->tableModel_->setChips(chips_);
}

void ChipDataWindow::connect()
{
	QObject::connect(
		this->ui->getDataPushButton,
		&QPushButton::clicked,
		this->getChipWindow,
		&GetChipWindow::show
	);
	QObject::connect(
		this->getChipWindow,
		&GetChipWindow::sendChipJsonObject,
		this,
		&ChipDataWindow::recvChipJsonObject
	);
}
