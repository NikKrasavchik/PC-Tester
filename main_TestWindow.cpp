#include "TestWindow.h"

TestWindow::TestWindow(WindowType testType, std::vector<Cable> cables, Can* can, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->testType = testType;
	this->can = can;
	standConected = false;

	statusFlags = new StandStatusFlags;

	if (testType == WindowType::IN_TEST_MANUAL_STAND ||
		testType == WindowType::OUT_TEST_MANUAL_STAND ||
		testType == WindowType::FULL_TEST_MANUAL_STAND)
	{
		th = new ManualStandTwoThread(can, cables, statusFlags);
	}
	else if (testType == WindowType::IN_MANUAL_TEST_AUTO_STAND ||
		testType == WindowType::OUT_MANUAL_TEST_AUTO_STAND ||
		testType == WindowType::IN_AUTO_TEST_AUTO_STAND ||
		testType == WindowType::OUT_AUTO_TEST_AUTO_STAND ||
		testType == WindowType::FULL_TEST_AUTO_STAND)
	{
		th = new AutoStandTwoThread(can, statusFlags);
	}

	initUiMain();
	initUiMainHeader();
	initUiTable();

	initLightStyleSheets();
	initDarkStyleSheets();
	initRecources();

	generateCableRows(testType, cables);

	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		initUiInTestManualStand();
		break;

	case WindowType::OUT_TEST_MANUAL_STAND:
		initUiOutTestManualStand();
		break;

	case WindowType::FULL_TEST_MANUAL_STAND:
		initUiFullTestManualStand();
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		initUiInManualTestAutoStand();
		break;

	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		initUiOutManualTestAutoStand();
		break;

	case WindowType::IN_AUTO_TEST_AUTO_STAND:
		initUiInAutoTestAutoStand();
		break;

	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		initUiOutAutoTestAutoStand();
		break;

	case WindowType::FULL_TEST_AUTO_STAND:
		initUiFullTestAutoStand();
		break;

	default:
		break;
	}

	initUiMainFooter();

	initTexts();
	initIcons();
	initConnections();
	initStyles();

	setStatusTableButtons(false);
	th->start();
}

TestWindow::~TestWindow()
{
	delete mainLayoutWidget;
	delete logoLightPixmap;
	delete logoDarkPixmap;
	delete themeLightPixmap;
	delete themeDarkPixmap;
	delete languageLightPixmap;
	delete languageDarkPixmap;
	delete backButtonLightPixmap;
	delete backButtonDarkPixmap;
	for (int i = 0; i < cableRows.size(); i++)
		delete cableRows[i];
	th->terminate();
	delete th;
}

