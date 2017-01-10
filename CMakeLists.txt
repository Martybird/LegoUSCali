project( ${PluginName} )

# define sources
set( PluginSrc legouscaliplugininterface.cpp legouscaliwidget.cpp )
set( PluginHdrMoc legouscaliwidget.h legouscaliplugininterface.h )
set( PluginUi legouscaliwidget.ui )

# Create plugin
DefinePlugin( "${PluginSrc}" "${PluginHdr}" "${PluginHdrMoc}" "${PluginUi}" )
