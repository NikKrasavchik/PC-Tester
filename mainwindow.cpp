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

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("MainFormLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

	initStyles();
	initRecources();
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

	switchStandSlider->setStatus(AUTO_STAND);
	switchStandButtons();
	switchStandSlider->setStatus(MANUAL_STAND);
	switchStandButtons();

	on_checkAdaptersButton_clicked();

	initAll = true;
}

void MainWindow::initUiLogo()
{
	logoLabel = new QLabel(this);
	logoLabel->setObjectName("LogoLabel");
	logoLabel->setText("");
	logoLabel->setEnabled(true);
	logoLabel->setPixmap(*logoLightPixmap);
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
	manualStandButton->setText(QString::fromLocal8Bit("Ручной"));
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
	autoStandButton->setText(QString::fromLocal8Bit("Автомат."));
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
	switchThemeButton->setIcon(QIcon(*themeLightPixmap));
	switchThemeButton->setStyleSheet(lightStyles.ThemaLangButton);
	connect(switchThemeButton, &QPushButton::clicked, this, &MainWindow::on_switchThemeButton_clicked);
	switchThemeLanguageVLayout->addWidget(switchThemeButton);

	// Language
	switchLanguageButton = new QPushButton(mainLayoutWidget);
	switchLanguageButton->setObjectName("switchLanguageButton");
	switchLanguageButton->setFixedSize(MIN_THEME_LANG_BUTTON, MIN_THEME_LANG_BUTTON);
	switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
	switchLanguageButton->setStyleSheet(lightStyles.ThemaLangButton);
	switchThemeLanguageVLayout->addWidget(switchLanguageButton);
	connect(switchLanguageButton, &QPushButton::clicked, this, &MainWindow::on_switchLanguageButton_clicked);
	topHLayout->addLayout(switchThemeLanguageVLayout);
}

