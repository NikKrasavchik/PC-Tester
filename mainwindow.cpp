#include "MainWindow.h"

#define GRID_COLOUMN_0  0
#define GRID_COLOUMN_1  1
#define GRID_ROW_0      0
#define GRID_ROW_1      1

#define MIN_STAND_BUTTON_WIDTH			144
#define MIN_STAND_BUTTON_HEIGHT			62
#define MIN_STAND_SWITCH_SLIDER_WIDTH	88
#define MIN_STAND_SWITCH_SLIDER_HEIGHT	46
#define MIN_THEME_LANG_BUTTON			30
#define MIN_ADAPTER_COMBO_HEIGHT		35
#define MIN_ADAPTER_BUTTON_SIZE			35
#define MIN_FREQUENCY_COMBO_HEIGHT		35
#define MIN_CONFIGURATOR_BUTTON_WIDTH	160
#define MIN_CONFIGURATOR_BUTTON_HEIGHT	66
#define MIN_FILE_SEL_BUTTON_WIDTH		166
#define MIN_FILE_SEL_BUTTON_HEIGHT		66
#define MIN_MAIN_IN_OUT_BUTTON_WIDTH	145
#define MIN_MAIN_IN_OUT_BUTTON_HEIGHT	46
#define MIN_MAIN_FUL_BUTTON_WIDTH		200
#define MIN_MAIN_FUL_BUTTON_HEIGHT		60

#define MAX_ADAPTER_COMBO_WIDTH			263
#define MAX_FREQUENCY_COMBO_WIDTH		300

#define COEF_STAND_BUTTON				0.1
#define COEF_STAND_SLIDER				0.06
#define COEF_THEME_LANG_BUTTON			0.03
#define COEF_ADAPTER_GROUP				0.05
#define COEF_FREQUENC_COMBO				0.05
#define COEF_CONFIGURATOR_BUTTON		0.05
#define COEF_FILE_SEL_BUTTON			0.05
#define COEF_MAIN_BUTTON				0.05

#define OVERCROWDED_SEL_FILE_LABEL		20
#define CFG_EXTENSION_LETTERS_COUNT		4

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	initUi();
}

MainWindow::~MainWindow()
{
	delete mainLayoutWidget;
	delete logoLightPixmap;
	delete logoDarkPixmap;
	delete themeLightPixmap;
	delete themeDarkPixmap;
	delete checkAdapterLightPixmap;
	delete checkAdapterDarkPixmap;
	delete languageLightPixmap;
	delete languageDarkPixmap;
	delete can;
}

void MainWindow::initUi()
{
	isAllInit = false;

	resize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
	setMinimumSize(QSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT));

	viewWindowState->appSize.width = MIN_SCREEN_WIDTH;
	viewWindowState->appSize.height = MIN_SCREEN_HEIGHT;

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("MainFormLayoutWidget");
	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, MIN_SCREEN_WIDTH - (BORDER_INDENT * 2), MIN_SCREEN_HEIGHT - (BORDER_INDENT * 2));

	initLightStyleSheets();
	initDarkStyleSheets();
	initRecources();
	initUiLogo();
	initUiTopHLayout();
	initUiLeftHLayout();
	initUiMainVLayout();

	autoStandWidget->show();

	mainGridLayout = new QGridLayout(mainLayoutWidget);
	mainGridLayout->setObjectName("MainFormLayout");
	mainGridLayout->setSpacing(0);
	mainGridLayout->setVerticalSpacing(0);

	mainGridLayout->addWidget(logoLabel, GRID_ROW_0, GRID_COLOUMN_0, Qt::AlignHCenter);
	mainGridLayout->addLayout(topHLayout, GRID_ROW_0, GRID_COLOUMN_1);
	mainGridLayout->addLayout(leftVLayout, GRID_ROW_1, GRID_COLOUMN_0);
	mainGridLayout->addLayout(mainVLayout, GRID_ROW_1, GRID_COLOUMN_1);

	selectedFileStandType = CFG_STAND_NOT_SET;
	selectedStand = AUTO_STAND;
	viewWindowState->appTheme = LIGHT_THEME;
	viewWindowState->appLanguage = RUSSIAN_LANG;

	initStyles();
	initTexts();
	initIcons();
	initConnections();
	fillComboBoxes();

	can = new Can();

	isAllInit = true;

	on_checkAdaptersButton_clicked();
	resetTheme();

#ifdef DEBUG
	selectFileButton->click();
#endif // DEBUG
}

void MainWindow::initUiLogo()
{
	logoLabel = new QLabel(this);
	logoLabel->setObjectName("LogoLabel");
	logoLabel->setText("");
	logoLabel->setEnabled(true);
}

void MainWindow::initUiTopHLayout()
{
	topHLayout = new QHBoxLayout();
	topHLayout->setObjectName("topHLayout");

	initUiSwitchStand();
	topHLayout->addItem(switchStandHLayout);

	initUiSwitchThemeLang();
	topHLayout->addItem(switchThemeLanguageVLayout);
}

void MainWindow::initUiSwitchStand()
{
	switchStandHLayout = new QHBoxLayout();
	switchStandHLayout->setObjectName("switchStandHLayout");

	leftManualStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	switchStandHLayout->addItem(leftManualStandSpacer);

	// Manual
	manualStandButton = new QPushButton(mainLayoutWidget);
	manualStandButton->setObjectName("manualStandButton");
	manualStandButton->setFixedSize(MIN_STAND_BUTTON_WIDTH, MIN_STAND_BUTTON_HEIGHT);
	switchStandHLayout->addWidget(manualStandButton);

	leftSwitchStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Preferred);
	switchStandHLayout->addItem(leftSwitchStandSpacer);

	// Switch stand
	switchStandSlider = new QSliderButton(mainLayoutWidget);
	switchStandSlider->setObjectName("switchStandButton");
	switchStandSlider->setFixedSize(MIN_STAND_SWITCH_SLIDER_WIDTH, MIN_STAND_SWITCH_SLIDER_HEIGHT);
	switchStandHLayout->addWidget(switchStandSlider);

	rightSwitchStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Preferred);
	switchStandHLayout->addItem(rightSwitchStandSpacer);

	// Auto
	autoStandButton = new QPushButton(mainLayoutWidget);
	autoStandButton->setObjectName("autoStandButton");
	autoStandButton->setFixedSize(MIN_STAND_BUTTON_WIDTH, MIN_STAND_BUTTON_HEIGHT);
	switchStandHLayout->addWidget(autoStandButton);

	rightAutoStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	switchStandHLayout->addItem(rightAutoStandSpacer);
}