void TestWindow::initUiMain()
{
	resize(viewWindowState->appSize.width, viewWindowState->appSize.height);
	setMinimumSize(QSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("mainLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

	mainVLayout = new QVBoxLayout(mainLayoutWidget);
	mainVLayout->setObjectName("mainVLayout");
}

void TestWindow::initUiMainHeader()
{
	headerLayoutWidget = new QWidget(mainLayoutWidget);
	headerLayoutWidget->setObjectName("headerLayoutWidget");
	headerLayoutWidget->setFixedHeight(FIXED_HEADER_HEIGHT);
	mainVLayout->addWidget(headerLayoutWidget);

	headerMainHLayout = new QHBoxLayout(headerLayoutWidget);
	headerMainHLayout->setObjectName("headerMainHLayout");
	headerMainHLayout->setSpacing(0);

	initUiLogo();
	headerMainHLayout->addWidget(logoLabel);

	usefulSpaceWidget = new QWidget(headerLayoutWidget);
	usefulSpaceWidget->setObjectName("usefulSpaceWidget");
	headerMainHLayout->addWidget(usefulSpaceWidget);

	usefulSpaceHLayout = new QHBoxLayout(usefulSpaceWidget);
	usefulSpaceHLayout->setObjectName("usefulSpaceHLayout");

	initUiTripleButtons();
	headerMainHLayout->addItem(tripleButtonsHLayout);

	headerLine = new QFrame(mainLayoutWidget);
	headerLine->setObjectName("headerLine");
	headerLine->setFrameShape(QFrame::HLine);
	mainVLayout->addWidget(headerLine);
}

void TestWindow::initUiLogo()
{
	logoLabel = new QLabel(this);
	logoLabel->setObjectName("LogoLabel");
	logoLabel->setText("");
	logoLabel->setFixedSize(FIXED_LOGO_WIDTH, FIXED_LOGO_HEIGHT);
	logoLabel->setEnabled(true);
}

void TestWindow::initUiTripleButtons()
{
	tripleButtonsHLayout = new QHBoxLayout();
	tripleButtonsHLayout->setObjectName("tripleButtonsHLayout");

	backButton = new QPushButton(headerLayoutWidget);
	backButton->setObjectName("backButton");
	backButton->setFixedSize(BACK_BUTTON_SIZE, BACK_BUTTON_SIZE);
	tripleButtonsHLayout->addWidget(backButton);

	tripleButtonsSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed);
	tripleButtonsHLayout->addItem(tripleButtonsSpacer);

	switchThemeLanguageVLayout = new QVBoxLayout();
	switchThemeLanguageVLayout->setObjectName("switchThemeLanguageVLayout");
	tripleButtonsHLayout->addItem(switchThemeLanguageVLayout);

	// Theme
	switchThemeButton = new QPushButton(headerLayoutWidget);
	switchThemeButton->setObjectName("switchThemeButton");
	switchThemeButton->setFixedSize(THEME_LANG_BUTTON_SIZE, THEME_LANG_BUTTON_SIZE);
	switchThemeLanguageVLayout->addWidget(switchThemeButton);

	// Language
	switchLanguageButton = new QPushButton(headerLayoutWidget);
	switchLanguageButton->setObjectName("switchLanguageButton");
	switchLanguageButton->setFixedSize(THEME_LANG_BUTTON_SIZE, THEME_LANG_BUTTON_SIZE);
	switchThemeLanguageVLayout->addWidget(switchLanguageButton);
}

void TestWindow::initUiTable()
{
	mainTableWidget = new QTableWidget(mainLayoutWidget);
	mainTableWidget->setObjectName("mainTableWidget");
	mainTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	mainTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
	mainVLayout->addWidget(mainTableWidget);
}

void TestWindow::initUiMainFooter()
{
	footerLine = new QFrame(mainLayoutWidget);
	footerLine->setObjectName("footerLine");
	footerLine->setFrameShape(QFrame::HLine);
	mainVLayout->addWidget(footerLine);

	footerLayoutWidget = new QWidget(mainLayoutWidget);
	footerLayoutWidget->setObjectName("footerLayoutWidget");
	footerLayoutWidget->setFixedHeight(FIXED_FOOTER_HEIGHT);
	mainVLayout->addWidget(footerLayoutWidget);

	footerMainHLayout = new QHBoxLayout(footerLayoutWidget);

	fileNameLabel = new QLabel(footerLayoutWidget);
	fileNameLabel->setObjectName("fileNameLabel");
	fileNameLabel->setFixedSize(FIXED_FILE_NAME_WIDTH, FIXED_FILE_NAME_HEIGHT);
	footerMainHLayout->addWidget(fileNameLabel);

	footerSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	footerMainHLayout->addItem(footerSpacer);

	reportHLayout = new QHBoxLayout(footerLayoutWidget);
	reportHLayout->setObjectName("reportHLayout");
	footerMainHLayout->addItem(reportHLayout);

	testerNameLineEdit = new QLineEdit(footerLayoutWidget);
	testerNameLineEdit->setObjectName("testerNameLineEdit");
	testerNameLineEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
	testerNameLineEdit->setFixedSize(FIXED_TESTER_NAME_WIDTH, FIXED_TESTER_NAME_HEIGHT);
	reportHLayout->addWidget(testerNameLineEdit);

	reportSpacer = new QSpacerItem(50, 0, QSizePolicy::Fixed);
	reportHLayout->addItem(reportSpacer);

	reportButton = new QPushButton(footerLayoutWidget);
	reportButton->setObjectName("reportButton");
	reportButton->setFixedSize(FIXED_REPORT_BUTTON_WIDTH, FIXED_REPORT_BUTTON_HEIGHT);
	reportHLayout->addWidget(reportButton);
}

void TestWindow::resizeEvent(QResizeEvent* event)
{
	viewWindowState->appSize.width = geometry().width();
	viewWindowState->appSize.height = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, viewWindowState->appSize.width - (BORDER_INDENT * 2), viewWindowState->appSize.height - (BORDER_INDENT * 2));
}

