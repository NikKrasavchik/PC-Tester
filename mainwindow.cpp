#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initUi();
}

MainWindow::~MainWindow()
{}

void MainWindow::initUi()
{
	resize(MINIMUM_SCREEN_WIDTH, MINIMUM_SCREEN_HEIGHT);
	setMinimumSize(QSize(MINIMUM_SCREEN_WIDTH, MINIMUM_SCREEN_HEIGHT));

	ui.centralWidget->setStyleSheet(lightStyles.screenColor);

	logoPixmap = new QPixmap(LOGO_LIGHT_QRC);

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("MainFormLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MINIMUM_SCREEN_WIDTH - (BORDER_INDENT * 2), MINIMUM_SCREEN_HEIGHT - (BORDER_INDENT * 2));

	initStyles();
	initUiLogo();
	initUiTopHLayout();
	initUiLeftVLayout();
	initUiMainVLayout();

	mainGridLayout = new QGridLayout(mainLayoutWidget);
	mainGridLayout->setObjectName("MainFormLayout");
	mainGridLayout->setSpacing(0);
	mainGridLayout->setVerticalSpacing(0);

	mainGridLayout->addWidget(logoLabel, GRID_ROW_0, GRID_COLOUMN_0);
	mainGridLayout->addLayout(topHLayout, GRID_ROW_0, GRID_COLOUMN_1);
	mainGridLayout->addLayout(leftVLayout, GRID_ROW_1, GRID_COLOUMN_0);
	mainGridLayout->addLayout(mainVLayout, GRID_ROW_1, GRID_COLOUMN_1);
}

void MainWindow::initUiLogo()
{
	logoLabel = new QLabel(this);
	logoLabel->setObjectName("LogoLabel");
	logoLabel->setText("");
	logoLabel->setEnabled(true);
	logoLabel->setPixmap(*logoPixmap);
}

void MainWindow::initUiTopHLayout()
{
	topHLayout = new QHBoxLayout();
	topHLayout->setObjectName("topHLayout");

	switchHLayout = new QHBoxLayout();
	switchHLayout->setObjectName("switchHLayout");

	leftManualStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);

	switchHLayout->addItem(leftManualStandSpacer);

	// Manual
	manualStandButton = new QPushButton(mainLayoutWidget);
	manualStandButton->setObjectName("manualStandButton");
	manualStandButton->setText("Manual");
	manualStandButton->setFixedSize(MINIMUM_SCREEN_WIDTH * COEFFICIENT_STAND_BUTTONS_WIDTH, MINIMUM_SCREEN_HEIGHT * COEFFICIENT_STAND_BUTTONS_HEIGHT);
	manualStandButton->setStyleSheet(lightStyles.standartButton);
	switchHLayout->addWidget(manualStandButton);
	connect(manualStandButton, &QPushButton::clicked, this, &MainWindow::on_manualStandButton_clicked);

	leftSwitchStandSpacer = new QSpacerItem(105, 0, QSizePolicy::Preferred);
	switchHLayout->addItem(leftSwitchStandSpacer);

	// Switch stand
	switchStandButton = new QPushButton(mainLayoutWidget);
	switchStandButton->setObjectName("switchStandButton");
	switchStandButton->setFixedSize(MINIMUM_SCREEN_WIDTH * COEFFICIENT_STAND_SWITCHER_WIDTH, MINIMUM_SCREEN_HEIGHT * COEFFICIENT_STAND_SWITCHER_HEIGHT);
	switchHLayout->addWidget(switchStandButton);
	connect(switchStandButton, &QPushButton::clicked, this, &MainWindow::on_switchStandButton_clicked);

	rightSwitchStandSpacer = new QSpacerItem(105, 0, QSizePolicy::Preferred);
	switchHLayout->addItem(rightSwitchStandSpacer);

	// Auto
	autoStandButton = new QPushButton(mainLayoutWidget);
	autoStandButton->setObjectName("autoStandButton");
	autoStandButton->setText("Auto");
	autoStandButton->setFixedSize(MINIMUM_SCREEN_WIDTH * COEFFICIENT_STAND_BUTTONS_WIDTH, MINIMUM_SCREEN_HEIGHT * COEFFICIENT_STAND_BUTTONS_HEIGHT);
	autoStandButton->setStyleSheet(lightStyles.standartButton);
	switchHLayout->addWidget(autoStandButton);
	connect(autoStandButton, &QPushButton::clicked, this, &MainWindow::on_autoStandButton_clicked);

	rightAutoStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	switchHLayout->addItem(rightAutoStandSpacer);

	topHLayout->addLayout(switchHLayout);

	switchThemeLanguageVLayout = new QVBoxLayout();
	switchThemeLanguageVLayout->setObjectName("switchThemeLanguageVLayout");

	// Theme
	switchThemeButton = new QPushButton(mainLayoutWidget);
	switchThemeButton->setObjectName("switchThemeButton");
	switchThemeButton->setFixedSize(MINIMUM_SCREEN_WIDTH * COEFFICIENT_THEME_BUTTON_SIZE, MINIMUM_SCREEN_HEIGHT * COEFFICIENT_THEME_BUTTON_SIZE);
	switchThemeLanguageVLayout->addWidget(switchThemeButton);

	// Language
	switchLanguageButton = new QPushButton(mainLayoutWidget);
	switchLanguageButton->setObjectName("switchLanguageButton");
	switchThemeLanguageVLayout->addWidget(switchLanguageButton);

	topHLayout->addLayout(switchThemeLanguageVLayout);
}