void MainWindow::initUiSwitchThemeLang()
{
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
}

void MainWindow::initUiLeftHLayout()
{
	leftHLayout = new QHBoxLayout();
	leftHLayout->setObjectName("leftHLayout");

	leftVLayout = new QVBoxLayout();
	leftVLayout->setObjectName("leftVLayout");
	leftHLayout->addItem(leftVLayout);

	topConfiguratorSpacer = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);
	leftVLayout->addItem(topConfiguratorSpacer);

	initUiConfigurator();
	leftVLayout->addItem(configuratorHLayout);

	topSettingsSpacer = new QSpacerItem(0, 100, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topSettingsSpacer);

	leftSettingsVLayout = new QVBoxLayout();
	leftSettingsVLayout->setObjectName("leftSettingsVLayout");
	leftVLayout->addItem(leftSettingsVLayout);

	initUiAdapter();
	leftSettingsVLayout->addItem(selectAdapterVLayout);

	topFrequencySpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftSettingsVLayout->addItem(topFrequencySpacer);

	initUiFrequency();
	leftSettingsVLayout->addItem(selectFrequencyVLayout);

	topSelectFileSpacer = new QSpacerItem(0, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftSettingsVLayout->addItem(topSelectFileSpacer);

	initUiSelectFile();
	leftSettingsVLayout->addItem(selectFileHLayout);

	botSettingsSpacer = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Preferred);
	leftVLayout->addItem(botSettingsSpacer);
}

void MainWindow::initUiConfigurator()
{
	configuratorHLayout = new QHBoxLayout();
	configuratorHLayout->setObjectName("configuratorHLayout");

	configuratorLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Maximum);
	configuratorHLayout->addItem(configuratorLeftSpacer);

	// Configurator
	configuratorButton = new QPushButton(mainLayoutWidget);
	configuratorButton->setObjectName("configuratorButton");
	configuratorButton->setFixedSize(MIN_CONFIGURATOR_BUTTON_WIDTH, MIN_CONFIGURATOR_BUTTON_HEIGHT);
	configuratorHLayout->addWidget(configuratorButton);

	configuratorLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Maximum);
	configuratorHLayout->addItem(configuratorLeftSpacer);
}

void MainWindow::initUiAdapter()
{
	selectAdapterVLayout = new QVBoxLayout();
	selectAdapterVLayout->setObjectName("selectAdapterVLayout");

	findAdapterHLayout = new QHBoxLayout();
	findAdapterHLayout->setObjectName("findAdapterHLayout");
	selectAdapterVLayout->addItem(findAdapterHLayout);

	// Adapter button
	checkAdaptersButton = new QPushButton(mainLayoutWidget);
	checkAdaptersButton->setObjectName("checkAdaptersButton");
	checkAdaptersButton->setFixedSize(MIN_ADAPTER_BUTTON_SIZE, MIN_ADAPTER_BUTTON_SIZE);
	findAdapterHLayout->addWidget(checkAdaptersButton);

	findAdapterCenterSpacer = new QSpacerItem(2, 0, QSizePolicy::Fixed);
	findAdapterHLayout->addItem(findAdapterCenterSpacer);

	// Adapter combo box
	selectAdapterComboBox = new QComboBox(mainLayoutWidget);
	selectAdapterComboBox->setObjectName("selectAdapterComboBox");
	selectAdapterComboBox->setFixedHeight(MIN_ADAPTER_COMBO_HEIGHT);
	selectAdapterComboBox->setMaximumWidth(MAX_ADAPTER_COMBO_WIDTH);
	findAdapterHLayout->addWidget(selectAdapterComboBox);

	selectAdapterMiddleSpacer = new QSpacerItem(0, 2, QSizePolicy::Fixed);
	selectAdapterVLayout->addItem(selectAdapterMiddleSpacer);

	// Adapter label
	selectAdapterLabel = new QLabel(mainLayoutWidget);
	selectAdapterLabel->setObjectName("selectAdapterLabel");
	selectAdapterLabel->setMaximumWidth(MAX_ADAPTER_COMBO_WIDTH);
	selectAdapterVLayout->addWidget(selectAdapterLabel);
}

void MainWindow::initUiFrequency()
{
	selectFrequencyVLayout = new QVBoxLayout();
	selectFrequencyVLayout->setObjectName("selectFrequencyVLayout");

	selectFrequencyComboBox = new QComboBox(mainLayoutWidget);
	selectFrequencyComboBox->setObjectName("selectAdapterComboBox");
	selectFrequencyComboBox->setFixedHeight(MIN_FREQUENCY_COMBO_HEIGHT);
	selectFrequencyComboBox->setMaximumWidth(MAX_FREQUENCY_COMBO_WIDTH);
	selectFrequencyVLayout->addWidget(selectFrequencyComboBox);

	frequencyMiddleSpacer = new QSpacerItem(0, 2, QSizePolicy::Fixed);
	selectFrequencyVLayout->addItem(frequencyMiddleSpacer);

	selectFrequencyLabel = new QLabel(mainLayoutWidget);
	selectFrequencyLabel->setObjectName("selectFrequencyLabel");
	selectFrequencyVLayout->addWidget(selectFrequencyLabel);
}

void MainWindow::initUiSelectFile()
{
	selectFileHLayout = new QHBoxLayout();
	selectFileHLayout->setObjectName("selectFileVLayout");

	selectFileLeftSpacer = new QSpacerItem(50, 0, QSizePolicy::Maximum);
	selectFileHLayout->addItem(selectFileLeftSpacer);

	selectFileVLayout = new QVBoxLayout();
	selectFileVLayout->setObjectName("selectFileHLayout");
	selectFileHLayout->addItem(selectFileVLayout);

	selectFileButton = new QPushButton(mainLayoutWidget);
	selectFileButton->setObjectName("selectFileButton");
	selectFileButton->setFixedSize(MIN_FILE_SEL_BUTTON_WIDTH, MIN_FILE_SEL_BUTTON_HEIGHT);
	selectFileVLayout->addWidget(selectFileButton);

	selectFileMiddleSpacer = new QSpacerItem(0, 5, QSizePolicy::Fixed);
	selectFileVLayout->addItem(selectFileMiddleSpacer);

	selectFileLabel = new QLabel(mainLayoutWidget);
	selectFileLabel->setObjectName("selectFileLabel");
	selectFileLabel->setAlignment(Qt::AlignHCenter);
	selectFileLabel->setFixedWidth(MIN_FILE_SEL_BUTTON_WIDTH);
	selectFileVLayout->addWidget(selectFileLabel);

	selectFileLine = new QFrame(mainLayoutWidget);
	selectFileLine->setObjectName("selectFileLine");
	selectFileLine->setFrameShape(QFrame::HLine);
	selectFileVLayout->addWidget(selectFileLine);

	selectFileRightSpacer = new QSpacerItem(50, 0, QSizePolicy::Maximum);
	selectFileHLayout->addItem(selectFileRightSpacer);
}

