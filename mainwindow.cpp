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
	resize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
	setMinimumSize(QSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	ui.centralWidget->setStyleSheet(lightStyles.screenColor);

	logoPixmap = new QPixmap(LOGO_LIGHT_QRC);

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("MainFormLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

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
	manualStandButton->setFixedSize(MIN_STAND_BUTTON_WIDTH, MIN_STAND_BUTTON_HEIGHT);
	manualStandButton->setStyleSheet(lightStyles.standartButton);
	switchHLayout->addWidget(manualStandButton);
	connect(manualStandButton, &QPushButton::clicked, this, &MainWindow::on_manualStandButton_clicked);

	leftSwitchStandSpacer = new QSpacerItem(105, 0, QSizePolicy::Preferred);
	switchHLayout->addItem(leftSwitchStandSpacer);

	// Switch stand
	switchStandButton = new QPushButton(mainLayoutWidget);
	switchStandButton->setObjectName("switchStandButton");
	switchStandButton->setFixedSize(MIN_STAND_SWITCH_BUTTON_WIDTH, MIN_STAND_SWITCH_BUTTON_HEIGHT);
	switchHLayout->addWidget(switchStandButton);
	connect(switchStandButton, &QPushButton::clicked, this, &MainWindow::on_switchStandButton_clicked);

	rightSwitchStandSpacer = new QSpacerItem(105, 0, QSizePolicy::Preferred);
	switchHLayout->addItem(rightSwitchStandSpacer);

	// Auto
	autoStandButton = new QPushButton(mainLayoutWidget);
	autoStandButton->setObjectName("autoStandButton");
	autoStandButton->setText("Auto");
	autoStandButton->setFixedSize(MIN_STAND_BUTTON_WIDTH, MIN_STAND_BUTTON_HEIGHT);
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
	switchThemeButton->setFixedSize(MIN_THEME_LANG_BUTTON, MIN_THEME_LANG_BUTTON);
	switchThemeLanguageVLayout->addWidget(switchThemeButton);

	// Language
	switchLanguageButton = new QPushButton(mainLayoutWidget);
	switchLanguageButton->setObjectName("switchLanguageButton");
	switchLanguageButton->setFixedSize(MIN_THEME_LANG_BUTTON, MIN_THEME_LANG_BUTTON);
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

	// Adapter combo box
	selectAdapterComboBox = new QComboBox(mainLayoutWidget);
	selectAdapterComboBox->setObjectName("selectAdapterComboBox");
	findAdapterHLayout->addWidget(selectAdapterComboBox);

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
	configuratorButton->setText("configurator");
	configuratorButton->setFixedSize(MIN_CONFIGURATOR_BUTTON_WIDTH, MIN_CONFIGURATOR_BUTTON_HEIGHT);
	leftVLayout->addWidget(configuratorButton);

	topSelectFileSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topSelectFileSpacer);

	// Select file
	selectFileVLayout = new QVBoxLayout();
	selectFileVLayout->setObjectName("selectFileVLayout");

	selectFileButton = new QPushButton(mainLayoutWidget);
	selectFileButton->setObjectName("selectFileButton");
	selectFileButton->setFixedSize(MIN_FILE_SEL_BUTTON_WIDTH, MIN_FILE_SEL_BUTTON_HEIGHT);
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
	outTestManualStandButton = new QPushButton(mainLayoutWidget);
	outTestManualStandButton->setObjectName("outTestManualStandButton");
	outTestManualStandButton->setText("Out");
	mainVLayout->addWidget(outTestManualStandButton);

	// in test auto stand
	inTestManualStandButton = new QPushButton(mainLayoutWidget);
	inTestManualStandButton->setObjectName("inTestManualStandButton");
	inTestManualStandButton->setText("In");
	mainVLayout->addWidget(inTestManualStandButton);

	manualTestAutoStandHLayout = new QHBoxLayout();
	manualTestAutoStandHLayout->setObjectName("manualTestAutoStandHLayout");

	// out manual test manual stand
	outManualTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outManualTestAutoStandButton->setObjectName("outManualTestAutoStandButton");
	outManualTestAutoStandButton->setText("Out");
	outManualTestAutoStandButton->hide();
	manualTestAutoStandHLayout->addWidget(outManualTestAutoStandButton);

	// in manual test manual stand
	inManualTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inManualTestAutoStandButton->setObjectName("inManualTestAutoStandButton");
	inManualTestAutoStandButton->setText("In");
	inManualTestAutoStandButton->hide();
	manualTestAutoStandHLayout->addWidget(inManualTestAutoStandButton);

	mainVLayout->addLayout(manualTestAutoStandHLayout);

	autoTestAutoStandHLayout = new QHBoxLayout();
	autoTestAutoStandHLayout->setObjectName("autoTestAutoStandHLayout");

	// out auto test manual stand
	outAutoTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outAutoTestAutoStandButton->setObjectName("outAutoTestAutoStandButton");
	outAutoTestAutoStandButton->setText("Out");
	outAutoTestAutoStandButton->hide();
	autoTestAutoStandHLayout->addWidget(outAutoTestAutoStandButton);

	// in auto test manual stand
	inAutoTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inAutoTestAutoStandButton->setObjectName("inAutoTestAutoStandButton");
	inAutoTestAutoStandButton->setText("In");
	inAutoTestAutoStandButton->hide();
	autoTestAutoStandHLayout->addWidget(inAutoTestAutoStandButton);

	mainVLayout->addLayout(autoTestAutoStandHLayout);

	// full test manual stand
	fullTestAutoStandButton = new QPushButton(mainLayoutWidget);
	fullTestAutoStandButton->setObjectName("fullTestAutoStandButton");
	fullTestAutoStandButton->setText("Out");
	fullTestAutoStandButton->hide();
	mainVLayout->addWidget(fullTestAutoStandButton);

	switchStandState = MANUAL_STAND;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
	int newWidth = geometry().width();
	int newHeight = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, newWidth - (BORDER_INDENT * 2), newHeight - (BORDER_INDENT * 2));

	manualStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	manualStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	autoStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	autoStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	switchStandButton->setFixedWidth(MIN_STAND_SWITCH_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	switchStandButton->setFixedHeight(MIN_STAND_SWITCH_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	//if (newHeight + 200 <= newWidth)
	//{
	//	switchThemeButton->setFixedHeight(MIN_THEME_LANG_BUTTON + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_THEME_LANG_BUTTON));
	//	switchThemeButton->setFixedWidth(MIN_THEME_LANG_BUTTON + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_THEME_LANG_BUTTON));
	//	switchLanguageButton->setFixedHeight(MIN_THEME_LANG_BUTTON + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_THEME_LANG_BUTTON));
	//	switchLanguageButton->setFixedWidth(MIN_THEME_LANG_BUTTON + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_THEME_LANG_BUTTON));
	//}
	//else
	//{
	//	switchThemeButton->setFixedHeight(MIN_THEME_LANG_BUTTON + ((newWidth - MIN_SCREEN_WIDTH) * COEF_THEME_LANG_BUTTON));
	//	switchThemeButton->setFixedWidth(MIN_THEME_LANG_BUTTON + ((newWidth - MIN_SCREEN_WIDTH) * COEF_THEME_LANG_BUTTON));
	//	switchLanguageButton->setFixedHeight(MIN_THEME_LANG_BUTTON + ((newWidth - MIN_SCREEN_WIDTH) * COEF_THEME_LANG_BUTTON));
	//	switchLanguageButton->setFixedWidth(MIN_THEME_LANG_BUTTON + ((newWidth - MIN_SCREEN_WIDTH) * COEF_THEME_LANG_BUTTON));
	//}

	configuratorButton->setFixedWidth(MIN_CONFIGURATOR_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_CONFIGURATOR_BUTTON));
	configuratorButton->setFixedHeight(MIN_CONFIGURATOR_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_CONFIGURATOR_BUTTON));

	selectFileButton->setFixedWidth(MIN_FILE_SEL_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_FILE_SEL_BUTTON));
	selectFileButton->setFixedHeight(MIN_FILE_SEL_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_FILE_SEL_BUTTON));

}

void MainWindow::switchStandButtons()
{
	if (switchStandState == AUTO_STAND)
	{
		outTestManualStandButton->hide();
		inTestManualStandButton->hide();

		outManualTestAutoStandButton->show();
		inManualTestAutoStandButton->show();
		outAutoTestAutoStandButton->show();
		inAutoTestAutoStandButton->show();
		fullTestAutoStandButton->show();
	}
	else if (switchStandState == MANUAL_STAND)
	{
		outTestManualStandButton->show();
		inTestManualStandButton->show();

		outManualTestAutoStandButton->hide();
		inManualTestAutoStandButton->hide();
		outAutoTestAutoStandButton->hide();
		inAutoTestAutoStandButton->hide();
		fullTestAutoStandButton->hide();
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