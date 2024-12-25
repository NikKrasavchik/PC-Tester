#include "ConfiguratorWindow.h"

#define COLOUMN_COUNT_FULL			13
#define COLOUMN_COUNT_MANUAL		9
#define COLOUMN_COUNT_AUTO			10

#define COLOUMN_CONNECTOR_WIDTH		120
#define COLOUMN_PIN_WIDTH			35
#define COLOUMN_DIRECTION_WIDTH		140
#define COLOUMN_TYPE_WIDTH			120
#define COLOUMN_CANID_WIDTH         70
#define COLOUMN_BIT_WIDTH			35
#define COLOUMN_THRESHOLDS_WIDTH	60
#define COLOUMN_NAME_WIDTH			45
#define COLOUMN_COMPONENT_WIDTH		80
#define COLOUMN_REMUVE_WIDTH		80

ConfiguratorWindow::ConfiguratorWindow(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	isAllInit = false;
	standTypeSelected = STAND_NOT_SET;

	initLightStyleSheets();
	initDarkStyleSheets();
	initUi();
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
	setMinimumSize(QSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("mainLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

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
	initText();
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

	loadButton = new QPushButton(usefulSpaceWidget);
	loadButton->setObjectName("loadButton");
	loadButton->setFixedSize(FIXED_HALF_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT);
	usefulSpaceHLayout->addWidget(loadButton);

	fileNameLineEdit = new QLineEdit(usefulSpaceWidget);
	fileNameLineEdit->setObjectName("fileNameLineEdit");
	fileNameLineEdit->setFixedSize(FIXED_FILE_NAME_WIDTH, FIXED_FILE_NAME_HEIGHT);
	fileNameLineEdit->setReadOnly(true);
	usefulSpaceHLayout->addWidget(fileNameLineEdit);

	saveButton = new QPushButton(usefulSpaceWidget);
	saveButton->setObjectName("saveButton");
	saveButton->setFixedSize(FIXED_HALF_BUTTON_WIDTH, FIXED_HEADER_BUTTON_HEIGHT);
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
	mainTableHeaderLabels = new QStringList();
	resetFullTable();
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
	QMetaObject::connectSlotsByName(this);
}

void ConfiguratorWindow::resetFullTable()
{
	mainTableWidget->setRowCount(0);
	mainTableWidget->setColumnCount(0);

	mainTableWidget->setColumnCount(COLOUMN_COUNT_FULL);

	resetHeaderLanguage(STAND_NOT_SET);

	mainTableWidget->setColumnWidth((int)FullColoumnName::CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::PIN,			COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::DIRECTION,	COLOUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::TYPE,			COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::CAN_ID,		COLOUMN_CANID_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::BYTE,			COLOUMN_BIT_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::MIN_CURRENT,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::MAX_CURRENT,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::MIN_VOLTAGE,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::MAX_VOLTAGE,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::COMPONENT,	COLOUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth((int)FullColoumnName::DEL,			COLOUMN_REMUVE_WIDTH);

	for (int i = 0; i < COLOUMN_COUNT_FULL; i++)
		mainTableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);

	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::DIRECTION,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::CAN_ID,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::BYTE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::MIN_CURRENT,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::MAX_CURRENT,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::MIN_VOLTAGE,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::MAX_VOLTAGE,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::COMPONENT,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)FullColoumnName::DEL,			QHeaderView::Fixed);
}

void ConfiguratorWindow::resetManualTable()
{
	mainTableWidget->setRowCount(0);
	mainTableWidget->setColumnCount(0);

	mainTableWidget->setColumnCount(COLOUMN_COUNT_MANUAL);

	resetHeaderLanguage(STAND_MANUAL);

	mainTableWidget->setColumnWidth((int)ManualColoumnName::CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::PIN,		COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::DIRECTION,	COLOUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::TYPE,		COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::CAN_ID,		COLOUMN_CANID_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::BYTE,		COLOUMN_BIT_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::COMPONENT,	COLOUMN_COMPONENT_WIDTH);
	mainTableWidget->setColumnWidth((int)ManualColoumnName::DEL,		COLOUMN_REMUVE_WIDTH);

	for (int i = 0; i < COLOUMN_COUNT_MANUAL; i++)
		mainTableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);

	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::CONNECTOR,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::DIRECTION,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::CAN_ID,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::BYTE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::COMPONENT,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)ManualColoumnName::DEL,			QHeaderView::Fixed);
}