void MainWindow::initUiLeftVLayout()
{
	leftVLayout = new QVBoxLayout();
	leftVLayout->setObjectName("leftVLayout");

	topAdapterSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topAdapterSpacer);

	selectAdapterVLayout = new QVBoxLayout();
	selectAdapterVLayout->setObjectName("selectAdapterVLayout");

	findAdapterHLayout = new QHBoxLayout();
	findAdapterHLayout->setObjectName("findAdapterHLayout");

	// Adapter combo box
	selectAdapterComboBox = new QComboBox(mainLayoutWidget);
	selectAdapterComboBox->setObjectName("selectAdapterComboBox");
	findAdapterHLayout->addWidget(selectAdapterComboBox);

	// Adapter button
	checkAdaptersButton = new QPushButton(mainLayoutWidget);
	checkAdaptersButton->setObjectName("checkAdaptersButton");
	findAdapterHLayout->addWidget(checkAdaptersButton);

	selectAdapterVLayout->addLayout(findAdapterHLayout);

	selectAdapterLabel = new QLabel(mainLayoutWidget);
	selectAdapterLabel->setObjectName("selectAdapterLabel");
	selectAdapterLabel->setText("Please choose adapter");
	selectAdapterVLayout->addWidget(selectAdapterLabel);

	leftVLayout->addLayout(selectAdapterVLayout);

	topFrequencySpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topFrequencySpacer);

	// Frequency
	selectFrequencyVLayout = new QVBoxLayout();
	selectFrequencyVLayout->setObjectName("selectFrequencyVLayout");

	selectFrequencyComboBox = new QComboBox(mainLayoutWidget);
	selectFrequencyComboBox->setObjectName("selectAdapterComboBox");
	selectFrequencyVLayout->addWidget(selectFrequencyComboBox);

	selectFrequencyLabel = new QLabel(mainLayoutWidget);
	selectFrequencyLabel->setObjectName("selectFrequencyLabel");
	selectFrequencyLabel->setText("Please choose frequency");
	selectFrequencyVLayout->addWidget(selectFrequencyLabel);

	leftVLayout->addLayout(selectFrequencyVLayout);

	topConfiguratorSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topConfiguratorSpacer);

	// Configurator
	configuratorButton = new QPushButton(mainLayoutWidget);
	configuratorButton->setObjectName("configuratorButton");
	configuratorButton->setText("Configurator");
	leftVLayout->addWidget(configuratorButton);

	topSelectFileSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topSelectFileSpacer);

	// Select file
	selectFileVLayout = new QVBoxLayout();
	selectFileVLayout->setObjectName("selectFileVLayout");

	selectFileButton = new QPushButton(mainLayoutWidget);
	selectFileButton->setObjectName("selectFileButton");
	selectFileVLayout->addWidget(selectFileButton);

	selectFileLabel = new QLabel(mainLayoutWidget);
	selectFileLabel->setObjectName("selectFileLabel");
	selectFileLabel->setText("Please choose file");
	selectFileVLayout->addWidget(selectFileLabel);

	leftVLayout->addLayout(selectFileVLayout);

	bottomSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(bottomSpacer);
}

