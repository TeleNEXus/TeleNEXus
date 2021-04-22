
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>

int main(int argc, char** argv)
{
  /* QCoreApplication app(argc, argv); */

  QTextStream outstream(stdout);
  QTextStream instream(stdin);
  QString instr;

  /* outstream << "==========================================" << endl; */
  /* outstream << "Input arguments"  << endl; */

  /* for(int i = 0; i < argc; i++) */
  /* { */
  /*   outstream << "Argument " << "i = " << QString(argv[i]) << endl; */
  /* } */

  /* outstream << "--------------------------" << endl; */
  /* outstream << "Please input data: "<< flush; */ 
  /* instream >> instr; */
  /* outstream << "Std input data = " << instr << endl; */




  /* qDebug() << "=========================================="; */
  /* qDebug() << "Input arguments"; */

  /* for(int i = 0; i < argc; i++) */
  /* { */
  /*   qDebug() << "Argument " << i << " = " << QString(argv[i]); */
  /* } */

  instream >> instr;

  /* qDebug() << "Input data = " << instr; */
  /* outstream << "Input data = " << instr; */
  outstream << "Interprocess input data: " << instr;
  return 0;

  /* return app.exec(); */
}