void MainWindow::initUiLeftVLayout()
{
	leftVLayout = new QVBoxLayout();
	leftVLayout->setObjectName("leftVLayout");

	topAdapterSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topAdapterSpacer);

	selectAdapterHLayout = new QHBoxLayout();
	selectAdapterHLayout->setObjectName("selectAdapterVLayout");

	adapterLeftSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	selectAdapterHLayout->addItem(adapterLeftSpacer);

	selectAdapterVLayout = new QVBoxLayout();
	selectAdapterVLayout->setObjectName("selectAdapterVLayout");

	findAdapterHLayout = new QHBoxLayout();
	findAdapterHLayout->setObjectName("findAdapterHLayout");

	// Adapter button
	checkAdaptersButton = new QPushButton(mainLayoutWidget);
	checkAdaptersButton->setObjectName("checkAdaptersButton");
	checkAdaptersButton->setFixedSize(MIN_ADAPTER_BUTTON_SIZE, MIN_ADAPTER_BUTTON_SIZE);
	checkAdaptersButton->setIcon(QIcon(*checkAdapterLightPixmap));
	connect(checkAdaptersButton, &QPushButton::clicked, this, &MainWindow::on_checkAdaptersButton_clicked);
	findAdapterHLayout->addWidget(checkAdaptersButton);

	findAdapterCenterSpacer = new QSpacerItem(2, 0, QSizePolicy::Fixed);
	findAdapterHLayout->addItem(findAdapterCenterSpacer);

	// Adapter combo box
	selectAdapterComboBox = new QComboBox(mainLayoutWidget);
	selectAdapterComboBox->setObjectName("selectAdapterComboBox");
	selectAdapterComboBox->setFixedSize(MIN_ADAPTER_COMBO_WIDTH, MIN_ADAPTER_COMBO_HEIGHT);
	selectAdapterComboBox->setStyleSheet(lightStyles.selectText);
	selectAdapterComboBox->addItem("...");
	connect(selectAdapterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectAdapterComboBox_changed(int)));
	findAdapterHLayout->addWidget(selectAdapterComboBox);

	selectAdapterVLayout->addLayout(findAdapterHLayout);

	selectAdapterMiddleSpacer = new QSpacerItem(2, 0, QSizePolicy::Fixed);
	selectAdapterVLayout->addItem(selectAdapterMiddleSpacer);

	selectAdapterLabel = new QLabel(mainLayoutWidget);
	selectAdapterLabel->setObjectName("selectAdapterLabel");
	selectAdapterLabel->setText(QString::fromLocal8Bit("Пожалуйста, выберите адаптер"));
	selectAdapterLabel->setStyleSheet(lightStyles.selectText);
	selectAdapterVLayout->addWidget(selectAdapterLabel);

	selectAdapterHLayout->addLayout(selectAdapterVLayout);

	adapterRightSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	selectAdapterHLayout->addItem(adapterRightSpacer);

	leftVLayout->addLayout(selectAdapterHLayout);

	topFrequencySpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topFrequencySpacer);

	// Frequency
	selectFrequencyVLayout = new QVBoxLayout();
	selectFrequencyVLayout->setObjectName("selectFrequencyVLayout");

	selectFrequencyComboBox = new QComboBox(mainLayoutWidget);
	selectFrequencyComboBox->setObjectName("selectAdapterComboBox");
	selectFrequencyComboBox->setFixedSize(MIN_FREQUENCY_COMBO_WIDTH, MIN_FREQUENCY_COMBO_HEIGHT);
	selectFrequencyComboBox->setStyleSheet(lightStyles.selectText);
	//connect(selectFrequencyComboBox, &QComboBox::currentIndexChanged, this, &MainWindow::on_selectFrequencyComboBox_changed);// так у меня почему то не получилось
	connect(selectFrequencyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectFrequencyComboBox_changed(int)));
	selectFrequencyVLayout->addWidget(selectFrequencyComboBox);
	// как то красиво это преобразовать надо
	selectFrequencyComboBox->addItem("...");
	selectFrequencyComboBox->addItem("10 000 pbs");
	selectFrequencyComboBox->addItem("50 000 pbs");
	selectFrequencyComboBox->addItem("100 000 pbs");
	selectFrequencyComboBox->addItem("125 000 pbs");
	selectFrequencyComboBox->addItem("250 000 pbs");
	selectFrequencyComboBox->addItem("500 000 pbs");
	selectFrequencyComboBox->addItem("1 000 000 pbs");
	//

	selectFrequencyLabel = new QLabel(mainLayoutWidget);
	selectFrequencyLabel->setObjectName("selectFrequencyLabel");
	selectFrequencyLabel->setText(QString::fromLocal8Bit("Пожалуйста, выберите частоту"));
	selectFrequencyLabel->setStyleSheet(lightStyles.selectText);
	selectFrequencyVLayout->addWidget(selectFrequencyLabel);

	leftVLayout->addLayout(selectFrequencyVLayout);

	topConfiguratorSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topConfiguratorSpacer);

	// Configurator
	configuratorButton = new QPushButton(mainLayoutWidget);
	configuratorButton->setObjectName("configuratorButton");
	configuratorButton->setText(QString::fromLocal8Bit("Конфигуратор"));
	configuratorButton->setStyleSheet(lightStyles.settingButton);
	configuratorButton->setFixedSize(MIN_CONFIGURATOR_BUTTON_WIDTH, MIN_CONFIGURATOR_BUTTON_HEIGHT);
	leftVLayout->addWidget(configuratorButton, Qt::AlignHCenter);

	topSelectFileSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topSelectFileSpacer);

	// Select file
	selectFileVLayout = new QVBoxLayout();
	selectFileVLayout->setObjectName("selectFileVLayout");

	selectFileButton = new QPushButton(mainLayoutWidget);
	selectFileButton->setObjectName("selectFileButton");
	selectFileButton->setText(QString::fromLocal8Bit("Выбрать файл"));
	selectFileButton->setStyleSheet(lightStyles.settingButton);
	selectFileButton->setFixedSize(MIN_FILE_SEL_BUTTON_WIDTH, MIN_FILE_SEL_BUTTON_HEIGHT);
	connect(selectFileButton, &QPushButton::clicked, this, &MainWindow::on_selectFileButton_clicked);
	selectFileVLayout->addWidget(selectFileButton, Qt::AlignHCenter);

	selectFileLabel = new QLabel(mainLayoutWidget);
	selectFileLabel->setObjectName("selectFileLabel");
	selectFileLabel->setText(QString::fromLocal8Bit("Пожалуйста, выберите файл"));
	selectFileLabel->setStyleSheet(lightStyles.selectText);
	selectFileLabel->setAlignment(Qt::AlignHCenter);
	selectFileLabel->setFixedWidth(MIN_FILE_SEL_BUTTON_WIDTH);
	selectFileVLayout->addWidget(selectFileLabel, Qt::AlignHCenter);

	leftVLayout->addLayout(selectFileVLayout);

	bottomSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(bottomSpacer);
}

