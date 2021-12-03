#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QDataStream>
#include <QByteArray>
#include <QSet>
#include <QMap>

int main(int argc, char** argv)
{
  qDebug() << "Helow world.";
  QString path = "/home/serg/pprj/tnex/xmltestprj/test1/";
  /* QString path = "/home/serg/pprj/tnex"; */

  QDir::setCurrent(path);
  QDir dir("./");

  /* QDirIterator it(dir, QDirIterator::IteratorFlag::Subdirectories); */

  QDir::Filters dir_filter =  QDir::Filter::NoDotAndDotDot | 
    QDir::Filter::Files;
    /* QDir::Filter::NoDot */ 
    /* | QDir::Filter::NoDotDot; */
     /* QDir::Filter::Hidden */
    /* | QDir::Filter::NoSymLinks; */

  QDirIterator it(path, dir_filter, QDirIterator::IteratorFlag::Subdirectories);
  QFile prjfile("../prjfile.tnp");
  prjfile.open(QFile::OpenModeFlag::WriteOnly);
  QDataStream data_stream(&prjfile);

  data_stream << (quint32)(0xa0b0c0d0); //magic number
  data_stream << (quint32)(1); //project file version


  QSet<QString> setout;
  int compress_level = 3;
  while(it.hasNext())
  {
    QString file_name = dir.relativeFilePath(it.next());
    qDebug() << file_name; 
    QFile file(file_name);
    data_stream << qCompress(file_name.toUtf8(), compress_level);
    file.open(QFile::OpenModeFlag::ReadOnly);
    data_stream << qCompress(file.readAll(), compress_level);
    setout << file_name;
    /* data_stream << file.readAll(); */
  }
  prjfile.close();
  prjfile.open(QFile::OpenModeFlag::ReadOnly);
  data_stream.setDevice(&prjfile);



  qDebug() << "==================================read project";
  int magic_number;
  int project_file_version;

  data_stream >> magic_number;
  data_stream >> project_file_version;

  qDebug() << "Magic number = " << magic_number;
  qDebug() << "Project file version = " << project_file_version;

  QMap<QString, QByteArray> prj_files_map;

  QSet<QString> setin;
  while(!data_stream.atEnd())
  {
    QString file_name;
    QByteArray file_name_data;
    QByteArray file_data;
    data_stream >> file_name_data >> file_data;
    file_name = qUncompress(file_name_data);
    qDebug() << file_name;
    setin << file_name;
    prj_files_map[file_name] = qUncompress(file_data);
  }

  qDebug() << "Compare setout and setin : " << ((setout == setin) ? (true) : (false));

  qDebug() << "==================================project data";

  /* for(auto it = prj_files_map.begin(); it != prj_files_map.end(); it++) */
  /* { */
  /*   qDebug() << "File name: " << it.key(); */
  /*   qDebug() << "File data: " << it.value(); */
  /* } */


  /* qDebug() << dir.entryList(); */
  return 0;
}