void MainWindow::initUiMainVLayout()
{
	mainVLayout = new QVBoxLayout();
	mainVLayout->setObjectName("mainVLayout");

	initUiManualStand();
	mainVLayout->addWidget(manualStandWidget);
	manualStandWidget->hide();

	initUiAutoStand();
	mainVLayout->addWidget(autoStandWidget);
	autoStandWidget->hide();
}

void MainWindow::initUiAutoStand()
{
	autoStandWidget = new QWidget(mainLayoutWidget);
	autoStandWidget->setObjectName("autoStandWidget");

	autoStandMainHLayout = new QHBoxLayout(autoStandWidget);
	autoStandMainHLayout->setObjectName("autoStandMainHLayout");

	autoStandMainLeftSpacer = new QSpacerItem(50, 0, QSizePolicy::Expanding);
	autoStandMainHLayout->addItem(autoStandMainLeftSpacer);

	autoStandMainVLayout = new QVBoxLayout();
	autoStandMainVLayout->setObjectName("autoStandMainVLayout");
	autoStandMainHLayout->addItem(autoStandMainVLayout);

	autoStandMainUpSpacer = new QSpacerItem(0, 50, QSizePolicy::Expanding);
	autoStandMainVLayout->addItem(autoStandMainUpSpacer);

	partitionTestAutoStandHLayout = new QHBoxLayout();
	partitionTestAutoStandHLayout->setObjectName("partitionTestAutoStandHLayout");
	autoStandMainVLayout->addItem(partitionTestAutoStandHLayout);

	initUiAutoStandManualTest();
	partitionTestAutoStandHLayout->addWidget(manualTestAutoStandWidget);

	partitionTestAutoStandSpacer = new QSpacerItem(20, 0, QSizePolicy::Fixed);
	partitionTestAutoStandHLayout->addItem(partitionTestAutoStandSpacer);

	initUiAutoStandAutoTest();
	partitionTestAutoStandHLayout->addWidget(autoTestAutoStandWidget);

	autoStandMainMiddleSpacer = new QSpacerItem(0, 30, QSizePolicy::Expanding);
	autoStandMainVLayout->addItem(autoStandMainMiddleSpacer);

	fullTestAutoStandMiddleHLayout = new QHBoxLayout();
	fullTestAutoStandMiddleHLayout->setObjectName("fullTestAutoStandHLayout");
	autoStandMainVLayout->addItem(fullTestAutoStandMiddleHLayout);

	fullTestAutoStandOuterLeftSpacer = new QSpacerItem(30, 0, QSizePolicy::Expanding);
	fullTestAutoStandMiddleHLayout->addItem(fullTestAutoStandOuterLeftSpacer);

	initUiAutoStandFullTest();
	fullTestAutoStandMiddleHLayout->addWidget(fullTestAutoStandWidget);

	fullTestAutoStandOuterRightSpacer = new QSpacerItem(30, 0, QSizePolicy::Expanding);
	fullTestAutoStandMiddleHLayout->addItem(fullTestAutoStandOuterRightSpacer);

	autoStandMainBottomSpacer = new QSpacerItem(0, 50, QSizePolicy::Expanding);
	autoStandMainVLayout->addItem(autoStandMainBottomSpacer);

	autoStandMainRightSpacer = new QSpacerItem(50, 0, QSizePolicy::Expanding);
	autoStandMainHLayout->addItem(autoStandMainRightSpacer);
}

void MainWindow::initUiAutoStandManualTest()
{
	manualTestAutoStandWidget = new QWidget(autoStandWidget);
	manualTestAutoStandWidget->setObjectName("autoTestAutoStandWidget");

	manualTestAutoStandHLayout = new QHBoxLayout(manualTestAutoStandWidget);
	manualTestAutoStandHLayout->setObjectName("manualTestAutoStandHLayout");

	manualTestAutoStandLeftSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	manualTestAutoStandHLayout->addItem(manualTestAutoStandLeftSpacer);

	manualTestAutoStandVLayout = new QVBoxLayout();
	manualTestAutoStandVLayout->setObjectName("manualTestAutoStandHLayout");
	manualTestAutoStandHLayout->addItem(manualTestAutoStandVLayout);

	manualTestAutoStandUpSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	manualTestAutoStandVLayout->addItem(manualTestAutoStandUpSpacer);

	// manual test auto stand label
	manualTestAutoStandLabel = new QLabel(manualTestAutoStandWidget);
	manualTestAutoStandLabel->setObjectName("manualTestAutoStandLabel");
	manualTestAutoStandLabel->setAlignment(Qt::AlignHCenter);
	manualTestAutoStandVLayout->addWidget(manualTestAutoStandLabel);

	manualTestAutoStandMiddleUpSpacer = new QSpacerItem(0, 20, QSizePolicy::Fixed);
	manualTestAutoStandVLayout->addItem(manualTestAutoStandMiddleUpSpacer);

	// out manual test auto stand button
	outManualTestAutoStandButton = new QPushButton(manualTestAutoStandWidget);
	outManualTestAutoStandButton->setObjectName("outManualTestAutoStandButton");
	manualTestAutoStandVLayout->addWidget(outManualTestAutoStandButton);

	manualTestAutoStandMiddleBottomSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	manualTestAutoStandVLayout->addItem(manualTestAutoStandMiddleBottomSpacer);

	// in manual test auto stand button
	inManualTestAutoStandButton = new QPushButton(manualTestAutoStandWidget);
	inManualTestAutoStandButton->setObjectName("inManualTestAutoStandButton");
	manualTestAutoStandVLayout->addWidget(inManualTestAutoStandButton);

	manualTestAutoStandBottomSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	manualTestAutoStandVLayout->addItem(manualTestAutoStandBottomSpacer);

	manualTestAutoStandRightSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	manualTestAutoStandHLayout->addItem(manualTestAutoStandRightSpacer);
}