void MainWindow::initUiMainVLayout()
{
	mainVLayout = new QVBoxLayout();
	mainVLayout->setObjectName("autoStandMainVLayout");

	manualStandMainHLayout = new QHBoxLayout();
	manualStandMainHLayout->setObjectName("manualStandMainHLayout");

	manualStandMainLeftSpacer = new QSpacerItem(1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	manualStandMainHLayout->addItem(manualStandMainLeftSpacer);

	manualStandMainVLayout = new QVBoxLayout();
	manualStandMainVLayout->setObjectName("manualStandMainVLayout");

	manualStaneMainUpSpacer = new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
	manualStandMainVLayout->addItem(manualStaneMainUpSpacer);

	// out test auto stand
	outTestManualStandButton = new QPushButton(mainLayoutWidget);
	outTestManualStandButton->setObjectName("outTestManualStandButton");
	outTestManualStandButton->setText(QString::fromLocal8Bit("Выходы"));
	outTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	manualStandMainVLayout->addWidget(outTestManualStandButton);

	manualStandMainMiddleSpacer = new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
	manualStandMainVLayout->addItem(manualStandMainMiddleSpacer);

	// in test auto stand
	inTestManualStandButton = new QPushButton(mainLayoutWidget);
	inTestManualStandButton->setObjectName("inTestManualStandButton");
	inTestManualStandButton->setText(QString::fromLocal8Bit("Входы"));
	inTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	manualStandMainVLayout->addWidget(inTestManualStandButton);

	manualStaneMainBottomSpacer = new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
	manualStandMainVLayout->addItem(manualStaneMainBottomSpacer);

	manualStandMainHLayout->addLayout(manualStandMainVLayout);

	manualStandMainRightSpacer = new QSpacerItem(1, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	manualStandMainHLayout->addItem(manualStandMainRightSpacer);

	mainVLayout->addLayout(manualStandMainHLayout);

	partitionTestAutoStandHLayout = new QHBoxLayout();
	partitionTestAutoStandHLayout->setObjectName("partitionTestAutoStandHLayout");

	manualTestAutoStandLeftSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	partitionTestAutoStandHLayout->addItem(manualTestAutoStandLeftSpacer);

	manualTestAutoStandWidget = new QWidget(mainLayoutWidget);
	manualTestAutoStandWidget->setObjectName("manualTestAutoStandWidget");
	manualTestAutoStandWidget->setStyleSheet("background-color: red;");

	manualTestAutoStandVLayout = new QVBoxLayout(manualTestAutoStandWidget);
	manualTestAutoStandVLayout->setObjectName("manualTestAutoStandVLayout");

	manualTestAutoStandLabel = new QLabel(manualTestAutoStandWidget);
	manualTestAutoStandLabel->setObjectName("manualTestAutoStandLabel");
	manualTestAutoStandLabel->setText("Manual");
	manualTestAutoStandLabel->setMaximumHeight(15);
	manualTestAutoStandLabel->setAlignment(Qt::AlignHCenter);
	manualTestAutoStandVLayout->addWidget(manualTestAutoStandLabel);

	// out manual test manual stand
	outManualTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outManualTestAutoStandButton->setObjectName("outManualTestAutoStandButton");
	outManualTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
	outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	outManualTestAutoStandButton->hide();
	manualTestAutoStandVLayout->addWidget(outManualTestAutoStandButton);

	manualTestAutoStandSpacer = new QSpacerItem(0, 0, QSizePolicy::Fixed);
	manualTestAutoStandVLayout->addItem(manualTestAutoStandSpacer);

	// in manual test manual stand
	inManualTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inManualTestAutoStandButton->setObjectName("inManualTestAutoStandButton");
	inManualTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
	inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inManualTestAutoStandButton->hide();
	manualTestAutoStandVLayout->addWidget(inManualTestAutoStandButton);

	manualTestAutoStandWidget->hide();
	partitionTestAutoStandHLayout->addWidget(manualTestAutoStandWidget);

	manualTestAutoStandMiddleSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	partitionTestAutoStandHLayout->addItem(manualTestAutoStandMiddleSpacer);

	autoTestAutoStandWidget = new QWidget(mainLayoutWidget);
	autoTestAutoStandWidget->setObjectName("autoTestAutoStandWidget");
	autoTestAutoStandWidget->setStyleSheet("background-color: blue;");

	autoTestAutoStandVLayout = new QVBoxLayout(autoTestAutoStandWidget);
	autoTestAutoStandVLayout->setObjectName("autoTestAutoStandVLayout");

	autoTestAutoStandLabel = new QLabel(manualTestAutoStandWidget);
	autoTestAutoStandLabel->setObjectName("autoTestAutoStandLabel");
	autoTestAutoStandLabel->setText("Auto");
	autoTestAutoStandLabel->setMaximumHeight(15);
	autoTestAutoStandLabel->setAlignment(Qt::AlignHCenter);
	autoTestAutoStandVLayout->addWidget(autoTestAutoStandLabel);

	// out auto test manual stand
	outAutoTestAutoStandButton = new QPushButton(mainLayoutWidget);
	outAutoTestAutoStandButton->setObjectName("outAutoTestAutoStandButton");
	outAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
	outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	outAutoTestAutoStandButton->hide();
	autoTestAutoStandVLayout->addWidget(outAutoTestAutoStandButton);

	autoTestAutoStandSpacer = new QSpacerItem(0, 0, QSizePolicy::Fixed);
	autoTestAutoStandVLayout->addItem(autoTestAutoStandSpacer);

	// in auto test manual stand
	inAutoTestAutoStandButton = new QPushButton(mainLayoutWidget);
	inAutoTestAutoStandButton->setObjectName("inAutoTestAutoStandButton");
	inAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
	inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inAutoTestAutoStandButton->hide();
	autoTestAutoStandVLayout->addWidget(inAutoTestAutoStandButton);

	autoTestAutoStandWidget->hide();
	partitionTestAutoStandHLayout->addWidget(autoTestAutoStandWidget);

	manualTestAutoStandRightSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	partitionTestAutoStandHLayout->addItem(manualTestAutoStandRightSpacer);

	mainVLayout->addLayout(partitionTestAutoStandHLayout);

	fullAutoStandMainHLayout = new QHBoxLayout();
	fullAutoStandMainHLayout->setObjectName("fullAutoStandMainHLayout");

	fullAutoStandMainLeftSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	fullAutoStandMainHLayout->addItem(fullAutoStandMainLeftSpacer);

	// full test manual stand
	fullTestAutoStandButton = new QPushButton(mainLayoutWidget);
	fullTestAutoStandButton->setObjectName("fullTestAutoStandButton");
	fullTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы/Входы"));
	fullTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	fullTestAutoStandButton->hide();
	fullAutoStandMainHLayout->addWidget(fullTestAutoStandButton, Qt::AlignHCenter);

	fullAutoStandMainRightSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	fullAutoStandMainHLayout->addItem(fullAutoStandMainRightSpacer);

	mainVLayout->addItem(fullAutoStandMainHLayout);

	manualTestAutoStandFooterSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding);
	mainVLayout->addItem(manualTestAutoStandFooterSpacer);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	int newWidth = geometry().width();
	int newHeight = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, newWidth - (BORDER_INDENT * 2), newHeight - (BORDER_INDENT * 2));

	// header
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

	// Язык
	switchLanguageButton->setFixedWidth(MIN_THEME_LANG_BUTTON + ((newWidth - MIN_SCREEN_WIDTH) * COEF_THEME_LANG_BUTTON));
	switchLanguageButton->setFixedHeight(MIN_THEME_LANG_BUTTON + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_THEME_LANG_BUTTON));

	// Тема
	switchThemeButton->setFixedWidth(MIN_THEME_LANG_BUTTON + ((newWidth - MIN_SCREEN_WIDTH) * COEF_THEME_LANG_BUTTON));
	switchThemeButton->setFixedHeight(MIN_THEME_LANG_BUTTON + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_THEME_LANG_BUTTON));

	// Настройка
	// adapter
	selectAdapterComboBox->setFixedWidth(MIN_ADAPTER_COMBO_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_ADAPTER_GROUP));
	selectAdapterComboBox->setFixedHeight(MIN_ADAPTER_COMBO_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_ADAPTER_GROUP));
	checkAdaptersButton->setFixedWidth(MIN_ADAPTER_BUTTON_SIZE + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_ADAPTER_GROUP));
	checkAdaptersButton->setFixedHeight(MIN_ADAPTER_BUTTON_SIZE + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_ADAPTER_GROUP));

	// frequency
	selectFrequencyComboBox->setFixedWidth(MIN_FREQUENCY_COMBO_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_FREQUENC_COMBO));
	selectFrequencyComboBox->setFixedHeight(MIN_FREQUENCY_COMBO_HEIGHT + ((newHeight - MIN_SCREEN_WIDTH) * COEF_FREQUENC_COMBO));

	// configurator
	configuratorButton->setFixedWidth(MIN_CONFIGURATOR_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_CONFIGURATOR_BUTTON));
	configuratorButton->setFixedHeight(MIN_CONFIGURATOR_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_CONFIGURATOR_BUTTON));

	// file
	selectFileButton->setFixedWidth(MIN_FILE_SEL_BUTTON_WIDTH + ((newWidth - MIN_SCREEN_WIDTH) * COEF_FILE_SEL_BUTTON));
	selectFileButton->setFixedHeight(MIN_FILE_SEL_BUTTON_HEIGHT + ((newHeight - MIN_SCREEN_HEIGHT) * COEF_FILE_SEL_BUTTON));
	selectFileLabel->setFixedWidth(MIN_FILE_SEL_BUTTON_WIDTH);

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
	switch (switchStandSlider->getStatus())
	{
	case AUTO_STAND:
		outTestManualStandButton->hide();
		inTestManualStandButton->hide();

		outManualTestAutoStandButton->show();
		inManualTestAutoStandButton->show();
		outAutoTestAutoStandButton->show();
		inAutoTestAutoStandButton->show();
		fullTestAutoStandButton->show();

		manualTestAutoStandWidget->show();
		autoTestAutoStandWidget->show();

		manualTestAutoStandSpacer->changeSize(0, 10, QSizePolicy::Expanding);
		autoTestAutoStandSpacer->changeSize(0, 10, QSizePolicy::Expanding);
		manualTestAutoStandFooterSpacer->changeSize(0, 10, QSizePolicy::Expanding);

		manualStandMainLeftSpacer->changeSize(0, 0, QSizePolicy::Fixed);
		manualStandMainRightSpacer->changeSize(0, 0, QSizePolicy::Fixed);
		manualStaneMainUpSpacer->changeSize(0, 0, QSizePolicy::Fixed);
		manualStandMainMiddleSpacer->changeSize(0, 0, QSizePolicy::Fixed);
		manualStaneMainBottomSpacer->changeSize(0, 0, QSizePolicy::Fixed);

		autoStandButton->setStyleSheet(lightStyles.alwaysActiveButton);
		manualStandButton->setStyleSheet(lightStyles.standartButton);
		break;

	case MANUAL_STAND:
		outTestManualStandButton->show();
		inTestManualStandButton->show();

		outManualTestAutoStandButton->hide();
		inManualTestAutoStandButton->hide();
		outAutoTestAutoStandButton->hide();
		inAutoTestAutoStandButton->hide();
		fullTestAutoStandButton->hide();

		manualTestAutoStandWidget->hide();
		autoTestAutoStandWidget->hide();

		manualTestAutoStandSpacer->changeSize(0, 0, QSizePolicy::Fixed);
		autoTestAutoStandSpacer->changeSize(0, 0, QSizePolicy::Fixed);

		manualStandMainLeftSpacer->changeSize(1, 0, QSizePolicy::Expanding);
		manualStandMainRightSpacer->changeSize(1, 0, QSizePolicy::Expanding);
		manualStaneMainUpSpacer->changeSize(0, 1, QSizePolicy::Expanding);
		manualStandMainMiddleSpacer->changeSize(0, 1, QSizePolicy::Expanding);
		manualStaneMainBottomSpacer->changeSize(0, 1, QSizePolicy::Expanding);

		autoStandButton->setStyleSheet(lightStyles.standartButton);
		manualStandButton->setStyleSheet(lightStyles.alwaysActiveButton);
		break;

	default:
		break;
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
}

