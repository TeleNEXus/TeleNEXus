/***********************************************************************************************************************
 * FILE NAME:       lexcept.cpp
 * CREATION DATE:   23.01.2020
 * AUTHOR:          KuzmenkoSS
 * CO-AUTHORS:
 * TITLE:           Файл описания исключений.
 * ********************************************************************************************************************/
#include "lexcept.h"

namespace lstd
{

static const char message_base[]               = "BASE";
static const char message_index_out_of_range[] = "INDEX OUT OF RANGE";
static const char message_length_error[]       = "LENGTH ERROR";


LExceptionBase::EExceptionCode  LExceptionBase::code(void){return EC_BASE;}

const char* LExceptionBase::message(void){ return message_base;}


LExceptionBase::EExceptionCode  LExcIndexOutOfRange::code(void){return EC_INDEX_OUT_OF_RANGE;}

const char* LExcIndexOutOfRange::message(void){ return message_index_out_of_range;}

LExceptionBase::EExceptionCode  LExcLengthError::code(void){return EC_INDEX_OUT_OF_RANGE;}

const char* LExcLengthError::message(void){ return message_length_error;}
}
