#include "MainWindow.h"
#include <QDesktopServices>
#define GRID_COLUMN_0	0
#define GRID_COLUMN_1	1
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
#define BLOCK_VERSION_COMBO_WIDTH		100
#define BLOCK_VERSION_COMBO_HEIGHT		20

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

	this->selectedTypeStand = TypeStand::EMPTY;
	viewWindowState->selectedBlock = TestBlockName::EMPTY;
	timerCheckAdapter = new QTimer();


	initBlockVersions();

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
	//setMaximumSize(QSize(2000, 2000));

	viewWindowState->appSize.width = MIN_SCREEN_WIDTH;
	viewWindowState->appSize.height = MIN_SCREEN_HEIGHT;

	mainLayoutWidget = new QWidget(this);
	mainLayoutWidget->setObjectName("MainLayoutWidget");
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

	mainGridLayout->addWidget(logoWidget, GRID_ROW_0, GRID_COLUMN_0, Qt::AlignHCenter);
	mainGridLayout->addWidget(topWidget, GRID_ROW_0, GRID_COLUMN_1);
	mainGridLayout->addWidget(leftHWidget, GRID_ROW_1, GRID_COLUMN_0);
	mainGridLayout->addLayout(mainVLayout, GRID_ROW_1, GRID_COLUMN_1);

	selectedTypeStand = TypeStand::AUTO;
	viewWindowState->appTheme = LIGHT_THEME;
	viewWindowState->appLanguage = RUSSIAN_LANG;

	initIcons();
	initConnections();
	fillComboBoxes();

	can = new Can();

	checkAdaptersButton->click();
	initConfig();

	resetTheme();
	resetLanguage();

	isAllInit = true;
}

void MainWindow::initUiLogo()
{
	logoWidget = new QWidget(mainLayoutWidget);
	logoWidget->setObjectName("logoWidget");

	logoHLayout = new QHBoxLayout(logoWidget);
	logoHLayout->setObjectName("logoHLayout");

	logoPixmapWidget = new QWidget(logoWidget);
	logoPixmapWidget->setObjectName("logoWidget");
	logoPixmapWidget->setFixedSize(FIXED_LOGO_WIDTH, FIXED_LOGO_HEIGHT);
	logoHLayout->addWidget(logoPixmapWidget);

	logoLabel = new QLabel(logoPixmapWidget);
	logoLabel->setObjectName("LogoLabel");
	logoLabel->setText("");
	logoLabel->setEnabled(true);
	logoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
}

void MainWindow::initUiTopHLayout()
{
	topWidget = new QWidget(mainLayoutWidget);
	topWidget->setObjectName("topWidget");

	topHLayout = new QHBoxLayout(topWidget);
	topHLayout->setObjectName("topHLayout");

	initUiSwitchType();
	topHLayout->addWidget(switchTypeWidget);

	initUiSwitchThemeLang();
	topHLayout->addWidget(switchThemeLanguageWidget);
}

void MainWindow::initUiSwitchType()
{
	switchTypeWidget = new QWidget(topWidget);
	switchTypeWidget->setObjectName("switchTypeWidget");

	switchTypeHLayout = new QHBoxLayout(switchTypeWidget);
	switchTypeHLayout->setObjectName("switchStandHLayout");

	leftManualStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	switchTypeHLayout->addItem(leftManualStandSpacer);

	// Manual
	manualStandButton = new QPushButton(switchTypeWidget);
	manualStandButton->setObjectName("manualStandButton");
	manualStandButton->setFixedSize(MIN_STAND_BUTTON_WIDTH, MIN_STAND_BUTTON_HEIGHT);
	switchTypeHLayout->addWidget(manualStandButton);

	leftSwitchStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Preferred);
	switchTypeHLayout->addItem(leftSwitchStandSpacer);

	// Switch stand
	switchStandSlider = new QSliderButton(true, switchTypeWidget);
	switchStandSlider->setObjectName("switchStandButton");
	switchStandSlider->setFixedSize(MIN_STAND_SWITCH_SLIDER_WIDTH, MIN_STAND_SWITCH_SLIDER_HEIGHT);
	switchTypeHLayout->addWidget(switchStandSlider);

	rightSwitchStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Preferred);
	switchTypeHLayout->addItem(rightSwitchStandSpacer);

	// Auto
	autoStandButton = new QPushButton(switchTypeWidget);
	autoStandButton->setObjectName("autoStandButton");
	autoStandButton->setFixedSize(MIN_STAND_BUTTON_WIDTH, MIN_STAND_BUTTON_HEIGHT);
	switchTypeHLayout->addWidget(autoStandButton);

	rightAutoStandSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	switchTypeHLayout->addItem(rightAutoStandSpacer);
}

void MainWindow::initUiSwitchThemeLang()
{
	switchThemeLanguageWidget = new QWidget(topWidget);
	switchThemeLanguageWidget->setObjectName("switchThemeLanguageWidget");

	switchThemeLanguageVLayout = new QVBoxLayout(switchThemeLanguageWidget);
	switchThemeLanguageVLayout->setObjectName("switchThemeLanguageVLayout");

	// Theme
	switchThemeButton = new QPushButton(switchThemeLanguageWidget);
	switchThemeButton->setObjectName("switchThemeButton");
	switchThemeButton->setFixedSize(MIN_THEME_LANG_BUTTON, MIN_THEME_LANG_BUTTON);
	switchThemeLanguageVLayout->addWidget(switchThemeButton);

	// Language
	switchLanguageButton = new QPushButton(switchThemeLanguageWidget);
	switchLanguageButton->setObjectName("switchLanguageButton");
	switchLanguageButton->setFixedSize(MIN_THEME_LANG_BUTTON, MIN_THEME_LANG_BUTTON);
	switchThemeLanguageVLayout->addWidget(switchLanguageButton);	
}

