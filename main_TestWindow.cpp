#include "TestWindow.h"

TestWindow::TestWindow(WindowType testType, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->testType = testType;

	initUiMain();
	initUiMainHeader();
	initUiTable();

	initLightStyleSheets();
	initDarkStyleSheets();

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
	}

	initUiMainFooter();

	initTexts();
	initRecources();
	initIcons();
	initConnections();
	initStyles();
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
			fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка: по нумерации"));
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			// Добавить заполнение combo box
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			// Добавить заполнение combo box
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			fullTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка: по нумерации"));
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));
		testerNameLineEdit->setText(QString("Ivanov Ivan Ivanovich"));
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandSortButton->setText(QString("Sort: num"));
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			// Добавить заполнение combo box
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			// Добавить заполнение combo box
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outAutoTestAutoStandConnectButton->setText(QString("Stand connected"));
			outAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inAutoTestAutoStandConnectButton->setText(QString("Stand connected"));
			inAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			fullTestAutoStandConnectButton->setText(QString("Stand connected"));
			fullTestAutoStandStartTestButton->setText(QString("Start"));
			fullTestAutoStandSortButton->setText(QString("Sort: num"));
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
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			outManualTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyle);
			outManualTestAutoStandTestTimeComboBox->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			inManualTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyle);
			inManualTestAutoStandTestTimeComboBox->setStyleSheet(lightStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			outAutoTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyle);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			inAutoTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyle);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			fullTestAutoStandConnectButton->setStyleSheet(lightStyles.testwindowConnectButtonStyle);
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
		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			outManualTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyle);
			outManualTestAutoStandTestTimeComboBox->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			inManualTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyle);
			inManualTestAutoStandTestTimeComboBox->setStyleSheet(darkStyles.testwindowTestTimeComboBox);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			outAutoTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyle);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			inAutoTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyle);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			fullTestAutoStandConnectButton->setStyleSheet(darkStyles.testwindowConnectButtonStyle);
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
			fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка: по нумерации"));
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			// Добавить заполнение combo box
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			// Добавить заполнение combo box
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			parentFrame->setTitle(WindowType::FULL_TEST_AUTO_STAND);
			fullTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка: по нумерации"));
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));
		testerNameLineEdit->setText(QString("Ivanov Ivan Ivanovich"));

		switch (testType)
		{
		case WindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandSortButton->setText(QString("Sort: num"));
			parentFrame->setTitle(WindowType::FULL_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::OUT_TEST_MANUAL_STAND);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			parentFrame->setTitle(WindowType::IN_TEST_MANUAL_STAND);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			parentFrame->setTitle(WindowType::OUT_MANUAL_TEST_AUTO_STAND);
			// Добавить заполнение combo box
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			parentFrame->setTitle(WindowType::IN_MANUAL_TEST_AUTO_STAND);
			// Добавить заполнение combo box
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outAutoTestAutoStandConnectButton->setText(QString("Stand connected"));
			parentFrame->setTitle(WindowType::OUT_AUTO_TEST_AUTO_STAND);
			outAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inAutoTestAutoStandConnectButton->setText(QString("Stand connected"));
			parentFrame->setTitle(WindowType::IN_AUTO_TEST_AUTO_STAND);
			inAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			fullTestAutoStandConnectButton->setText(QString("Stand connected"));
			parentFrame->setTitle(WindowType::FULL_TEST_AUTO_STAND);
			fullTestAutoStandStartTestButton->setText(QString("Start"));
			fullTestAutoStandSortButton->setText(QString("Sort: num"));
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