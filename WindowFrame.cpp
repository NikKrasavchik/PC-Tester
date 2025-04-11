#include "windowframe.h"
#include "ui_windowframe.h"

#include <windows.h>
#include <windowsx.h>

#include <QMouseEvent>

#define BORDER_SIZE 5

const QString appDarkIconPath = ":/Dark/icons/App_Logo_White.png";
const QString closeDarkIconPath = ":/Dark/icons/Close_White.png";
const QString collapseHideDarkIconPath = ":/Dark/icons/Collapse_Hide_White.png";
const QString collapseShowDarkIconPath = ":/Dark/icons/Collapse_Show_White.png";
const QString maximizeDarkIconPath = ":/Dark/icons/Maximize_White.png";
const QString minimizeDarkIconPath = ":/Dark/icons/Minimize_White.png";
const QString defaultSizeDarkIconPath = ":/Dark/icons/Default_Size_White.png";

const QString appLightIconPath = ":/Light/icons/App_Logo_Black.png";
const QString closeLightIconPath = ":/Light/icons/Close_Black.png";
const QString collapseHideLightIconPath = ":/Light/icons/Collapse_Hide_Black.png";
const QString collapseShowLightIconPath = ":/Light/icons/Collapse_Show_Black.png";
const QString maximizeLightIconPath = ":/Light/icons/Maximize_Black.png";
const QString minimizeLightIconPath = ":/Light/icons/Minimize_Black.png";
const QString defaultSizeLightIconPath = ":/Light/icons/Default_Size_Black.png";

/// @brief Constructor for the WindowFrame class.
/// @param parent The parent widget.
/// @param child The child widget to be added to the window (optional).
WindowFrame::WindowFrame(WindowType windowType, QWidget* parent, QWidget* child)
	: QFrame(parent), ui(new Ui::WindowFrame)
{
	ui->setupUi(this);
	mBorderSize = BORDER_SIZE;

	initRecources();
	initLightStyleSheets();
	initDarkStyleSheets();
	resetTheme();

	//ui->title->setText("PC-Tester");
	setTitle(windowType);

	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);
	if (child != nullptr) {
		ui->body->layout()->addWidget(child);
		mMainBody = child;
		mMainBody->installEventFilter(this);
		resize(child->size());
	}
	mIsCollapse = false;
}

/// @brief Destructor for the WindowFrame class.
WindowFrame::~WindowFrame()
{
	delete ui;
}

void WindowFrame::initRecources()
{
	closeLightIcon = QIcon(closeLightIconPath);
	collapseHideLightIcon = QIcon(collapseHideLightIconPath);
	collapseShowLightIcon = QIcon(collapseShowLightIconPath);
	maximizeLightIcon = QIcon(maximizeLightIconPath);
	minimizeLightIcon = QIcon(minimizeLightIconPath);
	defaultLightSizeIcon = QIcon(defaultSizeLightIconPath);

	closeDarkIcon = QIcon(closeDarkIconPath);
	collapseHideDarkIcon = QIcon(collapseHideDarkIconPath);
	collapseShowDarkIcon = QIcon(collapseShowDarkIconPath);
	maximizeDarkIcon = QIcon(maximizeDarkIconPath);
	minimizeDarkIcon = QIcon(minimizeDarkIconPath);
	defaultDarkSizeIcon = QIcon(defaultSizeDarkIconPath);
}

void WindowFrame::slot_switchThemeButton_clicked()
{
	resetTheme();
}

void WindowFrame::resetTheme()
{
	switch (viewWindowState->appTheme)
	{
	case LIGHT_THEME:
		this->setIcon(appLightIconPath);
		ui->close->setIcon(closeLightIcon);
		ui->collapse->setIcon(collapseHideLightIcon);
		ui->maximum->setIcon(maximizeLightIcon);
		ui->minimum->setIcon(minimizeLightIcon);

		ui->header->setStyleSheet(lightStyles.headerDefaultStyle);
		ui->body->setStyleSheet(lightStyles.bodyStyle);
		ui->close->setStyleSheet(lightStyles.buttonStyle);
		ui->collapse->setStyleSheet(lightStyles.buttonStyle);
		ui->maximum->setStyleSheet(lightStyles.buttonStyle);
		ui->minimum->setStyleSheet(lightStyles.buttonStyle);
		ui->title->setStyleSheet(lightStyles.titleStyle);
		break;

	case DARK_THEME:
		this->setIcon(appDarkIconPath);
		ui->close->setIcon(closeDarkIcon);
		ui->collapse->setIcon(collapseHideDarkIcon);
		ui->maximum->setIcon(maximizeDarkIcon);
		ui->minimum->setIcon(minimizeDarkIcon);

		ui->header->setStyleSheet(darkStyles.headerDefaultStyle);
		ui->body->setStyleSheet(darkStyles.bodyStyle);
		ui->close->setStyleSheet(darkStyles.buttonStyle);
		ui->collapse->setStyleSheet(darkStyles.buttonStyle);
		ui->maximum->setStyleSheet(darkStyles.buttonStyle);
		ui->minimum->setStyleSheet(darkStyles.buttonStyle);
		ui->title->setStyleSheet(darkStyles.titleStyle);
		break;
	}
}

