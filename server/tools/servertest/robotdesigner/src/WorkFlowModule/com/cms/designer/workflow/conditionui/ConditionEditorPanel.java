package com.cms.designer.workflow.conditionui;

import java.awt.Container;
import java.awt.event.ActionEvent;
import java.util.Iterator;

import javax.swing.AbstractAction;
import javax.swing.JCheckBox;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import com.cms.core.workflow.Transition;
import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.condition.Condition;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 * $Id: ConditionEditorPanel.java,v 1.7 2004/10/08 07:30:31 york Exp $
 * $Id: ConditionEditorPanel.java,v 1.7 2004/10/08 07:30:31 york Exp $
 */
public class ConditionEditorPanel extends JPanel
{
	private Container parent;
	//条件对象
	private Transition transition;
	private Condition condition;
	private WorkflowProcess wfprocess;

	private JTextArea	j_condition;
	
	//设置为缺省的条件
	private JCheckBox	defaultCondition =null;
	
	public ConditionEditorPanel( Container parent)
	{
		this.parent =parent;
		init();
	}
	
	public Condition getCondition()
	{
		condition.setText( j_condition.getText());

		if( defaultCondition.isSelected())
			condition.setIsdefault( true);
		else
			condition.setIsdefault( false);

		return condition;
	}
	
	public boolean save()
	{
		Condition cond =getCondition();
		transition.setCondition( cond);
		
		return true;
	}

	public void setCondition( Transition trans)
	{
		transition =trans;
		if( trans != null)
		{
			condition =trans.getCondition();
			if( condition.isIsdefault())
				defaultCondition.setSelected( true);
			else
				defaultCondition.setSelected( false);
			
			j_condition.setText( condition.getText());
		}
	}
	
	private void init()
	{
		setLayout( null);
		
		//是否是缺省的条件
		defaultCondition =new JCheckBox( new AbstractAction(){
			public void actionPerformed( ActionEvent e)
			{
				defaultChanged( true);
			}});
		defaultCondition.setText( "该条件设置为缺省的条件！");
		defaultCondition.setSelected( false);
		add( defaultCondition);
		defaultCondition.setBounds( 5, 3, 280, 23);

		j_condition =new JTextArea();
		j_condition.setAutoscrolls( true);
		JScrollPane panel = new JScrollPane( j_condition);
		add( panel);
		panel.setBounds( 5, 25, 280, 220);
	}
	
	private void defaultChanged( boolean validate)
	{
		if( validate)
		{
			if( defaultCondition.isSelected())
			{
				if( hasOtherDefault())
				{
					defaultCondition.setSelected( false);
					//提示
					JOptionPane.showMessageDialog( this, "在该条件的两个节点之间\r\n已经存在一个缺省转移！", "警告", JOptionPane.OK_OPTION);
					return;
				}
			}
		}

		if( defaultCondition.isSelected())
		{
			//查找是否已经有缺省的条件
			j_condition.setEnabled( false);
			j_condition.setText( "");
		}
		else
			j_condition.setEnabled( true);
	}
	
	/**
	 * @return Returns the wfprocess.
	 */
	public WorkflowProcess getWfprocess()
	{
		return wfprocess;
	}
	/**
	 * @param wfprocess The wfprocess to set.
	 */
	public void setWfprocess( WorkflowProcess wfprocess)
	{
		this.wfprocess = wfprocess;
	}
	
	private boolean hasOtherDefault()
	{
		boolean ret =false;
		
		String from =transition.getFrom();
		Iterator ff =wfprocess.getTransitions().iterator();
		while( ff.hasNext())
		{
			Transition ts =( Transition)ff.next();
			if( ts.getFrom().equals( from))
			{
				if( ts != this.transition && ts.getCondition().isIsdefault())
					return true;
			}
		}
		
		return ret;
	}
}
