// -*- mode: c++; c-file-style: "linux"; c-basic-offset: 2; indent-tabs-mode: nil -*-
//  Copyright (C) 2004-2015 Andrej Vodopivec <andrej.vodopivec@gmail.com>
//            (C) 2013 Doug Ilijev <doug.ilijev@gmail.com>
//            (C) 2014-2015 Gunter Königsmann <wxMaxima@physikbuch.de>
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

#ifndef WXMAXIMA_H
#define WXMAXIMA_H

#include "wxMaximaFrame.h"
#include "MathParser.h"

#include <wx/socket.h>
#include <wx/config.h>
#include <wx/process.h>
#include <wx/fdrepdlg.h>
#include <wx/regex.h>
#include <wx/html/htmlwin.h>
#include <wx/dnd.h>

#if defined (__WXMSW__)
 #include <wx/msw/helpchm.h>
#endif

#include <wx/html/helpctrl.h>

#define SOCKET_SIZE 1024
#define DOCUMENT_VERSION_MAJOR 1
#define DOCUMENT_VERSION_MINOR 2

class MyApp : public wxApp
{
public:
  virtual bool OnInit();
  wxLocale m_locale;
  void NewWindow(wxString file = wxEmptyString);
#if defined (__WXMAC__)
  wxWindowList topLevelWindows;
  void OnFileMenu(wxCommandEvent &ev);
  virtual void MacNewFile();
  virtual void MacOpenFile(const wxString& file);
#endif
};

DECLARE_APP(MyApp)

#ifndef __WXGTK__
class MyAboutDialog : public wxDialog
{
public:
  MyAboutDialog(wxWindow *parent, int id, const wxString title, wxString description);
  ~MyAboutDialog() {};
  void OnLinkClicked(wxHtmlLinkEvent& event);
  DECLARE_EVENT_TABLE()
};
#endif

/* The top-level window and the main application logic

 */
class wxMaxima : public wxMaximaFrame
{
public:
  
  //! An enum of individual IDs for all timers this class handles
  enum TimerIDs
  {
    KEYBOARD_INACTIVITY_TIMER_ID,
    AUTO_SAVE_TIMER_ID
  };

  /*! A timer that determines when to do the next autosave;

    The actual autosave is triggered if both this timer is expired and the keyboard
    has been inactive for >10s so the autosave won't cause the application to shortly
    stop responding due to saving the file while the user is typing a word.

    This timer is used in one-shot mode so in the unikely case that saving needs more
    time than this timer to expire the user still got a chance to do something against
    it between two expirys. 
   */
  wxTimer m_autoSaveTimer;
  //! Has m_autoSaveTimer expired since the last save?
  bool m_autoSaveIntervalExpired;
  //! Is triggered when a timer this class is responsible for requires
  void OnTimerEvent(wxTimerEvent& event);

  /*! The interval between auto-saves (in milliseconds). 

    Values <10000 mean: Auto-save is off.
  */
  long int m_autoSaveInterval;
  