void MainWindow::initUiLeftHLayout()
{
	leftHWidget = new QWidget(mainLayoutWidget);
	leftHWidget->setObjectName("leftWidget");
	leftHWidget->setFixedWidth(FIXED_LOGO_WIDTH);

	leftHLayout = new QHBoxLayout(leftHWidget);
	leftHLayout->setObjectName("leftHLayout");

	leftHLayoutLeftSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
	leftHLayout->addItem(leftHLayoutLeftSpacer);

	leftVWidget = new QWidget(leftHWidget);
	leftVWidget->setObjectName("leftVWidget");
	leftHLayout->addWidget(leftVWidget);

	leftVLayout = new QVBoxLayout(leftVWidget);
	leftVLayout->setObjectName("leftVLayout");

	initUiSwitchStand();
	leftVLayout->addWidget(leftSwitchBlockWidget);

	topSettingsSpacer = new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(topSettingsSpacer);

	leftSettingsWidget = new QWidget(leftVWidget);
	leftSettingsWidget->setObjectName("leftSettingsWidget");
	leftVLayout->addWidget(leftSettingsWidget);

	leftSettingsVLayout = new QVBoxLayout(leftSettingsWidget);
	leftSettingsVLayout->setObjectName("leftSettingsVLayout");

	initUiAdapter();
	leftSettingsVLayout->addWidget(selectAdapterWidget);

	topFrequencySpacer = new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftSettingsVLayout->addItem(topFrequencySpacer);

	initUiFrequency();
	leftSettingsVLayout->addWidget(selectFrequencyWidget);

	botSettingsSpacer = new QSpacerItem(0, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
	leftVLayout->addItem(botSettingsSpacer);

	leftHLayoutRightSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding);
	leftHLayout->addItem(leftHLayoutRightSpacer);
}

void MainWindow::initUiSwitchStand()
{
	leftSwitchBlockWidget = new QWidget(leftVWidget);
	leftSwitchBlockWidget->setObjectName("leftSwitchBlockWidget");

	leftSwitchBlockVLayout = new QVBoxLayout(leftSwitchBlockWidget);
	leftSwitchBlockVLayout->setObjectName("leftSwitchBlockVLayout");

	leftBlockBCMButton = new QPushButton(leftSwitchBlockWidget);
	leftBlockBCMButton->setObjectName("blockBCMButton");
	leftBlockBCMButton->setText("BCM");
	leftSwitchBlockVLayout->addWidget(leftBlockBCMButton);

	connect(leftBlockBCMButton, &QPushButton::clicked, this, &MainWindow::slot_leftBlockBCMButton_clicked);

	leftStandSwitchSpacer = new QSpacerItem(0, 30, QSizePolicy::Fixed);
	leftSwitchBlockVLayout->addItem(leftStandSwitchSpacer);

	leftBlockDMButton = new QPushButton(leftSwitchBlockWidget);
	leftBlockDMButton->setObjectName("blockDMButton");
	leftBlockDMButton->setText("DTM");
	leftSwitchBlockVLayout->addWidget(leftBlockDMButton);

	connect(leftBlockDMButton, &QPushButton::clicked, this, &MainWindow::slot_leftBlockDMButton_clicked);

	selectBlockVersionVWidget = new QWidget(leftSwitchBlockWidget);
	selectBlockVersionVWidget->setObjectName("selectBlockVersionVWidget");
	leftSwitchBlockVLayout->addWidget(selectBlockVersionVWidget);

	selectBlockVersionVLayout = new QVBoxLayout(selectBlockVersionVWidget);
	selectBlockVersionVLayout->setObjectName("selectBlockVersionVLayout");
	selectBlockVersionVLayout->setContentsMargins(0, 0, 0, 0);
	leftSwitchBlockVLayout->addLayout(selectBlockVersionVLayout);

	selectBlockVersionUpSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed);
	selectBlockVersionVLayout->addItem(selectBlockVersionUpSpacer);

	selectBlockVersionLabel = new QLabel(selectBlockVersionVWidget);
	selectBlockVersionLabel->setObjectName("selectBlockVersionLabel");
	selectBlockVersionLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
	selectBlockVersionLabel->setStyleSheet("color: white;");
	selectBlockVersionLabel->setText("asdf");
	selectBlockVersionVLayout->addWidget(selectBlockVersionLabel);

	selectBlockVersionHWidget = new QWidget(selectBlockVersionVWidget);
	selectBlockVersionHWidget->setObjectName("selectBlockVersionHWidget");
	selectBlockVersionVLayout->addWidget(selectBlockVersionHWidget);

	selectBlockVersionHLayout = new QHBoxLayout(selectBlockVersionHWidget);
	selectBlockVersionHLayout->setObjectName("selectBlockVersionHLayout");
	selectBlockVersionHLayout->setContentsMargins(0, 0, 0, 0);
	selectBlockVersionVLayout->addLayout(selectBlockVersionHLayout);

	selectBlockVersionLeftSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	selectBlockVersionHLayout->addItem(selectBlockVersionLeftSpacer);

	selectBlockVersionComboBox = new QComboBox(selectBlockVersionHWidget);
	selectBlockVersionComboBox->setObjectName("selectBlockVersionComboBox");
	selectBlockVersionComboBox->setFixedWidth(BLOCK_VERSION_COMBO_WIDTH);
	selectBlockVersionComboBox->setFixedHeight(BLOCK_VERSION_COMBO_HEIGHT);
	selectBlockVersionHLayout->addWidget(selectBlockVersionComboBox);

	selectBlockVersionRightSpacer = new QSpacerItem(100, 0, QSizePolicy::Expanding);
	selectBlockVersionHLayout->addItem(selectBlockVersionRightSpacer);
}

