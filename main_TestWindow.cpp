#include "TestWindow.h"

#define MIN_TEST_SCREEN_WIDTH	900

#define STATUS_NOT_SET			NOT_SET
#define STATUS_IN_TEST			5
#define STATUS_FULL_TEST		7

#define FIXED_HEADER_HEIGHT			80
#define FIXED_FOOTER_HEIGHT			80
#define THEME_LANG_BUTTON_SIZE		30
#define BACK_BUTTON_SIZE			50
#define FIXED_FILE_NAME_WIDTH		150
#define FIXED_FILE_NAME_HEIGHT		50
#define FIXED_TESTER_NAME_WIDTH		300
#define FIXED_TESTER_NAME_HEIGHT	50
#define FIXED_REPORT_BUTTON_WIDTH	100
#define FIXED_REPORT_BUTTON_HEIGHT	50
#define FIXED_WISE_PIXMAP_WIDTH		30
#define FIXED_WISE_PIXMAP_HEIGHT	30

TestWindow::TestWindow(WindowType testType, std::vector<Cable> cables, TestBlockName testingBlock, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->testType = testType;
	//this->can = can;
	this->testingBlock = testingBlock;
	isFullTestEnabled = false;
	if(cables.size() != 0)
		nextCheckCable = new Cable(cables[0].getConnector(), cables[0].getPin());
	statusFlags = new StandStatusFlags;
	statusFlags->StatusConnected = false;
	statusFlags->StatusTest = false;

	rotateTimer = new QTimer();
	connect(rotateTimer, &QTimer::timeout, this, &TestWindow::on_rotateTimer_timeout);
	
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
	Can::clearOldValue();

	rotateTimer->start(3);
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
}

