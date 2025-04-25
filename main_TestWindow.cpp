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
	if(viewWindowState->selectedBlock == TestBlockName::DTM)
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

	reportSpacerTwo = new QSpacerItem(10, 0, QSizePolicy::Fixed);
	reportHLayout->addItem(reportSpacerTwo);

	sleepButton = new QPushButton(footerLayoutWidget);
	sleepButton->setObjectName("sleepButton");
	sleepButton->setFixedSize(FIXED_REPORT_BUTTON_WIDTH + 50, FIXED_REPORT_BUTTON_HEIGHT);
	reportHLayout->addWidget(sleepButton);
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
	resetLanguage();
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
	connect(backButton, &QPushButton::clicked, this, &TestWindow::slot_backButton_clicked);
	connect(switchThemeButton, &QPushButton::clicked, this, &TestWindow::slot_switchThemeButton_clicked);
	connect(switchLanguageButton, &QPushButton::clicked, this, &TestWindow::slot_switchLanguageButton_clicked);
	connect(reportButton, &QPushButton::clicked, this, &TestWindow::slot_reportButton_clicked);
	connect(sleepButton, &QPushButton::clicked, this, &TestWindow::slot_sleepButton_clicked);
	connect(autoStandStartTestButton, &QPushButton::clicked, this, &TestWindow::slot_autoStandStartTestButton_clicked);
	connect(fullTestSortButton, &QPushButton::clicked, this, &TestWindow::slot_fullTestSortButton_clicked);
	connect(mainTableWidget, &QTableWidget::cellClicked, this, &TestWindow::slot_mainTableWidget_cellClicked);
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
				case TypeCable::CAN_OUT:
				case TypeCable::LIN_OUT:
					((CheckInfomationBus*)(cableRows[i]->buttons))->checkButton->setStyleSheet(lightStyles.inactiveTableButton);
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
				case TypeCable::CAN_OUT:
				case TypeCable::LIN_OUT:
					((CheckInfomationBus*)(cableRows[i]->buttons))->checkButton->setStyleSheet(lightStyles.inactiveTableButton);
					break;
				default:
					break;
				}
		break;
	}
}

void TestWindow::slot_backButton_clicked()
{
	this->close();
}

void TestWindow::slot_switchThemeButton_clicked()
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

void TestWindow::slot_switchLanguageButton_clicked()
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

void TestWindow::slot_reportButton_clicked()
{
	ReportWindow* reportWindow;

	reportWindow = new ReportWindow(cableRows, viewWindowState->selectedBlock);
	reportWindow->setTestingType(testType);

	WindowFrame w(WindowType::REPORTWINDOW, nullptr, reportWindow);
	w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
	w.show();
	reportWindow->exec();
}

