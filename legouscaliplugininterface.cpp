#include "legouscaliplugininterface.h"
#include "legouscaliwidget.h"
#include "application.h"
#include <QtPlugin>

LegoUSCaliPluginInterface::LegoUSCaliPluginInterface()
{
}

LegoUSCaliPluginInterface::~LegoUSCaliPluginInterface()
{
}

bool LegoUSCaliPluginInterface::CanRun()
{
    return true;
}

QWidget * LegoUSCaliPluginInterface::CreateFloatingWidget()
{
    LegoUSCaliWidget * widget = new LegoUSCaliWidget;
    return widget;
}

