#ifndef _CONTROLS_H_INCLUDED
#define _CONTROLS_H_INCLUDED

#include <windows.h>
#include <commctrl.h>


class SimpleControl
{
public:
    SimpleControl (HWND hwndParent, int id, BOOL initialState=TRUE)
        : _hWnd (GetDlgItem (hwndParent, id)),
					_id( id )
    {
        if (initialState == FALSE)
         Disable();
   }
   void Hide ()
   {
      ::ShowWindow(_hWnd, SW_HIDE);
   }
   void Show ()
   {
      ::ShowWindow(_hWnd, SW_SHOW);
   }
   BOOL IsVisible()
   {
      return(::IsWindowVisible(_hWnd ));
   }
   void SetFocus ()
   {
       ::SetFocus (_hWnd);
   }
   void Enable()
   {
       ::EnableWindow(_hWnd , TRUE);
   }
   void Disable()
   {
       ::EnableWindow(_hWnd , FALSE);
   }
	 HWND Hwnd () const { return _hWnd; }
	 void SetHwnd( HWND hwndParent )
	 {
		 _hWnd = GetDlgItem (hwndParent, _id);
	 }
	 void SetHwnd( HWND hwndParent, int id )
	 {
		 _id = id;
		 _hWnd = GetDlgItem (hwndParent, id);
	 }


protected:
    HWND _hWnd;
		int _id;
};


class Button : public SimpleControl
{
public:
   Button(HWND hwndParent, int id, BOOL initialState=TRUE)
       : SimpleControl(hwndParent, id, initialState)
   {}
   void SetName( char const * newName )
   {
      SendMessage(_hWnd, WM_SETTEXT, 0, (LPARAM)newName );
   }
	 void SetPushButton( int state )
	 {
      SendMessage(_hWnd, BM_SETSTATE, state, NULL );
	 }
};