void TestWindow::initUiMain()
{
	setMinimumSize(QSize(MIN_TEST_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("mainLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_TEST_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

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
	mainTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	mainTableHeaderLabels = new QStringList();
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
	if(testingBlock == TestBlockName::DTM)
		fileNameLabel->setText("DM");
	else
		fileNameLabel->setText("BCM");
	fileNameLabel->setFixedSize(FIXED_FILE_NAME_WIDTH, FIXED_FILE_NAME_HEIGHT);
	footerMainHLayout->addWidget(fileNameLabel);

	footerSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	footerMainHLayout->addItem(footerSpacer);

	reportHLayout = new QHBoxLayout(footerLayoutWidget);
	reportHLayout->setObjectName("reportHLayout");
	footerMainHLayout->addItem(reportHLayout);

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
	clockwiseLightPixmap = new QPixmap(":/Light/icons/Clockwise_Black.png");
	clockwiseDarkPixmap = new QPixmap(":/Dark/icons/Clockwise_White.png");
	counterclockwiseLightPixmap = new QPixmap(":/Light/icons/Counterclockwise_Black.png");
	counterclockwiseDarkPixmap = new QPixmap(":/Dark/icons/Counterclockwise_White.png");
	noClockwiseLightPixmap = new QPixmap(":/Light/icons/NoClockwise_Black.png");
	noClockwiseDarkPixmap = new QPixmap(":/Dark/icons/NoClockwise_White.png");
}

void TestWindow::initTexts()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		reportButton->setText(QString::fromLocal8Bit("Отчёт"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
				break;
			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо выходам"));
				break;
			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо входам"));
				break;
			}
			break;
		case WindowType::IN_TEST_MANUAL_STAND:
			inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			break;
		case WindowType::OUT_TEST_MANUAL_STAND:
			outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			break;
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
				break;
			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо выходам"));
				break;
			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо входам"));
				break;
			}
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandConnectButton->setText(QString("ECU\ndeconnected"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString("Sort:\nnum"));
				break;
			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString("Sort:\nout first"));
				break;
			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString("Sort:\nin first"));
				break;
			}
			break;
		case WindowType::IN_TEST_MANUAL_STAND:
			inTestManualStandConnectButton->setText(QString("ECU\ndeconnected"));
			break;
		case WindowType::OUT_TEST_MANUAL_STAND:
			outTestManualStandConnectButton->setText(QString("ECU\ndeconnected"));
			break;
		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString("Stand\ndeconnected"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString("Stand\ndeconnected"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString("Stand\ndeconnected"));
			autoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString("Stand\ndeconnected"));
			autoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			autoStandConnectButton->setText(QString("Stand\ndeconnected"));
			autoStandStartTestButton->setText(QString("Start"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString("Sort:\nnum"));
				break;
			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString("Sort:\nout first"));
				break;
			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString("Sort:\nin first"));
				break;
			}
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
			if (testType == WindowType::OUT_TEST_MANUAL_STAND ||
				testType == WindowType::IN_TEST_MANUAL_STAND ||
				testType == WindowType::FULL_TEST_MANUAL_STAND)
				switch (cableRows[i]->typeInt)
				{
				case TypeCable::DIG_OUT:
					((DigitalButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((DigitalButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				case TypeCable::PWM_OUT:
					((PWMButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				case TypeCable::VNH_OUT:
					((VNHButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				case TypeCable::HLD_OUT:
					((HLDButtons*)(cableRows[i]->buttons))->highButton->setStyleSheet(lightStyles.inactiveTableButton);
					((HLDButtons*)(cableRows[i]->buttons))->lowButton->setStyleSheet(lightStyles.inactiveTableButton);
					((HLDButtons*)(cableRows[i]->buttons))->zeroButton->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				default:
					break;
				}
		break;

	case TypeResetTableButtonsTheme::STAND_CONNECTED:
		for (int i = 0; i < cableRows.size(); i++)
			if (testType == WindowType::OUT_TEST_MANUAL_STAND ||
				testType == WindowType::IN_TEST_MANUAL_STAND ||
				testType == WindowType::FULL_TEST_MANUAL_STAND)
				switch (cableRows[i]->typeInt)
				{
				case TypeCable::DIG_OUT:
					((DigitalButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((DigitalButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.activeTableButton);
					break;

				case TypeCable::PWM_OUT:
					((PWMButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.activeTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((PWMButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				case TypeCable::VNH_OUT:
					((VNHButtons*)(cableRows[i]->buttons))->onButton->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->offButton->setStyleSheet(lightStyles.activeTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load0Button->setStyleSheet(lightStyles.activeTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load25Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load50Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load75Button->setStyleSheet(lightStyles.inactiveTableButton);
					((VNHButtons*)(cableRows[i]->buttons))->load100Button->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				case TypeCable::HLD_OUT:
					((HLDButtons*)(cableRows[i]->buttons))->highButton->setStyleSheet(lightStyles.activeTableButton);
					((HLDButtons*)(cableRows[i]->buttons))->lowButton->setStyleSheet(lightStyles.inactiveTableButton);
					((HLDButtons*)(cableRows[i]->buttons))->zeroButton->setStyleSheet(lightStyles.inactiveTableButton);
					break;

				default:
					break;
				}
		break;
	}
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
	//hallLabels[0].first = 0;
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
	ReportWindow* reportWindow;
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
		reportWindow = new ReportWindow(cableRows, manualChecks, testingBlock);
		break;

	case WindowType::IN_AUTO_TEST_AUTO_STAND:
	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
	case WindowType::FULL_TEST_AUTO_STAND:
		reportWindow = new ReportWindow(cableRows, testingBlock);
		break;

	default:
		// Error
		return;
		break;
	}
	reportWindow->setTestingBlock(testingBlock);
	reportWindow->setTestingType(testType);

	WindowFrame w(WindowType::REPORTWINDOW, nullptr, reportWindow);
	w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
	w.show();
	reportWindow->exec();
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
		fileNameLabel->setStyleSheet(lightStyles.testwindowLableBlock);
		resetIconMoreButton(LIGHT_THEME);

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				fullTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				fullTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			fullTestSortButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				outTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				outTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				inTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				inTestManualStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			outManualTestAutoStandTestTimeComboBox->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			inManualTestAutoStandTestTimeComboBox->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			autoStandStartTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			autoStandStartTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyleDisconnected);
			autoStandStartTestButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			fullTestSortButton->setStyleSheet(lightStyles.testwindowButtonStyle);
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
		fileNameLabel->setStyleSheet(darkStyles.testwindowLableBlock);
		resetIconMoreButton(DARK_THEME);

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				fullTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				fullTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			fullTestSortButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				outTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				outTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				inTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				inTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			inTestManualStandConnectButton->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			outManualTestAutoStandTestTimeComboBox->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			inManualTestAutoStandTestTimeComboBox->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			autoStandStartTestButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			autoStandStartTestButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleConnect);
			else
				autoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyleDisconnected);
			autoStandStartTestButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			fullTestSortButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		default:
			break;
		}
		break;
	}
	Can::clearOldValue();
}

void TestWindow::resetLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		reportButton->setText(QString::fromLocal8Bit("Отчёт"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
				break;
			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо выходам"));
				break;
			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо входам"));
				break;
			}
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			resetLanguageFullTestManualStand();
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			resetLanguageOutTestManualStand();
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			resetLanguageInTestManualStand();
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			resetLanguageOutManualTestAutoStand();
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			resetLanguageInManualTestAutoStand();
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			if(isFullTestEnabled)
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Пауза"));
			else
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			resetLanguageOutAutoTestAutoStand();
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			if (isFullTestEnabled)
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Пауза"));
			else
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			resetLanguageInAutoTestAutoStand();
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			parentFrame->setTitle(WindowType::FULL_TEST_AUTO_STAND);
			if (isFullTestEnabled)
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Пауза"));
			else
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
				break;

			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо выходам"));
				break;

			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо входам"));
				break;
			}
			resetLanguageFullTestAutoStand();
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				fullTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				fullTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString("Sort:\nnum"));
				break;

			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString("Sort:\nout first"));
				break;

			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString("Sort:\nin first"));
				break;
			}
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			resetLanguageFullTestManualStand();
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				outTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				outTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			resetLanguageOutTestManualStand();
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				inTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				inTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			resetLanguageInTestManualStand();
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			resetLanguageOutManualTestAutoStand();
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			resetLanguageInManualTestAutoStand();
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			if(isFullTestEnabled)
				autoStandStartTestButton->setText(QString("Pause"));
			else
				autoStandStartTestButton->setText(QString("Start"));
			resetLanguageOutAutoTestAutoStand();
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			if (isFullTestEnabled)
				autoStandStartTestButton->setText(QString("Pause"));
			else
				autoStandStartTestButton->setText(QString("Start"));
			resetLanguageInAutoTestAutoStand();
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			parentFrame->setTitle(WindowType::FULL_TEST_AUTO_STAND);
			if (isFullTestEnabled)
				autoStandStartTestButton->setText(QString("Pause"));
			else
				autoStandStartTestButton->setText(QString("Start"));
			switch (fullTestSortType)
			{
			case SORT_TYPE_INDEX:
				fullTestSortButton->setText(QString("Sort:\nnum"));
				break;

			case SORT_TYPE_DIRECTION_OUT:
				fullTestSortButton->setText(QString("Sort:\nout first"));
				break;

			case SORT_TYPE_DIRECTION_IN:
				fullTestSortButton->setText(QString("Sort:\nin first"));
				break;
			}
			resetLanguageFullTestAutoStand();
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
			switch (cableRows[i]->typeInt)
			{
			case TypeCable::DIG_OUT:
				((DigitalButtons*)(cableRows[i]->buttons))->onButton->setDisabled(statusButton);
				((DigitalButtons*)(cableRows[i]->buttons))->offButton->setDisabled(statusButton);
				break;
			case TypeCable::PWM_OUT:
				((PWMButtons*)(cableRows[i]->buttons))->load0Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load25Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load50Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load75Button->setDisabled(statusButton);
				((PWMButtons*)(cableRows[i]->buttons))->load100Button->setDisabled(statusButton);
				break;
			case TypeCable::VNH_OUT:
				((VNHButtons*)(cableRows[i]->buttons))->onButton->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->offButton->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load0Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load25Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load50Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load75Button->setDisabled(statusButton);
				((VNHButtons*)(cableRows[i]->buttons))->load100Button->setDisabled(statusButton);
				break;

			case TypeCable::HLD_OUT:
				((HLDButtons*)(cableRows[i]->buttons))->highButton->setDisabled(statusButton);
				((HLDButtons*)(cableRows[i]->buttons))->lowButton->setDisabled(statusButton);
				((HLDButtons*)(cableRows[i]->buttons))->zeroButton->setDisabled(statusButton);
				break;

			default:
				break;
			}
		}
	}
}

static int determineCurrentRowNum(int pad, int pin, std::vector<TestTableRowProperties*> cableRows)
{
	for (int currentRowNum = 0; currentRowNum < cableRows.size(); currentRowNum++)
		if ((int)(cableRows[currentRowNum]->connectorStr.toStdString()[0] - PRIMARY_CONNECTOR_SYMBOL) == pad && cableRows[currentRowNum]->pin.toInt() == pin)
			return currentRowNum;
	return NOT_SET;
}

void TestWindow::ProcAutoTest(int connector, int pin)
{
	for (int i = 0; i < cableRows.size(); i++)
		if (connector == (int)cableRows[i]->connectorInt && pin == cableRows[i]->pin.toInt())
			Can::sendTestMsg(cableRows[i]->connectorInt, cableRows[i]->pin.toInt(), cableRows[i]->typeInt, TestBlockName::BCM);
}


void TestWindow::on_autoStandStartTestButton_clicked()
{
	if (isFullTestEnabled)
	{
		isFullTestEnabled = false;
		for (int i = 0; i < cableRows.size(); i++)
			if (nextCheckCable->getConnector() == cableRows[i]->connectorInt && nextCheckCable->getPin() == cableRows[i]->pin.toInt())
			{
				if (i == cableRows.size() - 1)
				{
					// Тест закончен
					nextCheckCable->setConnector(cableRows[0]->connectorInt);
					nextCheckCable->setPin(cableRows[0]->pin.toInt());
					QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Тест закончен"));
					isFullTestEnabled = false;
					return;
				}
				nextCheckCable->setConnector(cableRows[i + 1]->connectorInt);
				nextCheckCable->setPin(cableRows[i + 1]->pin.toInt());
				break;
			}
	}
	else
	{
		if (!statusFlags->StatusConnected) // Если стенд не подключён, ничего не делаем
		{
			if (viewWindowState->appLanguage == RUSSIAN_LANG)
				QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Стенд не подключен"));
			else
				QMessageBox::warning(this, QString("Warning"), QString("Stand is not connected"));
			return;
		}
		else if (statusFlags->StatusTest) // Если идёт тест, ничего не делаем
		{
			if (viewWindowState->appLanguage == RUSSIAN_LANG)
				QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("В данный момент проходит тест"));
			else
				QMessageBox::warning(this, QString("Warning"), QString("Currently being tested"));
			return;
		}
	
		isFullTestEnabled = true;
		for (int i = 0; i < cableRows.size(); i++)
			if (nextCheckCable->getConnector() == cableRows[i]->connectorInt && nextCheckCable->getPin() == cableRows[i]->pin.toInt())
				mainTableWidget->item(i, testType == WindowType::FULL_TEST_AUTO_STAND ? 6 : 5)->setBackgroundColor(Qt::yellow);
				
		ProcAutoTest((int)nextCheckCable->getConnector(), nextCheckCable->getPin());
	}
	resetLanguage();
}
	

void TestWindow::Slot_AfterTest(int connector, int pin, std::vector<Measureds*> measureds)
{
	// Красим нужную ячейку 
	for (int i = 0; i < cableRows.size(); i++)
		if (connector == (int)cableRows[i]->connectorInt && pin == cableRows[i]->pin.toInt())
		{
			cableRows[i]->measureds = measureds;

			QAbstractItemModel* model = mainTableWidget->model();
			int currentRowNum = determineCurrentRowNum(connector, pin, cableRows);
			model->setData(model->index(currentRowNum, 6), QString(""));
			for (int j = 0; j < cableRows[i]->thresholds.size(); j++)
				if (cableRows[i]->thresholds[j].minVoltage > measureds[j]->voltage || 
					cableRows[i]->thresholds[j].maxVoltage < measureds[j]->voltage ||
					cableRows[i]->thresholds[j].minCurrent > measureds[j]->current || 
					cableRows[i]->thresholds[j].maxCurrent < measureds[j]->current)
					mainTableWidget->item(i, 6)->setBackgroundColor(Qt::red);
			if (mainTableWidget->item(i, 6)->backgroundColor() != Qt::red)
				mainTableWidget->item(i, 6)->setBackgroundColor(Qt::green);
			if (isFullTestEnabled)// запускаем следующий тест
			{
				for (int i = 0; i < cableRows.size(); i++)
					if (nextCheckCable->getConnector() == cableRows[i]->connectorInt && nextCheckCable->getPin() == cableRows[i]->pin.toInt())
					{
						if (i == cableRows.size() - 1)
						{
							// Тест закончен
							nextCheckCable->setConnector(cableRows[0]->connectorInt);
							nextCheckCable->setPin(cableRows[0]->pin.toInt());
							QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Тест закончен"));
							isFullTestEnabled = false;
							resetLanguage();
							return;
						}
						nextCheckCable->setConnector(cableRows[i + 1]->connectorInt);
						nextCheckCable->setPin(cableRows[i + 1]->pin.toInt());
						break;
					}
				mainTableWidget->item(i + 1, testType == WindowType::FULL_TEST_AUTO_STAND ? 6 : 5)->setBackgroundColor(Qt::yellow);
				ProcAutoTest((int)nextCheckCable->getConnector(), nextCheckCable->getPin());
			}
		}
}

static int detectProccessedHall(ConnectorId pad, int pin, std::vector<TestTableRowProperties*> cableRows)
{
	int counter = -1;
	for (int i = 0; i < cableRows.size(); i++)
	{
		if (cableRows[i]->typeInt == TypeCable::HALL_IN)
			counter++;
		if (cableRows[i]->connectorInt == pad && cableRows[i]->pin.toInt() == pin)
			return counter;
	}
	return counter;
}

void TestWindow::Slot_ChangedByte(ConnectorId pad, int pin, int newValue)
{
	QAbstractItemModel* model = mainTableWidget->model();
	for (int row = 0; row < cableRows.size(); row++)
	{
		if (cableRows[row]->connectorInt == pad && cableRows[row]->pin.toInt() == pin)
		{
			switch (testType)
			{
			case WindowType::IN_TEST_MANUAL_STAND:
				if (cableRows[row]->typeInt == TypeCable::HALL_IN)
				{
					if (newValue == 0)
					{
						int hallId = detectProccessedHall(pad, pin, cableRows);
						hallLabels[hallId].first = 0;
						switch (viewWindowState->appTheme)
						{
						case DARK_THEME:
							hallLabels[hallId].second->setPixmap(*clockwiseDarkPixmap);
							break;

						case LIGHT_THEME:
							hallLabels[hallId].second->setPixmap(*counterclockwiseLightPixmap);
							break;
						}
					}
					else if (newValue == 1)
					{
						QWidget* wiseWidget = new QWidget(mainTableWidget);
						QLabel* clockwiseLabel = new QLabel(wiseWidget);
						clockwiseLabel->setObjectName("clockwiseLabel");
						clockwiseLabel->setText("");
						clockwiseLabel->setFixedSize(FIXED_TESTER_NAME_WIDTH, FIXED_WISE_PIXMAP_HEIGHT);
						switch (viewWindowState->appTheme)
						{
						case DARK_THEME:
							clockwiseLabel->setPixmap(*clockwiseDarkPixmap);
							break;

						case LIGHT_THEME:
							clockwiseLabel->setPixmap(*clockwiseLightPixmap);
							break;
						}
						QHBoxLayout* clockwiseCellHLayout = new QHBoxLayout(wiseWidget);
						clockwiseCellHLayout->setObjectName("moreCellHLayout");
						QSpacerItem* leftWiseSpacer = new QSpacerItem(18, 0, QSizePolicy::Fixed);
						clockwiseCellHLayout->addItem(leftWiseSpacer);
						clockwiseCellHLayout->addWidget(clockwiseLabel);
						clockwiseCellHLayout->setContentsMargins(0, 0, 0, 0);
						wiseWidget->setLayout(clockwiseCellHLayout);
						mainTableWidget->setCellWidget(row, STATUS_IN_TEST, wiseWidget);
					}
				}
				else
					model->setData(model->index(row, 5), QString::number(newValue));
				break;
			case WindowType::FULL_TEST_MANUAL_STAND:
				if (cableRows[row]->typeInt == TypeCable::HALL_IN)
				{
					if (newValue == 0)
					{
						int hallId = detectProccessedHall(pad, pin, cableRows);
						hallLabels[hallId].first = 0;
						switch (viewWindowState->appTheme)
						{
						case DARK_THEME:
							hallLabels[hallId].second->setPixmap(*clockwiseDarkPixmap);
							break;

						case LIGHT_THEME:
							hallLabels[hallId].second->setPixmap(*counterclockwiseLightPixmap);
							break;
						}
					}
				}
				else
					model->setData(model->index(row, 7), QString::number(newValue));
				break;

			default:
				break;
			}
		}
	}
}

void TestWindow::Slot_ChangedStatusStandConnect(bool statusConnect)
{
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
	case WindowType::OUT_TEST_MANUAL_STAND:
	case WindowType::FULL_TEST_MANUAL_STAND:
		setStatusTableButtons(statusConnect);
		if (statusConnect)
		{
			resetTableButtonsTheme(TypeResetTableButtonsTheme::STAND_CONNECTED, 0, 0);
			for (int i = 0; i < cableRows.size(); i++)
			{
				switch (cableRows[i]->typeInt)
				{
				case TypeCable::DIG_OUT:
					cableRows[i]->switchButtonState(TestButtons::BUTTON_OFF);
					cableRows[i]->stateDigital = OFF_BUTTON_PRESSED;
					break;

				case TypeCable::PWM_OUT:
					cableRows[i]->switchButtonState(TestButtons::BUTTON_LOAD_0);
					cableRows[i]->statePWM = LOAD0_BUTTON_PRESSED;
					break;

				case TypeCable::VNH_OUT:
					cableRows[i]->switchButtonState(TestButtons::BUTTON_OFF);
					cableRows[i]->switchButtonState(TestButtons::BUTTON_LOAD_0);
					cableRows[i]->stateDigital = OFF_BUTTON_PRESSED;
					cableRows[i]->statePWM = LOAD0_BUTTON_PRESSED;
					break;

				case TypeCable::HLD_OUT:
					cableRows[i]->switchButtonState(TestButtons::BUTTON_ZERO);
					cableRows[i]->stateHLD = ZERO_BUTTON_PRESSED;
					break;

				default:
					break;
				}
			}
		}
		else
			resetTableButtonsTheme(TypeResetTableButtonsTheme::STAND_DISCONNECTED, 0, 0);
	}
	statusFlags->StatusConnected = statusConnect;
	resetLanguage();
	resetTheme();
}