void MainWindow::initUiAutoStandAutoTest()
{
	autoTestAutoStandWidget = new QWidget(autoStandWidget);
	autoTestAutoStandWidget->setObjectName("autoTestAutoStandWidget");

	autoTestAutoStandHLayout = new QHBoxLayout(autoTestAutoStandWidget);
	autoTestAutoStandHLayout->setObjectName("autoTestAutoStandHLayout");

	autoTestAutoStandLeftSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	autoTestAutoStandHLayout->addItem(autoTestAutoStandLeftSpacer);

	autoTestAutoStandVLayout = new QVBoxLayout();
	autoTestAutoStandVLayout->setObjectName("autoTestAutoStandHLayout");
	autoTestAutoStandHLayout->addItem(autoTestAutoStandVLayout);

	autoTestAutoStandUpSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	autoTestAutoStandVLayout->addItem(autoTestAutoStandUpSpacer);

	// auto test auto stand label
	autoTestAutoStandLabel = new QLabel(autoTestAutoStandWidget);
	autoTestAutoStandLabel->setObjectName("autoTestAutoStandLabel");
	autoTestAutoStandLabel->setAlignment(Qt::AlignHCenter);
	autoTestAutoStandVLayout->addWidget(autoTestAutoStandLabel);

	autoTestAutoStandMiddleUpSpacer = new QSpacerItem(0, 20, QSizePolicy::Fixed);
	autoTestAutoStandVLayout->addItem(autoTestAutoStandMiddleUpSpacer);

	// out auto test auto stand button
	outAutoTestAutoStandButton = new QPushButton(autoTestAutoStandWidget);
	outAutoTestAutoStandButton->setObjectName("outAutoTestAutoStandButton");
	autoTestAutoStandVLayout->addWidget(outAutoTestAutoStandButton);

	autoTestAutoStandMiddleBottomSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	autoTestAutoStandVLayout->addItem(autoTestAutoStandMiddleBottomSpacer);

	// in auto test auto stand button
	inAutoTestAutoStandButton = new QPushButton(autoTestAutoStandWidget);
	inAutoTestAutoStandButton->setObjectName("inAutoTestAutoStandButton");
	autoTestAutoStandVLayout->addWidget(inAutoTestAutoStandButton);

	autoTestAutoStandBottomSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	autoTestAutoStandVLayout->addItem(autoTestAutoStandBottomSpacer);

	autoTestAutoStandRightSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	autoTestAutoStandHLayout->addItem(autoTestAutoStandRightSpacer);
}

void MainWindow::initUiAutoStandFullTest()
{
	fullTestAutoStandWidget = new QWidget(autoStandWidget);
	fullTestAutoStandWidget->setObjectName("fullTestAutoStandWidget");

	fullTestAutoStandHLayout = new QHBoxLayout(fullTestAutoStandWidget);
	fullTestAutoStandHLayout->setObjectName("fullTestAutoStandHLayout");

	fullTestAutoStandLeftSpacer = new QSpacerItem(20, 0, QSizePolicy::Preferred);
	fullTestAutoStandHLayout->addItem(fullTestAutoStandLeftSpacer);

	fullTestAutoStandVLayout = new QVBoxLayout();
	fullTestAutoStandVLayout->setObjectName("fullTestAutoStandVLayout");
	fullTestAutoStandHLayout->addItem(fullTestAutoStandVLayout);

	fullTestAutoStandUpSpacer = new QSpacerItem(0, 20, QSizePolicy::Preferred);
	fullTestAutoStandVLayout->addItem(fullTestAutoStandUpSpacer);

	// full test auto stand button
	fullTestAutoStandButton = new QPushButton(fullTestAutoStandWidget);
	fullTestAutoStandButton->setObjectName("fullTestAutoStandButton");
	fullTestAutoStandVLayout->addWidget(fullTestAutoStandButton);

	fullTestAutoStandBottomSpacer = new QSpacerItem(0, 20, QSizePolicy::Preferred);
	fullTestAutoStandVLayout->addItem(fullTestAutoStandBottomSpacer);

	fullTestAutoStandRightSpacer = new QSpacerItem(20, 0, QSizePolicy::Preferred);
	fullTestAutoStandHLayout->addItem(fullTestAutoStandRightSpacer);
}

void MainWindow::initUiManualStand()
{
	manualStandWidget = new QWidget(mainLayoutWidget);
	manualStandWidget->setObjectName("manualStandWidget");

	manualStandMainHLayout = new QHBoxLayout(manualStandWidget);
	manualStandMainHLayout->setObjectName("manualStandMainHLayout");

	manualStandMainLeftSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	manualStandMainHLayout->addItem(manualStandMainLeftSpacer);

	manualStandMainVLayout = new QVBoxLayout();
	manualStandMainVLayout->setObjectName("manualStandMainVLayout");
	manualStandMainHLayout->addItem(manualStandMainVLayout);

	manualStandMainUpSpacer = new QSpacerItem(0, 100, QSizePolicy::Expanding);
	manualStandMainVLayout->addItem(manualStandMainUpSpacer);

	backgroundManualStandWidget = new QWidget(manualStandWidget);
	backgroundManualStandWidget->setObjectName("backgroundManualStandWidget");

	backgroundManualStandHLayout = new QHBoxLayout(backgroundManualStandWidget);
	backgroundManualStandHLayout->setObjectName("backgroundManualStandHLayout");
	manualStandMainVLayout->addWidget(backgroundManualStandWidget);

	backgroundManualStandMainLeftSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	backgroundManualStandHLayout->addItem(backgroundManualStandMainLeftSpacer);

	testManualStandVLayout = new QVBoxLayout();
	testManualStandVLayout->setObjectName("testManualStandVLayout");
	backgroundManualStandHLayout->addItem(testManualStandVLayout);

	backgroundManualStandMainUpSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(backgroundManualStandMainUpSpacer);

	manualStandLabel = new QLabel(backgroundManualStandWidget);
	manualStandLabel->setObjectName("manualStandLabel");
	manualStandLabel->setMaximumHeight(30);
	manualStandLabel->setAlignment(Qt::AlignCenter);
	testManualStandVLayout->addWidget(manualStandLabel);

	testManualStandMiddleUpSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(testManualStandMiddleUpSpacer);

	//// out test manual stand
	outTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	outTestManualStandButton->setObjectName("outTestManualStandButton");
	outTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(outTestManualStandButton);

	testManualStandMiddleBottomSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(testManualStandMiddleBottomSpacer);

	//// in test manual stand
	inTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	inTestManualStandButton->setObjectName("inTestManualStandButton");
	inTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(inTestManualStandButton);

	testManualStandMiddleFooterSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(testManualStandMiddleFooterSpacer);

	//// full test manual stand
	fullTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	fullTestManualStandButton->setObjectName("fullTestManualStandButton");
	fullTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(fullTestManualStandButton);

	backgroundManualStandMainBottomSpacer = new QSpacerItem(0, 40, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(backgroundManualStandMainBottomSpacer);

	backgroundManualStandMainRightSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	backgroundManualStandHLayout->addItem(backgroundManualStandMainRightSpacer);

	manualStandMainBottomSpacer = new QSpacerItem(0, 75, QSizePolicy::Expanding);
	manualStandMainVLayout->addItem(manualStandMainBottomSpacer);

	manualStandMainRightSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	manualStandMainHLayout->addItem(manualStandMainRightSpacer);
}

void MainWindow::initStyles()
{
	// Header
	// selectStand
	manualStandButton->setStyleSheet(lightStyles.standButtons); // manualButton
	autoStandButton->setStyleSheet(lightStyles.standButtons); // autoButton
	switchStandSlider->setStyleSheet(lightStyles.roundSlider, lightStyles.bgSlider); // slider

	// themeLanguage
	switchThemeButton->setStyleSheet(lightStyles.mainButton);
	switchLanguageButton->setStyleSheet(lightStyles.mainButton);

	// Main
	// button
	outTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	fullTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
	fullTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);

	// substrate
	backgroundManualStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
	autoTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
	manualTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
	fullTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);

	//Setting
	//
	checkAdaptersButton->setStyleSheet(lightStyles.mainButton);
	configuratorButton->setStyleSheet(lightStyles.mainButton);
	selectFileButton->setStyleSheet(lightStyles.mainButton);
}

