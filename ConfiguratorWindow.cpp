#include "ConfiguratorWindow.h"

ConfiguratorWindow::ConfiguratorWindow(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	isAllInit = false;

	initUi();

	QMetaObject::connectSlotsByName(this);
}

ConfiguratorWindow::~ConfiguratorWindow()
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

void ConfiguratorWindow::initUi()
{
	resize(viewWindowState->appSize.width, viewWindowState->appSize.height);
	setMinimumSize(QSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("mainLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));
	//mainLayoutWidget->setStyleSheet("background-color: red;");

	mainVLayout = new QVBoxLayout(mainLayoutWidget);
	mainVLayout->setObjectName("mainVLayout");

	initUiHeader();
	mainVLayout->addWidget(headerLayoutWidget);

	headerLine = new QFrame(mainLayoutWidget);
	headerLine->setObjectName("headerLine");
	headerLine->setFrameShape(QFrame::HLine);
	mainVLayout->addWidget(headerLine);

	initUiTable();
	mainVLayout->addWidget(mainTableWidget);

	footerLine = new QFrame(mainLayoutWidget);
	footerLine->setObjectName("footerLine");
	footerLine->setFrameShape(QFrame::HLine);
	mainVLayout->addWidget(footerLine);

	initUiFooter();
	mainVLayout->addWidget(footerLayoutWidget);

	initRecources();
	initConnections();

	resetTheme();
	resetLanguage();
}

void ConfiguratorWindow::initUiHeader()
{
	headerLayoutWidget = new QWidget(mainLayoutWidget);
	headerLayoutWidget->setObjectName("headerLayoutWidget");
	headerLayoutWidget->setFixedHeight(FIXED_HEADER_HEIGHT);
	//headerLayoutWidget->setStyleSheet("background-color: green;");

	headerMainHLayout = new QHBoxLayout(headerLayoutWidget);
	headerMainHLayout->setObjectName("headerMainHLayout");
	headerMainHLayout->setSpacing(0);

	initUiLogo();
	headerMainHLayout->addWidget(logoLabel);

	initUiUsefulSpace();
	headerMainHLayout->addWidget(usefulSpaceWidget);

	initUiTripleButtons();
	headerMainHLayout->addItem(tripleButtonsHLayout);
}

void ConfiguratorWindow::initUiUsefulSpace()
{
	usefulSpaceWidget = new QWidget(headerLayoutWidget);
	usefulSpaceWidget->setObjectName("usefulSpaceWidget");
	//usefulSpaceWidget->setStyleSheet("background-color: blue;");

	usefulSpaceHLayout = new QHBoxLayout(usefulSpaceWidget);
	usefulSpaceHLayout->setObjectName("usefulSpaceHLayout");

	fileNameLineEdit = new QLineEdit(usefulSpaceWidget);
	fileNameLineEdit->setObjectName("fileNameLineEdit");
	fileNameLineEdit->setFixedSize(FIXED_FILE_NAME_WIDTH, FIXED_FILE_NAME_HEIGHT);
	usefulSpaceHLayout->addWidget(fileNameLineEdit);

	saveButton = new QPushButton(usefulSpaceWidget);
	saveButton->setObjectName("saveButton");
	saveButton->setFixedSize(FIXED_HEADER_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT);
	usefulSpaceHLayout->addWidget(saveButton);
}

void ConfiguratorWindow::initUiTripleButtons()
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

void ConfiguratorWindow::initUiLogo()
{
	logoLabel = new QLabel(this);
	logoLabel->setObjectName("LogoLabel");
	logoLabel->setText("");
	logoLabel->setFixedSize(FIXED_LOGO_WIDTH, FIXED_LOGO_HEIGHT);
	logoLabel->setEnabled(true);
}

void ConfiguratorWindow::initUiFooter()
{
	footerLayoutWidget = new QWidget(mainLayoutWidget);
	footerLayoutWidget->setObjectName("footerLayoutWidget");
	//footerLayoutWidget->setStyleSheet("background-color: yellow;");

	footerMainHLayout = new QHBoxLayout(footerLayoutWidget);
	footerMainHLayout->setObjectName("footerMainHLayout");
	footerMainHLayout->setSpacing(0);

	footerCombosHLayout = new QHBoxLayout();
	footerCombosHLayout->setSpacing(20);
	footerMainHLayout->addItem(footerCombosHLayout);

	selectStandTypeComboBox = new QComboBox(footerLayoutWidget);
	selectStandTypeComboBox->setObjectName("selectStandTypeComboBox");
	selectStandTypeComboBox->setFixedSize(FIXED_STAND_COMBO_WIDTH, FIXED_STAND_COMBO_HEIGHT);
	footerMainHLayout->addWidget(selectStandTypeComboBox);

	footerSpacer = new QSpacerItem(500, 0, QSizePolicy::Expanding);
	footerMainHLayout->addItem(footerSpacer);

	addRowButton = new QPushButton(footerLayoutWidget);
	addRowButton->setObjectName("addRowButton");
	addRowButton->setFixedSize(FIXED_FOOTER_BUTTON_WIDTH, FIXED_FOOTER_BUTTON_HEIGHT);
	footerMainHLayout->addWidget(addRowButton);
}

void ConfiguratorWindow::initUiTable()
{
	mainTableWidget = new QTableWidget(mainLayoutWidget);
	mainTableWidget->setObjectName("mainTableWidget");

	mainTableWidget->setColumnCount(12);

	mainTableWidget->setHorizontalHeaderLabels(QStringList() << "Connector"
		<< "Pin"
		<< "Direction"
		<< "Connector type"
		<< "Can ID"
		<< "Bit"
		<< "Min A"
		<< "Max A"
		<< "Min V"
		<< "Max V"
		<< "Name"
		<< "");
}

void ConfiguratorWindow::initRecources()
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

void ConfiguratorWindow::initConnections()
{
	//connect(selectStandTypeComboBox, &QComboBox::activated, this, &ConfiguratorWindow::on_selectStandTypeComboBox_activated);
	//connect(selectStandTypeComboBox, &QComboBox::activated, this, &ConfiguratorWindow::on_selectStandTypeComboBox_activated);
	//connect(selectStandTypeComboBox, &QComboBox::activated, this, &ConfiguratorWindow::on_selectStandTypeComboBox_activated);
	//connect(selectStandTypeComboBox, SIGNAL(activated(int)), this, SLOT(on_selectStandTypeComboBox_activated(int)));
	//connect(selectTestTypeComboBox, SIGNAL(activated(int)), this, SLOT(on_selectTestTypeComboBox_activated(int)));
	//connect(selectDirectionComboBox, SIGNAL(activated(int)), this, SLOT(on_selectDirectionComboBox_activated(int)));
}

void ConfiguratorWindow::resetLanguage()
{
	int standTypeState = -1;
	if (selectStandTypeComboBox->count())
	{
		standTypeState = selectStandTypeComboBox->currentIndex();
		selectStandTypeComboBox->clear();
	}

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		if (fileNameLineEdit->text() == "" || fileNameLineEdit->text() == "Write file name")
			fileNameLineEdit->setText(QString::fromLocal8Bit("Введите имя файла"));

		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		addRowButton->setText(QString::fromLocal8Bit("Добавить элемент"));

		selectStandTypeComboBox->addItem(QString::fromLocal8Bit("Тип стенда"));
		selectStandTypeComboBox->addItem(QString::fromLocal8Bit("Ручной"));
		selectStandTypeComboBox->addItem(QString::fromLocal8Bit("Автоматический"));
		if (standTypeState != -1)
			selectStandTypeComboBox->setCurrentIndex(standTypeState);

		break;

	case ENGLISH_LANG:
		if (fileNameLineEdit->text() == "" || fileNameLineEdit->text() == QString::fromLocal8Bit("Введите имя файла"))
			fileNameLineEdit->setText("Write file name");

		saveButton->setText("Save");
		addRowButton->setText("Add element");

		selectStandTypeComboBox->addItem(QString("Stand type"));
		selectStandTypeComboBox->addItem(QString("Manual"));
		selectStandTypeComboBox->addItem(QString("Auto"));
		if (standTypeState != -1)
			selectStandTypeComboBox->setCurrentIndex(standTypeState);

		break;
	}

	resetPresets();
}

void ConfiguratorWindow::resetTheme()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:
		logoLabel->setPixmap(*logoLightPixmap);
		switchThemeButton->setIcon(QIcon(*themeLightPixmap));
		switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
		backButton->setIcon(QIcon(*backButtonLightPixmap));
		backButton->setIconSize(backButton->size());

		mainTableWidget->setStyleSheet(darkStyles.testwindowTableWidget);
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

void ConfiguratorWindow::on_backButton_clicked()
{
	close();
}

void ConfiguratorWindow::on_switchThemeButton_clicked()
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

void ConfiguratorWindow::on_switchLanguageButton_clicked()
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

void ConfiguratorWindow::setParentFrame(WindowFrame* parentFrame)
{
	this->parentFrame = parentFrame;

	connect(switchThemeButton, &QPushButton::clicked, parentFrame, &WindowFrame::on_switchThemeButton_clicked);
}

void ConfiguratorWindow::resizeEvent(QResizeEvent* event)
{
	viewWindowState->appSize.width = geometry().width();
	viewWindowState->appSize.height = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, viewWindowState->appSize.width - (BORDER_INDENT * 2), viewWindowState->appSize.height - (BORDER_INDENT * 2));
}