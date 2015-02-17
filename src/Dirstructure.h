//  Copyright (C) 2004-2014 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2013 Doug Ilijev <doug.ilijev@gmail.com>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef DIRSTRUCTURE_H
#define DIRSTRUCTURE_H

#include <wx/string.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>

/*! An object that represents the directory structure wxMaxima is installed in

  wxMaxima finds its data in different places on different operating systems:
 - wxStandardPaths::GetExecutablePath() on windows
 - PREFIX+"/share/wxMaxima/" on Linux
 - wxStandardPaths::GetExecutablePath()+"/wxMaxima.app/Contents/Resources" on mac.

   - on linux in 
*/
class Dirstructure
{
private:
  //! The directory all data is stored relative to.
  static wxString ResourcesDir();
  //! The directory the user stores its data in.
  static wxString UserConfDir();

public:
  //! The directory general data is stored in
#if defined __WXMSW__
  static wxString DataDir() {return ResourcesDir()+wxT("data/");}
#else
  static wxString DataDir() {return ResourcesDir();}
#endif

  //! The directory the help file is stored in
#if defined __WXMAC__
  static wxString HelpDir() {return ResourcesDir()+wxT("help/");}
#elif defined __WXMSW__
  static wxString HelpDir() {return ResourcesDir()+wxT("help/");}
#else
  static wxString HelpDir() {return Prefix()+wxT("/share/doc/wxmaxima/");}
#endif

  /*! The file private accellerator key information is stored in

    \todo Document this file in the texinfo manual
   */
#if defined __WXMSW__
  static wxString UserAutocompleteFile() {return UserConfDir()+wxT("wxmax.ac");}
#else
  static wxString UserAutocompleteFile() {return UserConfDir()+wxT(".wxmaxima.ac");}
#endif

  //! The path to wxMaxima's own AutoComplete file
  static wxString AutocompleteFile() {return DataDir() + wxT("autocomplete.txt");}
  
  //! The directory art is stored relative to
#if defined __WXMAC__
  static wxString ArtDir() {return ResourcesDir();}
#elif defined __WXMSW__
  static wxString ArtDir() {return ResourcesDir()+wxT("art/");}
#else
  static wxString ArtDir() {return ResourcesDir();}
#endif


  //! The directory config art is stored relative to
#if defined __WXMAC__
  static wxString ConfigArtDir() {return ArtDir()+wxT("config/");}
#elif defined __WXMSW__
  static wxString ConfigArtDir() {return ArtDir()+wxT("config/");}
#else
  static wxString ConfigArtDir() {return ResourcesDir();}
#endif

  //! The directory config art is stored relative to
#if defined __WXMAC__
  static wxString ConfigToolbarDir() {return ArtDir()+wxT("toolbar/");}
#elif defined __WXMSW__
  static wxString ConfigToolbarDir() {return ArtDir()+wxT("toolbar/");}
#else
  static wxString ConfigToolbarDir() {return ResourcesDir();}
#endif

  /*! The directory the locale data is to be found in

    Is only used on MSW and MAC
   */
  static wxString LocaleDir() {return ResourcesDir()+wxT("/locale");}
  
  //! The path maxima is found at by default.
#if defined __WXMAC__
  static wxString MaximaDefaultName() {return wxT("/Applications/Maxima.app");}
#elif defined __WXMSW__
  static wxString MaximaDefaultName() {return cwd.Replace(ResourcesDir()+wxT("wxMaxima"), wxT("\\bin\\maxima.bat"));}
#else
  static wxString MaximaDefaultName() {return wxT("maxima");}
#endif

  //! The path we pass to the operating system if we want it to locate maxima instead
  static wxString MaximaDefaultLocation();

  /*! The contents of the PREFIX macro as a wxString

    wxWidgets 3.0.2 refuses to directly concatenate two wxT-generated strings.
    To avoid triggering this bug we store the prefix here.
  */
  static wxString Prefix();
};

#endif // DIRSTRUCTURE_H