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
	manualStandButton->setStyleSheet(lightStyles.alwaysActiveButton);
	switchHLayout->addWidget(manualStandButton);
	connect(manualStandButton, &QPushButton::clicked, this, &MainWindow::on_manualStandButton_clicked);

	leftSwitchStandSpacer = new QSpacerItem(105, 0, QSizePolicy::Preferred);
	switchHLayout->addItem(leftSwitchStandSpacer);

	// Switch stand
	switchStandSlider = new QSliderButton(mainLayoutWidget);
	switchStandSlider->setStyleSheet("background-color: #979797;");
	switchStandSlider->setObjectName("switchStandButton");
	switchStandSlider->setFixedSize(MIN_STAND_SWITCH_SLIDER_WIDTH, MIN_STAND_SWITCH_SLIDER_HEIGHT);
	switchHLayout->addWidget(switchStandSlider);
	connect(switchStandSlider, &QSliderButton::on_sliderSwitchStand_click, this, &MainWindow::on_sliderSwitchStand_click);
	connect(this, &MainWindow::resizeStandSlider, switchStandSlider, &QSliderButton::resizeSlider);

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
	configuratorButton->setText("Configurator");
	configuratorButton->setStyleSheet(lightStyles.settingButton);
	configuratorButton->setFixedSize(MIN_CONFIGURATOR_BUTTON_WIDTH, MIN_CONFIGURATOR_BUTTON_HEIGHT);
	leftVLayout->addWidget(configuratorButton);

	topSelectFileSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topSelectFileSpacer);

	// Select file
	selectFileHLayout = new QHBoxLayout();
	selectFileHLayout->setObjectName("selectFileHLayout");

	selectFileLeftSpacer = new QSpacerItem(20, 0, QSizePolicy::Preferred);
	selectFileHLayout->addItem(selectFileLeftSpacer);

	selectFileVLayout = new QVBoxLayout();
	selectFileVLayout->setObjectName("selectFileVLayout");

	selectFileButton = new QPushButton(mainLayoutWidget);
	selectFileButton->setObjectName("selectFileButton");
	selectFileButton->setText("Choice file");
	selectFileButton->setStyleSheet(lightStyles.settingButton);
	selectFileButton->setFixedSize(MIN_FILE_SEL_BUTTON_WIDTH, MIN_FILE_SEL_BUTTON_HEIGHT);
	connect(selectFileButton, &QPushButton::clicked, this, &MainWindow::on_selectFileButton_clicked);
	selectFileVLayout->addWidget(selectFileButton);

	selectFileLabel = new QLabel(mainLayoutWidget);
	selectFileLabel->setObjectName("selectFileLabel");
	selectFileLabel->setText("Please choose file");
	selectFileLabel->setStyleSheet(lightStyles.selectFileText);
	selectFileLabel->setAlignment(Qt::AlignHCenter);
	selectFileVLayout->addWidget(selectFileLabel);

	selectFileHLayout->addLayout(selectFileVLayout);

	selectFileRightSpacer = new QSpacerItem(20, 0, QSizePolicy::Preferred);
	selectFileHLayout->addItem(selectFileRightSpacer);

	leftVLayout->addLayout(selectFileHLayout);

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
	outTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	mainVLayout->addWidget(outTestManualStandButton);

	// in test auto stand
	inTestManualStandButton = new QPushButton(mainLayoutWidget);
	inTestManualStandButton->setObjectName("inTestManualStandButton");
	inTestManualStandButton->setText("In");
	inTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	mainVLayout->addWidget(inTestManualStandButton);

	manualTestAutoStandHLayout = new QHBoxLayout();
	manualTestAutoStandHLayout->setObjectName("manualTestAutoStandHLayout");

	// out manual test manual stand
	outManualTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outManualTestAutoStandButton->setObjectName("outManualTestAutoStandButton");
	outManualTestAutoStandButton->setText("Out");
	outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	outManualTestAutoStandButton->hide();
	manualTestAutoStandHLayout->addWidget(outManualTestAutoStandButton);

	// in manual test manual stand
	inManualTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inManualTestAutoStandButton->setObjectName("inManualTestAutoStandButton");
	inManualTestAutoStandButton->setText("In");
	inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inManualTestAutoStandButton->hide();
	manualTestAutoStandHLayout->addWidget(inManualTestAutoStandButton);

	mainVLayout->addLayout(manualTestAutoStandHLayout);

	autoTestAutoStandHLayout = new QHBoxLayout();
	autoTestAutoStandHLayout->setObjectName("autoTestAutoStandHLayout");

	// out auto test manual stand
	outAutoTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outAutoTestAutoStandButton->setObjectName("outAutoTestAutoStandButton");
	outAutoTestAutoStandButton->setText("Out");
	outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	outAutoTestAutoStandButton->hide();
	autoTestAutoStandHLayout->addWidget(outAutoTestAutoStandButton);

	// in auto test manual stand
	inAutoTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inAutoTestAutoStandButton->setObjectName("inAutoTestAutoStandButton");
	inAutoTestAutoStandButton->setText("In");
	inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inAutoTestAutoStandButton->hide();
	autoTestAutoStandHLayout->addWidget(inAutoTestAutoStandButton);

	mainVLayout->addLayout(autoTestAutoStandHLayout);

	// full test manual stand
	fullTestAutoStandButton = new QPushButton(mainLayoutWidget);
	fullTestAutoStandButton->setObjectName("fullTestAutoStandButton");
	fullTestAutoStandButton->setText("Out");
	fullTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	fullTestAutoStandButton->hide();
	mainVLayout->addWidget(fullTestAutoStandButton);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	int newWidth = geometry().width();
	int newHeight = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, newWidth - (BORDER_INDENT * 2), newHeight - (BORDER_INDENT * 2));

	// Выбор стенда
	// manual
	manualStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	manualStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	// auto
	autoStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	autoStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	// slider
	resizeStandSlider(MIN_STAND_SWITCH_SLIDER_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_SLIDER), MIN_STAND_SWITCH_SLIDER_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_SLIDER));
	switchStandSlider->setFixedWidth(MIN_STAND_SWITCH_SLIDER_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_STAND_SLIDER));
	switchStandSlider->setFixedHeight(MIN_STAND_SWITCH_SLIDER_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_STAND_SLIDER));

	// Настройка
	// configurator
	configuratorButton->setFixedWidth(MIN_CONFIGURATOR_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_CONFIGURATOR_BUTTON));
	configuratorButton->setFixedHeight(MIN_CONFIGURATOR_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_CONFIGURATOR_BUTTON));

	// file
	selectFileButton->setFixedWidth(MIN_FILE_SEL_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_FILE_SEL_BUTTON));
	selectFileButton->setFixedHeight(MIN_FILE_SEL_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_FILE_SEL_BUTTON));

	// Main
	// out stend manual
	outTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in stend manual
	inTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// out manual stend auto
	outManualTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outManualTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in manual stend auto
	inManualTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inManualTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in auto stend suto
	outAutoTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outAutoTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in auto stend auto
	inAutoTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inAutoTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// full stend auto
	fullTestAutoStandButton->setFixedWidth(MIN_MAIN_FUL_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	fullTestAutoStandButton->setFixedHeight(MIN_MAIN_FUL_BUTTON_HEIGHT + ((newWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
}

void MainWindow::switchStandButtons()
{
	if (switchStandSlider->getStatus() == AUTO_STAND)
	{
		outTestManualStandButton->hide();
		inTestManualStandButton->hide();

		outManualTestAutoStandButton->show();
		inManualTestAutoStandButton->show();
		outAutoTestAutoStandButton->show();
		inAutoTestAutoStandButton->show();
		fullTestAutoStandButton->show();

		autoStandButton->setStyleSheet(lightStyles.alwaysActiveButton);
		manualStandButton->setStyleSheet(lightStyles.standartButton);
	}
	else if (switchStandSlider->getStatus() == MANUAL_STAND)
	{
		outTestManualStandButton->show();
		inTestManualStandButton->show();

		outManualTestAutoStandButton->hide();
		inManualTestAutoStandButton->hide();
		outAutoTestAutoStandButton->hide();
		inAutoTestAutoStandButton->hide();
		fullTestAutoStandButton->hide();

		autoStandButton->setStyleSheet(lightStyles.standartButton);
		manualStandButton->setStyleSheet(lightStyles.alwaysActiveButton);
	}
}

void MainWindow::on_sliderSwitchStand_click()
{
	switchStandButtons();
}

void MainWindow::on_autoStandButton_clicked()
{
	if (switchStandSlider->getStatus() != AUTO_STAND)
	{
		switchStandSlider->setStatus(AUTO_STAND);
		switchStandButtons();
	}
}

void MainWindow::on_manualStandButton_clicked()
{
	if (switchStandSlider->getStatus() != MANUAL_STAND)
	{
		switchStandSlider->setStatus(MANUAL_STAND);
		switchStandButtons();
	}
}

void MainWindow::on_selectFileButton_clicked()
{
	// пока это написал здесь для реализации
	// потом нужно это будет делать после того как пользователь:
	// 1. выбрал адаптер
	// 2. выбрал частоту
	// 3. выбрал файл

	outTestManualStandButton->setStyleSheet(lightStyles.mainButton);
	inTestManualStandButton->setStyleSheet(lightStyles.mainButton);
	outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
	inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
	outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
	inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
	fullTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
}