  wxMaxima(wxWindow *parent, int id, const wxString title,
           const wxPoint pos, const wxSize size = wxDefaultSize);
  ~wxMaxima();
  void ShowTip(bool force);
  wxString GetHelpFile();
  void ShowMaximaHelp(wxString keyword = wxEmptyString);
  void ShowWxMaximaHelp();
  void InitSession();
  void SetOpenFile(wxString file)
  {
    m_openFile = file;
  }
  void StripComments(wxString& s);
  void SendMaxima(wxString s, bool history = false);
  void OpenFile(wxString file,
                wxString command = wxEmptyString); //!< Open a file
  bool DocumentSaved() { return m_fileSaved; }
  void LoadImage(wxString file) { m_console->OpenHCaret(file, GC_TYPE_IMAGE); }
private:
  //! Can we display the "ready" prompt right now?
  bool m_ready;
  /*! A human-readable presentation of eventual unmatched-parenthesis type errors

    If text doesn't contain any error this function returns wxEmptyString
   */
  wxString GetUnmatchedParenthesisState(wxString text);
protected:
  //! Called when the "Scroll to currently evaluated" button is pressed.
  void OnFollow(wxCommandEvent& event);
  void ShowCHMHelp(wxString helpfile,wxString keyword);
  /*! Launches the HTML help browser

    \param helpfile The name of the file the help browser has to be launched with
    \param otherhelpfile We offer help for maxima and wxMaxima in separate manuals.
                         This parameter contains the filename of the manual we aren't
                         using currently so the help browser can open a tab containing
                         this file.
  */
  void ShowHTMLHelp(wxString helpfile,wxString otherhelpfile,wxString keyword);
  void CheckForUpdates(bool reportUpToDate = false);
  void OnRecentDocument(wxCommandEvent& event);
  void OnIdle(wxIdleEvent& event);
  void MenuCommand(wxString cmd);                  //
  void FileMenu(wxCommandEvent& event);            //
  void MaximaMenu(wxCommandEvent& event);          //
  void AlgebraMenu(wxCommandEvent& event);         //
  void EquationsMenu(wxCommandEvent& event);       //
  void CalculusMenu(wxCommandEvent& event);        //!< event handling for menus
  void SimplifyMenu(wxCommandEvent& event);        //
  void PlotMenu(wxCommandEvent& event);            //
  void NumericalMenu(wxCommandEvent& event);       //
  void HelpMenu(wxCommandEvent& event);            //
  void EditMenu(wxCommandEvent& event);            //
  void Interrupt(wxCommandEvent& event);           //
  /* Make the menu item, toolbars and panes visible that should be visible right now.

     \todo Didn't update the stats pane. I assume this was a bug.
   */
  void UpdateMenus(wxUpdateUIEvent& event);
  void UpdateToolBar(wxUpdateUIEvent& event);      //
  void UpdateSlider(wxUpdateUIEvent& event);       //
  /*! Toggle the visibility of a pane
    \param event The event that triggered calling this function.
   */
  void ShowPane(wxCommandEvent& event);            
  void OnProcessEvent(wxProcessEvent& event);      //
  void PopupMenu(wxCommandEvent& event);           //
  void StatsMenu(wxCommandEvent& event);           //

  //! Is triggered when the "Find" button in the search dialog is pressed
  void OnFind(wxFindDialogEvent& event);
  //! Is triggered when the "Close" button in the search dialog is pressed
  void OnFindClose(wxFindDialogEvent& event);
  //! Is triggered when the "Replace" button in the search dialog is pressed
  void OnReplace(wxFindDialogEvent& event);
  //! Is triggered when the "Replace All" button in the search dialog is pressed
  void OnReplaceAll(wxFindDialogEvent& event);
  
  void SanitizeSocketBuffer(char *buffer, int length);  //!< fix early nulls
  void ServerEvent(wxSocketEvent& event);          //!< server event: maxima connection
  /*! Is triggered on Input or disconnect from maxima

    \todo An :lisp-quiet command causes the status line to be changed to "reading maxima
    output". But since there will be no prompt afterwards the status line won't change 
    back. This is for example triggered by opening a file. Is there a way to fix this?
   */
  void ClientEvent(wxSocketEvent& event);

  void ConsoleAppend(wxString s, int type);        //!< append maxima output to console
  void DoConsoleAppend(wxString s, int type,       //
                       bool newLine = true, bool bigSkip = true);
  void DoRawConsoleAppend(wxString s, int type);   //

  /*! Spawn the "configure" menu.

    \todo Inform maxima about the new default plot window size.
  */
  void EditInputMenu(wxCommandEvent& event);
  void EvaluateEvent(wxCommandEvent& event);       //
  void InsertMenu(wxCommandEvent& event);          //
  void PrintMenu(wxCommandEvent& event);
  void SliderEvent(wxScrollEvent& event);
  //! Issued on double click on a history item
  void HistoryDClick(wxCommandEvent& event);
  //! Issued on double click on a table of contents item
  void StructureDClick(wxCommandEvent& event);
  void OnInspectorEvent(wxCommandEvent& ev);
  void DumpProcessOutput();
  void TryEvaluateNextInQueue();
  void TryUpdateInspector();

