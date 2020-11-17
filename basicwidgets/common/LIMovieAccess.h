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