void MainWindow::initTexts()
{
	manualStandButton->setText(QString::fromLocal8Bit("Ручной"));
	autoStandButton->setText(QString::fromLocal8Bit("Автомат."));
	configuratorButton->setText(QString::fromLocal8Bit("Конфигуратор"));
	selectFileButton->setText(QString::fromLocal8Bit("Выбрать файл"));
	inTestManualStandButton->setText(QString::fromLocal8Bit("Входы"));
	outTestManualStandButton->setText(QString::fromLocal8Bit("Выходы"));
	fullTestManualStandButton->setText(QString::fromLocal8Bit("Полная"));
	inManualTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
	outManualTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
	inAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
	outAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
	fullTestAutoStandButton->setText(QString::fromLocal8Bit("Полная"));
	manualTestAutoStandLabel->setText(QString::fromLocal8Bit("Ручная"));
	autoTestAutoStandLabel->setText(QString::fromLocal8Bit("Авто"));
	selectAdapterLabel->setText(QString::fromLocal8Bit("Выберите адаптер"));
	selectFrequencyLabel->setText(QString::fromLocal8Bit("Выберите частоту"));
	manualStandLabel->setText(QString::fromLocal8Bit("Ручная"));
	selectFileLabel->setText(QString::fromLocal8Bit("Выберите файл"));
}

void MainWindow::fillComboBoxes()
{
	selectFrequencyComboBox->addItem("...");
	selectFrequencyComboBox->addItem("10 000 pbs");
	selectFrequencyComboBox->addItem("50 000 pbs");
	selectFrequencyComboBox->addItem("100 000 pbs");
	selectFrequencyComboBox->addItem("125 000 pbs");
	selectFrequencyComboBox->addItem("250 000 pbs");
	selectFrequencyComboBox->addItem("500 000 pbs");
	selectFrequencyComboBox->addItem("1 000 000 pbs");

	selectAdapterComboBox->addItem("...");
}

void MainWindow::initIcons()
{
	switchThemeButton->setIcon(QIcon(*themeLightPixmap));
	switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
	checkAdaptersButton->setIcon(QIcon(*checkAdapterLightPixmap));
	logoLabel->setPixmap(*logoLightPixmap);
}

void MainWindow::initRecources()
{
	logoLightPixmap = new QPixmap(":/Light/icons/Logo_Black.png");
	logoDarkPixmap = new QPixmap(":/Dark/icons/Logo_White.png");
	themeLightPixmap = new QPixmap(":/Light/icons/Moon_Black.png");
	themeDarkPixmap = new QPixmap(":/Dark/icons/Sun_White.png");
	checkAdapterLightPixmap = new QPixmap(":/Light/icons/Update_Black.png");
	checkAdapterDarkPixmap = new QPixmap(":/Dark/icons/Update_White.png");
	languageLightPixmap = new QPixmap(":/Light/icons/Language_Black.png");
	languageDarkPixmap = new QPixmap(":/Dark/icons/Language_White.png");
	appstylePath = ":/recources/style/appstyles.qss";
	lightStylePath = ":/styles/style/lightTheme.qss";
	darkStylePath = ":/styles/style/darkTheme.qss";
}

void MainWindow::initConnections()
{
	connect(this, &MainWindow::resizeStandSlider, switchStandSlider, &QSliderButton::resizeSlider);
	connect(switchStandSlider, &QSliderButton::on_sliderSwitchStand_click, this, &MainWindow::on_sliderSwitchStand_click);
	//connect(autoStandButton, &QPushButton::clicked, this, &MainWindow::on_autoStandButton_clicked);
	//connect(manualStandButton, &QPushButton::clicked, this, &MainWindow::on_manualStandButton_clicked);

	//connect(resetThemeButton, &QPushButton::clicked, this, &MainWindow::on_resetThemeButton_clicked);
	//connect(resetLanguageButton, &QPushButton::clicked, this, &MainWindow::on_resetLanguageButton_clicked);

	//connect(configuratorButton, &QPushButton::clicked, this, &MainWindow::on_configuratorButton_clicked);
	connect(selectAdapterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectAdapterComboBox_changed(int)));
	connect(selectFrequencyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectFrequencyComboBox_changed(int)));
	//connect(selectFileButton, &QPushButton::clicked, this, &MainWindow::on_selectFileButton_clicked);

	//connect(outTestManualStandButton, &QPushButton::clicked, this, &MainWindow::on_outTestManualStandButton_clicked);
	//connect(inTestManualStandButton, &QPushButton::clicked, this, &MainWindow::on_inTestManualStandButton_clicked);
	//connect(fullTestManualStandButton, &QPushButton::clicked, this, &MainWindow::on_fullTestManualStandButton_clicked);
	//connect(inManualTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::on_inManualTestAutoStandButton_clicked);
	//connect(outManualTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::on_outManualTestAutoStandButton_clicked);
	//connect(inAutoTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::on_inAutoTestAutoStandButton_clicked);
	//connect(outAutoTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::on_outAutoTestAutoStandButton_clicked);
	//connect(fullTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::on_fullTestAutoStandButton_clicked);

	QMetaObject::connectSlotsByName(this);
}