void MainWindow::initUiAdapter()
{
	selectAdapterWidget = new QWidget(leftSettingsWidget);
	selectAdapterWidget->setObjectName("selectAdapterWidget");

	selectAdapterVLayout = new QVBoxLayout(selectAdapterWidget);
	selectAdapterVLayout->setContentsMargins(0, 0, 0, 0);
	selectAdapterVLayout->setObjectName("selectAdapterVLayout");

	findAdapterWidget = new QWidget(selectAdapterWidget);
	findAdapterWidget->setObjectName("findAdapterWidget");
	selectAdapterVLayout->addWidget(findAdapterWidget);

	findAdapterHLayout = new QHBoxLayout(findAdapterWidget);
	findAdapterHLayout->setObjectName("findAdapterHLayout");
	findAdapterHLayout->setContentsMargins(0, 0, 0, 0);

	// Adapter button
	checkAdaptersButton = new QPushButton(findAdapterWidget);
	checkAdaptersButton->setObjectName("checkAdaptersButton");
	checkAdaptersButton->setFixedSize(MIN_ADAPTER_BUTTON_SIZE, MIN_ADAPTER_BUTTON_SIZE);
	findAdapterHLayout->addWidget(checkAdaptersButton);

	findAdapterCenterSpacer = new QSpacerItem(2, 0, QSizePolicy::Fixed);
	findAdapterHLayout->addItem(findAdapterCenterSpacer);

	// Adapter combo box
	selectAdapterComboBox = new QComboBox(findAdapterWidget);
	selectAdapterComboBox->setObjectName("selectAdapterComboBox");
	selectAdapterComboBox->setFixedHeight(MIN_ADAPTER_COMBO_HEIGHT);
	selectAdapterComboBox->setMaximumWidth(MAX_ADAPTER_COMBO_WIDTH);
	findAdapterHLayout->addWidget(selectAdapterComboBox);

	selectAdapterMiddleSpacer = new QSpacerItem(0, 2, QSizePolicy::Fixed);
	selectAdapterVLayout->addItem(selectAdapterMiddleSpacer);

	// Adapter label
	selectAdapterLabel = new QLabel(findAdapterWidget);
	selectAdapterLabel->setObjectName("selectAdapterLabel");
	selectAdapterLabel->setMaximumWidth(MAX_ADAPTER_COMBO_WIDTH);
	selectAdapterVLayout->addWidget(selectAdapterLabel);
}

void MainWindow::initUiFrequency()
{
	selectFrequencyWidget = new QWidget(leftSettingsWidget);
	selectFrequencyWidget->setObjectName("selectFrequencyWidget");

	selectFrequencyVLayout = new QVBoxLayout(selectFrequencyWidget);
	selectFrequencyVLayout->setObjectName("selectFrequencyVLayout");

	selectFrequencyComboBox = new QComboBox(selectFrequencyWidget);
	selectFrequencyComboBox->setObjectName("selectAdapterComboBox");
	selectFrequencyComboBox->setFixedHeight(MIN_FREQUENCY_COMBO_HEIGHT);
	selectFrequencyComboBox->setMaximumWidth(MAX_FREQUENCY_COMBO_WIDTH);
	selectFrequencyVLayout->addWidget(selectFrequencyComboBox);
	
	frequencyMiddleSpacer = new QSpacerItem(0, 2, QSizePolicy::Fixed);
	selectFrequencyVLayout->addItem(frequencyMiddleSpacer);

	selectFrequencyLabel = new QLabel(selectFrequencyWidget);
	selectFrequencyLabel->setObjectName("selectFrequencyLabel");
	selectFrequencyVLayout->addWidget(selectFrequencyLabel);
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

	autoStandMainUpSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding);
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

	autoStandMainBottomSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding);
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

	testManualStandMiddleUpSpacer = new QSpacerItem(0, 20, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(testManualStandMiddleUpSpacer);

	// out test manual stand
	outTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	outTestManualStandButton->setObjectName("outTestManualStandButton");
	outTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(outTestManualStandButton);

	testManualStandMiddleBottomSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(testManualStandMiddleBottomSpacer);

	// in test manual stand
	inTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	inTestManualStandButton->setObjectName("inTestManualStandButton");
	inTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(inTestManualStandButton);

	testManualStandMiddleFooterSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(testManualStandMiddleFooterSpacer);

	// full test manual stand
	fullTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	fullTestManualStandButton->setObjectName("fullTestManualStandButton");
	fullTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(fullTestManualStandButton);

	backgroundManualStandMainBottomSpacer = new QSpacerItem(0, 10, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(backgroundManualStandMainBottomSpacer);

	// verificationtest test manual stand
	verificationtestTestManualStandButton = new QPushButton(backgroundManualStandWidget);
	verificationtestTestManualStandButton->setObjectName("verificationtest");
	verificationtestTestManualStandButton->setFixedSize(MIN_MAIN_IN_OUT_BUTTON_WIDTH, MIN_MAIN_IN_OUT_BUTTON_HEIGHT);
	testManualStandVLayout->addWidget(verificationtestTestManualStandButton);

#ifndef VERIFACATION_TEST
	verificationtestTestManualStandButton->hide();
#endif // VERIFACATION_TEST

	backgroundManualStandMainBottomSecondSpacer = new QSpacerItem(0, 20, QSizePolicy::Fixed);
	testManualStandVLayout->addItem(backgroundManualStandMainBottomSecondSpacer);

	backgroundManualStandMainRightSpacer = new QSpacerItem(25, 0, QSizePolicy::Fixed);
	backgroundManualStandHLayout->addItem(backgroundManualStandMainRightSpacer);

	manualStandMainBottomSpacer = new QSpacerItem(0, 75, QSizePolicy::Expanding);
	manualStandMainVLayout->addItem(manualStandMainBottomSpacer);

	manualStandMainRightSpacer = new QSpacerItem(1, 0, QSizePolicy::Expanding);
	manualStandMainHLayout->addItem(manualStandMainRightSpacer);
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
}

void MainWindow::initConnections()
{
	connect(this, &MainWindow::resizeStandSlider, switchStandSlider, &QSliderButton::resizeSlider);
	connect(switchStandSlider, &QSliderButton::on_sliderSwitchStand_click, this, &MainWindow::slot_sliderSwitchStand_clicked);
	connect(selectAdapterComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectAdapterComboBox_changed(int)));
	connect(selectFrequencyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectFrequencyComboBox_changed(int)));
	connect(selectBlockVersionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_selectBlockVersionComboBox_changed(int)));
	connect(timerCheckAdapter, SIGNAL(timeout()), this, SLOT(Timer_CheckAdapter()));
	connect(manualStandButton, &QPushButton::clicked, this, &MainWindow::slot_manualStandButton_clicked);
	connect(autoStandButton, &QPushButton::clicked, this, &MainWindow::slot_autoStandButton_clicked);
	connect(switchThemeButton, &QPushButton::clicked, this, &MainWindow::slot_switchThemeButton_clicked);
	connect(switchLanguageButton, &QPushButton::clicked, this, &MainWindow::slot_switchLanguageButton_clicked);
	connect(checkAdaptersButton, &QPushButton::clicked, this, &MainWindow::slot_checkAdaptersButton_clicked);
	connect(leftBlockBCMButton, &QPushButton::clicked, this, &MainWindow::slot_leftBlockBCMButton_clicked);
	connect(leftBlockDMButton, &QPushButton::clicked, this, &MainWindow::slot_leftBlockDMButton_clicked);
	connect(outTestManualStandButton, &QPushButton::clicked, this, &MainWindow::slot_outTestManualStandButton_clicked);
	connect(inTestManualStandButton, &QPushButton::clicked, this, &MainWindow::slot_inTestManualStandButton_clicked);
	connect(fullTestManualStandButton, &QPushButton::clicked, this, &MainWindow::slot_fullTestManualStandButton_clicked);
	connect(verificationtestTestManualStandButton, &QPushButton::clicked, this, &MainWindow::slot_verificationtestTestManualStandButton_clicked);
	connect(inManualTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::slot_inManualTestAutoStandButton_clicked);
	connect(outManualTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::slot_outManualTestAutoStandButton_clicked);
	connect(inAutoTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::slot_inAutoTestAutoStandButton_clicked);
	connect(outAutoTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::slot_outAutoTestAutoStandButton_clicked);
	connect(fullTestAutoStandButton, &QPushButton::clicked, this, &MainWindow::slot_fullTestAutoStandButton_clicked);
}