void TestWindow::initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget)
{
	interactionButtonsWidget->setObjectName("interactionButtonsWidget");
	QVBoxLayout* interactionButtonsCellVLayout = new QVBoxLayout(interactionButtonsWidget);
	interactionButtonsCellVLayout->setObjectName("interactionButtonsCellVLayout");
	if (testType == WindowType::OUT_TEST_MANUAL_STAND || testType == WindowType::FULL_TEST_MANUAL_STAND)
	{
		if (cableRows[currentRowNum]->typeStr == "DIGITAL" && cableRows[currentRowNum]->direction == "OUT")
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

			mainTableWidget->setRowHeight(currentRowNum, COLUMN_DIGITAL_HEIGHT);
		}
		else if (cableRows[currentRowNum]->typeStr == "PWM")
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

			mainTableWidget->setRowHeight(currentRowNum, COLUMN_PWM_HEIGHT);
		}
		else if (cableRows[currentRowNum]->typeStr == "VNH")
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

			mainTableWidget->setRowHeight(currentRowNum, COLUMN_VNH_HEIGHT);
		}
		else if (cableRows[currentRowNum]->typeStr == "HLD")
		{
			QHBoxLayout* interactionButtonsActiveHLayout = new QHBoxLayout(interactionButtonsWidget);
			interactionButtonsActiveHLayout->setObjectName("interactionButtonsActiveHLayout");
			interactionButtonsCellVLayout->addLayout(interactionButtonsActiveHLayout);
			QVBoxLayout* interactionButtonsActiveVLayout = new QVBoxLayout(interactionButtonsWidget);
			interactionButtonsActiveVLayout->setObjectName("interactionButtonsActiveVLayout");

			QSpacerItem* leftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* rightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);

			interactionButtonsActiveHLayout->addItem(leftSpacer);
			interactionButtonsActiveHLayout->addLayout(interactionButtonsActiveVLayout);
			interactionButtonsActiveHLayout->addItem(rightSpacer);

			QSpacerItem* upSpacer = new QSpacerItem(0, 10, QSizePolicy::Expanding);
			QSpacerItem* middleUpSpacer = new QSpacerItem(0, 10, QSizePolicy::Expanding);
			QSpacerItem* middleDownSpacer = new QSpacerItem(0, 10, QSizePolicy::Expanding);
			QSpacerItem* downSpacer = new QSpacerItem(0, 10, QSizePolicy::Expanding);

			interactionButtonsActiveVLayout->addItem(upSpacer);
			interactionButtonsActiveVLayout->addWidget(((HLDButtons*)cableRows[currentRowNum]->buttons)->highButton);
			interactionButtonsActiveVLayout->addItem(middleUpSpacer);
			interactionButtonsActiveVLayout->addWidget(((HLDButtons*)cableRows[currentRowNum]->buttons)->lowButton);
			interactionButtonsActiveVLayout->addItem(middleDownSpacer);
			interactionButtonsActiveVLayout->addWidget(((HLDButtons*)cableRows[currentRowNum]->buttons)->zeroButton);
			interactionButtonsActiveVLayout->addItem(downSpacer);

			connect(((HLDButtons*)cableRows[currentRowNum]->buttons)->highButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_high_clicked);
			connect(((HLDButtons*)cableRows[currentRowNum]->buttons)->lowButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_low_clicked);
			connect(((HLDButtons*)cableRows[currentRowNum]->buttons)->zeroButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_zero_clicked);

			mainTableWidget->setRowHeight(currentRowNum, COLUMN_HLD_HEIGHT);
		}
	}

	interactionButtonsCellVLayout->setContentsMargins(0, 0, 0, 0);
	interactionButtonsWidget->setLayout(interactionButtonsCellVLayout);
}