void MainWindow::initUiMainVLayout()
{
	mainVLayout = new QVBoxLayout();
	mainVLayout->setObjectName("autoStandMainVLayout");

	// out test auto stand
	outTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outTestAutoStandButton->setObjectName("outTestAutoStandButton");
	outTestAutoStandButton->setText("Out");
	mainVLayout->addWidget(outTestAutoStandButton);

	// in test auto stand
	inTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inTestAutoStandButton->setObjectName("inTestAutoStandButton");
	inTestAutoStandButton->setText("In");
	mainVLayout->addWidget(inTestAutoStandButton);

	manualTestManualStandHLayout = new QHBoxLayout();
	manualTestManualStandHLayout->setObjectName("manualTestManualStandHLayout");

	// out manual test manual stand
	outManualTestManualStandButton = new QPushButton(mainLayoutWidget);
	outManualTestManualStandButton->setObjectName("outManualTestManualStandButton");
	outManualTestManualStandButton->setText("Out");
	outManualTestManualStandButton->hide();
	manualTestManualStandHLayout->addWidget(outManualTestManualStandButton);

	// in manual test manual stand
	inManualTestManualStandButton = new QPushButton(mainLayoutWidget);
	inManualTestManualStandButton->setObjectName("inManualTestManualStandButton");
	inManualTestManualStandButton->setText("In");
	inManualTestManualStandButton->hide();
	manualTestManualStandHLayout->addWidget(inManualTestManualStandButton);

	mainVLayout->addLayout(manualTestManualStandHLayout);

	autoTestManualStandHLayout = new QHBoxLayout();
	autoTestManualStandHLayout->setObjectName("autoTestManualStandHLayout");

	// out auto test manual stand
	outAutoTestManualStandButton = new QPushButton(mainLayoutWidget);
	outAutoTestManualStandButton->setObjectName("outAutoTestManualStandButton");
	outAutoTestManualStandButton->setText("Out");
	outAutoTestManualStandButton->hide();
	autoTestManualStandHLayout->addWidget(outAutoTestManualStandButton);

	// in auto test manual stand
	inAutoTestManualStandButton = new QPushButton(mainLayoutWidget);
	inAutoTestManualStandButton->setObjectName("inAutoTestManualStandButton");
	inAutoTestManualStandButton->setText("In");
	inAutoTestManualStandButton->hide();
	autoTestManualStandHLayout->addWidget(inAutoTestManualStandButton);

	mainVLayout->addLayout(autoTestManualStandHLayout);

	// full test manual stand
	fullTestManualStandButton = new QPushButton(mainLayoutWidget);
	fullTestManualStandButton->setObjectName("fullTestManualStandButton");
	fullTestManualStandButton->setText("Out");
	fullTestManualStandButton->hide();
	mainVLayout->addWidget(fullTestManualStandButton);

	switchStandState = MANUAL_STAND;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	int newWidth = geometry().width();
	int newHeight = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, newWidth - (BORDER_INDENT * 2), newHeight - (BORDER_INDENT * 2));

	//logoLabel->setGeometry(BORDER_INDENT, BORDER_INDENT - TOOLBAR_SIZE, newWidth * COEFFICIENT_WIDTH_LOGO, newHeight * COEFFICIENT_HEIGHT_LOGO);
	//QPixmap logoPixmapp(*logoPixmap);
	//logoLabel->setPixmap(logoPixmapp.scaled(logoLabel->width() - 0.1, logoLabel->height() - 0.1, Qt::KeepAspectRatio));

	manualStandButton->setFixedSize(newWidth * COEFFICIENT_STAND_BUTTONS_WIDTH, newHeight * COEFFICIENT_STAND_BUTTONS_HEIGHT);
	autoStandButton->setFixedSize(newWidth * COEFFICIENT_STAND_BUTTONS_WIDTH, newHeight * COEFFICIENT_STAND_BUTTONS_HEIGHT);
	switchStandButton->setFixedSize(newWidth * COEFFICIENT_STAND_SWITCHER_WIDTH, newHeight * COEFFICIENT_STAND_SWITCHER_HEIGHT);
	switchThemeButton->setFixedSize((newHeight <= newWidth - 200 ? newHeight : newWidth - 200) * COEFFICIENT_THEME_BUTTON_SIZE, (newHeight <= newWidth - 200 ? newHeight : newWidth - 200) * COEFFICIENT_THEME_BUTTON_SIZE);
}

void MainWindow::switchStandButtons()
{
	if (switchStandState == AUTO_STAND)
	{
		outTestAutoStandButton->hide();
		inTestAutoStandButton->hide();

		outManualTestManualStandButton->show();
		inManualTestManualStandButton->show();
		outAutoTestManualStandButton->show();
		inAutoTestManualStandButton->show();
		fullTestManualStandButton->show();
	}
	else if (switchStandState == MANUAL_STAND)
	{
		outTestAutoStandButton->show();
		inTestAutoStandButton->show();

		outManualTestManualStandButton->hide();
		inManualTestManualStandButton->hide();
		outAutoTestManualStandButton->hide();
		inAutoTestManualStandButton->hide();
		fullTestManualStandButton->hide();
	}
}

void MainWindow::on_switchStandButton_clicked()
{
	if (switchStandState == MANUAL_STAND)
		switchStandState = AUTO_STAND;
	else if (switchStandState == AUTO_STAND)
		switchStandState = MANUAL_STAND;

	switchStandButtons();
}

void MainWindow::on_autoStandButton_clicked()
{
	if (switchStandState != AUTO_STAND)
	{
		switchStandState = AUTO_STAND;
		switchStandButtons();
	}
}

void MainWindow::on_manualStandButton_clicked()
{
	if (switchStandState != MANUAL_STAND)
	{
		switchStandState = MANUAL_STAND;
		switchStandButtons();
	}
}