void MainWindow::initConfig()
{
	QFile file("cables.cfg");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream cable(&file);

	QString config = cable.readLine();
	config = cable.readLine();
	QStringList list = config.split(u';');

	if (list[0] == "LIGHT_THEME" || list[0] == "NOT_SET") // Òåìà
		viewWindowState->appTheme = LIGHT_THEME;
	else
		viewWindowState->appTheme = DARK_THEME;

	if (list[1] == "RUSSIAN_LANG" || list[1] == "NOT_SET") // ßçûê
		viewWindowState->appLanguage = RUSSIAN_LANG;
	else
		viewWindowState->appLanguage = ENGLISH_LANG;

	if (list[2] != "NOT_SET") // Áëîê
	{
		if (list[2].left(3) == "DTM")
			leftBlockDMButton->click();
		else
			leftBlockBCMButton->click();

		QString version = list[2].remove(list[2].left(3));
		for (int i = 0; i < selectBlockVersionComboBox->count(); i++)
			if (selectBlockVersionComboBox->itemText(i) == version)
			{
				selectBlockVersionComboBox->setCurrentIndex(i);
				cables.clear();
				loadCables(viewWindowState->selectedBlock, selectBlockVersionComboBox->itemText(i));
			}
	}

	if (list[3] == "MANUAL_STAND") // Ñòåíä
	{
		selectedTypeStand = TypeStand::MANUAL;
		switchStandSlider->setStatus(TypeStand::MANUAL);
	}
	else
	{
		selectedTypeStand = TypeStand::AUTO;
		switchStandSlider->setStatus(TypeStand::AUTO);
	}
	switchStandButtons();

	bool search = false;
	if (list[4] == "FREQUENCY_50K") // ×àñòîòà
	{

		selectFrequencyComboBox->setCurrentIndex(2);
		search = true;
	}
	else if (list[4] == "FREQUENCY_100K")
	{
		selectFrequencyComboBox->setCurrentIndex(3);
		search = true;
	}
	else if (list[4] == "FREQUENCY_125K")
	{
		selectFrequencyComboBox->setCurrentIndex(4);
		search = true;
	}
	else if (list[4] == "FREQUENCY_250K")
	{
		selectFrequencyComboBox->setCurrentIndex(5);
		search = true;
	}
	else if (list[4] == "FREQUENCY_500K")
	{
		selectFrequencyComboBox->setCurrentIndex(6);
		search = true;
	}
	else if (list[4] == "FREQUENCY_1000K")
	{
		selectFrequencyComboBox->setCurrentIndex(7);
		search = true;
	}
	if (search)
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			selectFrequencyLabel->setText(QString("Частота: ") + selectFrequencyComboBox->currentText());
		else if (viewWindowState->appLanguage == ENGLISH_LANG)
			selectFrequencyLabel->setText(QString("Frequency: ") + selectFrequencyComboBox->currentText());
		can->setSelectedFrequency(selectFrequencyComboBox->currentText());
		search = false;
	}


	if (list[5] != "NOT_SET") // ???????
	{
		std::vector<QString> nameAdapters = can->getNameAdapters();

		for (int i = 0; i < nameAdapters.size(); i++)
			if (nameAdapters[i].remove("\n") == list[5])
			{
				selectAdapterComboBox->setCurrentIndex(i + 1);
				search = true;

			}
	}
	if (search)
	{
		can->setSelectedAdapterNeme(selectAdapterComboBox->currentText());
		selectAdapterLabel->setText("");
		timerCheckAdapter->start(1000);
	}

	switchStyleMainButtons();
	file.close();
}
void MainWindow::resetConfig()
{
	QFile file("cables.cfg");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream cable(&file);

	QString config = cable.readLine();
	QString conf = cable.readLine();

	if (viewWindowState->appTheme == LIGHT_THEME) // Òåìà
		conf = "LIGHT_THEME;";
	else
		conf = "DARK_THEME;";

	if (viewWindowState->appLanguage == RUSSIAN_LANG) // ßçûê
		conf += "RUSSIAN_LANG;";
	else
		conf += "ENGLISH_LANG;";

	if(viewWindowState->selectedBlock == TestBlockName::EMPTY) // Áëîê
		conf += "NOT_SET;";
	else if (viewWindowState->selectedBlock == TestBlockName::DTM)
		conf += "DTM" + selectBlockVersionComboBox->currentText() + ";";
	else if (viewWindowState->selectedBlock == TestBlockName::BCM)
		conf += "BCM" + selectBlockVersionComboBox->currentText() + ";";

	if (selectedTypeStand == TypeStand::MANUAL) // Ñòåíä
		conf += "MANUAL_STAND;";
	else if(selectedTypeStand == TypeStand::AUTO)
		conf += "AUTO_STAND;";

	if (selectFrequencyComboBox->currentIndex() == 2) // ×àñòîòà
		conf += "FREQUENCY_50K;";
	else if (selectFrequencyComboBox->currentIndex() == 3)
		conf += "FREQUENCY_100K;";
	else if (selectFrequencyComboBox->currentIndex() == 4)
		conf += "FREQUENCY_125K;";
	else if (selectFrequencyComboBox->currentIndex() == 5)
		conf += "FREQUENCY_250K;";
	else if (selectFrequencyComboBox->currentIndex() == 6)
		conf += "FREQUENCY_500K;";
	else if (selectFrequencyComboBox->currentIndex() == 7)
		conf += "FREQUENCY_1000K;";

	if (selectAdapterComboBox->currentText() == "...") // Àäàïòåð
		conf += "NOT_SET";
	else
		conf += selectAdapterComboBox->currentText().remove("\n");


	config += "\n" + conf;
	config += "\n" + cable.readAll();

	file.close();

	std::ofstream fout;
	fout.open("cables.cfg");
	fout << config.toStdString();
	fout.close();
}