void TestWindow::initRecources()
{
	logoLightPixmap = new QPixmap(":/Light/icons/Logo_Black.png");
	logoDarkPixmap = new QPixmap(":/Dark/icons/Logo_White.png");
	themeLightPixmap = new QPixmap(":/Light/icons/Moon_Black.png");
	themeDarkPixmap = new QPixmap(":/Dark/icons/Sun_White.png");
	languageLightPixmap = new QPixmap(":/Light/icons/Language_Black.png");
	languageDarkPixmap = new QPixmap(":/Dark/icons/Language_White.png");
	backButtonLightPixmap = new QPixmap(":/Light/icons/Back_Black.png");
	backButtonDarkPixmap = new QPixmap(":/Dark/icons/Back_White.png");
	moreButtonLightPixmap = new QPixmap(":/Light/icons/More_Black.svg");
	moreButtonDarkPixmap = new QPixmap(":/Dark/icons/More_White.svg");
}

void TestWindow::initTexts()
{
	fileNameLabel->setText(fileName);

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		reportButton->setText(QString::fromLocal8Bit("Отчёт"));
		testerNameLineEdit->setText(QString::fromLocal8Bit("Иванов Иван Иванович"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			if (fullTestManualStandSortType)
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
			else
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			break;
		case WindowType::IN_TEST_MANUAL_STAND:
			inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			break;
		case WindowType::OUT_TEST_MANUAL_STAND:
			outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			break;
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			fullTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			if (fullTestAutoStandTypeSort)
				fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
			else
				fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));
		testerNameLineEdit->setText(QString("Ivanov Ivan Ivanovich"));
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandConnectButton->setText(QString("ECU\ndeconnected"));
			if (fullTestManualStandSortType)
				fullTestManualStandSortButton->setText(QString("Sort:\ntype"));
			else
				fullTestManualStandSortButton->setText(QString("Sort:\nnum"));
			break;
		case WindowType::IN_TEST_MANUAL_STAND:
			inTestManualStandConnectButton->setText(QString("ECU\ndeconnected"));
			break;
		case WindowType::OUT_TEST_MANUAL_STAND:
			outTestManualStandConnectButton->setText(QString("ECU\ndeconnected"));
			break;
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			outManualTestAutoStandConnectButton->setText(QString("Stand\ndeconnected"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			inManualTestAutoStandConnectButton->setText(QString("Stand\ndeconnected"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			outAutoTestAutoStandConnectButton->setText(QString("Stand\ndeconnected"));
			outAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			inAutoTestAutoStandConnectButton->setText(QString("Stand\ndeconnected"));
			inAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			fullTestAutoStandConnectButton->setText(QString("Stand\ndeconnected"));
			fullTestAutoStandStartTestButton->setText(QString("Start"));
			if (fullTestAutoStandTypeSort)
				fullTestAutoStandSortButton->setText(QString("Sort:\ntype"));
			else
				fullTestAutoStandSortButton->setText(QString("Sort:\nnum"));

			break;

		default:
			break;
		}
		break;
	}
}

void TestWindow::initIcons()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:
		logoLabel->setPixmap(*logoLightPixmap);
		switchThemeButton->setIcon(QIcon(*themeLightPixmap));
		switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
		backButton->setIcon(QIcon(*backButtonLightPixmap));
		backButton->setIconSize(backButton->size());
		break;

	case DARK_THEME:
		logoLabel->setPixmap(*logoDarkPixmap);
		switchThemeButton->setIcon(QIcon(*themeDarkPixmap));
		switchLanguageButton->setIcon(QIcon(*languageDarkPixmap));
		backButton->setIcon(QIcon(*backButtonDarkPixmap));
		backButton->setIconSize(backButton->size());
		break;
	}
}

