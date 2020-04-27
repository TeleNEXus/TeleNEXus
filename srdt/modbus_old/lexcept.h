/***********************************************************************************************************************
 * FILE NAME:       lexcept.h
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:           Заголовочный файл описания исключений.
 * ********************************************************************************************************************/
#ifndef LEXCEPT_H
#define LEXCEPT_H

namespace lstd {

/*
 * Базовый класс для ислючений.
 */
class LExceptionBase
{
public:
    typedef enum
    {
        EC_BASE                 = 0,
        EC_INDEX_OUT_OF_RANGE   = 10,
        EC_LENGTH_ERR           = 20
    }EExceptionCode;

public:
    LExceptionBase(){}
    virtual ~LExceptionBase(){}
    virtual EExceptionCode  code(void);
    virtual const char*     message(void);
private:
};

/*
 * Выход индекса за диапазон.
 */
class LExcIndexOutOfRange : public LExceptionBase
{

public:
    LExcIndexOutOfRange(){}
    virtual ~LExcIndexOutOfRange(){}
    virtual EExceptionCode  code(void);
    virtual const char*     message(void);
};

/*
 * Ошибка длины.
 */
class LExcLengthError : public LExceptionBase
{

public:
    LExcLengthError(){}
    virtual ~LExcLengthError(){}
    virtual EExceptionCode  code(void);
    virtual const char*     message(void);
};

} //namespace lstd
#endif // LEXCEPT_H
