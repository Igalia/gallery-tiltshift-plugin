#include "gallerytiltshiftaboutwidget.h"
#include <MLabel>
#include <MWidgetCreator>
#include <QGraphicsLinearLayout>

M_REGISTER_WIDGET_NO_CREATE(GalleryTiltShiftAboutWidget)

GalleryTiltShiftAboutWidget::GalleryTiltShiftAboutWidget() :
    MWidgetController(),
    m_aboutLabel(new MLabel),
    m_disclaimerLabel(new MLabel)
{
    m_disclaimerLabel->setStyleName("TiltShiftAboutDisclaimer");
    m_disclaimerLabel->setText("Tilt Shift plugin has been proudly brought to you by <a href=\"http://www.igalia.com\">Igalia</a>");

    m_aboutLabel->setStyleName("TiltShiftAboutLink");
    m_aboutLabel->setText("<a href=\"about\">About</a>");


    connect(m_disclaimerLabel, SIGNAL(linkActivated(QString)),
            this, SIGNAL(linkActivated(QString)));
    connect(m_aboutLabel, SIGNAL(linkActivated(QString)),
            this, SIGNAL(linkActivated(QString)));

    QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addItem(m_disclaimerLabel);
    mainLayout->addStretch();
    mainLayout->addItem(m_aboutLabel);

    setLayout(mainLayout);
}

GalleryTiltShiftAboutWidget::~GalleryTiltShiftAboutWidget()
{
    delete m_aboutLabel;
    delete m_disclaimerLabel;
}

#include <QDebug>
void GalleryTiltShiftAboutWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    emit linkActivated("noLink");
}