void TestWindow::initConnections()
{
	QMetaObject::connectSlotsByName(this);
	// manualThread
	connect((ManualStandTwoThread*)th, &ManualStandTwoThread::msgToTestWindowStatusConnect_ManualTwoThread, this, &TestWindow::msgToTestWindowStatusConnect_ManualTwoThread);
	connect((ManualStandTwoThread*)th, &ManualStandTwoThread::msgToTestWindowChangeValue_ManualTwoThread, this, &TestWindow::msgToTestWindowChangeValue_ManualTwoThread);

	//connect((AutoStandTwoThread*)th, &AutoStandTwoThread::msgToTestWindowBeforeTest_AutoTwoThread, this, &TestWindow::msgToTestWindowBeforeTest_AutoTwoThread);
	//connect((AutoStandTwoThread*)th, &AutoStandTwoThread::msgToTestWindowAfterTest_AutoTwoThread, this, &TestWindow::msgToTestWindowAfterTest_AutoTwoThread);
}

void TestWindow::initStyles()
{
	resetTableButtonsTheme(TypeResetTableButtonsTheme::STAND_DISCONNECTED, 0, 0);
	resetTheme();
}

void TestWindow::resetTableButtonsTheme(TypeResetTableButtonsTheme typeResetTheme, int pad, int pin)
{
	switch (typeResetTheme)
	{
	case TypeResetTableButtonsTheme::STAND_DISCONNECTED:
		for (int i = 0; i < cableRows.size(); i++)
		{
			if (testType == WindowType::OUT_TEST_MANUAL_STAND ||
				testType == WindowType::IN_TEST_MANUAL_STAND ||
				testType == WindowType::FULL_TEST_MANUAL_STAND)
			{
				if (cableRows[i]->type == "DIGITAL" && cableRows[i]->direction == "OUT")
				{
					((DigitalButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((DigitalButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.inactiveTableButton);
				}
				if (cableRows[i]->type == "PWM" && cableRows[i]->direction == "OUT")
				{
					((PWMButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
				}
				if (cableRows[i]->type == "VNH" && cableRows[i]->direction == "OUT")
				{
					((VNHButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
				}
			}
		}
		break;
	case TypeResetTableButtonsTheme::STAND_CONNECTED:
		for (int i = 0; i < cableRows.size(); i++)
		{
			if (testType == WindowType::OUT_TEST_MANUAL_STAND ||
				testType == WindowType::IN_TEST_MANUAL_STAND ||
				testType == WindowType::FULL_TEST_MANUAL_STAND)
			{
				if (cableRows[i]->type == "DIGITAL" && cableRows[i]->direction == "OUT")
				{
					((DigitalButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((DigitalButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.activeTableButton);
				}
				if (cableRows[i]->type == "PWM" && cableRows[i]->direction == "OUT")
				{
					((PWMButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.activeTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
				}
				if (cableRows[i]->type == "VNH" && cableRows[i]->direction == "OUT")
				{
					((VNHButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.activeTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.activeTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
				}
			}
		}
		break;
	}
}

void TestWindow::switchActiveTableButton(void* activeButton, void* inactiveButton)
{
	//(DigitalButtons*)(activeButton)->onButton;
}

void TestWindow::setFileName(QString fileName)
{
	this->fileName = fileName;
	fileNameLabel->setText(this->fileName);
}

void TestWindow::on_backButton_clicked()
{
	this->close();
}

void TestWindow::on_switchThemeButton_clicked()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:
		viewWindowState->appTheme = DARK_THEME;
		break;

	case DARK_THEME:
		viewWindowState->appTheme = LIGHT_THEME;
		break;
	}
	resetTheme();
}

void TestWindow::on_switchLanguageButton_clicked()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		viewWindowState->appLanguage = ENGLISH_LANG;
		break;

	case ENGLISH_LANG:
		viewWindowState->appLanguage = RUSSIAN_LANG;
		break;
	}
	resetLanguage();
}

void TestWindow::on_reportButton_clicked()
{
}

void TestWindow::resetTheme()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:

		logoLabel->setPixmap(*logoLightPixmap);
		switchThemeButton->setIcon(QIcon(*themeLightPixmap));
		switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
		backButton->setIcon(QIcon(*backButtonLightPixmap));
		backButton->setIconSize(backButton->size());

		switchThemeButton->setStyleSheet(lightStyles.testwindowMoveButtonStyle);
		switchLanguageButton->setStyleSheet(lightStyles.testwindowMoveButtonStyle);
		backButton->setStyleSheet(lightStyles.testwindowMoveButtonStyle);
		reportButton->setStyleSheet(lightStyles.testwindowMoveButtonStyle);
		mainTableWidget->setStyleSheet(lightStyles.testwindowTableWidget);
		testerNameLineEdit->setStyleSheet(lightStyles.testwindowNameLineEdit);
		fileNameLabel->setStyleSheet(lightStyles.testwindowNameLineEdit);
		resetIconMoreButton(LIGHT_THEME);

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (standConected)
				fullTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				fullTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			fullTestManualStandSortButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (standConected)
				outTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				outTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (standConected)
				inTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				inTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			inTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				outManualTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				outManualTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			outManualTestAutoStandTestTimeComboBox->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				inManualTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				inManualTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			inManualTestAutoStandTestTimeComboBox->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (standConected)
				outAutoTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				outAutoTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			outAutoTestAutoStandStartTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (standConected)
				inAutoTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				inAutoTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			inAutoTestAutoStandStartTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (standConected)
				fullTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				fullTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			fullTestAutoStandStartTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			fullTestAutoStandSortButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		default:
			break;
		}
		break;

	case DARK_THEME:
		logoLabel->setPixmap(*logoDarkPixmap);
		switchThemeButton->setIcon(QIcon(*themeDarkPixmap));
		switchLanguageButton->setIcon(QIcon(*languageDarkPixmap));
		backButton->setIcon(QIcon(*backButtonDarkPixmap));
		backButton->setIconSize(backButton->size());

		switchThemeButton->setStyleSheet(darkStyles.testwindowMoveButtonStyle);
		switchLanguageButton->setStyleSheet(darkStyles.testwindowMoveButtonStyle);
		backButton->setStyleSheet(darkStyles.testwindowMoveButtonStyle);
		reportButton->setStyleSheet(darkStyles.testwindowMoveButtonStyle);
		mainTableWidget->setStyleSheet(darkStyles.testwindowTableWidget);
		testerNameLineEdit->setStyleSheet(darkStyles.testwindowNameLineEdit);
		fileNameLabel->setStyleSheet(darkStyles.testwindowNameLineEdit);
		resetIconMoreButton(DARK_THEME);

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (standConected)
				fullTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				fullTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			fullTestManualStandSortButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (standConected)
				outTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				outTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (standConected)
				inTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				inTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			inTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				outManualTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				outManualTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			outManualTestAutoStandTestTimeComboBox->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				inManualTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				inManualTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			inManualTestAutoStandTestTimeComboBox->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (standConected)
				outAutoTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				outAutoTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			outAutoTestAutoStandStartTestButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (standConected)
				inAutoTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				inAutoTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			inAutoTestAutoStandStartTestButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (standConected)
				fullTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				fullTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			fullTestAutoStandStartTestButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			fullTestAutoStandSortButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		default:
			break;
		}
		break;
	}
}

void TestWindow::resetLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		reportButton->setText(QString::fromLocal8Bit("Отчёт"));
		testerNameLineEdit->setText(QString::fromLocal8Bit("Иванов Иван Иванович"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (standConected)
				fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			if (fullTestManualStandSortType)
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
			else
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (standConected)
				outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (standConected)
				inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (standConected)
				outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (standConected)
				inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (standConected)
				fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::FULL_TEST_AUTO_STAND);
			fullTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			if (fullTestAutoStandTypeSort)
				fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
			else
				fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));
		testerNameLineEdit->setText(QString("Ivanov Ivan Ivanovich"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (standConected)
				fullTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				fullTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			if (fullTestManualStandSortType)
				fullTestManualStandSortButton->setText(QString("Sort:\ntype"));
			else
				fullTestManualStandSortButton->setText(QString("Sort:\nnum"));
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (standConected)
				outTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				outTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (standConected)
				inTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				inTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				outManualTestAutoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				outManualTestAutoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				inManualTestAutoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				inManualTestAutoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (standConected)
				outAutoTestAutoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				outAutoTestAutoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			outAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (standConected)
				inAutoTestAutoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				inAutoTestAutoStandConnectButton->setText(QString("Stand\disconnected"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			inAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (standConected)
				fullTestAutoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				fullTestAutoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::FULL_TEST_AUTO_STAND);
			fullTestAutoStandStartTestButton->setText(QString("Start"));
			if (fullTestAutoStandTypeSort)
				fullTestAutoStandSortButton->setText(QString("Sort:\ntype"));
			else
				fullTestAutoStandSortButton->setText(QString("Sort:\nnum"));

			break;

		default:
			break;
		}
		break;
	}
}

void TestWindow::setParentFrame(WindowFrame* parentFrame)
{
	this->parentFrame = parentFrame;

	connect(switchThemeButton, &QPushButton::clicked, parentFrame, &WindowFrame::on_switchThemeButton_clicked);
}

void TestWindow::createItemManualTestAutoStandTestTimeComboBox(QComboBox* comboBox)
{
	comboBox->clear();
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
	{
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n5 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n10 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n15 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n20 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n25 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n30 секунд"));
	}
	else
	{
		comboBox->addItem(QString("Check duration: 5 seconds"));
		comboBox->addItem(QString("Check duration: 10 seconds"));
		comboBox->addItem(QString("Check duration: 15 seconds"));
		comboBox->addItem(QString("Check duration: 20 seconds"));
		comboBox->addItem(QString("Check duration: 25 seconds"));
		comboBox->addItem(QString("Check duration: 30 seconds"));
	}
}

void TestWindow::setStatusTableButtons(bool statusButton)
{
	statusButton = !statusButton;
	for (int i = 0; i < cableRows.size(); i++)
	{
		if (testType == WindowType::OUT_TEST_MANUAL_STAND ||
			testType == WindowType::IN_TEST_MANUAL_STAND ||
			testType == WindowType::FULL_TEST_MANUAL_STAND)
		{
			if (cableRows[i]->type == "DIGITAL" && cableRows[i]->direction == "OUT")
			{
				((DigitalButtons*)(cableRows[i]->buttons))->onButton->setDisabled(statusButton);
				((DigitalButtons*)(cableRows[i]->buttons))->offButton->setDisabled(statusButton);
			}
			if (cableRows[i]->type == "PWM" && cableRows[i]->direction == "OUT")
			{
				((PWMButtons*)(cableRows[i]->buttons))->load0Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load25Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load50Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load75Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load100Button->setDisabled(statusButton);
			}
			if (cableRows[i]->type == "VNH" && cableRows[i]->direction == "OUT")
			{
				((VNHButtons*)(cableRows[i]->buttons))->onButton->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->offButton->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load0Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load25Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load50Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load75Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load100Button->setDisabled(statusButton);
			}
		}
	}
}

void TestWindow::msgToTestWindowStatusConnect_ManualTwoThread(bool statusConnect)
{
	setStatusTableButtons(statusConnect);
	if (statusConnect)
		resetTableButtonsTheme(TypeResetTableButtonsTheme::STAND_CONNECTED, 0, 0);
	else
		resetTableButtonsTheme(TypeResetTableButtonsTheme::STAND_DISCONNECTED, 0, 0);
	standConected = statusConnect;
	resetLanguage();
	resetTheme();
	//outTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
}

static int determineCurrentRowNum(int pad, int pin, std::vector<TestTableRowProperties*> cableRows)
{
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
		if ((int)(cableRows[currentRowNum]->connector.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL) == pad && cableRows[currentRowNum]->pin.toInt() == pin)
			return currentRowNum;
	return -1;
}

void TestWindow::msgToTestWindowChangeValue_ManualTwoThread(int pad, int pin, int newValue)
{
	int currentColoumnNum = -1;
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		currentColoumnNum = 4;
		break;

	case WindowType::FULL_TEST_MANUAL_STAND:
		currentColoumnNum = 6;
		break;

	default:
		// ERROR
		break;
	}
	int currentRowNum = determineCurrentRowNum(pad, pin, cableRows);
	QAbstractItemModel* model = mainTableWidget->model();
	model->setData(model->index(currentRowNum, currentColoumnNum), QString::number(newValue));
}

void TestWindow::initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget)
{
	interactionButtonsWidget->setObjectName("interactionButtonsWidget");
	QVBoxLayout* interactionButtonsCellVLayout = new QVBoxLayout(interactionButtonsWidget);
	interactionButtonsCellVLayout->setObjectName("interactionButtonsCellVLayout");
	if (testType == WindowType::OUT_TEST_MANUAL_STAND || testType == WindowType::FULL_TEST_MANUAL_STAND)
	{
		if (cableRows[currentRowNum]->type == "DIGITAL" && cableRows[currentRowNum]->direction == "OUT")
		{
			QHBoxLayout* interactionButtonsCellHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellHLayout->setObjectName("interactionButtonsCellHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellHLayout);

			QSpacerItem* leftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* middleSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Maximum);
			QSpacerItem* rightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);

			interactionButtonsCellHLayout->addItem(leftSpacer);
			interactionButtonsCellHLayout->addWidget(((DigitalButtons*)cableRows[currentRowNum]->buttons)->onButton);
			interactionButtonsCellHLayout->addItem(middleSpacer);
			interactionButtonsCellHLayout->addWidget(((DigitalButtons*)cableRows[currentRowNum]->buttons)->offButton);
			interactionButtonsCellHLayout->addItem(rightSpacer);

			connect(((DigitalButtons*)cableRows[currentRowNum]->buttons)->onButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_onButton_clicked);
			connect(((DigitalButtons*)cableRows[currentRowNum]->buttons)->offButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_offButton_clicked);

			mainTableWidget->setRowHeight(currentRowNum, COLOUMN_DIGITAL_HEIGHT);
		}
		else if (cableRows[currentRowNum]->type == "PWM")
		{
			QHBoxLayout* interactionButtonsCellFirstHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellFirstHLayout->setObjectName("interactionButtonsCellFirstHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellFirstHLayout);
			QHBoxLayout* interactionButtonsCellSecondHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellSecondHLayout->setObjectName("interactionButtonsCellSecondHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellSecondHLayout);
			QHBoxLayout* interactionButtonsCellThirdHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellThirdHLayout->setObjectName("interactionButtonsCellThirdHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellThirdHLayout);

			QSpacerItem* firstLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* firstMiddleSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Maximum);
			QSpacerItem* firstRightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* secondLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* secondMiddleSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Maximum);
			QSpacerItem* secondRightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);

			interactionButtonsCellFirstHLayout->addItem(firstLeftSpacer);
			interactionButtonsCellFirstHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load0Button);
			interactionButtonsCellFirstHLayout->addItem(firstMiddleSpacer);
			interactionButtonsCellFirstHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load25Button);
			interactionButtonsCellFirstHLayout->addItem(firstRightSpacer);
			interactionButtonsCellSecondHLayout->addItem(secondLeftSpacer);
			interactionButtonsCellSecondHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load50Button);
			interactionButtonsCellSecondHLayout->addItem(secondMiddleSpacer);
			interactionButtonsCellSecondHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load75Button);
			interactionButtonsCellSecondHLayout->addItem(secondRightSpacer);
			interactionButtonsCellThirdHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load100Button);

			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load0Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load0Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load25Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load25Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load50Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load50Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load75Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load75Button_clicked);
			connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load100Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load100Button_clicked);

			mainTableWidget->setRowHeight(currentRowNum, COLOUMN_PWM_HEIGHT);
		}
		else if (cableRows[currentRowNum]->type == "VNH")
		{
			QHBoxLayout* interactionButtonsCellFirstHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellFirstHLayout->setObjectName("interactionButtonsCellFirstHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellFirstHLayout);
			QHBoxLayout* interactionButtonsCellSecondHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellSecondHLayout->setObjectName("interactionButtonsCellSecondHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellSecondHLayout);
			QHBoxLayout* interactionButtonsCellThirdHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellThirdHLayout->setObjectName("interactionButtonsCellThirdHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellThirdHLayout);
			QHBoxLayout* interactionButtonsCellFourthHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsCellFourthHLayout->setObjectName("interactionButtonsCellFourthHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsCellFourthHLayout);

			QSpacerItem* firstLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* firstMiddleSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Maximum);
			QSpacerItem* firstRightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* secondLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* secondMiddleSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Maximum);
			QSpacerItem* secondRightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* thirdLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* thirdMiddleSpacer = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Maximum);
			QSpacerItem* thirdRightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);

			interactionButtonsCellFirstHLayout->addItem(firstLeftSpacer);
			interactionButtonsCellFirstHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load0Button);
			interactionButtonsCellFirstHLayout->addItem(firstMiddleSpacer);
			interactionButtonsCellFirstHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load25Button);
			interactionButtonsCellFirstHLayout->addItem(firstRightSpacer);
			interactionButtonsCellSecondHLayout->addItem(secondLeftSpacer);
			interactionButtonsCellSecondHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load50Button);
			interactionButtonsCellSecondHLayout->addItem(secondMiddleSpacer);
			interactionButtonsCellSecondHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load75Button);
			interactionButtonsCellSecondHLayout->addItem(secondRightSpacer);
			interactionButtonsCellThirdHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load100Button);
			interactionButtonsCellFourthHLayout->addItem(thirdLeftSpacer);
			interactionButtonsCellFourthHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->onButton);
			interactionButtonsCellFourthHLayout->addItem(thirdMiddleSpacer);
			interactionButtonsCellFourthHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->offButton);
			interactionButtonsCellFourthHLayout->addItem(thirdRightSpacer);

			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load0Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load0Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load25Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load25Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load50Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load50Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load75Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load75Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load100Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load100Button_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->onButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_onButton_clicked);
			connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->offButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_offButton_clicked);

			mainTableWidget->setRowHeight(currentRowNum, COLOUMN_VNH_HEIGHT);
		}
	}
	connect(cableRows[currentRowNum], &TestTableRowProperties::msgToTwoThreadStartTest_ManualTwoThread, (ManualStandTwoThread*)th, &ManualStandTwoThread::msgToTwoThreadStartTest_ManualTwoThread);
	connect(cableRows[currentRowNum], &TestTableRowProperties::switchActiveTableButton, this, &TestWindow::switchActiveTableButton);
	//connect(cableRows[currentRowNum], &TestTableRowProperties::msgToAutoTwoThreadStartTest, th, &AutoStandTwoThread::msgToAutoTwoThreadStartTest);

	interactionButtonsCellVLayout->setContentsMargins(0, 0, 0, 0);
	interactionButtonsWidget->setLayout(interactionButtonsCellVLayout);
}

