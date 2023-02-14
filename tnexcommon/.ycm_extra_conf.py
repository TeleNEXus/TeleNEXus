import os
import platform
import os.path as p
import subprocess

DIR_OF_THIS_SCRIPT = p.abspath( p.dirname( __file__ ) )
DIR_OF_THIRD_PARTY = p.join( DIR_OF_THIS_SCRIPT, 'third_party' )
DIR_OF_WATCHDOG_DEPS = p.join( DIR_OF_THIRD_PARTY, 'watchdog_deps' )
SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

database = None

flags = [
             '-x',
             'c++',
             '-pipe',
             '-O2',
             '-std=gnu++11',
             '-Wall',
             '-W',
             '-D_REENTRANT',
             '-fPIC',
             '-DQT_DEPRECATED_WARNINGS',
             '-DQT_NO_DEBUG',
             '-DQT_WIDGETS_LIB',
             '-DQT_GUI_LIB',
             '-DQT_NETWORK_LIB',
             '-DQT_XML_LIB',
             '-DQT_CORE_LIB',
             '-I', '../tnexcommon',
             '-I', '.',
             '-I', '../tnexcommon/interfaces',
             '-I', '../tnexcommon/interfaces/remotedata',
             '-I', '../tnexcommon/interfaces/xmlbuilders',
             '-I', '../tnexcommon',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtWidgets',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtGui',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtNetwork',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtXml',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtCore',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtSerialBus',
             '-isystem',
             '/usr/include/x86_64-linux-gnu/qt5/QtSerialPort',
             '-I', '.',
             '-isystem',
             '/usr/include/libdrm',
             '-I', '/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++',
             '-isystem',
             '/usr/include/c++/7'

]

compilation_database_folder = ''


def IsHeaderFile( filename ):
  extension = p.splitext( filename )[ 1 ]
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ]


def FindCorrespondingSourceFile( filename ):
  if IsHeaderFile( filename ):
    basename = p.splitext( filename )[ 0 ]
    for extension in SOURCE_EXTENSIONS:
      replacement_file = basename + extension
      if p.exists( replacement_file ):
        return replacement_file
  return filename


def PathToPythonUsedDuringBuild():
  try:
    filepath = p.join( DIR_OF_THIS_SCRIPT, 'PYTHON_USED_DURING_BUILDING' )
    with open( filepath ) as f:
      return f.read().strip()
  except OSError:
    return None

def Settings( **kwargs ):
  import ycm_core

  global database
  if database is None and p.exists( compilation_database_folder ):
    database = ycm_core.CompilationDatabase( compilation_database_folder )

  language = kwargs[ 'language' ]

  if language == 'cfamily':
    filename = FindCorrespondingSourceFile( kwargs[ 'filename' ] )

    if not database:
      return {
        'flags': flags,
        'include_paths_relative_to_dir': DIR_OF_THIS_SCRIPT,
        'override_filename': filename
      }

    compilation_info = database.GetCompilationInfoForFile( filename )
    if not compilation_info.compiler_flags_:
      return {}

    final_flags = list( compilation_info.compiler_flags_ )

    try:
      final_flags.remove( '-stdlib=libc++' )
    except ValueError:
      pass

    return {
      'flags': final_flags,
      'include_paths_relative_to_dir': compilation_info.compiler_working_dir_,
      'override_filename': filename
    }

  if language == 'python':
    return {
      'interpreter_path': PathToPythonUsedDuringBuild()
    }

  return {}


def PythonSysPath( **kwargs ):
  sys_path = kwargs[ 'sys_path' ]

  interpreter_path = kwargs[ 'interpreter_path' ]
  major_version = subprocess.check_output( [
    interpreter_path, '-c', 'import sys; print( sys.version_info[ 0 ] )' ]
  ).rstrip().decode( 'utf8' )

  sys_path[ 0:0 ] = [ p.join( DIR_OF_THIS_SCRIPT ),
                      p.join( DIR_OF_THIRD_PARTY, 'bottle' ),
                      p.join( DIR_OF_THIRD_PARTY, 'cregex',
                              'regex_{}'.format( major_version ) ),
                      p.join( DIR_OF_THIRD_PARTY, 'frozendict' ),
                      p.join( DIR_OF_THIRD_PARTY, 'jedi_deps', 'jedi' ),
                      p.join( DIR_OF_THIRD_PARTY, 'jedi_deps', 'parso' ),
                      p.join( DIR_OF_THIRD_PARTY, 'requests_deps', 'requests' ),
                      p.join( DIR_OF_THIRD_PARTY, 'requests_deps',
                                                  'urllib3',
                                                  'src' ),
                      p.join( DIR_OF_THIRD_PARTY, 'requests_deps',
                                                  'chardet' ),
                      p.join( DIR_OF_THIRD_PARTY, 'requests_deps',
                                                  'certifi' ),
                      p.join( DIR_OF_THIRD_PARTY, 'requests_deps',
                                                  'idna' ),
                      p.join( DIR_OF_WATCHDOG_DEPS, 'watchdog', 'build', 'lib3' ),
                      p.join( DIR_OF_WATCHDOG_DEPS, 'pathtools' ),
                      p.join( DIR_OF_THIRD_PARTY, 'waitress' ) ]

  sys_path.append( p.join( DIR_OF_THIRD_PARTY, 'jedi_deps', 'numpydoc' ) )
  return sys_path
