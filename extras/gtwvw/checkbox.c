/*
 * Video subsystem for Windows using GUI windows instead of Console
 * with multiple windows support
 *   Copyright 2004 Budyanto Dj. <budyanto@centrin.net.id>
 * gtwvw checkbox/progressbar functions
 * GTWVW is initially created based on:
 * =Id: gtwvt.c,v 1.60 2004-01-26 08:14:07 vouchcac Exp =
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option )
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.txt.   If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site https://www.gnu.org/ ).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.   To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "hbgtwvw.h"

/* wvw_cxCreate( [nWinNum], nTop, nLeft, nBottom, nRight, cText, cImage/nImage, bBlock, aOffset, ;
 *               nStretchBitmap, lMap3Dcolors)
 * create CHECKBOX for window nWinNum
 * nTop: row of top/left corner (in character unit)
 * nLeft: col of top/left corner (in character unit)
 * nBottom: row of bottom/right corner (in character unit) defaults==nTop
 * nRight: col of bottom/right corner (in character unit) defaults==??
 * cText: caption, default == ""
 *
 * cImage: bitmap file name, can be supplied as nImage: bitmap resource id
 * nStretchBitmap: a number between 0 and 1 (inclusive) as a factor to
 *                stretch the bitmap.
 *                1.0: bitmap covers the whole button
 *                0.5: bitmap covers 50% of button
 *                0: bitmap is not stretch
 *               (default is 1)
 * lMap3Dcolors: defaults to .F.
 *           if .T. the following color mapping will be performed:
 *              RGB( 192, 192, 192 ) --> COLOR_3DFACE   ("transparent")
 *              RGB( 128, 128, 128 ) --> COLOR_3DSHADOW
 *              RGB( 223, 223, 223 ) --> COLOR_3DLIGHT
 *           This might be desirable to have transparent effect.
 *           LIMITATION: this will work on 256 colored bitmaps only
 *
 * aOffset: array {y1,x1,y2,x2} of offsets to corner pixels, to adjust
 *         dimension of CHECKBOX.
 *         defaults for CHECKBOX: {-2,-2,+2,+2}
 *
 * bBlock:  codeblock to execute on every BN_CLICK event.
 *         This codeblock will be evaluated with these parameters:
 *         nWinNum: window number
 *         nCXid  : CHECKBOX id
 *
 * returns control id of newly created CHECKBOX of windows nWinNum
 * returns 0 if failed
 */

HB_FUNC( WVW_CXCREATE )
{
   if( HB_ISEVALITEM( 8 ) )
   {
      USHORT usTop    = ( USHORT ) hb_parni( 2 ),
             usLeft   = ( USHORT ) hb_parni( 3 ),
             usBottom = ( USHORT ) hb_parni( 4 ),
             usRight  = ( USHORT ) hb_parni( 5 );

      LPCTSTR      lpszCaption  = hb_parc( 6 );
      const char * szBitmap     = hb_parc( 7 );
      HB_UINT      uiBitmap     = ( HB_UINT ) hb_parnl( 7 );
      double       dStretch     = HB_ISNUM( 10 ) ? hb_parnd( 10 ) : 1;
      HB_BOOL      bMap3Dcolors = hb_parl( 11 );

      int iOffTop    = HB_ISARRAY( 9 ) ? hb_parvni( 9, 1 ) : -2;
      int iOffLeft   = HB_ISARRAY( 9 ) ? hb_parvni( 9, 2 ) : -2;
      int iOffBottom = HB_ISARRAY( 9 ) ? hb_parvni( 9, 3 ) : 2;
      int iOffRight  = HB_ISARRAY( 9 ) ? hb_parvni( 9, 4 ) : 2;

      hb_retnl( hb_gt_wvw_ButtonCreate( WVW_WHICH_WINDOW, usTop, usLeft, usBottom, usRight, lpszCaption,
                                        szBitmap, uiBitmap, hb_param( 8, HB_IT_EVALITEM ),
                                        iOffTop, iOffLeft, iOffBottom, iOffRight,
                                        dStretch, bMap3Dcolors,
                                        BS_AUTOCHECKBOX ) );
   }
   else
      hb_retnl( 0 );
}

/* wvw_cxDestroy( [nWinNum], nCXid )
 * destroy checkbox nCXid for window nWinNum
 */