void MainWindow::switchStyleMainButtons()
{
	if (isFileSelected && can->getFrequencySelected() && can->getAdapterSelected())
	{
		if (viewWindowState->appTheme == LIGHT_THEME)
		{
			// button
			outTestManualStandButton->setStyleSheet(lightStyles.mainButton);
			inTestManualStandButton->setStyleSheet(lightStyles.mainButton);
			fullTestManualStandButton->setStyleSheet(lightStyles.mainButton);
			outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
			inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
			outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
			inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
			fullTestAutoStandButton->setStyleSheet(lightStyles.mainButton);
			// substrate
			backgroundManualStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
			autoTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
			manualTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
			fullTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
		}
		else
		{
			// button
			outTestManualStandButton->setStyleSheet(darkStyles.mainButton);
			inTestManualStandButton->setStyleSheet(darkStyles.mainButton);
			fullTestManualStandButton->setStyleSheet(darkStyles.mainButton);
			outManualTestAutoStandButton->setStyleSheet(darkStyles.mainButton);
			inManualTestAutoStandButton->setStyleSheet(darkStyles.mainButton);
			outAutoTestAutoStandButton->setStyleSheet(darkStyles.mainButton);
			inAutoTestAutoStandButton->setStyleSheet(darkStyles.mainButton);
			fullTestAutoStandButton->setStyleSheet(darkStyles.mainButton);
			// substrate
			backgroundManualStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
			autoTestAutoStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
			manualTestAutoStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
			fullTestAutoStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
		}
	}
	else
	{
		if (viewWindowState->appTheme == LIGHT_THEME)
		{
			// button
			outTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			inTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			fullTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			outManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			inManualTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			outAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			inAutoTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			fullTestAutoStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
			// substrate
			backgroundManualStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
			autoTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
			manualTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
			fullTestAutoStandWidget->setStyleSheet(lightStyles.mainSubstrateButtons);
		}
		else
		{
			// button
			outTestManualStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			inTestManualStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			fullTestManualStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			outManualTestAutoStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			inManualTestAutoStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			outAutoTestAutoStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			inAutoTestAutoStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			fullTestAutoStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
			// substrate
			backgroundManualStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
			autoTestAutoStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
			manualTestAutoStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
			fullTestAutoStandWidget->setStyleSheet(darkStyles.mainSubstrateButtons);
		}
	}
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	viewWindowState->appSize.width = geometry().width();
	viewWindowState->appSize.height = geometry().height();

	mainLayoutWidget->setGeometry(BORDER_INDENT, BORDER_INDENT, viewWindowState->appSize.width - (BORDER_INDENT * 2), viewWindowState->appSize.height - (BORDER_INDENT * 2));

	// Выбор стенда
	// manual
	manualStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	manualStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	// auto
	autoStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	autoStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	// slider
	resizeStandSlider(MIN_STAND_SWITCH_SLIDER_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_SLIDER), MIN_STAND_SWITCH_SLIDER_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_SLIDER));
	switchStandSlider->setFixedWidth(MIN_STAND_SWITCH_SLIDER_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_SLIDER));
	switchStandSlider->setFixedHeight(MIN_STAND_SWITCH_SLIDER_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_SLIDER));

	// Main
	// out stend manual
	outTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in stend manual
	inTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// full stend manual
	fullTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	fullTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// out manual stend auto
	outManualTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outManualTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in manual stend auto
	inManualTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inManualTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in auto stend suto
	outAutoTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outAutoTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in auto stend auto
	inAutoTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inAutoTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// full stend auto
	fullTestAutoStandButton->setFixedWidth(MIN_MAIN_FUL_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	fullTestAutoStandButton->setFixedHeight(MIN_MAIN_FUL_BUTTON_HEIGHT + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
}

void MainWindow::on_selectFileButton_clicked()
{
#ifdef DEBUG
	fileName = QString::fromLocal8Bit("И пиздец...");
	selectedFileFullName = "C:\Users\nikita.stachinskiy\Desktop\PC-tester\PC-tester\Config files/test_1.csv";
#else
	selectedFileFullName = QFileDialog::getOpenFileName(this, "Open File", "", "CSV (*.csv)");
	int localFileNameInd = selectedFileFullName.lastIndexOf("/");

	QString fullPrintedFileName = "";
	fileName = "";

	bool isTypeStart = false;
	for (int i = localFileNameInd + 1; i < selectedFileFullName.size(); i++)
	{
		if (selectedFileFullName[i] == '.')
			isTypeStart = true;

		if (!isTypeStart)
			fullPrintedFileName += selectedFileFullName[i];

		fileName += selectedFileFullName[i];
	}
#endif // DEBUG


	//selectFileLabel->setText((isFileNameOvercrowded ? printedFileName : fullPrintedFileName));
	if (fullPrintedFileName.size() <= OVERCROWDED_SEL_FILE_LABEL)
		printedfileName = fullPrintedFileName;
	else
	{
		printedfileName = "";
		for (int i = 0; i < OVERCROWDED_SEL_FILE_LABEL - 3; i++)
			printedfileName += fullPrintedFileName[i];
		printedfileName += "...";
	}
	selectFileLabel->setText(printedfileName);

	proccessSelectedFile(selectedFileFullName);

	isFileSelected = true;
	switchStyleMainButtons(); //
}

void MainWindow::on_switchThemeButton_clicked()
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

void MainWindow::resetTheme()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:
		switchThemeButton->setIcon(QIcon(*themeLightPixmap));
		switchLanguageButton->setIcon(QIcon(*languageLightPixmap));
		checkAdaptersButton->setIcon(QIcon(*checkAdapterLightPixmap));

		logoLabel->setPixmap(*logoLightPixmap);

		// Header
		// selectStand
		switchStandSlider->setStyleSheet(lightStyles.roundSlider, lightStyles.bgSlider); // slider
		if (switchStandSlider->getStatus() == AUTO_STAND) // button
		{
			autoStandButton->setStyleSheet(lightStyles.alwaysActiveStandButton);
			manualStandButton->setStyleSheet(lightStyles.standButtons);
		}
		else
		{
			autoStandButton->setStyleSheet(lightStyles.standButtons);
			manualStandButton->setStyleSheet(lightStyles.alwaysActiveStandButton);
		}

		// themeLanguage
		switchThemeButton->setStyleSheet(lightStyles.themeLangButton); // theme
		switchLanguageButton->setStyleSheet(lightStyles.themeLangButton); // language

		// Setting
		// адаптер
		checkAdaptersButton->setStyleSheet(lightStyles.themeLangButton); // button обновления
		selectAdapterComboBox->setStyleSheet(lightStyles.settingComboBox); // settingComboBox
		selectAdapterLabel->setStyleSheet(lightStyles.settingSelectText); // lable адаптера

		// частота
		selectFrequencyComboBox->setStyleSheet(lightStyles.settingComboBox); // settingComboBox
		selectFrequencyLabel->setStyleSheet(lightStyles.settingSelectText);  // lable частоты

		// конфигуратор
		configuratorButton->setStyleSheet(lightStyles.settingButton); // button конфигуратор

		// файл
		selectFileButton->setStyleSheet(lightStyles.settingButton); // button файл
		selectFileLabel->setStyleSheet(lightStyles.settingSelectText);  // lable файла
		selectFileLine->setStyleSheet(lightStyles.settingSelectText); // line

		// Main
		// button
		switchStyleMainButtons();

		// lable
		autoTestAutoStandLabel->setStyleSheet(lightStyles.mainText);
		manualTestAutoStandLabel->setStyleSheet(lightStyles.mainText);
		manualStandLabel->setStyleSheet(lightStyles.mainText);

		break;

	case DARK_THEME:
		switchThemeButton->setIcon(QIcon(*themeDarkPixmap));
		switchLanguageButton->setIcon(QIcon(*languageDarkPixmap));
		checkAdaptersButton->setIcon(QIcon(*checkAdapterDarkPixmap));

		logoLabel->setPixmap(*logoDarkPixmap);

		// Header
		// selectStand
		switchStandSlider->setStyleSheet(darkStyles.roundSlider, darkStyles.bgSlider); // slider
		if (switchStandSlider->getStatus() == AUTO_STAND) // button
		{
			autoStandButton->setStyleSheet(darkStyles.alwaysActiveStandButton);
			manualStandButton->setStyleSheet(darkStyles.standButtons);
		}
		else
		{
			autoStandButton->setStyleSheet(darkStyles.standButtons);
			manualStandButton->setStyleSheet(darkStyles.alwaysActiveStandButton);
		}

		// themeLanguage
		switchThemeButton->setStyleSheet(darkStyles.themeLangButton); // theme
		switchLanguageButton->setStyleSheet(darkStyles.themeLangButton); // language

		// Setting
		// адаптер
		checkAdaptersButton->setStyleSheet(darkStyles.themeLangButton); // button обновления
		selectAdapterComboBox->setStyleSheet(darkStyles.settingComboBox); // settingComboBox
		selectAdapterLabel->setStyleSheet(darkStyles.settingSelectText); // lable адаптера

		// частота
		selectFrequencyComboBox->setStyleSheet(darkStyles.settingComboBox); // settingComboBox
		selectFrequencyLabel->setStyleSheet(darkStyles.settingSelectText);  // lable частоты

		// конфигуратор
		configuratorButton->setStyleSheet(darkStyles.settingButton); // button конфигуратор

		// файл
		selectFileButton->setStyleSheet(darkStyles.settingButton);  // button файл
		selectFileLabel->setStyleSheet(darkStyles.settingSelectText);  // lable файла
		selectFileLine->setStyleSheet(darkStyles.settingSelectText); // line

		// Main
		// button
		switchStyleMainButtons();

		// lable
		autoTestAutoStandLabel->setStyleSheet(darkStyles.mainText);
		manualTestAutoStandLabel->setStyleSheet(darkStyles.mainText);
		manualStandLabel->setStyleSheet(darkStyles.mainText);

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

void MainWindow::switchStandButtons()
{
	switch (switchStandSlider->getStatus())
	{
	case AUTO_STAND:
		manualStandWidget->hide();
		autoStandWidget->show();

		switch (viewWindowState->appTheme)
		{
		case LIGHT_THEME:
			manualStandButton->setStyleSheet(lightStyles.standButtons);
			autoStandButton->setStyleSheet(lightStyles.alwaysActiveStandButton);
			break;

		case DARK_THEME:
			manualStandButton->setStyleSheet(darkStyles.standButtons);
			autoStandButton->setStyleSheet(darkStyles.alwaysActiveStandButton);
			break;
		}
		break;

	case MANUAL_STAND:
		autoStandWidget->hide();
		manualStandWidget->show();

		switch (viewWindowState->appTheme)
		{
		case LIGHT_THEME:
			autoStandButton->setStyleSheet(lightStyles.standButtons);
			manualStandButton->setStyleSheet(lightStyles.alwaysActiveStandButton);
			break;

		case DARK_THEME:
			autoStandButton->setStyleSheet(darkStyles.standButtons);
			manualStandButton->setStyleSheet(darkStyles.alwaysActiveStandButton);
			break;
		}
		break;
	}
}

void MainWindow::on_switchLanguageButton_clicked()
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

void MainWindow::resetLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		manualStandButton->setText(QString::fromLocal8Bit("Ручной"));
		autoStandButton->setText(QString::fromLocal8Bit("Автомат."));
		configuratorButton->setText(QString::fromLocal8Bit("Конфигуратор"));
		selectFileButton->setText(QString::fromLocal8Bit("Выбрать файл"));
		inTestManualStandButton->setText(QString::fromLocal8Bit("Входы"));
		outTestManualStandButton->setText(QString::fromLocal8Bit("Выходы"));
		fullTestManualStandButton->setText(QString::fromLocal8Bit("Полная"));
		inManualTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
		outManualTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
		inAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Входы"));
		outAutoTestAutoStandButton->setText(QString::fromLocal8Bit("Выходы"));
		fullTestAutoStandButton->setText(QString::fromLocal8Bit("Полная"));

		if (!can->getAdapterSelected())
			selectAdapterLabel->setText(QString::fromLocal8Bit("Выберите адаптер"));
		if (!can->getFrequencySelected())
			selectFrequencyLabel->setText(QString::fromLocal8Bit("Выберите частоту"));
		manualTestAutoStandLabel->setText(QString::fromLocal8Bit("Ручная"));
		autoTestAutoStandLabel->setText(QString::fromLocal8Bit("Авто"));

		selectFileLabel->setText(QString::fromLocal8Bit("Выберите файл"));	// Должно стоять условие, что после того, как файл будет выбрать не перезаписывать
		break;

	case ENGLISH_LANG:
		manualStandButton->setText(QString("Manual"));
		autoStandButton->setText(QString("Auto"));
		configuratorButton->setText(QString("Configurator"));
		selectFileButton->setText(QString("Selct file"));
		inTestManualStandButton->setText(QString("In"));
		outTestManualStandButton->setText(QString("Out"));
		fullTestManualStandButton->setText(QString("Full"));
		inManualTestAutoStandButton->setText(QString("In"));
		outManualTestAutoStandButton->setText(QString("Out"));
		inAutoTestAutoStandButton->setText(QString("In"));
		outAutoTestAutoStandButton->setText(QString("Out"));
		fullTestAutoStandButton->setText(QString("Full"));

		if (!can->getAdapterSelected())
			selectAdapterLabel->setText(QString("Select adapter"));
		if (!can->getFrequencySelected())
			selectFrequencyLabel->setText(QString("Select frequency"));
		manualTestAutoStandLabel->setText(QString("Manual"));
		autoTestAutoStandLabel->setText(QString("Auto"));

		selectFileLabel->setText(QString("Select file"));	// Должно стоять условие, что после того, как файл будет выбрать не перезаписывать
		break;
	}
	if (can->getFrequencySelected())
		on_selectFrequencyComboBox_changed(1);
}

