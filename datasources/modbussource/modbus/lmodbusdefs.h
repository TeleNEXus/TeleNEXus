/* 
 * TeleNEXus is a simple SCADA programm
 *
 * Copyright (C) 2020 Sergey S. Kuzmenko
 *
 * This file is part of TeleNEXus.
 *
 * TeleNEXus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TeleNEXus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with TeleNEXus.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef LMODBUSDEFS_H
#define LMODBUSDEFS_H

/*
 * Общие определения.
 */
#define MODBUS_MAX_RTU_ADU_LENGTH               256     /* Максимальная длина пакета ModbusRTU      */
#define MODBUS_MIN_RTU_ADU_LENGTH               3       /* Минимальная длина пакета ModbusRTU       */
#define MODBUS_TCP_HEADER_MBAP_SIZE             (7)     /* Длина MODBUS Application Protocol header */
#define MODBUS_MIN_TCP_ADU_LENGTH               (7+2)   /* Мнимальная длина пакета ModbusTCP        */
#define MODBUS_MAX_TCP_ADU_LENGTH               260     /* Максимальная длина пакета ModbusTCP      */

#define MODBUS_MAX_REGISTER_VALUE               0xFFFF
#define MODBUS_MIN_START_ADDRESS                0x0000
#define MODBUS_MAX_START_ADDRESS                0xFFFF
#define MODBUS_MAX_READ_COIL_COUNT              0x7D0
#define MODBUS_MAX_WRITE_COIL_COUNT             0x7B0
#define MODBUS_MAX_READ_DISCRETE_INPUTS_COUNT   MODBUS_MAX_READ_COIL_COUNT
#define MODBUS_MAX_READ_REGISTER_COUNT          0x7D
#define MODBUS_MAX_WRITE_REGISTER_COUNT         0x7B
#define MODBUS_MAX_FIFO_COUNT                   31
#define MODBUS_MIN_SERVER_ADDRESS               1
#define MODBUS_MAX_SERVER_ADDRESS               247
#define MODBUS_TCP_PORT                         502
#define MODBUS_PROTOCOL_ID                      0
#define MODBUS_ASCII_CODE_CR                    0xd
#define MODBUS_ASCII_CODE_LF                    0xa
#define MODBUS_ASCII_CODE_COLON                 0x3a
#define MODBUS_COIL_VALUE_ON                    0xff00
#define MODBUS_COIL_VALUE_OFF                   0x0000
#define MODBUS_MESSAGE_BODY_OFFSET              2         //bytes

/*
 * Коды функций.
 */
#define MBFC_READ_COILS                         0x01  //    Получение текущего состояния (ON/OFF)
                                                      // группы логических ячеек.
#define MBFC_READ_DISCRETE_INPUTS               0x02  //    Получение текущего состояния (ON/OFF) группы
                                                      // дискретных входов.
#define MBFC_READ_HOLDING_REGISTERS             0x03  //    Получение текущего значения одного или нескольких
                                                      // регистров хранения.
#define MBFC_READ_INPUT_REGISTERS               0x04  //    Получение текущего значения одного или нескольких
                                                      // входных регистров.
#define MBFC_WRITE_SINGLE_COIL                  0x05  //    Изменение логической ячейки в состояние ON или OFF.
#define MBFC_WRITE_SINGLE_REGISTER              0x06  //    Запись нового значения в регистр хранения.
#define MBFC_READ_EXCEPTION_STATUS              0x07  //    Получение состояния (ON/OFF) восьми внутренних
                                                      // логических ячеек, чье
                                                      // назначение зависит от типа контроллера.
                                                      //    Пользователь может использовать эти ячейки по своему выбору.
#define MBFC_DIAGNOSTICS                        0x08  //    Тестовое сообщение, посылаемое SL для получения
                                                      // данных о связи.
#define MBFC_GET_COMM_EVENT_COUNTER             0x0B  //    Позволяет MS путем последовательной посылки
                                                      // одного сообщения определить выполнение операции.
#define MBFC_GET_COMM_EVENT_LOG                 0x0C  //    Позволяет MS получить журнал связи, который содержит
                                                      // информацию о каждой Modbus транзакции
                                                      // данного SL. Если транзакция не выполнена, в журнал заносится
                                                      // информация об ошибки.
#define MBFC_WRITE_MULTIPLE_COILS               0x0F  //    Изменить состояние (ON/OFF) нескольких последовательных
                                                      // логических ячеек.
#define MBFC_WRITE_MULTIPLE_REGISTERS           0x10  //    Установить новые значения нескольких последовательных
                                                      // регистров.
#define MBFC_REPORT_SLAVE_ID                    0x11  //    Позволяет MS определить тип адресуемого SL и его рабочее
                                                      // состояние.
#define MBFC_READ_FILE_RECORD                   0x14
#define MBFC_WRITE_FILE_RECORD                  0x15
#define MBFC_MASK_WRITE_REGISTER                0x16
#define MBFC_READ_WRITE_MULTIPLE_REGISTERS      0x17
#define MBFC_READ_FIFO_QUEUE                    0x18
#define MBFC_ENCAPSULATED_INTERFACE_TRANSPORT   0x2B

/*
 * Коды ошибок.
 */
#define MBEC_NO_EXCEPTION                               0x00    //  Ошибок нет
#define MBEC_ILLEGAL_FUNCTION                           0x01    //  Функция в принятом сообщении не поддерживается на
                                                                // данном SL.
#define MBEC_ILLEGAL_DATA_ADDRESS                       0x02    //  Адрес, указанный в поле данных, является
                                                                // недопустимым для данного SL.
#define MBEC_ILLEGAL_DATA_VALUE                         0x03    //  Значения в поле данных недопустимы для данного SL.
#define MBEC_SLAVE_DEVICE_FAILURE                       0x04    //  SL не может ответить на запрос или произошла авария.
#define MBEC_ACKNOWLEDGE                                0x05    // SL принял запрос и начал выполнять долговременную
                                                                // операцию программирования. Для определения момента
                                                                // завершения операции используйте запрос типа
                                                                // POLL PROGRAM COMPLETE.
                                                                // Если этот запрос был послан до завершения операции
                                                                // программирования, то SL ответит сообщением
                                                                // REJECTED MESSAGE.
#define MBEC_SLAVE_DEVICE_BUSY                          0x06    //  Сообщение было принято без ошибок, но SL в данный
                                                                // момент выполняет долговременную операцию
                                                                // программирования. Запрос необходимо ретранслировать
                                                                // позднее.
#define MBEC_MEMORY_PARITY_ERROR                        0x08
#define MBEC_GATEWAY_PATH_UNAVAILABLE                   0x0A
#define MBEC_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND    0x0B

/*
 * Общий код ошибки.
 */
#define MODBUS_EXCEPTION_FLAG 0x80

#define MB_BYTES_TO_REG(_p_bytes, _reg) _reg   =    ((TUint16)(*_p_bytes)) << 8;    \
                                        _reg   |=   (TUint16)(*(_p_bytes + 1))

#define MB_REG_TO_BYTES(_reg, _p_bytes) (*_p_bytes) = (TUint8)(_reg >> 8);          \
                                        (*(_p_bytes + 1)) = (TUint8)(_reg & 0x00ff);

#endif // LMODBUSDEFS_H