void MainWindow::switchStyleMainButtons()
{
	if (can->getStatusFrequencySelected() && can->getStatusAdapterSelected() && viewWindowState->selectedBlock != TestBlockName::EMPTY)
	{
		if (viewWindowState->appTheme == LIGHT_THEME)
		{
			// button
			outTestManualStandButton->setStyleSheet(lightStyles.mainButton);
			inTestManualStandButton->setStyleSheet(lightStyles.mainButton);
			fullTestManualStandButton->setStyleSheet(lightStyles.mainButton);
			verificationtestTestManualStandButton->setStyleSheet(lightStyles.mainButton);
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
			verificationtestTestManualStandButton->setStyleSheet(darkStyles.mainButton);
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
			verificationtestTestManualStandButton->setStyleSheet(lightStyles.mainButtonNoActive);
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
			verificationtestTestManualStandButton->setStyleSheet(darkStyles.mainButtonNoActive);
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

	logoWidget->setFixedWidth(FIXED_LOGO_WIDTH + (viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON);
	logoWidget->setFixedHeight(FIXED_LOGO_HEIGHT + (viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON);

	leftHWidget->setFixedWidth(FIXED_LOGO_WIDTH + (viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON);

	// ????? ??????
	// manual
	manualStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	manualStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	// auto
	autoStandButton->setFixedWidth(MIN_STAND_BUTTON_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_BUTTON));
	autoStandButton->setFixedHeight(MIN_STAND_BUTTON_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_BUTTON));

	// stand slider slider 
	resizeStandSlider(MIN_STAND_SWITCH_SLIDER_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_SLIDER), MIN_STAND_SWITCH_SLIDER_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_SLIDER));
	switchStandSlider->setFixedWidth(MIN_STAND_SWITCH_SLIDER_WIDTH + ((viewWindowState->appSize.width - MIN_SCREEN_WIDTH) * COEF_STAND_SLIDER));
	switchStandSlider->setFixedHeight(MIN_STAND_SWITCH_SLIDER_HEIGHT + ((viewWindowState->appSize.height - MIN_SCREEN_HEIGHT) * COEF_STAND_SLIDER));

	// Main
	int normalizedManualTestButtonWidth = geometry().height() * 1.33;
	// out stend manual
	outTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in stend manual
	inTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// full stend manual
	fullTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	fullTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	
	// full stend manual
	verificationtestTestManualStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	verificationtestTestManualStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedManualTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	int normalizedAutoTestButtonWidth = geometry().height() * 1.645;
	// out manual stend auto
	outManualTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outManualTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in manual stend auto
	inManualTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inManualTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in auto stend suto
	outAutoTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	outAutoTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// in auto stend auto
	inAutoTestAutoStandButton->setFixedWidth(MIN_MAIN_IN_OUT_BUTTON_WIDTH + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	inAutoTestAutoStandButton->setFixedHeight(MIN_MAIN_IN_OUT_BUTTON_HEIGHT + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));

	// full stend auto
	fullTestAutoStandButton->setFixedWidth(MIN_MAIN_FUL_BUTTON_WIDTH + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
	fullTestAutoStandButton->setFixedHeight(MIN_MAIN_FUL_BUTTON_HEIGHT + ((normalizedAutoTestButtonWidth - MIN_SCREEN_WIDTH) * COEF_MAIN_BUTTON));
}

void MainWindow::slot_switchThemeButton_clicked()
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
	resetConfig();
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
		if (switchStandSlider->getStatus() == TypeStand::AUTO) // button
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
		// ???????
		checkAdaptersButton->setStyleSheet(lightStyles.themeLangButton); // button ??????????
		selectAdapterComboBox->setStyleSheet(lightStyles.settingComboBox); // settingComboBox
		selectAdapterLabel->setStyleSheet(lightStyles.settingSelectText); // lable ????????
		selectBlockVersionComboBox->setStyleSheet(lightStyles.settingComboBox);
		selectBlockVersionLabel->setStyleSheet(lightStyles.settingSelectText);

		// ???????
		selectFrequencyComboBox->setStyleSheet(lightStyles.settingComboBox); // settingComboBox
		selectFrequencyLabel->setStyleSheet(lightStyles.settingSelectText);  // lable ???????

		// Main
		// button
		switchStyleMainButtons();

		// lable
		autoTestAutoStandLabel->setStyleSheet(lightStyles.mainText);
		manualTestAutoStandLabel->setStyleSheet(lightStyles.mainText);
		manualStandLabel->setStyleSheet(lightStyles.mainText);
		if (viewWindowState->selectedBlock == TestBlockName::BCM) // button
		{
			leftBlockDMButton->setStyleSheet(lightStyles.standButtons);
			leftBlockBCMButton->setStyleSheet(lightStyles.alwaysActiveStandButton);
		}
		else if (viewWindowState->selectedBlock == TestBlockName::DTM)
		{
			leftBlockDMButton->setStyleSheet(lightStyles.alwaysActiveStandButton);
			leftBlockBCMButton->setStyleSheet(lightStyles.standButtons);
		}
		else
		{
			leftBlockDMButton->setStyleSheet(lightStyles.standButtons);
			leftBlockBCMButton->setStyleSheet(lightStyles.standButtons);
		}

		break;

	case DARK_THEME:
		switchThemeButton->setIcon(QIcon(*themeDarkPixmap));
		switchLanguageButton->setIcon(QIcon(*languageDarkPixmap));
		checkAdaptersButton->setIcon(QIcon(*checkAdapterDarkPixmap));

		logoLabel->setPixmap(*logoDarkPixmap);

		// Header
		// selectStand
		switchStandSlider->setStyleSheet(darkStyles.roundSlider, darkStyles.bgSlider); // slider
		if (switchStandSlider->getStatus() == TypeStand::AUTO) // button
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
		// ???????
		checkAdaptersButton->setStyleSheet(darkStyles.themeLangButton); // button ??????????
		selectAdapterComboBox->setStyleSheet(darkStyles.settingComboBox); // settingComboBox
		selectAdapterLabel->setStyleSheet(darkStyles.settingSelectText); // lable ????????
		selectBlockVersionComboBox->setStyleSheet(darkStyles.settingComboBox);
		selectBlockVersionLabel->setStyleSheet(darkStyles.settingSelectText);

		// ???????
		selectFrequencyComboBox->setStyleSheet(darkStyles.settingComboBox); // settingComboBox
		selectFrequencyLabel->setStyleSheet(darkStyles.settingSelectText);  // lable ???????

		// Main
		// button
		switchStyleMainButtons();

		// lable
		autoTestAutoStandLabel->setStyleSheet(darkStyles.mainText);
		manualTestAutoStandLabel->setStyleSheet(darkStyles.mainText);
		manualStandLabel->setStyleSheet(darkStyles.mainText);
		if (viewWindowState->selectedBlock == TestBlockName::BCM) // button
		{
			leftBlockDMButton->setStyleSheet(darkStyles.standButtons);
			leftBlockBCMButton->setStyleSheet(darkStyles.alwaysActiveStandButton);
		}
		else if (viewWindowState->selectedBlock == TestBlockName::DTM)
		{
			leftBlockDMButton->setStyleSheet(darkStyles.alwaysActiveStandButton);
			leftBlockBCMButton->setStyleSheet(darkStyles.standButtons);
		}
		else
		{
			leftBlockDMButton->setStyleSheet(darkStyles.standButtons);
			leftBlockBCMButton->setStyleSheet(darkStyles.standButtons);
		}
		break;
	}
}