void MainWindow::on_configuratorButton_clicked()
{
	ConfiguratorWindow* configuratorWindow = new ConfiguratorWindow(this);

	WindowFrame w(WindowType::CONFIGURATOR, nullptr, configuratorWindow);
	configuratorWindow->setParentFrame(&w);
	w.show();
	this->hide();
	configuratorWindow->exec();
	resetWindowView();
	this->show();
}

void MainWindow::on_selectFrequencyComboBox_changed(int index)
{
	if (!isAllInit)
		return;

	can->setFrequency(selectFrequencyComboBox->currentText());
	switchStyleMainButtons();

	if (index == 0)
		resetLanguage(); // Ставим предупреждающий lable согласно языку
	else if (index > 0)
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			selectFrequencyLabel->setText(QString::fromLocal8Bit("Частота: ") + selectFrequencyComboBox->currentText());
		else if (viewWindowState->appLanguage == ENGLISH_LANG)
			selectFrequencyLabel->setText(QString("Frequency: ") + selectFrequencyComboBox->currentText());
	}
}

void MainWindow::on_selectAdapterComboBox_changed(int index)
{
	if (!isAllInit)
		return;

	can->setAdapterNeme(selectAdapterComboBox->currentText()); // Адаптер не выбранна
	switchStyleMainButtons();

	if (index == 0)
		resetLanguage(); // Ставим предупреждающий lable согласно языку
	else if (index > 0)
		selectAdapterLabel->setText("");
}