HB_FUNC( WVW_CXDESTROY )
{
   WVW_WIN *  wvw_win = hb_gt_wvw_GetWindowsData( WVW_WHICH_WINDOW );
   HB_UINT    nCtrlId = ( HB_UINT ) hb_parnl( 2 );
   WVW_CTRL * pcd     = wvw_win->pcdList;
   WVW_CTRL * pcdPrev = NULL;

   while( pcd )
   {
      if( pcd->nClass == WVW_CONTROL_CHECKBOX && pcd->nId == nCtrlId )
         break;
      pcdPrev = pcd;
      pcd     = pcd->pNext;
   }

   if( pcd )
   {
      DestroyWindow( pcd->hWnd );

      if( pcdPrev )
         pcdPrev->pNext = pcd->pNext;
      else
         wvw_win->pcdList = pcd->pNext;

      if( pcd->pBlock )
         hb_itemRelease( pcd->pBlock );

      hb_xfree( pcd );
   }
}

/* wvw_cxSetFocus( [nWinNum], nButtonId )
 * set the focus to checkbox nButtonId in window nWinNum
 */
HB_FUNC( WVW_CXSETFOCUS )
{
   HWND hWnd = hb_gt_wvw_FindControlHandle( WVW_WHICH_WINDOW, WVW_CONTROL_CHECKBOX, ( HB_UINT ) hb_parnl( 2 ), NULL );

   hb_retl( hWnd && SetFocus( hWnd ) != NULL );
}

/* wvw_cxEnable( [nWinNum], nButtonId, [lToggle] )
 * enable/disable checkbox nButtonId on window nWinNum
 * (lToggle defaults to .T., ie. enabling the checkbox)
 * return previous state of the checkbox (TRUE:enabled FALSE:disabled)
 * (if nButtonId is invalid, this function returns FALSE too)
 */
HB_FUNC( WVW_CXENABLE )
{
   HB_UINT nWin = WVW_WHICH_WINDOW;
   HWND    hWnd = hb_gt_wvw_FindControlHandle( nWin, WVW_CONTROL_CHECKBOX, ( HB_UINT ) hb_parnl( 2 ), NULL );

   if( hWnd )
   {
      HB_BOOL bEnable = hb_parldef( 3, HB_TRUE );

      hb_retl( EnableWindow( hWnd, ( BOOL ) bEnable ) == 0 );

      if( ! bEnable )
      {
         WVW_WIN * wvw_win = hb_gt_wvw_GetWindowsData( nWin );
         SetFocus( wvw_win->hWnd );
      }
   }
   else
      hb_retl( HB_FALSE );
}

/* wvw_cxSetCodeblock( [nWinNum], nCXid, bBlock )
 * assign (new) codeblock bBlock to button nCXid for window nWinNum
 *
 * return .T. if successful
 */
HB_FUNC( WVW_CXSETCODEBLOCK )
{
   WVW_GLOB * wvw         = hb_gt_wvw_GetWvwData();
   WVW_CTRL * pcd         = hb_gt_wvw_GetControlData( WVW_WHICH_WINDOW, WVW_CONTROL_CHECKBOX, NULL, ( HB_UINT ) hb_parnl( 2 ) );
   PHB_ITEM   pBlock      = hb_param( 3, HB_IT_EVALITEM );
   HB_BOOL    fOldSetting = wvw->fRecurseCBlock;

   if( pBlock && pcd && ! pcd->fBusy )
   {
      wvw->fRecurseCBlock = HB_FALSE;
      pcd->fBusy = HB_TRUE;

      if( pcd->pBlock )
         hb_itemRelease( pcd->pBlock );

      pcd->pBlock = hb_itemNew( pBlock );

      pcd->fBusy = HB_FALSE;
      wvw->fRecurseCBlock = fOldSetting;

      hb_retl( HB_TRUE );
   }
   else
      hb_retl( HB_FALSE );
}

/* wvw_cxSetCheck( [nWinNum], nCXid, nCheckState )
 * assigns check-state of checkbox nCXid
 *           0==unchecked     BST_UNCHECKED
 *           1==checked       BST_CHECKED
 *           2==indeterminate BST_INDETERMINATE
 * this function always returns .T.
 */
HB_FUNC( WVW_CXSETCHECK )
{
   WVW_CTRL * pcd = hb_gt_wvw_GetControlData( WVW_WHICH_WINDOW, WVW_CONTROL_CHECKBOX, NULL, ( HB_UINT ) hb_parnl( 2 ) );

   if( pcd->hWnd )
      SendMessage( pcd->hWnd, BM_SETCHECK, ( WPARAM ) ( ULONG ) hb_parnidef( 3, BST_CHECKED ), 0 );

   hb_retl( HB_TRUE );
}