void MainWindow::slot_sliderSwitchStand_clicked()
{
	if (selectedTypeStand == TypeStand::MANUAL)
	{
		selectedTypeStand = TypeStand::AUTO;
		switchStandSlider->setStatus(TypeStand::AUTO);

	}
	else
	{
		selectedTypeStand = TypeStand::MANUAL;
		switchStandSlider->setStatus(TypeStand::MANUAL);
	}
	switchStandButtons();
	resetConfig();
}

void MainWindow::slot_autoStandButton_clicked()
{
	if (selectedTypeStand != TypeStand::AUTO)
	{
		selectedTypeStand = TypeStand::AUTO;
		switchStandSlider->setStatus(TypeStand::AUTO);
		switchStandButtons();
		resetConfig();
	}
}

void MainWindow::slot_manualStandButton_clicked()
{
	if (selectedTypeStand != TypeStand::MANUAL)
	{
		selectedTypeStand = TypeStand::MANUAL;
		switchStandSlider->setStatus(TypeStand::MANUAL);
		switchStandButtons();
		resetConfig();
	}
}

void MainWindow::switchStandButtons()
{
	switch (switchStandSlider->getStatus())
	{
	case TypeStand::AUTO:
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

	case TypeStand::MANUAL:
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

void MainWindow::slot_switchLanguageButton_clicked()
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
	resetConfig();
	resetLanguage();
}

void MainWindow::resetLanguage()
{
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		manualStandButton->setText(QString("Ручной"));
		autoStandButton->setText(QString("Атомат."));
		inTestManualStandButton->setText(QString("Входы"));
		outTestManualStandButton->setText(QString("Выходы"));
		fullTestManualStandButton->setText(QString("Полная"));
		verificationtestTestManualStandButton->setText(QString("Валид."));
		inManualTestAutoStandButton->setText(QString("Входы"));
		outManualTestAutoStandButton->setText(QString("Выходы"));
		inAutoTestAutoStandButton->setText(QString("Входы"));
		outAutoTestAutoStandButton->setText(QString("Выходы"));
		fullTestAutoStandButton->setText(QString("Полная"));
		selectBlockVersionLabel->setText(QString("Версия блока"));
		if (!can->getStatusAdapterSelected())
			selectAdapterLabel->setText(QString("Выберите адаптер"));
		if (!can->getStatusFrequencySelected())
			selectFrequencyLabel->setText(QString("Выберите частоту"));
		manualTestAutoStandLabel->setText(QString("Ручной"));
		autoTestAutoStandLabel->setText(QString("Авто"));
		manualStandLabel->setText(QString("Ручной"));
		break;

	case ENGLISH_LANG:
		manualStandButton->setText(QString("Manual"));
		autoStandButton->setText(QString("Auto"));
		inTestManualStandButton->setText(QString("In"));
		outTestManualStandButton->setText(QString("Out"));
		fullTestManualStandButton->setText(QString("Full"));
		verificationtestTestManualStandButton->setText(QString("Verificationtest"));
		inManualTestAutoStandButton->setText(QString("In"));
		outManualTestAutoStandButton->setText(QString("Out"));
		inAutoTestAutoStandButton->setText(QString("In"));
		outAutoTestAutoStandButton->setText(QString("Out"));
		fullTestAutoStandButton->setText(QString("Full"));
		selectBlockVersionLabel->setText(QString("Block version"));
		if (!can->getStatusAdapterSelected())
			selectAdapterLabel->setText(QString("Select adapter"));
		if (!can->getStatusFrequencySelected())
			selectFrequencyLabel->setText(QString("Select frequency"));
		manualTestAutoStandLabel->setText(QString("Manual"));
		autoTestAutoStandLabel->setText(QString("Auto"));
		manualStandLabel->setText(QString("Manual"));

		break;
	}
	if (can->getStatusFrequencySelected())
		slot_selectFrequencyComboBox_changed(1);
}

