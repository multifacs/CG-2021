#include "window.h"
#include "view.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(1200,600);

    glWidget = new View;
    glWidget->LoadData("torso1.bin");

    createControls(tr("Controls"));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(controlsGroup);
    layout->addWidget(glWidget);
    setLayout(layout);

    setWindowTitle(tr("Sliders"));
}
//! [2]

//! [3]
void Window::createControls(const QString &title)
//! [3] //! [4]
{
    controlsGroup = new QGroupBox(title);

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

    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMin);
    connect(minimumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMin);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMax);
    connect(maximumSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            glWidget, &View::SetMax);

    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->addWidget(minimumLabel, 0, 0);
    controlsLayout->addWidget(maximumLabel, 1, 0);

    controlsLayout->addWidget(minimumSpinBox, 0, 1);
    controlsLayout->addWidget(maximumSpinBox, 1, 1);
    controlsLayout->addWidget(radio1, 2, 0);
    controlsLayout->addWidget(radio2, 2, 1);
    controlsLayout->addWidget(radio3, 2, 2);
    controlsLayout->addWidget(modeLabel, 3, 0);
    controlsLayout->addWidget(modeCurrent, 3, 1);

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
    }
    else if (key_pressed == Qt::Key_S)
    {
        glWidget->PressS();
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
