#include "window.h"
#include "view.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

Window::Window(QWidget *parent, const char* input)
    : QWidget(parent)
{
    setMinimumSize(1200,600);

    glWidget = new View;
    QSurfaceFormat format;
    QSurfaceFormat::setDefaultFormat(format);
    glWidget->setFormat(format);
    glWidget->LoadData(input);

    document = input;

    createControls(tr("Controls"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(controlsGroup);
    layout->addWidget(glWidget);
    setLayout(layout);

    setWindowTitle(tr("Sliders"));
}

void Window::createControls(const QString &title)
{
    controlsGroup = new QGroupBox(title);

    documentLabel = new QLabel(tr("Document name:"));
    documentName = new QLabel(tr(document));

    minimumLabel = new QLabel(tr("Minimum value:"));
    maximumLabel = new QLabel(tr("Maximum value:"));

    modeLabel = new QLabel(tr("Visualisation mode:"));
    modeCurrent = new QLabel(tr("QUADS"));

    minimumSpinBox = new QSpinBox;
    minimumSpinBox->setRange(-50000, 50000);
    minimumSpinBox->setSingleStep(1);

    maximumSpinBox = new QSpinBox;
    maximumSpinBox->setRange(-50000, 50000);
    maximumSpinBox->setSingleStep(1);

    radio1 = new QRadioButton(tr("&X-Axis"));
    radio2 = new QRadioButton(tr("Y&-Axis"));
    radio3 = new QRadioButton(tr("Z-&Axis"));

    connect(radio1, &QRadioButton::clicked, this, &Window::handleButton);
    connect(radio2, &QRadioButton::clicked, this, &Window::handleButton);
    connect(radio3, &QRadioButton::clicked, this, &Window::handleButton);

    radio1->setChecked(true);

    dimLabel1 = new QLabel(tr("Width:"));
    dimLabel2 = new QLabel(tr("Height:"));
    dimLabel3 = new QLabel(tr("Depth:"));
    dimWidth = new QLabel(tr(std::to_string(glWidget->getDim()[0]).c_str()));
    dimHeight = new QLabel(tr(std::to_string(glWidget->getDim()[1]).c_str()));
    dimDepth = new QLabel(tr(std::to_string(glWidget->getDim()[2]).c_str()));

    layerLabel = new QLabel(tr("Current layer:"));
    layerCurrent = new QLabel(tr(std::to_string(glWidget->getLayer()).c_str()));


    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMin);
    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMin);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMax);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMax);

    QGridLayout *controlsLayout = new QGridLayout;

    controlsLayout->addWidget(documentLabel, 0, 0);
    controlsLayout->addWidget(documentName, 0, 1);

    controlsLayout->addWidget(minimumLabel, 1, 0);
    controlsLayout->addWidget(minimumSpinBox, 1, 1);

    controlsLayout->addWidget(maximumLabel, 2, 0);
    controlsLayout->addWidget(maximumSpinBox, 2, 1);

    controlsLayout->addWidget(radio1, 3, 0);
    controlsLayout->addWidget(radio2, 3, 1);
    controlsLayout->addWidget(radio3, 3, 2);

    controlsLayout->addWidget(modeLabel, 4, 0);
    controlsLayout->addWidget(modeCurrent, 4, 1);

    controlsLayout->addWidget(dimLabel1, 5, 0);
    controlsLayout->addWidget(dimWidth, 5, 1);

    controlsLayout->addWidget(dimLabel2, 6, 0);
    controlsLayout->addWidget(dimHeight, 6, 1);

    controlsLayout->addWidget(dimLabel3, 7, 0);
    controlsLayout->addWidget(dimDepth, 7, 1);

    controlsLayout->addWidget(layerLabel, 8, 0);
    controlsLayout->addWidget(layerCurrent, 8, 1);

    controlsGroup->setLayout(controlsLayout);
}

void Window::handleButton()
{
  if (radio1->isChecked())
  {
      qDebug() << "SET X";
      glWidget->SetX();
  }
  else if (radio2->isChecked())
  {
      qDebug() << "SET Y";
      glWidget->SetY();
  }
  else
  {
      qDebug() << "SET Z";
      glWidget->SetZ();
  }
}
//! [8]
//!
void Window::keyPressEvent(QKeyEvent *event)
{
    quint32 key_pressed = event->nativeVirtualKey();
    qDebug() << "PRESSED";

    if (key_pressed == Qt::Key_W)
    {
        glWidget->PressW();
        layerCurrent->setText(std::to_string(glWidget->getLayer()).c_str());
    }
    else if (key_pressed == Qt::Key_S)
    {
        glWidget->PressS();
        layerCurrent->setText(std::to_string(glWidget->getLayer()).c_str());
    }
    else if (key_pressed == Qt::Key_N)
    {
        glWidget->PressN();

        qDebug() << "STATE: " << glWidget->getState();

        switch (glWidget->getState())
        {
            case 0:
                modeCurrent->setText("QUADS");
                break;
            case 1:
                modeCurrent->setText("QUAD_STRIP");
                break;
            case 2:
                modeCurrent->setText("TEX");
                break;
        }
    }
}