/// @brief Show header menu.
/// @param pos position mouse click.
void WindowFrame::showHeaderContextMenu(const QPoint& pos) {
	QMenu contextMenu(this);

	QAction* exitAction = contextMenu.addAction(tr("&Exit"));
	connect(exitAction, &QAction::triggered, this, &WindowFrame::close);

	contextMenu.addAction(exitAction);
	contextMenu.exec(mapToGlobal(pos));
}

/// @brief Handler for the "Close" button click signal.
void WindowFrame::on_close_clicked()
{
	mMainBody->close();
}

/// @brief Handler for the "Maximize/Restore" button click signal.
void WindowFrame::on_maximum_clicked() {
	if (isMaximized()) {
		switch (viewWindowState->appTheme)
		{
		case LIGHT_THEME:
			ui->maximum->setIcon(maximizeLightIcon);
			mIsCollapse ? ui->header->setStyleSheet(lightStyles.headerCollapseStyle) : ui->header->setStyleSheet(lightStyles.headerDefaultStyle);
			break;

		case DARK_THEME:
			ui->maximum->setIcon(maximizeDarkIcon);
			mIsCollapse ? ui->header->setStyleSheet(darkStyles.headerCollapseStyle) : ui->header->setStyleSheet(darkStyles.headerDefaultStyle);
			break;
		}

		showNormal();
	}
	else {
		switch (viewWindowState->appTheme)
		{
		case LIGHT_THEME:
			ui->maximum->setIcon(defaultLightSizeIcon);
			ui->header->setStyleSheet(lightStyles.headerMaximizeStyle);
			break;

		case DARK_THEME:
			ui->maximum->setIcon(defaultDarkSizeIcon);
			ui->header->setStyleSheet(darkStyles.headerMaximizeStyle);
			break;
		}
		showMaximized();
	}
}

/// @brief Handler for the "Minimize" button click signal.
void WindowFrame::on_minimum_clicked() {
	showMinimized();
}

/// @brief Handler for the "Collapse" button click signal.
void WindowFrame::on_collapse_clicked() {
	if (mIsCollapse) {
		ui->body->setVisible(true);
		mIsCollapse = false;

		switch (viewWindowState->appTheme)
		{
		case LIGHT_THEME:
			ui->collapse->setIcon(collapseHideLightIcon);
			isMaximized() ? ui->header->setStyleSheet(lightStyles.headerMaximizeStyle) : ui->header->setStyleSheet(lightStyles.headerDefaultStyle);
			break;

		case DARK_THEME:
			ui->collapse->setIcon(collapseHideDarkIcon);
			isMaximized() ? ui->header->setStyleSheet(darkStyles.headerMaximizeStyle) : ui->header->setStyleSheet(darkStyles.headerDefaultStyle);
			break;
		}
	}
	else {
		ui->body->setVisible(false);
		mIsCollapse = true;

		switch (viewWindowState->appTheme)
		{
		case LIGHT_THEME:
			ui->collapse->setIcon(collapseShowLightIcon);
			isMaximized() ? ui->header->setStyleSheet(lightStyles.headerMaximizeStyle) : ui->header->setStyleSheet(lightStyles.headerCollapseStyle);
			break;

		case DARK_THEME:
			ui->collapse->setIcon(collapseShowDarkIcon);
			isMaximized() ? ui->header->setStyleSheet(darkStyles.headerMaximizeStyle) : ui->header->setStyleSheet(darkStyles.headerCollapseStyle);
			break;
		}
	}
}

/// @brief Handler for the mouse press event.
/// @param event Pointer to the QMouseEvent object containing event information.
void WindowFrame::mousePressEvent(QMouseEvent* event) {
	if (event->buttons() == Qt::LeftButton) {
		QWidget* widget = childAt(event->x(), event->y());
		if (widget == ui->LHeader || widget == ui->title || widget == ui->icon) {
			mPosition.setX(event->x());
			mPosition.setY(event->y());
		}
	}
	if (event->button() == Qt::RightButton) {
		QWidget* widget = childAt(event->x(), event->y());
		if (widget == ui->LHeader || widget == ui->title || widget == ui->icon) {
			showHeaderContextMenu(event->pos());
		}
	}
}