void TestWindow::initTableAdditionalManualChecks(int currentRowNum, QWidget* manualChecksWidget)
{
	manualChecksWidget->setObjectName("manualChecksWidget");

	QHBoxLayout* manualChecksHLayout = new QHBoxLayout(manualChecksWidget);
	manualChecksHLayout->setObjectName("manualChecksHLayout");
	manualChecksHLayout->setContentsMargins(0, 0, 0, 0);

	QSpacerItem* leftSpacer = new QSpacerItem(10, 1, QSizePolicy::Expanding);
	manualChecksHLayout->addItem(leftSpacer);
	
	QCheckBox* checkBox = new QCheckBox(manualChecksWidget);
	checkBox->setStyleSheet(lightStyles.testwindowManualCheckBox);
	manualChecks.push_back(checkBox);
	manualChecksHLayout->addWidget(manualChecks[manualChecks.size() - 1]);

	QSpacerItem* rightSpacer = new QSpacerItem(10, 1, QSizePolicy::Expanding);
	manualChecksHLayout->addItem(rightSpacer);

	manualChecksWidget->setLayout(manualChecksHLayout);
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
	connect(cableRows[currentRowNum]->moreButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_moreButton_clicked);
	moreCellWidget->setLayout(moreCellHLayout);
}

void TestWindow::resetIconMoreButton(bool theme)
{
	for (int row = 0; row < cableRows.size(); row++)
		if (theme)
			cableRows[row]->moreButton->setIcon(QIcon(*moreButtonDarkPixmap));
		else
			cableRows[row]->moreButton->setIcon(QIcon(*moreButtonLightPixmap));
}