void TestWindow::slot_sleepButton_clicked()
{
	if (statusFlags->StatusConnected)
		Can::sendGoToSleepMsg(true);
	else
		Can::sendGoToSleepMsg(false);
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
		sleepButton->setStyleSheet(lightStyles.testwindowMoveButtonStyle);
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
		sleepButton->setStyleSheet(darkStyles.testwindowMoveButtonStyle);
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
#ifdef QT5
		reportButton->setText(QString::fromLocal8Bit("Отчёт"));
		if (statusFlags->StatusConnected)
			sleepButton->setText(QString::fromLocal8Bit("Заснуть"));
		else
			sleepButton->setText(QString::fromLocal8Bit("Проснуться"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				fullTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			switch (fullTestSortType)
			{
			case SortType::SortIndex:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо порядку"));
				break;
			case SortType::SortComponents:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо компонентам"));
				break;
			case SortType::SortType:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
				break;
			default:
				break;
			}
			resetLanguageFullTestManualStand();
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				outTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			resetLanguageOutTestManualStand();
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nподключён"));
			else
				inTestManualStandConnectButton->setText(QString::fromLocal8Bit("Блок\nотключен"));
			resetLanguageInTestManualStand();
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			resetLanguageOutManualTestAutoStand();
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			resetLanguageInManualTestAutoStand();
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				autoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nотключен"));
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
			if (isFullTestEnabled)
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Пауза"));
			else
				autoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			switch (fullTestSortType)
			{
			case SortType::SortIndex:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо порядку"));
				break;
			case SortType::SortComponents:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо компонентам"));
				break;
			case SortType::SortType:
				fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
				break;
			default:
				break;
			}
			resetLanguageFullTestAutoStand();
#elif QT6

#endif // QT5
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));
		if (statusFlags->StatusConnected)
			sleepButton->setText(QString("Go to sleep"));
		else
			sleepButton->setText(QString("Wake up"));


		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				fullTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				fullTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			switch (fullTestSortType)
			{
			case SortType::SortIndex:
				fullTestSortButton->setText(QString("Sort:\nin order"));
				break;
			case SortType::SortComponents:
				fullTestSortButton->setText(QString("Sort:\nby component"));
				break;
			case SortType::SortType:
				fullTestSortButton->setText(QString("Sort:\nby type"));
				break;
			default:
				break;
			}
			resetLanguageFullTestManualStand();
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				outTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				outTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			resetLanguageOutTestManualStand();
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			if (statusFlags->StatusConnected)
				inTestManualStandConnectButton->setText(QString("ECU\nconnected"));
			else
				inTestManualStandConnectButton->setText(QString("ECU\ndisconnected"));
			resetLanguageInTestManualStand();
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			resetLanguageOutManualTestAutoStand();
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			resetLanguageInManualTestAutoStand();
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (statusFlags->StatusConnected)
				autoStandConnectButton->setText(QString("Stand\nconnected"));
			else
				autoStandConnectButton->setText(QString("Stand\ndisconnected"));
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
			if (isFullTestEnabled)
				autoStandStartTestButton->setText(QString("Pause"));
			else
				autoStandStartTestButton->setText(QString("Start"));
			switch (fullTestSortType)
			{
			case SortType::SortIndex:
				fullTestSortButton->setText(QString("Sort:\nin order"));
				break;
			case SortType::SortComponents:
				fullTestSortButton->setText(QString("Sort:\nby component"));
				break;
			case SortType::SortType:
				fullTestSortButton->setText(QString("Sort:\nby type"));
				break;
			default:
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

	connect(switchThemeButton, &QPushButton::clicked, parentFrame, &WindowFrame::slot_switchThemeButton_clicked);
}

void TestWindow::createItemManualTestAutoStandTestTimeComboBox(QComboBox* comboBox)
{
	comboBox->clear();
	if (viewWindowState->appLanguage == RUSSIAN_LANG)
	{
#ifdef QT5
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n5 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n10 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n15 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n20 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n25 секунд"));
		comboBox->addItem(QString::fromLocal8Bit("Длительность проверки:\n30 секунд"));
#elif QT6

#endif // QT5
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
			case TypeCable::CAN_OUT:
			case TypeCable::LIN_OUT:
				((CheckInfomationBus*)(cableRows[i]->buttons))->checkButton->setDisabled(statusButton);
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


void TestWindow::slot_autoStandStartTestButton_clicked()
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

void TestWindow::Slot_ChangedByte(int idCable, int newValue)
{
	QAbstractItemModel* model = mainTableWidget->model();

	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		if (cableRows[offsetMap[idCable]]->typeInt == TypeCable::HALL_IN)
		{
			if (newValue == 0)
			{
				int hallId = detectProccessedHall(cableRows[offsetMap[idCable]]->connectorInt, cableRows[offsetMap[idCable]]->pin.toInt(), cableRows);
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
		{
			if (idCable == 0)
			{
				1;
			}
			model->setData(model->index(offsetMap[idCable], 5), QString::number(newValue));
		}
		break;
	case WindowType::FULL_TEST_MANUAL_STAND:
		if (cableRows[offsetMap[idCable]]->typeInt == TypeCable::HALL_IN)
		{
			if (newValue == 0)
			{
				int hallId = detectProccessedHall(cableRows[offsetMap[idCable]]->connectorInt, cableRows[offsetMap[idCable]]->pin.toInt(), cableRows);
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
			model->setData(model->index(offsetMap[idCable], 7), QString::number(newValue));
		break;

	default:
		break;
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

void TestWindow::Slot_changeStatusCheckInformationBus(int id, bool status)
{
	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		if(status)
			mainTableWidget->item(offsetMap[id], 5)->setBackgroundColor(Qt::green);
		else
			mainTableWidget->item(offsetMap[id], 5)->setBackgroundColor(Qt::red);
		break;
	case WindowType::FULL_TEST_MANUAL_STAND:
		if(status)
			mainTableWidget->item(offsetMap[id], 7)->setBackgroundColor(Qt::green);
		else
			mainTableWidget->item(offsetMap[id], 7)->setBackgroundColor(Qt::red);
		break;
	}
}

void TestWindow::initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget)
{
	interactionButtonsWidget->setObjectName("interactionButtonsWidget");
	QVBoxLayout* interactionButtonsCellVLayout = new QVBoxLayout(interactionButtonsWidget);
	interactionButtonsCellVLayout->setObjectName("interactionButtonsCellVLayout");
	mainTableWidget->setRowHeight(currentRowNum, COLUMN_STANDART_HEIGHT);
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


			mainTableWidget->setRowHeight(currentRowNum, COLUMN_HLD_HEIGHT);
		}
		else if(cableRows[currentRowNum]->typeInt == TypeCable::CAN_OUT || cableRows[currentRowNum]->typeInt == TypeCable::LIN_OUT)
		{
			QHBoxLayout *hLayout = new QHBoxLayout(interactionButtonsWidget);
			QVBoxLayout *vLayout = new QVBoxLayout();

			QSpacerItem* leftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
			QSpacerItem* centerSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
			QSpacerItem* rightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);

			interactionButtonsCellVLayout->addLayout(hLayout);

			hLayout->addItem(leftSpacer);
			hLayout->addLayout(vLayout);
			hLayout->addItem(rightSpacer);
			vLayout->addWidget(((CheckInfomationBus*)cableRows[currentRowNum]->buttons)->checkButton);
			vLayout->addItem(centerSpacer);
			vLayout->addWidget(((CheckInfomationBus*)cableRows[currentRowNum]->buttons)->comboBox);

			mainTableWidget->setRowHeight(currentRowNum, COLUMN_INFORMATION_HEIGHT);
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
	cableRows[currentRowNum]->manualCheckBox = checkBox;
	manualChecksHLayout->addWidget(cableRows[currentRowNum]->manualCheckBox);

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

void TestWindow::rewriteCableRows()
{
	std::vector<TestTableRowProperties*> tmpCableRows(cableRows);
	QMap<TypeCable, std::vector<TestTableRowProperties*>> sortTypeMap;

	switch (fullTestSortType)
	{
	case SortType::SortIndex:
		for (int i = 0; i < cableRows.size(); i++)
		{
			bool flag = true;
			for (int j = 0; j < cableRows.size() - (i + 1); j++)
				if (cableRows[j]->id > cableRows[j + 1]->id)
				{
					flag = false;
					std::swap(cableRows[j], cableRows[j + 1]);
					std::swap(offsetMap[j + 1], offsetMap[j + 2]);
				}
			if (flag)
				break;
		}
		break;

	case SortType::SortComponents:
		for (int i = 0; i < sortComponents.size(); i++)
			for (int j = 0; j < cableRows.size(); j++)
			{
				if (sortComponents[i].first == cableRows[j]->component && sortComponents[i].second == nullptr)
				{
					sortComponents[i].second = cableRows[j];
					cableRows.erase(cableRows.begin() + j);
				}

			}
		//cableRows.clear();
		hallLabels.clear();
		for (int i = 0; i < sortComponents.size(); i++)
		{


				if (sortComponents[i].second != nullptr)
				{

					cableRows.push_back(sortComponents[i].second);
					offsetMap[cableRows[cableRows.size() - 1]->id] = cableRows.size() - 1;
					if (sortComponents[i].second->typeInt == TypeCable::HALL_IN)
					{
						hallLabels.push_back(std::pair<int, QLabel*> {-1, new QLabel()});
					}
					sortComponents[i].second = nullptr;
				}
		}
		break;

	case SortType::SortType:
		for (int i = 0; i < cableRows.size(); i++)
			sortTypeMap[cableRows[i]->typeInt].push_back(cableRows[i]);
		cableRows.clear();
		for(TypeCable type : sortTypeMap.keys())
			for (int i = 0; i < sortTypeMap[type].size(); i++)
				cableRows.push_back(sortTypeMap[type][i]);
		break;
	}
}

void TestWindow::slot_fullTestSortButton_clicked()
{
	switch (fullTestSortType)
	{
	case SortType::SortIndex:
		fullTestSortType = SortType::SortComponents;
		if(viewWindowState->appLanguage == RUSSIAN_LANG)
			fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
		else
			fullTestSortButton->setText(QString("Sort:\nby type"));
		break;
	case SortType::SortComponents:
		fullTestSortType = SortType::SortType;
		if(viewWindowState->appLanguage == RUSSIAN_LANG)
			fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо компонентам"));
		else
			fullTestSortButton->setText(QString("Sort:\nby component"));
		break;
	case SortType::SortType:
		fullTestSortType = SortType::SortIndex;
		if(viewWindowState->appLanguage == RUSSIAN_LANG)
			fullTestSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо порядку"));
		else
			fullTestSortButton->setText(QString("Sort:\nin order"));
		break;
	default:
		break;
	}

	rewriteCableRows();

	mainTableWidget->clear();

	if (testType == WindowType::FULL_TEST_MANUAL_STAND)
		resetLanguageFullTestManualStand();
	else if (testType == WindowType::FULL_TEST_AUTO_STAND)
		resetLanguageFullTestAutoStand();


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