void ConfiguratorWindow::resetAutoTable()
{
	mainTableWidget->setRowCount(0);
	mainTableWidget->setColumnCount(0);

	mainTableWidget->setColumnCount(COLOUMN_COUNT_AUTO);

	resetHeaderLanguage(STAND_AUTO);

	mainTableWidget->setColumnWidth((int)AutoColoumnName::CONNECTOR,	COLOUMN_CONNECTOR_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::PIN,			COLOUMN_PIN_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::DIRECTION,	COLOUMN_DIRECTION_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::TYPE,			COLOUMN_TYPE_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::MIN_CURRENT,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::MAX_CURRENT,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::MIN_VOLTAGE,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::MAX_VOLTAGE,	COLOUMN_THRESHOLDS_WIDTH);
	mainTableWidget->setColumnWidth((int)AutoColoumnName::DEL,			COLOUMN_REMUVE_WIDTH);

	for (int i = 0; i < COLOUMN_COUNT_AUTO; i++)
		mainTableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Interactive);

	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::CONNECTOR,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::PIN,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::DIRECTION,		QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::TYPE,			QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::MIN_CURRENT,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::MIN_VOLTAGE,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::MAX_VOLTAGE,	QHeaderView::Fixed);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::NAME,			QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode((int)AutoColoumnName::DEL,			QHeaderView::Fixed);
}

void ConfiguratorWindow::resetHeaderLanguage(int standType)
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		switch (standType)
		{
		case STAND_NOT_SET:
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Коннектор"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Пин"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Направление"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Can ID"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Бит"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Мин A"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Макс A"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Мин V"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Макс V"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Название"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Компонент"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit(""));
			break;

		case STAND_MANUAL:
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Коннектор"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Пин"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Направление"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Can ID"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Бит"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Название"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Компонент"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit(""));
			break;

		case STAND_AUTO:
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Коннектор"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Пин"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Направление"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Тип"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Мин A"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Макс A"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Мин V"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Макс V"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit("Название"));
			mainTableHeaderLabels->push_back(QString::fromLocal8Bit(""));
			break;
		}
		break;

	case ENGLISH_LANG:
		delete mainTableHeaderLabels;
		mainTableHeaderLabels = new QStringList();
		switch (standType)
		{
		case STAND_NOT_SET:
			mainTableHeaderLabels->push_back("Connector");
			mainTableHeaderLabels->push_back("Pin");
			mainTableHeaderLabels->push_back("Direction");
			mainTableHeaderLabels->push_back("Connector type");
			mainTableHeaderLabels->push_back("Can ID");
			mainTableHeaderLabels->push_back("Bit");
			mainTableHeaderLabels->push_back("Min A");
			mainTableHeaderLabels->push_back("Max A");
			mainTableHeaderLabels->push_back("Min V");
			mainTableHeaderLabels->push_back("Max V");
			mainTableHeaderLabels->push_back("Name");
			mainTableHeaderLabels->push_back("Component");
			mainTableHeaderLabels->push_back("");
			break;

		case STAND_MANUAL:
			mainTableHeaderLabels->push_back("Connector");
			mainTableHeaderLabels->push_back("Pin");
			mainTableHeaderLabels->push_back("Direction");
			mainTableHeaderLabels->push_back("Connector type");
			mainTableHeaderLabels->push_back("Can ID");
			mainTableHeaderLabels->push_back("Bit");
			mainTableHeaderLabels->push_back("Name");
			mainTableHeaderLabels->push_back("Component");
			mainTableHeaderLabels->push_back("");
			break;

		case STAND_AUTO:
			mainTableHeaderLabels->push_back("Connector");
			mainTableHeaderLabels->push_back("Pin");
			mainTableHeaderLabels->push_back("Direction");
			mainTableHeaderLabels->push_back("Connector type");
			mainTableHeaderLabels->push_back("Min A");
			mainTableHeaderLabels->push_back("Max A");
			mainTableHeaderLabels->push_back("Min V");
			mainTableHeaderLabels->push_back("Max V");
			mainTableHeaderLabels->push_back("Name");
			mainTableHeaderLabels->push_back("");
			break;
		}
		break;
	}
	mainTableWidget->setHorizontalHeaderLabels(*mainTableHeaderLabels);
}

