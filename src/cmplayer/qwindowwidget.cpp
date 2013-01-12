#include "qwindowwidget.hpp"
#include <QtCore/qdebug.h>
#include <QtGui/qevent.h>

QWindowFrame::QWindowFrame(QWidget *parent)
	: QFrame(parent), m_EmbeddedWindow(0)
{
//	m_top = topLevelWidget()
}

void QWindowFrame::setEmbeddedWindow(QWindow *window)
{
	m_EmbeddedWindow = window;

	this->window()->winId(); // force parent (top-level) creation
	m_EmbeddedWindow->setParent(this->window()->windowHandle());
}

QWindow *QWindowFrame::embeddedWindow() const
{
	return m_EmbeddedWindow;
}

bool QWindowFrame::event(QEvent *event) {
	if (m_EmbeddedWindow) {
		switch (event->type()) {
		case QEvent::Show:
			m_EmbeddedWindow->show();
			break;
		case QEvent::Hide:
			m_EmbeddedWindow->hide();
			break;
		case QEvent::Resize:
		case QEvent::Move: {
			const QPoint p =mapTo(window(), pos()) - pos();
			const QSize s = size();
			m_EmbeddedWindow->setGeometry(p.x()+1, p.y()+1, s.width()-2, s.height()-2);
			break;
		} default: break;
		}
	}
	return QWidget::event(event);
}