void MainWindow::initRecources()
{
	logoLightPixmap = new QPixmap(":/Light/Recources/Logo_Light.png");
	logoDarkPixmap = new QPixmap("");
	themeLightPixmap = new QPixmap(":/Light/Recources/Moon_Dark.png");
	themeDarkPixmap = new QPixmap(":/Dark/Recources/Sun_Light.png");
	checkAdapterLightPixmap = new QPixmap(":/Light/Recources/Update_Dark.png");
	checkAdapterDarkPixmap = new QPixmap(":/Dark/Recources/Update_Light.png");
	languageLightPixmap = new QPixmap(":/Light/Recources/Language_Dark.png");
	languageDarkPixmap = new QPixmap(":/Dark/Recources/Language_Light.png");
}

void MainWindow::on_switchThemeButton_clicked()
{
	if (appTheme == LIGHT_THEME)
		appTheme = DARK_THEME;
	else if (appTheme == DARK_THEME)
		appTheme = LIGHT_THEME;

	switchTheme();
}

void MainWindow::switchTheme()
{
	switch (appTheme)
	{
	case LIGHT_THEME:
		switchThemeButton->setIcon(QIcon(*themeLightPixmap));
		switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
		checkAdaptersButton->setIcon(QIcon(*checkAdapterLightPixmap));
		break;

	case DARK_THEME:
		switchThemeButton->setIcon(QIcon(*themeDarkPixmap));
		switchLanguageButton->setIcon(QIcon(*languageDarkPixmap));
		checkAdaptersButton->setIcon(QIcon(*checkAdapterDarkPixmap));

		//
		ui.centralWidget->setStyleSheet(darkStyles.screenColor);

		break;

	default:
		break;
	}
}

