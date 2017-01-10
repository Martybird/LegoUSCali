#ifndef __LegoUSCaliPluginInterface_h_
#define __LegoUSCaliPluginInterface_h_

#include <QObject>
#include "toolplugininterface.h"

class LegoUSCaliWidget;

class LegoUSCaliPluginInterface : public QObject, public ToolPluginInterface
{

    Q_OBJECT
    Q_INTERFACES(ToolPluginInterface)
    Q_PLUGIN_METADATA(IID "Ibis.LegoUSCaliPluginInterface" )

public:

    LegoUSCaliPluginInterface();
    ~LegoUSCaliPluginInterface();
    virtual QString GetPluginName() { return QString("LegoUSCali"); }
    bool CanRun();
    QString GetMenuEntryString() { return QString("Lego US Calibration"); }

    QWidget * CreateFloatingWidget();

};

#endif
