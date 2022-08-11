package com.cms.designer.util;

import java.util.ArrayList;

/**
 * @author Administrator
 * 常量定义
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: ConstValue.java,v 1.1 2004/06/30 06:36:46 jeffery Exp $
 */
public class ConstValue
{
	//文件菜单
	public final static String IFileOperator ="core.global.fileoperator";
	//new
	public final static String CoreFileNew ="core.operator.filenew";
	//open
	public final static String CoreFileOpen ="core.operator.fileopen";
	//close
	public final static String CoreFileClose ="core.operator.fileclose";
	//revert
	public final static String CoreFileRevert ="core.operator.filerevert";
	//save
	public final static String CoreFileSave ="core.operator.filesave";
	//saveas
	public final static String CoreFileSaveas ="core.operator.filesaveas";
	//print
	public final static String CoreFilePrint ="core.operator.fileprint";
	//quit
	public final static String CoreSysQuit ="core.operator.sysquit";
	
	//编辑菜单
	public final static String IEditOperator ="core.global.editoperator";
	//cut
	public final static String CoreEditCut ="core.operator.editcut";
	//copy
	public final static String CoreEditCopy ="core.operator.editcopy";
	//paste
	public final static String CoreEditPaste ="core.operator.editpaste";
	//redo
	public final static String CoreEditRedo ="core.operator.editredo";
	//undo
	public final static String CoreEditUndo ="core.operator.editundo";
	//select all
	public final static String CoreEditSelectAll ="core.operator.editselectall";
	//select none
	public final static String CoreEditSelectNone ="core.operator.editselectnone";

	//视图菜单
	public final static String IViewOperator ="core.global.viewoperator";
	public final static String CoreViewZoomin ="core.operator.zoomin";
	public final static String CoreViewZoomout ="core.operator.zoomout";

	public final static String CoreWinCascade ="core.window.cascade";
	public final static String CoreWinHorizontal ="core.window.horizontal";
	public final static String CoreWinVertical ="core.window.vertical";
	public final static String CoreWinClose ="core.window.close";
	
	public final static String CoreHelpTopic ="core.help.topic";
	public final static String CoreHelpAbout ="core.help.about";
	
	private static ArrayList SystemMenus =new ArrayList();
	static
	{
		SystemMenus.add( CoreFileNew);
		SystemMenus.add( CoreFileOpen);
		SystemMenus.add( CoreFileClose);
		SystemMenus.add( CoreFileRevert);
		SystemMenus.add( CoreFileSave);
		SystemMenus.add( CoreFileSaveas);
		SystemMenus.add( CoreFilePrint);
		
		SystemMenus.add( CoreEditCut);
		SystemMenus.add( CoreEditCopy);
		SystemMenus.add( CoreEditPaste);
		SystemMenus.add( CoreEditRedo);
		SystemMenus.add( CoreEditUndo);
		SystemMenus.add( CoreEditSelectAll);
		SystemMenus.add( CoreEditSelectNone);

		SystemMenus.add( CoreEditSelectNone);

		SystemMenus.add( CoreViewZoomin);
		SystemMenus.add( CoreViewZoomout);
	}
	private static ArrayList MenuType =new ArrayList();
	static
	{
		MenuType.add( IFileOperator);
		MenuType.add( IEditOperator);
		MenuType.add( IViewOperator);
	}
	
	public static boolean containSystemMenu( String id)
	{
		return SystemMenus.contains( id);
	}
	
	public static boolean containMenuType( String t)
	{
		return MenuType.contains( t);
	}
	
	//项目管理菜单
	public final static String ProjectSave ="core.project.save";
	public final static String ProjectNew ="core.project.new";
	public final static String ProjectImport ="core.project.import";
	public final static String ProjectExport ="core.project.export";
	public final static String ProjectDelete ="core.project.delete";
}