/* wvw_cxGetCheck( [nWinNum], nCXid )
 * returns check-state of checkbox nCXid
 *           0==unchecked     BST_UNCHECKED
 *           1==checked       BST_CHECKED
 *           2==indeterminate BST_INDETERMINATE
 */
HB_FUNC( WVW_CXGETCHECK )
{
   ULONG      ulCheck;
   WVW_CTRL * pcd = hb_gt_wvw_GetControlData( WVW_WHICH_WINDOW, WVW_CONTROL_CHECKBOX, NULL, ( HB_UINT ) hb_parnl( 2 ) );

   if( pcd->hWnd )
      ulCheck = ( int ) SendMessage( pcd->hWnd, BM_GETCHECK, 0, 0 );
   else
      ulCheck = 0;

   hb_retnl( ulCheck );
}

/* wvw_cxSetFont( [nWinNum], cFontFace, nHeight, nWidth, nWeight, nQUality, lItalic, lUnderline, lStrikeout ) */
HB_FUNC( WVW_CXSETFONT )
{
   WVW_GLOB * wvw     = hb_gt_wvw_GetWvwData();
   WVW_WIN *  wvw_win = hb_gt_wvw_GetWindowsData( WVW_WHICH_WINDOW );

   HB_BOOL fResult = HB_TRUE;

   wvw->lfCX.lfHeight         = hb_parnldef( 3, wvw_win->fontHeight - 2 );
   wvw->lfCX.lfWidth          = hb_parnldef( 4, wvw->lfCX.lfWidth );
   wvw->lfCX.lfEscapement     = 0;
   wvw->lfCX.lfOrientation    = 0;
   wvw->lfCX.lfWeight         = hb_parnldef( 5, wvw->lfCX.lfWeight );
   wvw->lfCX.lfQuality        = ( BYTE ) hb_parnidef( 6, wvw->lfCX.lfQuality );
   wvw->lfCX.lfItalic         = ( BYTE ) hb_parldef( 7, wvw->lfCX.lfItalic );
   wvw->lfCX.lfUnderline      = ( BYTE ) hb_parldef( 8, wvw->lfCX.lfUnderline );
   wvw->lfCX.lfStrikeOut      = ( BYTE ) hb_parldef( 9, wvw->lfCX.lfStrikeOut );
   wvw->lfCX.lfCharSet        = DEFAULT_CHARSET;
   wvw->lfCX.lfPitchAndFamily = FF_DONTCARE;

   if( HB_ISCHAR( 2 ) )
      hb_strncpy( wvw->lfCX.lfFaceName, hb_parc( 2 ), sizeof( wvw->lfCX.lfFaceName ) - 1 );

   if( wvw_win->hCXfont )
   {
      HFONT hOldFont = wvw_win->hCXfont;
      HFONT hFont    = CreateFontIndirect( &wvw->lfCX );
      if( hFont )
      {
#if 0
         WVW_CTRL * pcd = wvw_win->pcdList;

         while( pcd )
         {
            if( pcd->nClass == WVW_CONTROL_PUSHBUTTON &&
                ( HFONT ) SendMessage( pcd->hWnd, WM_GETFONT, 0, 0 ) == hOldFont )
               SendMessage( pcd->hWnd, WM_SETFONT, ( WPARAM ) hFont, ( LPARAM ) TRUE );

            pcd = pcd->pNext;
         }
#endif
         wvw_win->hCXfont = hFont;
         DeleteObject( hOldFont );
      }
      else
         fResult = HB_FALSE;
   }

   hb_retl( fResult );
}

HB_FUNC( WVW_CXSTATUSFONT )
{
   HB_UINT    nWin    = WVW_WHICH_WINDOW;
   WVW_WIN *  wvw_win = hb_gt_wvw_GetWindowsData( nWin );
   WVW_CTRL * pcd     = hb_gt_wvw_GetControlData( nWin, WVW_CONTROL_PUSHBUTTON, NULL, ( HB_UINT ) hb_parnl( 2 ) );

   if( pcd->hWnd )
   {
      if( hb_parldef( 3, HB_TRUE ) /* lFocus */ )
         SendMessage( pcd->hWnd, WM_SETFONT, ( WPARAM ) wvw_win->hCXfont, ( LPARAM ) TRUE );
      else
         SendMessage( pcd->hWnd, WM_SETFONT, ( WPARAM ) wvw_win->hPBfont, ( LPARAM ) TRUE );
   }

   hb_retl( HB_TRUE );
}