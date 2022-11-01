package com.cms.designer.coremodule;

import java.util.Observable;

import com.cms.designer.util.ConstValue;

/**
 * @author Administrator
 * 文件菜单
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public abstract class IFileOperator extends Observable
{
	//菜单项状态
	private boolean m_new =false;
	private boolean m_open =false;
	private boolean m_close =false;

	private boolean m_revert =false;
	private boolean m_save =false;
	private boolean m_saveas =false;
	private boolean m_print =false;

	public IFileOperator(){
	}
	
	public void notifyChanged()
	{
		setChanged();
		notifyObservers( new MenuStatusEvent( ConstValue.IFileOperator, this));
	}

	/**
	 * @return
	 */
	public boolean isClose()
	{
		return m_close;
	}

	/**
	 * @param m_close
	 */
	public void setClose(boolean m_close)
	{
		this.m_close = m_close;
	}

	/**
	 * @return
	 */
	public boolean isNew()
	{
		return m_new;
	}

	/**
	 * @param m_new
	 */
	public void setNew(boolean m_new)
	{
		this.m_new = m_new;
	}

	/**
	 * @return
	 */
	public boolean isOpen()
	{
		return m_open;
	}

	/**
	 * @param m_open
	 */
	public void setOpen(boolean m_open)
	{
		this.m_open = m_open;
	}

	/**
	 * @return
	 */
	public boolean isPrint()
	{
		return m_print;
	}

	/**
	 * @param m_print
	 */
	public void setPrint(boolean m_print)
	{
		this.m_print = m_print;
	}

	/**
	 * @return
	 */
	public boolean isRevert()
	{
		return m_revert;
	}

	/**
	 * @param m_revert
	 */
	public void setRevert(boolean m_revert)
	{
		this.m_revert = m_revert;
	}

	/**
	 * @return
	 */
	public boolean isSave()
	{
		return m_save;
	}

	/**
	 * @param m_save
	 */
	public void setSave(boolean m_save)
	{
		this.m_save = m_save;
	}

	/**
	 * @return
	 */
	public boolean isSaveas()
	{
		return m_saveas;
	}

	/**
	 * @param m_saveas
	 */
	public void setSaveas(boolean m_saveas)
	{
		this.m_saveas = m_saveas;
	}

}