void MainWindow::slot_selectFrequencyComboBox_changed(int index)
{
	if (!isAllInit)
		return;

	can->setSelectedFrequency(selectFrequencyComboBox->currentText());
	switchStyleMainButtons();

	if (index == 0)
		resetLanguage(); // ?????? ??????????????? lable ???????? ?????
	else if (index > 0)
	{
		if (viewWindowState->appLanguage == RUSSIAN_LANG)
			selectFrequencyLabel->setText(QString("Частота: ") + selectFrequencyComboBox->currentText());
		else if (viewWindowState->appLanguage == ENGLISH_LANG)
			selectFrequencyLabel->setText(QString("Frequency: ") + selectFrequencyComboBox->currentText());
	}
	resetConfig();
}

void MainWindow::slot_selectAdapterComboBox_changed(int index)
{
	if (!isAllInit)
		return;

	can->setSelectedAdapterNeme(selectAdapterComboBox->currentText());
	switchStyleMainButtons();

	if (index == 0)
		resetLanguage(); // Ñòàâèì ïðåäóïðåæäàþùèé lable ñîãëàñíî ÿçûêó
	else if (index > 0)
	{
		selectAdapterLabel->setText("");
		timerCheckAdapter->start(1000);
	}
	resetConfig();
}

void MainWindow::slot_checkAdaptersButton_clicked()
{


	selectAdapterComboBox->clear();
	selectAdapterComboBox->addItem("...");

	std::vector<QString> nameAdapters = can->getNameAdapters();

	for (int i = 0; i < nameAdapters.size(); i++)
		selectAdapterComboBox->addItem(nameAdapters[i]);
}

void MainWindow::slot_outTestManualStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	std::vector<Cable> preparedCables = {};
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].getDirection() == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::OUT_TEST_MANUAL_STAND, preparedCables);
}

void MainWindow::slot_inTestManualStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	std::vector<Cable> preparedCables = {};
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].getDirection() == DIRECTION_IN)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::IN_TEST_MANUAL_STAND, preparedCables);
}

void MainWindow::slot_fullTestManualStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG
#ifdef DEBUG_OUTPUT
	qDebug() << QTime::currentTime().toString("hh:mm:ss:z") << "Press button";
#endif
	createTestWindow(WindowType::FULL_TEST_MANUAL_STAND, cables);
}
void MainWindow::slot_verificationtestTestManualStandButton_clicked()
{
	this->hide();
	VerificationTest dlgVerification;

	can->initCan(WindowType::VERIFICATIONTEST);
	connect(can, &Can::Signal_ReciveMsg, &dlgVerification, &VerificationTest::Slot_ReciveMsg);

	WindowFrame w(WindowType::VERIFICATIONTEST, this, &dlgVerification);
	w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
	
	dlgVerification.exec();
	can->deinitCan();
	this->show();
}
void MainWindow::slot_inManualTestAutoStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].getDirection() == DIRECTION_IN)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::IN_MANUAL_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::slot_outManualTestAutoStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].getDirection() == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::OUT_MANUAL_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::slot_inAutoTestAutoStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].getDirection() == DIRECTION_IN)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::IN_AUTO_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::slot_outAutoTestAutoStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	std::vector<Cable> preparedCables;
	for (int i = 0; i < cables.size(); i++)
		if (cables[i].getDirection() == DIRECTION_OUT)
			preparedCables.push_back(cables[i]);

	createTestWindow(WindowType::OUT_AUTO_TEST_AUTO_STAND, preparedCables);
}

void MainWindow::slot_fullTestAutoStandButton_clicked()
{
#ifdef DEBUG
	selectAdapterComboBox->setCurrentIndex(1);
	selectFrequencyComboBox->setCurrentIndex(6);
	//selectedFileStandType = CFG_STAND_MANUAL;
	leftBlockDMButton->click();
#endif // DEBUG

	createTestWindow(WindowType::FULL_TEST_AUTO_STAND, cables);
}

void MainWindow::createTestWindow(WindowType testType, std::vector<Cable> preparedCables)
{
	if (!can->getStatusAdapterSelected())
	{
		generateWarning(Warnings::MainWindow::TEST_ACCESS_ADAPTER_SEL);
		return;
	}
	if (!can->getStatusFrequencySelected())
	{
		generateWarning(Warnings::MainWindow::TEST_ACCESS_FREQUENCY_SEL);
		return;
	}
	if (viewWindowState->selectedBlock == TestBlockName::EMPTY)
	{
		generateWarning(Warnings::MainWindow::NOT_SELECTED_BLOCK);
		return;
	}
	if (preparedCables.size() == 0)
	{
		generateWarning(Warnings::MainWindow::SIZE_CABLE_NUL);
		return;
	}
	std::vector<QString> nameAdapters = can->getNameAdapters();
	bool isHaveAdapter = false;
	for (int j = 0; j < nameAdapters.size(); j++)
		if (selectAdapterComboBox->currentText() == nameAdapters[j])
		{
			isHaveAdapter = true;
			break;
}
	if (!isHaveAdapter)
	{
		generateWarning(Warnings::MainWindow::ADAPTERS_CHANGED);
		slot_checkAdaptersButton_clicked();
		return;
	}

	TestWindow* testWindow = new TestWindow(testType, preparedCables, viewWindowState->selectedBlock, this);
	connect(can, &Can::Signal_ChangedStatusStandConnect, testWindow, &TestWindow::Slot_ChangedStatusStandConnect);
	connect(can, &Can::Signal_AfterTest, testWindow, &TestWindow::Slot_AfterTest);
	connect(can, &Can::Signal_ChangedByte, testWindow, &TestWindow::Slot_ChangedByte);
	connect(can, &Can::Signal_changeStatusCheckInformationBus, testWindow, &TestWindow::Slot_changeStatusCheckInformationBus);

	timerCheckAdapter->stop();
	can->setCable(preparedCables);
	can->initCan(testType);
	WindowFrame w(testType, nullptr, testWindow);
	w.setWindowIcon(QIcon(QPixmap(appLogoPath)));
	testWindow->setParentFrame(&w);
	w.show();
	this->hide();
	testWindow->exec();
	resetWindowView();
	can->deinitCan();
	timerCheckAdapter->start();
	this->show();
}

void MainWindow::resetWindowView()
{
	resetLanguage();
	resetTheme();
	parentFrame->resetTheme();
}

// ------------------------------------
// Name: setParentFrame
//		?????????? ????????????? ????????
// Variables: 
//			WindowFrame* parentFrame: ???????????? ???????
// ------------------------------------
void MainWindow::setParentFrame(WindowFrame* parentFrame)
{
	this->parentFrame = parentFrame;

	connect(switchThemeButton, &QPushButton::clicked, parentFrame, &WindowFrame::slot_switchThemeButton_clicked);
}

