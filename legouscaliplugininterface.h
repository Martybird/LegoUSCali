#ifndef LEGOUSCALPLUGININTERFACE_H
#define LEGOUSCALPLUGININTERFACE_H

#include <QObject>
#include "toolplugininterface.h"

class LegoUSCaliWidget;

class LegoUSCaliPluginInterface : public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES( IbisPlugin )
    Q_PLUGIN_METADATA( IID "Ibis.LegoUSCaliPluginInterface" )

public:

    LegoUSCaliPluginInterface();
    ~LegoUSCaliPluginInterface();
    virtual QString GetPluginName() { return QString( "LegoUSCali" ); }
    bool CanRun();
    QString GetMenuEntryString() { return QString( "Lego US Calibration" ); }

    QWidget * CreateFloatingWidget();

};

#endif
