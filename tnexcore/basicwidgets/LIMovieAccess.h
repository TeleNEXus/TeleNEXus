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
#ifndef LIMOVIEACCESS_H_
#define LIMOVIEACCESS_H_

class QMovie;
class QSize;

class LIMovieAccess
{
public:
    LIMovieAccess(){};
    virtual ~LIMovieAccess(){};
    virtual QMovie* getMovie() = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    /* virtual QSize getSize() = 0; */
};

#endif  /* LIMOVIEACCESS_H_ */