void ConfiguratorWindow::initText()
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
		if (fileNameLineEdit->text() == "" || fileNameLineEdit->text() == "New config")
			fileNameLineEdit->setText(QString::fromLocal8Bit("Новая конфигурация"));

		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		loadButton->setText(QString::fromLocal8Bit("Открыть"));
		addRowButton->setText(QString::fromLocal8Bit("Добавить элемент"));

		selectStandTypeComboBox->addItem(QString::fromLocal8Bit("Тип стенда"));
		selectStandTypeComboBox->addItem(QString::fromLocal8Bit("Ручной"));
		selectStandTypeComboBox->addItem(QString::fromLocal8Bit("Автоматический"));
		if (standTypeState != -1)
			selectStandTypeComboBox->setCurrentIndex(standTypeState);
		break;

	case ENGLISH_LANG:
		if (fileNameLineEdit->text() == "" || fileNameLineEdit->text() == QString::fromLocal8Bit("Новая конфигурация"))
			fileNameLineEdit->setText("New config");

		saveButton->setText("Save");
		loadButton->setText("Open");
		addRowButton->setText("Add element");

		selectStandTypeComboBox->addItem(QString("Stand type"));
		selectStandTypeComboBox->addItem(QString("Manual"));
		selectStandTypeComboBox->addItem(QString("Auto"));
		if (standTypeState != -1)
			selectStandTypeComboBox->setCurrentIndex(standTypeState);
		break;
	}
	resetHeaderLanguage(selectStandTypeComboBox->currentIndex());
	resetPresets();
}

void ConfiguratorWindow::resetLanguage()
{
	int standTypeState = -1;
	if (selectStandTypeComboBox->count())
	{
		standTypeState = selectStandTypeComboBox->currentIndex();
		delete selectStandTypeComboBox;
		selectStandTypeComboBox = new QComboBox(footerLayoutWidget);
		selectStandTypeComboBox->setObjectName("selectStandTypeComboBox");
		selectStandTypeComboBox->setFixedSize(FIXED_STAND_COMBO_WIDTH, FIXED_STAND_COMBO_HEIGHT);

		connect(selectStandTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectFrequencyComboBox_changed(int)));
		resetTheme();

		footerMainHLayout->insertWidget(0,selectStandTypeComboBox);
	}

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		if (fileNameLineEdit->text() == "" || fileNameLineEdit->text() == "Write file name")
			fileNameLineEdit->setText(QString::fromLocal8Bit("Введите имя файла"));

		saveButton->setText(QString::fromLocal8Bit("Сохранить"));
		loadButton->setText(QString::fromLocal8Bit("Открыть"));
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
		loadButton->setText("Open");
		addRowButton->setText("Add element");

		selectStandTypeComboBox->addItem(QString("Stand type"));
		selectStandTypeComboBox->addItem(QString("Manual"));
		selectStandTypeComboBox->addItem(QString("Auto"));
		if (standTypeState != -1)
			selectStandTypeComboBox->setCurrentIndex(standTypeState);

		break;
	}
	resetHeaderLanguage(selectStandTypeComboBox->currentIndex());
	parentFrame->setTitle(WindowType::CONFIGURATOR);
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

		saveButton->setStyleSheet(lightStyles.configuratorButton);
		loadButton->setStyleSheet(lightStyles.configuratorButton);
		addRowButton->setStyleSheet(lightStyles.configuratorButton);
		backButton->setStyleSheet(lightStyles.configuratorMoveButtonStyle);
		switchThemeButton->setStyleSheet(lightStyles.configuratorMoveButtonStyle);
		switchLanguageButton->setStyleSheet(lightStyles.configuratorMoveButtonStyle);
		fileNameLineEdit->setStyleSheet(lightStyles.configuratorLineEdit);
		selectStandTypeComboBox->setStyleSheet(lightStyles.configuratorComboBox);
		mainTableWidget->setStyleSheet(lightStyles.configuratorTableWidget);
		break;

	case DARK_THEME:
		logoLabel->setPixmap(*logoDarkPixmap);
		switchThemeButton->setIcon(QIcon(*themeDarkPixmap));
		switchLanguageButton->setIcon(QIcon(*languageDarkPixmap));
		backButton->setIcon(QIcon(*backButtonDarkPixmap));
		backButton->setIconSize(backButton->size());

		saveButton->setStyleSheet(darkStyles.configuratorButton);
		loadButton->setStyleSheet(darkStyles.configuratorButton);
		addRowButton->setStyleSheet(darkStyles.configuratorButton);
		backButton->setStyleSheet(darkStyles.configuratorMoveButtonStyle);
		switchThemeButton->setStyleSheet(darkStyles.configuratorMoveButtonStyle);
		switchLanguageButton->setStyleSheet(darkStyles.configuratorMoveButtonStyle);
		fileNameLineEdit->setStyleSheet(darkStyles.configuratorLineEdit);
		selectStandTypeComboBox->setStyleSheet(darkStyles.configuratorComboBox);
		mainTableWidget->setStyleSheet(darkStyles.configuratorTableWidget);
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