void MainWindow::on_checkAdaptersButton_clicked()
{
	if (!isAllInit)
		return;

	selectAdapterComboBox->clear();
	selectAdapterComboBox->addItem("...");

	std::vector<QString> nameAdapters = can->getNameAdapters();

	for (int i = 0; i < nameAdapters.size(); i++)
		selectAdapterComboBox->addItem(nameAdapters[i]);
}

void MainWindow::on_outTestManualStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::OUT_TEST_MANUAL_STAND, preparedCables);
}

void MainWindow::on_inTestManualStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_IN)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::IN_TEST_MANUAL_STAND, preparedCables);
}

void MainWindow::on_fullTestManualStandButton_clicked()
{
	createTestWindow(WindowType::FULL_TEST_MANUAL_STAND, cables);
}

void MainWindow::on_inManualTestAutoStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_IN)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::IN_MANUAL_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::on_outManualTestAutoStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::OUT_MANUAL_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::on_inAutoTestAutoStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_IN)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::IN_AUTO_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::on_outAutoTestAutoStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::OUT_AUTO_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::on_fullTestAutoStandButton_clicked()
{
	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].direction == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::FULL_TEST_AUTO_STAND, cables);
}

void MainWindow::createTestWindow(WindowType testType, std::vector<Cable> preparedCables)
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	selectedFileStandType = CFG_STAND_MANUAL;
#endif // DEBUG

	if (isFileSelected && can->getFrequencySelected() && can->getAdapterSelected())
	{
		if (selectedFileStandType != CFG_STAND_NOT_SET)
		{
			TestWindow* testWindow = new TestWindow(testType, preparedCables, can, this);
			testWindow->setFileName(fileName);

			WindowFrame w(testType, nullptr, testWindow);
			testWindow->setParentFrame(&w);
			w.show();
			this->hide();
			testWindow->exec();
			resetWindowView();
			this->show();
		}
	}
	else if (!isFileSelected)
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите конфигурационный файл перед началом работы"));
		else
			QMessageBox::warning(this, QString("Warning"), QString("Select a configuration file before you begin"));
	}
	else if (!can->getFrequencySelected())
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите частоту Can-шины перед началом работы"));
		else
			QMessageBox::warning(this, QString("Warning"), QString("Select Can Bus frequency before starting work"));
	}
	else if (!can->getAdapterSelected())
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			QMessageBox::warning(this, QString::fromLocal8Bit("Внимание"), QString::fromLocal8Bit("Выберите Can-адаптер перед началом работы"));
		else
			QMessageBox::warning(this, QString("Warning"), QString("Select Can Adapter before starting"));
	}
}

void MainWindow::resetWindowView()
{
	resetLanguage();
	resetTheme();
	parentFrame->resetTheme();
	selectFileLabel->setText(printedfileName);
}

void MainWindow::setParentFrame(WindowFrame* parentFrame)
{
	this->parentFrame = parentFrame;

	connect(switchThemeButton, &QPushButton::clicked, parentFrame, &WindowFrame::on_switchThemeButton_clicked);
}