void TestWindow::initAutoCheckButton(int currentRowNum, QWidget* autoCheckCellWidget)
{
	autoCheckCellWidget->setObjectName("autoCheckCellWidget");
	QHBoxLayout* autoCheckCellHLayout = new QHBoxLayout(autoCheckCellWidget);
	autoCheckCellHLayout->setObjectName("autoCheckCellHLayout");
	autoCheckCellHLayout->addWidget(((CheckButton*)cableRows[currentRowNum]->buttons)->checkButton);
	autoCheckCellHLayout->setContentsMargins(0, 0, 0, 0);
	autoCheckCellWidget->setLayout(autoCheckCellHLayout);
}

void TestWindow::initMoreButton(int currentRowNum, QWidget* moreCellWidget)
{
	cableRows[currentRowNum]->moreButton->setIcon(QIcon(*moreButtonLightPixmap));
	cableRows[currentRowNum]->moreButton->setIconSize(QSize(FIXED_MORE_BUTTON_SIZE, FIXED_MORE_BUTTON_SIZE));

	moreCellWidget->setObjectName("moreCellWidget");
	QHBoxLayout* moreCellHLayout = new QHBoxLayout(moreCellWidget);
	moreCellHLayout->setObjectName("moreCellHLayout");
	moreCellHLayout->addWidget(cableRows[currentRowNum]->moreButton);
	moreCellHLayout->setContentsMargins(0, 0, 0, 0);
	moreCellWidget->setLayout(moreCellHLayout);
}

void TestWindow::resetIconMoreButton(bool theme)
{
	for (int row = 0; row < cableRows.size(); row++)
	{
		if (theme)
			cableRows[row]->moreButton->setIcon(QIcon(*moreButtonDarkPixmap));
		else
			cableRows[row]->moreButton->setIcon(QIcon(*moreButtonLightPixmap));
	}
}