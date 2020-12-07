#ifndef LCLOCALSOURCEHIDEN_H_
#define LCLOCALSOURCEHIDEN_H_

#include <QObject>
#include "lcqlocaldatawriter.h"

class LCQLocalDataReader;
class LCQLocalDataWriter;

class LCQLocalSourceHiden final : public QObject 
{
public:

  explicit LCQLocalSourceHiden();
  virtual ~LCQLocalSourceHiden();

private:

  void connectReader(LCQLocalDataReader* _p_reader);
  void disconnectReader(LCQLocalDataReader* _p_reader);
  void read(LCQLocalDataReader* _p_reader);
  void write(const QByteArray& _data, LCQLocalDataWriter* _writer);

  virtual void customEvent(QEvent* _event) override;

  friend class LCQLocalDataReader;
  friend class LCQLocalDataWriter;
};

#endif //LCLOCALSOURCEHIDEN_H_
