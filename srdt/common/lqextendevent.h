/***********************************************************************************************************************
 * FILE NAME:       lqextevent.h
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:           Макросы регистрации нового типа события в библиатеке Qt
 * ********************************************************************************************************************/
#ifndef LQEXTEVENT_H
#define LQEXTEVENT_H

#include <QEvent>

#define __LQ_EXTENDED_QEVENT_REGISTERED extendEventRegistaredType()

#define __LQ_EXTENDED_QEVENT_DECLARATION    \
public: \
    static QEvent::Type msExtendedEventType; \
private:\
    static QEvent::Type extendEventRegistaredType();

#define __LQ_EXTENDED_QEVENT_IMPLEMENTATION(classname)                          \
    QEvent::Type classname::msExtendedEventType = QEvent::None;                 \
    QEvent::Type classname::extendEventRegistaredType()                         \
    {                                                                           \
        if (msExtendedEventType == QEvent::None)                                \
        {                                                                       \
            int generatedType = QEvent::registerEventType();                    \
            msExtendedEventType = static_cast<QEvent::Type>(generatedType);     \
        }                                                                       \
        return msExtendedEventType;                                             \
    }

#endif // LQEXTEVENT_H