class CheckBox : public Button
{
public:
   CheckBox (HWND hwndParent, int id, BOOL initialState=TRUE)
       : Button(hwndParent, id, initialState)
   {}
   BOOL IsChecked()
   {
      return( SendMessage(_hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED );
   }
   void Check()
   {
      SendMessage( _hWnd, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
   }
   void UnCheck()
   {
      SendMessage( _hWnd, BM_SETCHECK, (WPARAM) BST_UNCHECKED, 0);
   }
};


class RadioButton: public Button
{
public:
   RadioButton (HWND hwndParent, int id, BOOL initialState=TRUE)
       : Button(hwndParent, id, initialState)
   {}
   BOOL IsSelected()
   {
      return( SendMessage( _hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED );
   }
   void Select()
   {
      SendMessage( _hWnd, BM_SETCHECK, (WPARAM) BST_CHECKED, 0);
   }
};


class Edit: public SimpleControl
{
public:
   Edit (HWND hwndParent, int id, BOOL initialState=TRUE)
        :	SimpleControl (hwndParent, id, initialState)
   {}

   void SetString (char* buf)
   {
			SendMessage (_hWnd, WM_SETTEXT, 0, (LPARAM) buf);
   }

   // code is the HIWORD (wParam)
   static BOOL IsChanged (int code)
   {
       return code == EN_CHANGE;
   }

   int GetLength ()
   {
       return (int)(SendMessage (_hWnd, WM_GETTEXTLENGTH, 0, 0));
   }

   void GetString (char* buf, int len)
   {
       SendMessage (_hWnd, WM_GETTEXT, (WPARAM) len+1, (LPARAM) buf);
   }

   void Select ()
   {
       SendMessage (_hWnd, EM_SETSEL, 0, -1);
   }

   void ClearSelection ()
   {
       SendMessage (_hWnd, EM_SETSEL, -1, 0);
   }
};


class ListBox: public SimpleControl
{
public:
   ListBox (HWND hwndParent, int id, BOOL initialState=TRUE)
        :	SimpleControl (hwndParent, id, initialState)
		{}

		void ClearSelection ()
		{
			SendMessage (_hWnd, LB_RESETCONTENT, 0, 0);
		}

		void AddString ( char *str )
		{
			SendMessage (_hWnd, LB_ADDSTRING, 0, (LPARAM) str);
		}

		int GetSelection()
		{
			return SendMessage (_hWnd, LB_GETCURSEL, 0, 0);
		}

		int SetSelection( int num )
		{
			return SendMessage (_hWnd, LB_SETCURSEL, (WPARAM) num, 0 );
		}

		void SetWidth( int len )
		{
			SendMessage (_hWnd, LB_SETHORIZONTALEXTENT, (WPARAM) len, 0 );
		}
};


class ComboBox: public SimpleControl
{
public:
   ComboBox (HWND hwndParent, int id, BOOL initialState=TRUE)
        :	SimpleControl (hwndParent, id, initialState)
		{}

		void ClearSelection ()
		{
			SendMessage (_hWnd, CB_RESETCONTENT, 0, 0);
		}

		void AddString ( char *str )
		{
			SendMessage (_hWnd, CB_ADDSTRING, 0, (LPARAM) str);
		}

		int GetSelection()
		{
			return SendMessage (_hWnd, CB_GETCURSEL, 0, 0);
		}

		void SetSelection( int num )
		{
			SendMessage( _hWnd, CB_SETCURSEL, num, 0 );
		}
};


class TreeView: public SimpleControl
{
public:
   TreeView (HWND hwndParent, int id, BOOL initialState=TRUE)
        :	SimpleControl (hwndParent, id, initialState)
		{
			InitCommonControls();	    // make our tree control to work
		}


	HTREEITEM AddString ( char *str )
	{
		// add to tree
		tvinsert.hParent = NULL;			// top most level no need handle
		tvinsert.hInsertAfter = TVI_ROOT; // work as root level
		tvinsert.item.mask = TVIF_TEXT;
		tvinsert.item.pszText = str;

		last_sub = TVI_FIRST;
		last_root = (HTREEITEM) SendMessage (_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
		return last_root;
	}


	HTREEITEM AddStringSub ( char *str )
	{
		// add to tree
		tvinsert.hParent = last_root;
		tvinsert.hInsertAfter = last_sub;
		tvinsert.item.mask = TVIF_TEXT;
		tvinsert.item.pszText = str;

		last_sub = (HTREEITEM) SendMessage (_hWnd, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
		return last_sub;
	}


	void Clear()
	{
		SendMessage( _hWnd, TVM_DELETEITEM, 0, 0 );
	}


	void SetText( HTREEITEM item, char *str )
	{
		TVITEM tvi;

		memset(&tvi,0,sizeof(tvi)); // set all items to 0

		tvi.hItem = item;
		tvi.mask = TVIF_TEXT;
		tvi.pszText = str;

		//SendMessage( _hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item );
		SendMessage( _hWnd, TVM_SETITEM, 0, (WPARAM)&tvi );
	}


	void SetCheckbox( HTREEITEM item, int state )
	{
		TVITEM tvi;

		memset(&tvi,0,sizeof(tvi)); // set all items to 0

		// INDEXTOSTATEIMAGEMASK(2) is the checked state
		// INDEXTOSTATEIMAGEMASK(1) is the unchecked state
		// INDEXTOSTATEIMAGEMASK(0) removes the checkbox (no state)

		if( state == 0 )
			tvi.state = INDEXTOSTATEIMAGEMASK( 1 );
		if( state == 1 )
			tvi.state = INDEXTOSTATEIMAGEMASK( 2 );

		tvi.hItem = item;
		tvi.mask = TVIF_STATE;
		tvi.stateMask = TVIS_STATEIMAGEMASK;

		//SendMessage( _hWnd, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item );
		SendMessage( _hWnd, TVM_SETITEM, 0, (WPARAM)&tvi );
	}


	int GetCheckbox( HTREEITEM item )
	{
		TVITEM tvi;

		memset(&tvi,0,sizeof(tvi)); // set all items to 0
		tvi.hItem = item;
		tvi.mask = TVIF_STATE;

		SendMessage( _hWnd, TVM_GETITEM, 0, (WPARAM)&tvi );


		// INDEXTOSTATEIMAGEMASK(2) is the checked state
		// INDEXTOSTATEIMAGEMASK(1) is the unchecked state
		// INDEXTOSTATEIMAGEMASK(0) removes the checkbox (no state)

		if( tvi.state & INDEXTOSTATEIMAGEMASK(2) )
			return 1;
		if( tvi.state & INDEXTOSTATEIMAGEMASK(1) )
			return 0;

		return -1;
	}


protected:
	TV_INSERTSTRUCT tvinsert;   // struct to config out tree control
	
	HTREEITEM last_root;
	HTREEITEM last_sub;
};

#endif // _CONTROLS_H_INCLUDED