  wxString ExtractFirstExpression(wxString entry);
  wxString GetDefaultEntry();
  bool StartServer();                              //!< starts the server
  bool StartMaxima();                              //!< starts maxima (uses getCommand)
  void CleanUp();                                  //!< shuts down server and client on exit
  void OnClose(wxCloseEvent& event);               //!< close wxMaxima window
  wxString GetCommand(bool params = true);         //!< returns the command to start maxima
                                                   //    (uses guessConfiguration)

  void ReadFirstPrompt();            //!< reads everything before first prompt
  // setsup m_pid
  void ReadPrompt();                 //!< reads prompts
  void ReadMath();                   //!< reads output other than prompts
  void ReadLispError();              //!< lisp errors (no prompt prefix/suffix)
  void ReadLoadSymbols();            //!< functions after load command
#ifndef __WXMSW__
  void ReadProcessOutput();          //!< reads output of maxima command
#endif
  bool SaveNecessary();              //!< Does this file contain anything worth saving?

  /*!
    This method is called once when maxima starts. It loads wxmathml.lisp
    and sets some option variables.

    \todo Set pngcairo to be the default terminal as soon as the mac platform 
    supports it.
 */
  void SetupVariables();
  void KillMaxima();                 //!< kills the maxima process
  void ResetTitle(bool saved);
  void FirstOutput(wxString s);

  // Opens a wxm file
  bool OpenWXMFile(wxString file, MathCtrl *document, bool clearDocument = true);
  //! Opens a wxmx file
  bool OpenWXMXFile(wxString file, MathCtrl *document, bool clearDocument = true);
  GroupCell* CreateTreeFromXMLNode(wxXmlNode *xmlcells, wxString wxmxfilename = wxEmptyString);
  GroupCell* CreateTreeFromWXMCode(wxArrayString *wxmLines);
  bool SaveFile(bool forceSave = false);
  int SaveDocumentP();
  void SetCWD(wxString file);

  wxSocketBase *m_client;
  wxSocketServer *m_server;
  bool m_isConnected;
  bool m_isRunning;
  bool m_first;
  long m_pid;
  wxProcess *m_process;
  wxInputStream *m_input;
  int m_port;
  wxString m_currentOutput;
  wxString m_promptSuffix;
  wxString m_promptPrefix;
  wxString m_firstPrompt;
  bool m_readingPrompt;
  bool m_dispReadOut;               //!< what is displayed in statusbar
  bool m_inLispMode;                //!< don't add ; in lisp mode
  wxString m_lastPrompt;
  wxString m_lastPath;
  MathParser m_MParser;
  wxPrintData* m_printData;
  bool m_closing;
  wxString m_openFile;
  wxString m_currentFile;
  bool m_fileSaved;
  bool m_variablesOK;
  wxString m_chmhelpFile;
  bool m_htmlHelpInitialized;
  wxString m_maximaVersion;
  wxString m_lispVersion;
#if defined (__WXMSW__)
  wxCHMHelpController m_chmhelpCtrl;
#endif
  wxHtmlHelpController m_htmlhelpCtrl;
  wxFindReplaceDialog *m_findDialog;
  wxFindReplaceData m_findData;
  wxRegEx m_funRegEx;
  wxRegEx m_varRegEx;
  wxRegEx m_blankStatementRegEx;
#if wxUSE_DRAG_AND_DROP
  friend class MyDropTarget;
#endif
  DECLARE_EVENT_TABLE()
};

#if wxUSE_DRAG_AND_DROP

class MyDropTarget : public wxFileDropTarget
{
public:
  MyDropTarget(wxMaxima * wxmax) { m_wxmax = wxmax; }
  bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& files);
private:
  wxMaxima *m_wxmax;
};

#endif

#endif // WXMAXIMA_H