/// @brief Handler for the mouse move event within the window.
/// @param event Pointer to the mouse move event object (QMouseEvent).
/// @return No return value.
void WindowFrame::mouseMoveEvent(QMouseEvent* event) {
	if (event->buttons() == Qt::LeftButton) {
		if (mPosition.x() != 0 || mPosition.y() != 0) {
			move(event->globalX() - mPosition.x(), event->globalY() - mPosition.y());
		}
	}
}

/// @brief Handler for the mouse release event within the window.
/// @param event Pointer to the mouse release event object (QMouseEvent).
void WindowFrame::mouseReleaseEvent(QMouseEvent* event) {
	Q_UNUSED(event);
	mPosition.setX(0);
	mPosition.setY(0);
}

/// @brief Handler for the mouse double-click event within the window.
/// @param event Pointer to the mouse double-click event object (QMouseEvent).
void WindowFrame::mouseDoubleClickEvent(QMouseEvent* event) {
	if (event->buttons() == Qt::LeftButton) {
		QWidget* widget = childAt(event->x(), event->y());
		if (widget == ui->LHeader) {
			if (isMaximized()) {
				switch (viewWindowState->appTheme)
				{
				case LIGHT_THEME:
					ui->maximum->setIcon(maximizeLightIcon);
					mIsCollapse ? ui->header->setStyleSheet(lightStyles.headerCollapseStyle) : ui->header->setStyleSheet(lightStyles.headerDefaultStyle);
					break;

				case DARK_THEME:
					ui->maximum->setIcon(maximizeDarkIcon);
					mIsCollapse ? ui->header->setStyleSheet(darkStyles.headerCollapseStyle) : ui->header->setStyleSheet(darkStyles.headerDefaultStyle);
					break;
				}

				showNormal();
			}
			else {
				switch (viewWindowState->appTheme)
				{
				case LIGHT_THEME:
					ui->maximum->setIcon(defaultLightSizeIcon);
					ui->header->setStyleSheet(lightStyles.headerMaximizeStyle);
					break;

				case DARK_THEME:
					ui->maximum->setIcon(defaultDarkSizeIcon);
					ui->header->setStyleSheet(darkStyles.headerMaximizeStyle);
					break;
				}
				showMaximized();
			}
		}
	}
}

/// @brief Handler for the native window event.
/// @param eventType The typeComboBox of event, as a byte array (QByteArray).
/// @param message Pointer to a structure containing event information (void*).
/// @param result Pointer to a variable for returning the result (long*).
/// @return The return value, true if the event was handled, otherwise false.
bool WindowFrame::nativeEvent(const QByteArray& eventType, void* message, long* result) {
	Q_UNUSED(eventType)
		MSG* param = static_cast<MSG*>(message);

	if (param->message == WM_NCHITTEST) {
		QPoint globalPos(GET_X_LPARAM(param->lParam), GET_Y_LPARAM(param->lParam));
		QPoint localPos = mapFromGlobal(globalPos);

		int nX = localPos.x();
		int nY = localPos.y();

		if (nX >= 0 && nX < mBorderSize) {
			if (nY >= 0 && nY < mBorderSize) {
				*result = HTTOPLEFT;
			}
			else if (nY >= height() - mBorderSize) {
				*result = HTBOTTOMLEFT;
			}
			else {
				*result = HTLEFT;
			}
		}
		else if (nX >= width() - mBorderSize) {
			if (nY >= 0 && nY < mBorderSize) {
				*result = HTTOPRIGHT;
			}
			else if (nY >= height() - mBorderSize) {
				*result = HTBOTTOMRIGHT;
			}
			else {
				*result = HTRIGHT;
			}
		}
		else if (nY >= 0 && nY < mBorderSize) {
			*result = HTTOP;
		}
		else if (nY >= height() - mBorderSize) {
			*result = HTBOTTOM;
		}
		else {
			return QWidget::nativeEvent(eventType, message, result);
		}

		return true;
	}

	return QWidget::nativeEvent(eventType, message, result);
}

/// @brief Show or hide the window minimization button.
/// @param enable If true, the button will be shown; if false, it will be hidden.
void WindowFrame::enableMinimum(bool enable) {
	!enable ? ui->minimum->hide() : ui->minimum->show();
}

/// @brief Show or hide the window maximization button.
/// @param enable If true, the button will be shown; if false, it will be hidden.
void WindowFrame::enableMaximum(bool enable) {
	!enable ? ui->maximum->hide() : ui->maximum->show();
}

/// @brief Show or hide the window close button.
/// @param enable If true, the button will be shown; if false, it will be hidden.
void WindowFrame::enableClose(bool enable) {
	!enable ? ui->close->hide() : ui->close->show();
}

/// @brief set window icon
/// @param iconPath - path to icon file
void WindowFrame::setIcon(const QString& iconPath) {
	QPixmap pixmap(iconPath);
	ui->icon->setPixmap(pixmap);
	ui->icon->setScaledContents(true);
	ui->icon->setAlignment(Qt::AlignCenter);
	ui->icon->resize(24, 24);
}

