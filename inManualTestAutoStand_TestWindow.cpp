#include "TestWindow.h"

void TestWindow::initUiInManualTestAutoStand()
{
	inManualTestAutoStandConnectButton = new QPushButton(usefulSpaceWidget);
	inManualTestAutoStandConnectButton->setObjectName("inManualTestAutoStandConnectButton");
	inManualTestAutoStandConnectButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(inManualTestAutoStandConnectButton);

	inManualTestAutoStandTestTimeComboBox = new QComboBox(usefulSpaceWidget);
	inManualTestAutoStandTestTimeComboBox->setObjectName("inManualTestAutoStandTestTimeComboBox");
	inManualTestAutoStandTestTimeComboBox->setFixedSize(QSize(FIXED_HEADER_COMBO_WIDTH, FIXED_HEADER_COMBO_HEIGHT));
	usefulSpaceHLayout->addWidget(inManualTestAutoStandTestTimeComboBox);

	initUiTableInManualTestAutoStand();
}

void TestWindow::initUiTableInManualTestAutoStand()
{
	mainTableWidget->setColumnCount(7);
	mainTableWidget->setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("��������")
															<< QString::fromLocal8Bit("���")
															<< QString::fromLocal8Bit("�����")
															<< QString::fromLocal8Bit("��")
															<< QString::fromLocal8Bit("���������"));
}

void TestWindow::on_inManualTestAutoStandConnectButton_clicked()
{

}

void TestWindow::on_inManualTestAutoStandTestTimeComboBox_changed(int ind)
{

}