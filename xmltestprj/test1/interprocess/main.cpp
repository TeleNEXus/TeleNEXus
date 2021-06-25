#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <string>
#include <iostream>
#include <cstdio>
using namespace std;
int main(int argc, char** argv)
{
  /* QCoreApplication app(argc, argv); */

  
  QTextStream outstream(stdout);
  QTextStream instream(stdin);
  instream.setCodec("CP866");
  QString instr;
  QByteArray inarr;

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



  /* instream >> instr; */

  /* /1* qDebug() << "Input data = " << instr; *1/ */
  /* /1* outstream << "Input data = " << instr; *1/ */
  /* outstream << "Interprocess input data: data size = " << instr.length() << " data = " << instr; */
  /* string strin, strout; */


  /* cin >> ws >> strin; */
  /* cout << strin; */




//--------------------------------------------------------------
  /* QFile fin,fout; */

  /* fin.open(stdin,QFile::OpenModeFlag::ReadOnly); */
  /* fout.open(stdout,QFile::OpenModeFlag::WriteOnly); */

  /* /1* for(;;) *1/ */
  /* /1* { *1/ */
  /*   inarr = fin.read(5); */

  /*   instr = QString::fromLocal8Bit(inarr); */
  /*   instr = instr.toUpper(); */

  /*   fout.write(instr.toLocal8Bit()); */

  /*   inarr = fin.read(5); */

  /*   instr = QString::fromLocal8Bit(inarr); */
  /*   instr = instr.toUpper(); */

  /*   fout.write(instr.toLocal8Bit()); */
  /* /1* } *1/ */
//--------------------------------------------------------------

//--------------------------------------------------------------
  QFile fin,fout;

  fin.open(stdin,QFile::OpenModeFlag::ReadOnly);
  fout.open(stdout,QFile::OpenModeFlag::WriteOnly);


  for(int i = 0; i < 3; i++)
  {

    inarr = fin.read(5000000);
    /* fin.close(); */

    instr = QString::fromLocal8Bit(inarr);
    instr = instr.toUpper();

    fout.write(instr.toLocal8Bit());
    fout.flush();

    /* inarr = fin.read(5000000); */

    /* instr = QString::fromLocal8Bit(inarr); */
    /* instr = instr.toUpper(); */

    /* fout.write(instr.toLocal8Bit()); */
    /* fout.flush(); */

  }
//--------------------------------------------------------------


  /* fout.write(inarr); */
  

  /* QDataStream sin(&fin),sout(&fout); */
  /* sin >> instr; */
  /* sout << instr; */
  
  return 0;

  /* return app.exec(); */
}