/// @brief set title for the window
/// @param title
void WindowFrame::setTitle(WindowType windowType) {
	QString tmpStr = "";
#ifdef FOR_DEVELOPER
	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
#ifdef QT5
		tmpStr = QString::fromLocal8Bit(" | Для разработчика");
#elif QT6

#endif // QT5
		break;
	case ENGLISH_LANG:
		tmpStr = " | For developer";
		break;
	}
#endif // !FOR_DEVELOPER

	switch (viewWindowState->appLanguage)
	{
	case RUSSIAN_LANG:
		switch (windowType)
		{
#ifdef QT5
		case WindowType::MAINWINDOW:
			ui->title->setText("PC-Tester" + tmpStr);
			break;

		case WindowType::CONFIGURATOR:
			ui->title->setText(QString::fromLocal8Bit("Конфигуратор") + tmpStr);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			ui->title->setText(QString::fromLocal8Bit("Входы | Ручной стенд") + tmpStr);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			ui->title->setText(QString::fromLocal8Bit("Выходы | Ручной стенд") + tmpStr);
			break;

		case WindowType::FULL_TEST_MANUAL_STAND:
			ui->title->setText(QString::fromLocal8Bit("Полная проверка | Ручной стенд") + tmpStr);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			ui->title->setText(QString::fromLocal8Bit("Входы ручная проверка | Автоматический стенд") + tmpStr);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			ui->title->setText(QString::fromLocal8Bit("Выходы ручная проверка | Автоматический стенд") + tmpStr);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			ui->title->setText(QString::fromLocal8Bit("Входы автоматическая проверка | Автоматический стенд") + tmpStr);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			ui->title->setText(QString::fromLocal8Bit("Выходы автоматическаяя проверка | Автоматический стенд") + tmpStr);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			ui->title->setText(QString::fromLocal8Bit("Полная автоматическая проверка | Автоматический стенд") + tmpStr);
			break;

		case WindowType::MOREWINDOW:
			ui->title->setText(QString::fromLocal8Bit("Подробнее...") + tmpStr);
			break;

		case WindowType::REPORTWINDOW:
			ui->title->setText(QString::fromLocal8Bit("Отчёт") + tmpStr);
			break;
		}
		break;
#elif QT6

#endif // QT5

	case ENGLISH_LANG:
		switch (windowType)
		{
		case WindowType::MAINWINDOW:
			ui->title->setText("PC-Tester" + tmpStr);
			break;

		case WindowType::CONFIGURATOR:
			ui->title->setText(QString("Configurator") + tmpStr);
			break;

		case WindowType::IN_TEST_MANUAL_STAND:
			ui->title->setText(QString("In test | Manual stend") + tmpStr);
			break;

		case WindowType::OUT_TEST_MANUAL_STAND:
			ui->title->setText(QString("Out test | Manual stend") + tmpStr);
			break;

		case WindowType::FULL_TEST_MANUAL_STAND:
			ui->title->setText(QString("Full test | Manual stend") + tmpStr);
			break;

		case WindowType::IN_MANUAL_TEST_AUTO_STAND:
			ui->title->setText(QString("In manual test | Auto stend") + tmpStr);
			break;

		case WindowType::OUT_MANUAL_TEST_AUTO_STAND:
			ui->title->setText(QString("Out manual test | Auto stend") + tmpStr);
			break;

		case WindowType::IN_AUTO_TEST_AUTO_STAND:
			ui->title->setText(QString("In auto test | Auto stend") + tmpStr);
			break;

		case WindowType::OUT_AUTO_TEST_AUTO_STAND:
			ui->title->setText(QString("Out auto test | Auto stend") + tmpStr);
			break;

		case WindowType::FULL_TEST_AUTO_STAND:
			ui->title->setText(QString("Full auto test | Auto stend") + tmpStr);
			break;

		case WindowType::MOREWINDOW:
			ui->title->setText(QString("More...") + tmpStr);
			break;

		case WindowType::REPORTWINDOW:
			ui->title->setText(QString("Report") + tmpStr);
			break;
		}
	}
}

/// @brief Override event filtering function for the WindowFrame class.
/// @param obj Pointer to the object for which the event was generated.
/// @param event Pointer to the QEvent object representing the event.
/// @return `true` if the event was handled, otherwise `false`.
bool WindowFrame::eventFilter(QObject* obj, QEvent* event) {
	if (obj == mMainBody) {
		if (event->type() == QEvent::HideToParent) {
			hide();
			return true;
		}
		if (event->type() == QEvent::ShowToParent) {
			show();
			return true;
		}
		return QObject::eventFilter(obj, event);
	}
	else {
		return QFrame::eventFilter(obj, event);
	}
	return false;
}