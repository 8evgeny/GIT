#ifndef STATIONS_MACRO_H
#define STATIONS_MACRO_H

#ifdef IPS_USING_QT
#include <QDebug> //:TODO make define dependent includes
#else
#endif  //IPS_USING_QT

#ifdef  IPS_USING_QT
#define IPS_DEBUG_OUTPUT_FCN    qDebug()
#elif   IPS_USING_STD
#define IPS_DEBUG_OUTPUT_FCN    std::cout
#else
//Define your own debug output function
#endif

#ifdef IPS_DEBUG_MESSAGES
#define IPS_FUNCTION_DBG    IPS_DEBUG_OUTPUT_FCN<<__PRETTY_FUNCTION__<<"\n";
#else
#define IPS_FUNCTION_DBG
#endif

#endif  // STATIONS_MACRO_H
