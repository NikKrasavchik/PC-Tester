#include "TestWindow.h"

TestWindow::TestWindow(WindowType testType, std::vector<Cable> cables, Can* can, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->testType = testType;
	this->can = can;
	standConected = false;

	initUiMain();
	initUiMainHeader();
	initUiTable();

	initLightStyleSheets();
	initDarkStyleSheets();

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
	initRecources();
	initIcons();
	initConnections();
	initStyles();

	th = new autoStandTwoThread(can);
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

void TestWindow::generateRowsInteractionButtons(TestTableRowProperties* rowTable)
{
	mainTableWidget->setRowCount(cableRows.size());

	switch (testType)
	{
	case WindowType::IN_TEST_MANUAL_STAND:
		break;

	case WindowType::OUT_TEST_MANUAL_STAND:
		break;

	case WindowType::FULL_TEST_MANUAL_STAND:
		break;

	case WindowType::IN_MANUAL_TEST_AUTO_STAND:
		break;

	case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
		break;

	case WindowType::IN_AUTO_TEST_AUTO_STAND:
		break;

	case WindowType::OUT_AUTO_TEST_AUTO_STAND:
		break;

	case WindowType::FULL_TEST_AUTO_STAND:
		break;

	default:
		break;
	}
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
			if (fullTestManualStandTypeSort)
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
			else
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
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
			if (fullTestManualStandTypeSort)
				fullTestManualStandSortButton->setText(QString("Sort:\ntype"));
			else
				fullTestManualStandSortButton->setText(QString("Sort:\nnum"));
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
}

void TestWindow::initStyles()
{
	resetTheme();
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
	// ПЕРЕДЕЛАТЬ - надо перенести в слот срабатывающий от сигнала второго потока о том что стенд подключён или откючен
	standConected = !standConected;
	resetLanguage();
	resetTheme();
	//
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
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandSortButton->setStyleSheet(lightStyles.testwindowButtonStyle);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:

			break;

		case WindowType::IN_TEST_MANUAL_STAND:

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
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandSortButton->setStyleSheet(darkStyles.testwindowButtonStyle);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:

			break;

		case WindowType::IN_TEST_MANUAL_STAND:

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
			if (fullTestManualStandTypeSort)
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо типу"));
			else
				fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка:\nпо нумерации"));
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(outManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			if (standConected)
				inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			createItemManualTestAutoStandTestTimeComboBox(inManualTestAutoStandTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			if (standConected)
				outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			if (standConected)
				inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			if (standConected)
				fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nподключён"));
			else
				fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд\nнеподключён"));
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
			if (fullTestManualStandTypeSort)
				fullTestManualStandSortButton->setText(QString("Sort:\ntype"));
			else
				fullTestManualStandSortButton->setText(QString("Sort:\nnum"));
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
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

void TestWindow::initTableRowButtons(int currentRowNum, QWidget* interactionButtonsWidget, QWidget* moreCellWidget)
{	
	interactionButtonsWidget->setObjectName("interactionButtonsWidget");
	QVBoxLayout* interactionButtonsCellVLayout = new QVBoxLayout(interactionButtonsWidget);
	interactionButtonsCellVLayout->setObjectName("interactionButtonsCellVLayout");

	if (cableRows[currentRowNum]->type == "DIGITAL")
	{
		QHBoxLayout* interactionButtonsCellHLayout = new QHBoxLayout(interactionButtonsWidget);
		interactionButtonsCellHLayout->setObjectName("interactionButtonsCellHLayout");
		interactionButtonsCellVLayout->addLayout(interactionButtonsCellHLayout);
		interactionButtonsCellHLayout->addWidget(((DigitalButtons*)cableRows[currentRowNum]->buttons)->onButton);
		interactionButtonsCellHLayout->addWidget(((DigitalButtons*)cableRows[currentRowNum]->buttons)->offButton);

		connect(((DigitalButtons*)cableRows[currentRowNum]->buttons)->onButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_onButton_clicked);
		connect(((DigitalButtons*)cableRows[currentRowNum]->buttons)->offButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_offButton_clicked);

		mainTableWidget->setRowHeight(currentRowNum, 50);
	}
	else if (cableRows[currentRowNum]->type == "PWM")
	{
		QHBoxLayout* interactionButtonsCellFirstHLayout = new QHBoxLayout(interactionButtonsWidget);
		interactionButtonsCellFirstHLayout->setObjectName("interactionButtonsCellFirstHLayout");
		interactionButtonsCellVLayout->addLayout(interactionButtonsCellFirstHLayout);
		QHBoxLayout* interactionButtonsCellSecondHLayout = new QHBoxLayout(interactionButtonsWidget);
		interactionButtonsCellSecondHLayout->setObjectName("interactionButtonsCellSecondHLayout");
		interactionButtonsCellVLayout->addLayout(interactionButtonsCellSecondHLayout);

		interactionButtonsCellFirstHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load0Button);
		interactionButtonsCellFirstHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load25Button);
		interactionButtonsCellSecondHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load50Button);
		interactionButtonsCellSecondHLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load75Button);
		interactionButtonsCellVLayout->addWidget(((PWMButtons*)cableRows[currentRowNum]->buttons)->load100Button);

		connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load0Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load0Button_clicked);
		connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load25Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load25Button_clicked);
		connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load50Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load50Button_clicked);
		connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load75Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load75Button_clicked);
		connect(((PWMButtons*)cableRows[currentRowNum]->buttons)->load100Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load100Button_clicked);

		mainTableWidget->setRowHeight(currentRowNum, 100);
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

		interactionButtonsCellFirstHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load0Button);
		interactionButtonsCellFirstHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load25Button);
		interactionButtonsCellSecondHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load50Button);
		interactionButtonsCellSecondHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load75Button);
		interactionButtonsCellVLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->load100Button);
		interactionButtonsCellThirdHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->onButton);
		interactionButtonsCellThirdHLayout->addWidget(((VNHButtons*)cableRows[currentRowNum]->buttons)->offButton);
		interactionButtonsCellVLayout->addLayout(interactionButtonsCellThirdHLayout);

		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load0Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load0Button_clicked);
		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load25Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load25Button_clicked);
		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load50Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load50Button_clicked);
		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load75Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load75Button_clicked);
		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->load100Button, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_load100Button_clicked);
		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->onButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_onButton_clicked);
		connect(((VNHButtons*)cableRows[currentRowNum]->buttons)->offButton, &QPushButton::clicked, cableRows[currentRowNum], &TestTableRowProperties::on_offButton_clicked);

		mainTableWidget->setRowHeight(currentRowNum, 150);
	}

	interactionButtonsCellVLayout->setContentsMargins(0, 0, 0, 0);
	interactionButtonsWidget->setLayout(interactionButtonsCellVLayout);

	cableRows[currentRowNum]->moreButton = new QPushButton(mainLayoutWidget);
	cableRows[currentRowNum]->moreButton->setObjectName("moreButton");

	moreCellWidget->setObjectName("deleteCellWidget");
	QHBoxLayout* moreCellHLayout = new QHBoxLayout(moreCellWidget);
	moreCellHLayout->setObjectName("deleteCellWidget");
	moreCellHLayout->addWidget(cableRows[currentRowNum]->moreButton);
	moreCellHLayout->setContentsMargins(0, 0, 0, 0);
	moreCellWidget->setLayout(moreCellHLayout);
}