void MainWindow::on_switchLanguageButton_clicked()
{
	if (appLanguage == RUSSIAN_LANG)
		appLanguage = ENGLISH_LANG;
	else if (appLanguage == ENGLISH_LANG)
		appLanguage = RUSSIAN_LANG;

	switchLanguage();
}

void MainWindow::switchLanguage()
{
	switch (appLanguage)
	{
	case RUSSIAN_LANG:
		manualStandButton->setText(QString::fromLocal8Bit("Ручной"));
		autoStandButton->setText(QString::fromLocal8Bit("Автомат."));
		configuratorButton->setText(QString::fromLocal8Bit("Конфигуратор"));
		selectFileButton->setText(QString::fromLocal8Bit("Выбрать файл"));
		inTestManualStandButton->setText(QString::fromLocal8Bit("Входы"));
		outTestManualStandButton->setText(QString::fromLocal8Bit("Выходы"));
		inManualTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
		outManualTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
		inAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
		outAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
		fullTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы/Входы"));

		if (!isAdapterSet)
			selectAdapterLabel->setText(QString::fromLocal8Bit("Пожалуйста, выберите адаптер"));
		if (!isFrequencySet)
			selectFrequencyLabel->setText(QString::fromLocal8Bit("Пожалуйста, выберите частоту"));
		manualTestAutoStandLabel->setText(QString::fromLocal8Bit("Ручная"));
		autoTestAutoStandLabel->setText(QString::fromLocal8Bit("Авто"));

		selectFileLabel->setText(QString::fromLocal8Bit("Пожалуйста, выберите файл"));	// Должно стоять условие, что после того, как файл будет выбрать не перезаписывать
		break;

	case ENGLISH_LANG:
		manualStandButton->setText(QString("Manual"));
		autoStandButton->setText(QString("Auto"));
		configuratorButton->setText(QString("Configurator"));
		selectFileButton->setText(QString("Selct file"));
		inTestManualStandButton->setText(QString("In"));
		outTestManualStandButton->setText(QString("Out"));
		inManualTestAutoStandButton->setText(QString("In"));
		outManualTestAutoStandButton->setText(QString("Out"));
		inAutoTestAutoStandButton->setText(QString("In"));
		outAutoTestAutoStandButton->setText(QString("Out"));
		fullTestAutoStandButton->setText(QString("Full"));

		if (!isAdapterSet)
			selectAdapterLabel->setText(QString("Please, select adapter"));
		if (!isFrequencySet)
			selectFrequencyLabel->setText(QString("Please, select frequency"));
		manualTestAutoStandLabel->setText(QString("Manual"));
		autoTestAutoStandLabel->setText(QString("Auto"));

		selectFileLabel->setText(QString("Selected file"));	// Должно стоять условие, что после того, как файл будет выбрать не перезаписывать
		break;

	default:
		break;
	}
	if (isFrequencySet)
		on_selectFrequencyComboBox_changed(1);
}

