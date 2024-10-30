#include "TestWindow.h"

TestWindow::TestWindow(TestWindowType testType, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	this->testType = testType;

	initUiMain();
	initUiMainHeader();
	initUiTable();

	switch (testType)
	{
	case TestWindowType::IN_TEST_MANUAL_STAND:
		initUiInTestManualStand();
		break;

	case TestWindowType::OUT_TEST_MANUAL_STAND:
		initUiOutTestManualStand();
		break;

	case TestWindowType::FULL_TEST_MANUAL_STAND:
		initUiFullTestManualStand();
		break;

	case TestWindowType::IN_MANUAL_TEST_AUTO_STAND:
		initUiInManualTestAutoStand();
		break;

	case TestWindowType::OUT_MANUAL_TEST_AUTO_STAND:
		initUiOutManualTestAutoStand();
		break;

	case TestWindowType::IN_AUTO_TEST_AUTO_STAND:
		initUiInAutoTestAutoStand();
		break;

	case TestWindowType::OUT_AUTO_TEST_AUTO_STAND:
		initUiOutAutoTestAutoStand();
		break;

	case TestWindowType::FULL_TEST_AUTO_STAND:
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
	resize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
	setMinimumSize(QSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("mainLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

	mainVlayout = new QVBoxLayout(mainLayoutWidget);
	mainVlayout->setObjectName("mainVlayout");
}

void TestWindow::initUiMainHeader()
{
	headerLayoutWidget = new QWidget(mainLayoutWidget);
	headerLayoutWidget->setObjectName("headerLayoutWidget");
	headerLayoutWidget->setFixedHeight(FIXED_HEADER_HEIGHT);
	mainVlayout->addWidget(headerLayoutWidget);

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
	mainVlayout->addWidget(headerLine);
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
	mainVlayout->addWidget(mainTableWidget);
}

void TestWindow::initUiMainFooter()
{
	footerLine = new QFrame(mainLayoutWidget);
	footerLine->setObjectName("footerLine");
	footerLine->setFrameShape(QFrame::HLine);
	mainVlayout->addWidget(footerLine);

	footerLayoutWidget = new QWidget(mainLayoutWidget);
	footerLayoutWidget->setObjectName("footerLayoutWidget");
	footerLayoutWidget->setFixedHeight(FIXED_FOOTER_HEIGHT);
	mainVlayout->addWidget(footerLayoutWidget);

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

	switchLanguage();
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
	connect(backButton,				&QPushButton::clicked, this, &TestWindow::on_backButton_clicked);
	connect(switchThemeButton,		&QPushButton::clicked, this, &TestWindow::on_switchThemeButton_clicked);
	connect(switchLanguageButton,	&QPushButton::clicked, this, &TestWindow::on_switchLanguageButton_clicked);
	connect(reportButton,			&QPushButton::clicked, this, &TestWindow::on_reportButton_clicked);

	connect(fullTestManualStandSortButton,			&QPushButton::clicked, this, &TestWindow::on_fullTestManualStandSortButton_clicked);
	connect(inManualTestAutoStandConnectButton,		&QPushButton::clicked, this, &TestWindow::on_inManualTestAutoStandConnectButton_clicked);
	connect(inManualTestAutoStandTestTimeComboBox,	SIGNAL(currentIndexChanged(int)), this, SLOT(on_inManualTestAutoStandTestTimeComboBox_changed(int)));
	connect(outManualTestAutoStandConnectButton,	&QPushButton::clicked, this, &TestWindow::on_outManualTestAutoStandConnectButton_clicked);
	connect(outManualTestAutoStandTestTimeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_outManualTestAutoStandTestTimeComboBox_changed(int)));
	connect(inAutoTestAutoStandConnectButton,		&QPushButton::clicked, this, &TestWindow::on_inAutoTestAutoStandConnectButton_clicked);
	connect(inAutoTestAutoStandStartTestButton,		&QPushButton::clicked, this, &TestWindow::on_inAutoTestAutoStandStartTestButton_clicked);
	connect(outAutoTestAutoStandConnectButton,		&QPushButton::clicked, this, &TestWindow::on_outAutoTestAutoStandConnectButton_clicked);
	connect(outAutoTestAutoStandStartTestButton,	&QPushButton::clicked, this, &TestWindow::on_outAutoTestAutoStandStartTestButton_clicked);
	connect(fullTestAutoStandConnectButton,			&QPushButton::clicked, this, &TestWindow::on_fullTestAutoStandConnectButton_clicked);
	connect(fullTestAutoStandStartTestButton,		&QPushButton::clicked, this, &TestWindow::on_fullTestAutoStandStartTestButton_clicked);
	connect(fullTestAutoStandSortButton,			&QPushButton::clicked, this, &TestWindow::on_fullTestAutoStandSortButton_clicked);

}

void TestWindow::initStyles()
{

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
	switchTheme();
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
	switchLanguage();
}

void TestWindow::on_reportButton_clicked()
{

}

void TestWindow::switchTheme()
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

void TestWindow::switchLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		reportButton->setText(QString::fromLocal8Bit("Отчёт"));

		switch (testType)
		{
		case TestWindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandSortButton->setText(QString::fromLocal8Bit("Сортировка: по нумерации"));
			break;

		case TestWindowType::OUT_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			// Добавить заполнение combo box
			break;

		case TestWindowType::IN_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inManualTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			// Добавить заполнение combo box
			break;

		case TestWindowType::IN_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			inAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case TestWindowType::OUT_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outAutoTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			outAutoTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			break;

		case TestWindowType::FULL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			fullTestAutoStandConnectButton->setText(QString::fromLocal8Bit("Стенд подключён"));
			fullTestAutoStandStartTestButton->setText(QString::fromLocal8Bit("Старт"));
			fullTestAutoStandSortButton->setText(QString::fromLocal8Bit("Сортировка: по нумерации"));
			break;

		default:
			break;
		}
		break;

	case ENGLISH_LANG:
		reportButton->setText(QString("Report"));

		switch (testType)
		{
		case TestWindowType::FULL_TEST_MANUAL_STAND:
			fullTestManualStandSortButton->setText(QString("Sort: num"));
			break;

		case TestWindowType::OUT_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			// Добавить заполнение combo box
			break;

		case TestWindowType::IN_MANUAL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			// Добавить заполнение combo box
			break;

		case TestWindowType::IN_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			inAutoTestAutoStandConnectButton->setText(QString("Stand connected"));
			inAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case TestWindowType::OUT_AUTO_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			outManualTestAutoStandConnectButton->setText(QString("Stand connected"));
			outAutoTestAutoStandStartTestButton->setText(QString("Start"));
			break;

		case TestWindowType::FULL_TEST_AUTO_STAND:
			// Добавить условие на подключение стенда
			fullTestAutoStandConnectButton->setText(QString("Stand connected"));
			fullTestAutoStandStartTestButton->setText(QString("Start"));
			fullTestAutoStandSortButton->setText(QString("Sort: num"));
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