void TestWindow::rewriteCableRows(std::vector<TestTableRowProperties*>* cableRows, int sortType)
{
	std::vector<TestTableRowProperties*> tmpCableRows(*cableRows);

	switch (sortType)
	{
	case SORT_TYPE_INDEX:
		for (int i = 0; i < cableRows->size(); i++)
		{
			bool flag = true;
			for (int j = 0; j < cableRows->size() - (i + 1); j++)
				if ((*cableRows)[j]->id > (*cableRows)[j + 1]->id) 
				{
					flag = false;
					std::swap((*cableRows)[j], (*cableRows)[j + 1]);
				}
			if (flag) 
				break;
		}
		break;

	case SORT_TYPE_DIRECTION_OUT:
		cableRows->clear();
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "OUT")
			{
				auto p = new TestTableRowProperties();
				cableRows->push_back(p);
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "IN")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		break;

	case SORT_TYPE_DIRECTION_IN:
		cableRows->clear();
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "IN")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		for (int i = 0; i < tmpCableRows.size(); i++)
			if (tmpCableRows[i]->direction == "OUT")
			{
				cableRows->push_back(new TestTableRowProperties());
				(*cableRows)[cableRows->size() - 1] = tmpCableRows[i];
			}
		break;
	}
}

void TestWindow::on_fullTestSortButton_clicked()
{
	switch (fullTestSortType)
	{
	case SORT_TYPE_INDEX:
		fullTestSortType = SORT_TYPE_DIRECTION_OUT;

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо выходам"));
			break;

		case ENGLISH_LANG:
			fullTestSortButton->setText(QString("Sort:\nout first"));
			break;
		}
		break;

	case SORT_TYPE_DIRECTION_OUT:
		fullTestSortType = SORT_TYPE_DIRECTION_IN;

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо входам"));
			break;

		case ENGLISH_LANG:
			fullTestSortButton->setText(QString("Sort:\nin first"));
			break;
		}
		break;

	case SORT_TYPE_DIRECTION_IN:
		fullTestSortType = SORT_TYPE_INDEX;

		switch (viewWindowState->appLanguage)
		{
		case RUSSIAN_LANG:
			fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			break;

		case ENGLISH_LANG:
			fullTestSortButton->setText(QString("Sort:\nnum"));
			break;
		}
		break;
	}

	if (testType == WindowType::FULL_TEST_MANUAL_STAND)
	{
		for (int i = 0; i < cableRows.size(); i++)
			cableRows[i]->manualChecked = manualChecks[i]->isChecked();
		manualChecks.clear();
	}

	rewriteCableRows(&cableRows, fullTestSortType);

	mainTableWidget->clear();

	if (testType == WindowType::FULL_TEST_MANUAL_STAND)
	{
		resetTableHeaderFullTestManualStand();
		resetTableRowsFullTestManualStand();

		for (int i = 0; i < manualChecks.size(); i++)
			manualChecks[i]->setChecked(cableRows[i]->manualChecked);
	}
	else if (testType == WindowType::FULL_TEST_AUTO_STAND)
	{
		resetTableHeaderFullTestAutoStand();
		resetTableRowsFullTestAutoStand();
	}

	Can::clearOldValue();
}

void TestTableRowProperties::on_moreButton_clicked()
{
	MoreWindow* moreWindow = nullptr;
	switch (typeInt)
	{
	case TypeCable::DIG_OUT:
	case TypeCable::PWM_OUT:
	case TypeCable::VNH_OUT:
	case TypeCable::HLD_OUT:
		moreWindow = new MoreWindowOut(this);
		break;

	case TypeCable::DIG_IN:
	case TypeCable::HALL_IN:
		moreWindow = new MoreWindowIn(this);
		break;

	case TypeCable::ANALOG_IN:
		moreWindow = new MoreWindowInAnalog(this);
		break;

	default:
		generateWarning(Warnings::TestWindow::OPEN_MORE_WINDOW);
		break;
	}
		
	WindowFrame w(WindowType::MOREWINDOW, nullptr, (MoreWindow*)moreWindow);
	w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
	w.show();
	moreWindow->exec();
}

void TestTableRowProperties::on_checkButton_clicked()
{
	Can::sendTestMsg(connectorInt, pin.toInt(), typeInt, TestBlockName::BCM);
}