void MainWindow::on_selectFrequencyComboBox_changed(int index)
{
	if (!initAll)
		return;

	if (index == 0)
	{
		isFrequencySet = false; // Частота не выбранна
		switchLanguage(); // Ставим предупреждающий lable согласно языку
		deinitCanAdapter();
	}
	else if (index > 0)
	{
		isFrequencySet = true; // Частота выбранна
		if (appLanguage == RUSSIAN_LANG)
			selectFrequencyLabel->setText(QString::fromLocal8Bit("Частота: ") + selectFrequencyComboBox->currentText());
		else if (appLanguage == ENGLISH_LANG)
			selectFrequencyLabel->setText(QString("Frequency: ") + selectFrequencyComboBox->currentText());

		initCanAdapter(); // Инитим can или переинитим
	}

	switchStyleMainButton();
}

void MainWindow::on_selectAdapterComboBox_changed(int index)
{
	if (!initAll)
		return;

	if (index == 0)
	{
		isAdapterSet = false; // Частота не выбранна
		switchLanguage(); // Ставим предупреждающий lable согласно языку
		deinitCanAdapter();
	}
	else if (index > 0)
	{
		isAdapterSet = true; // Частота выбранна
		selectAdapterLabel->setText("");

		initCanAdapter(); // Инитим can или переинитим
	}

	switchStyleMainButton();
}

