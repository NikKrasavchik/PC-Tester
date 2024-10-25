#include "TestWindow.h"

void TestWindow::initUiFullTestManualStand()
{
	fullTestManualStandSortButton = new QPushButton(usefulSpaceWidget);
	fullTestManualStandSortButton->setObjectName("fullTestManualStandSortButton");
	fullTestManualStandSortButton->setFixedSize(QSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT));
	usefulSpaceHLayout->addWidget(fullTestManualStandSortButton);

	initUiTableFullTestManualStand();
}

void TestWindow::initUiTableFullTestManualStand()
{

}