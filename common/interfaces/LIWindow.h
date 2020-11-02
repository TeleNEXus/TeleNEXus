#ifndef LIWINDOW_H_ 
#define LIWINDOW_H_

#include <QSharedPointer>

class LIApplication;

class LIWindow
{
public:
    LIWindow(){}
    virtual ~LIWindow(){}

    /*
     * Показывает окно.
     */
    virtual void show() = 0;

    /*
     * Скрывает окно.
     */
    virtual void hide() = 0;

    /*
     * Возвращает указатель на другое окно с заданным идентификатором.
     */
    virtual QSharedPointer<LIWindow> getOtherWindow(const QString& _windowId) = 0;
};

#endif // LIWINDOW_H_