void MainWindow::on_checkAdaptersButton_clicked()
{
	selectAdapterComboBox->clear();

	selectAdapterComboBox->addItem("...");
	canHandle hndTmp;		// Переменная, в которой хранится идентификатор CAN-сессии
	int chanCount;             // Кол-во адаптеров kvaser
	char charNameAdapter[255];     // Название адаптера

	canInitializeLibrary();
	hndTmp = canOpenChannel(0, canOPEN_ACCEPT_VIRTUAL);
	canSetBusParams(hndTmp, canBITRATE_10K, 0, 0, 0, 0, 0);
	canBusOn(hndTmp);
	canGetNumberOfChannels(&chanCount);

	QString strNameAdapter;
	for (int i = 0; i < chanCount; i++) {
		canGetChannelData(i, canCHANNELDATA_DEVDESCR_ASCII, charNameAdapter, sizeof(charNameAdapter));
		if (charNameAdapter[0] == 0)
			continue;
		strNameAdapter = QString::fromStdString(charNameAdapter);
		QString str;
		bool isHaveTransfer = false;
		for (int j = 0; j < strNameAdapter.size(); j++)
		{
			if (j > 10 && strNameAdapter[j] == ' ' && !isHaveTransfer)
			{
				str += '\n';
				isHaveTransfer = true;
				continue;
			}
			str += strNameAdapter[j];
		}
		selectAdapterComboBox->addItem(str);
	}

	canBusOff(hndTmp); // Закрытие канала связи
	canClose(hndTmp); // Завершение работы с api
}

void MainWindow::initCanAdapter()
{
	int pop = selectFrequencyComboBox->currentIndex();
}

void MainWindow::deinitCanAdapter()
{
}

void MainWindow::switchStyleMainButton()
{
	if (isFrequencySet && isAdapterSet)
	{
		outTestManualStandButton->setStyleSheet(lightStyles.mainButton);
		inTestManualStandButton->setStyleSheet(lightStyles.mainButton);
		outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
		inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
		outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
		inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
		fullTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
	}
	else
	{
		outTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
		inTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
		outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
		inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
		outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
		inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
		fullTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	}
}