static Cable fillCable(int id, ConnectorId connector, int pin, int direction, int type, int canId, int bit, std::vector<Thresholds> thresholds, std::vector<Measureds> measureds, QString name, QString component)
{
	Cable cable;
	cable.setId(id);
	cable.setConnector(connector);
	cable.setPin(pin);
	cable.setDirection(direction);
	cable.setType(type);
	cable.setCanId(canId);
	cable.setBit(bit);
	cable.setThresholds(thresholds);
	cable.setMeasureds(measureds);
	cable.setName(name);
	cable.setComponent(component);
	return cable;
}

void MainWindow::initBlockVersions()
{
	QFile config("cables.cfg");
	if (!config.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		generateWarning(Warnings::MainWindow::FILE_NOT_FOUND);
		return;
	}

	QTextStream cable(&config);
	TestBlockName block = TestBlockName::EMPTY;
	while (!cable.atEnd())
	{
		QString line = cable.readLine();
		if (line == "CONFIG")
		{
			QString config = cable.readLine();

			continue;
		}
		if (line == "DTM")
		{
			block = TestBlockName::DTM;
			continue;
		}

		if (line == "BCM")
		{
			block = TestBlockName::BCM;
			continue;
		}

		if (line[0] == ":")
		{
			line.remove(":");
			switch (block)
			{
			case TestBlockName::DTM:
				blockVersionsDTM.push_back(line);
				break;

			case TestBlockName::BCM:
				blockVersionsBCM.push_back(line);
				break;

			case TestBlockName::EMPTY:
				// ERROR
				break;
			}
		}
	}
}

void MainWindow::loadCables(TestBlockName block, QString version)
{
	if (block == TestBlockName::EMPTY)
		return;

	viewWindowState->actualVersion = version;

	QFile config("cables.cfg");
	if (!config.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream cable(&config);
	bool admissionBlock = false;
	bool admissionVersion = false;
	int id = 0;
	while (!cable.atEnd())
	{
		QString line = cable.readLine();
		if (line == "CONFIG")
		{
			QString config = cable.readLine();

			continue;
		}
		if (line == "DTM")
			if (block == TestBlockName::DTM)
			{
				admissionBlock = true;
				continue;
			}
			else
			{
				admissionBlock = false;
				continue;
			}
		if (line == "BCM")
			if (block == TestBlockName::BCM)
			{
				admissionBlock = true;
				continue;
			}
			else
			{
				admissionBlock = false;
				continue;
			}

		QString oldLine = line;
		if (line[0] == ":")
			line.remove(":");
		if (admissionBlock && line == version)
		{
			admissionVersion = true;
			continue;
		}

		if (admissionVersion && oldLine[0] == ":")
		{
			admissionBlock = false;
			admissionVersion = false;
		}

		if (admissionVersion)
		{
			QStringList list = line.split(u';');
			id += 1;
			if (list.size() == 7)
			{
				Cable cable;
				cable.setId(id);
				cable.setConnector((ConnectorId)(list[0].toInt()));
				cable.setComponent(list[1]);
				cable.setType(list[3].toInt());
				cable.setCanId(list[4].toInt(nullptr,16));
				cable.setName(list[5]);
				cable.setBit(0);
				cables.push_back(cable);
				continue;
			}
			ConnectorId connector = (ConnectorId)(list[0].toInt());
			int pin = list[1].toInt();
			int direction = list[2].toInt();
			int type = list[3].toInt();
			int canId = list[4].toInt(nullptr, 16);
			int bit = list[5].toInt();
			QString name = list[6];
			QString component = list[7];

			std::vector<Thresholds> thresholds;
			std::vector<Measureds> measureds;
			for (int i = 8; i < list.size(); i += 2)
			{
				if (direction != DIRECTION_IN)
				{
					int minCurrent = list[i].toInt();
					int maxCurrent = list[i + 1].toInt();
					int minVoltage = list[i + 2].toInt();
					int maxVoltage = list[i + 3].toInt();

					thresholds.push_back(Thresholds(minCurrent, maxCurrent, minVoltage, maxVoltage));
					i += 2;
				}
				else
				{
					int minValue = list[i].toInt();
					int maxValue = list[i + 1].toInt();

					thresholds.push_back(Thresholds(minValue, maxValue));
				}
				measureds.push_back(Measureds());
			}
			cables.push_back(fillCable(id, connector, pin, direction, type, canId, bit, thresholds, measureds, name, component));
		}
	}
}

void MainWindow::slot_leftBlockBCMButton_clicked()
{
	if (viewWindowState->selectedBlock != TestBlockName::BCM)
	{
		viewWindowState->selectedBlock = TestBlockName::BCM;
		resetTheme();
		cables.clear();
		selectBlockVersionComboBox->clear();
		for (int i = 0; i < blockVersionsBCM.size(); i++)
			selectBlockVersionComboBox->addItem(blockVersionsBCM[i]);
		selectBlockVersionComboBox->setCurrentIndex((int)blockVersionsBCM.size() - 1);
		switchStyleMainButtons();
		if (isAllInit)
			resetConfig();
	}
}

void MainWindow::slot_leftBlockDMButton_clicked()
{
	verificationtestTestManualStandButton->click(); // УБРАТЬ!!!
	return; // УБРАТЬ!!!
	if (viewWindowState->selectedBlock != TestBlockName::DTM)
	{
		viewWindowState->selectedBlock = TestBlockName::DTM;
		resetTheme();
		cables.clear();
		selectBlockVersionComboBox->clear();
		for (int i = 0; i < blockVersionsDTM.size(); i++)
			selectBlockVersionComboBox->addItem(blockVersionsDTM[i]);
		selectBlockVersionComboBox->setCurrentIndex((int)blockVersionsDTM.size() - 1);
		switchStyleMainButtons();
		if(isAllInit)
		resetConfig();
	}
}

void MainWindow::slot_selectBlockVersionComboBox_changed(int index)
{
	if (!isAllInit)
		return;

	cables.clear();
	loadCables(viewWindowState->selectedBlock, selectBlockVersionComboBox->itemText(index));
	//resetConfig();

}

