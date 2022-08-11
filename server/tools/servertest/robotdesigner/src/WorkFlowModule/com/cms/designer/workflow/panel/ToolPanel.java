package com.cms.designer.workflow.panel;

import java.util.HashMap;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

import javax.swing.DefaultCellEditor;
import javax.swing.DefaultListSelectionModel;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableColumn;

import com.cms.core.workflow.WorkflowProcess;
import com.cms.core.workflow.activity.Tool;
import com.cms.core.workflow.activity.ToolSet;

import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;
import java.util.Map;

/**
 * @author Administrator
 * 
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates. To enable and disable the creation of type
 * comments go to Window>Preferences>Java>Code Generation.
 */
public class ToolPanel extends JPanel
{
	private WorkflowProcess process;
	private ToolSet toolSet;
	private Tool tool;
	
	//输入参数设置
	private List data = new ArrayList();
	private EditableTableModel callFunModel;
	private DefaultListSelectionModel callFunListModel;
	private JTable callFunTable;
	
	private JTextField pm_value;
	
	//返回值下拉矿
	private JComboBox classComboBox;
	
	public ToolPanel( WorkflowProcess process, ToolSet toolSet, Tool tool)
	{
		this.process = process;
		this.tool = tool;
		this.toolSet = toolSet;

		init();
		
		initValidateClass();
		
		//设置class是否可以编辑
		classComboBox.setEditable( false);
	}
	
	public Tool getTool()
	{
		return tool;
	}
	
	private Object findAppClass( String cl)
	{
		for( int i =0; i< classComboBox.getItemCount(); ++i)
		{
			AppSelector as =( AppSelector)classComboBox.getItemAt( i);
			if( as.getName().equals( cl))
				return as;
		}
		
		return null;
	}
	
	public boolean save()
	{
		AppSelector as =( AppSelector)classComboBox.getSelectedItem();
		if( as == null)
			return false;
		//设置输入参数
		Map mm =Tool.getParamsOfAction( as.getName());
		Map m1 =new HashMap();

		for( int i =0; i<data.size(); ++i)
		{
			ParameterSelector ps =( ParameterSelector)data.get( i);
			String app =ps.getPm_n();
			if( !mm.containsKey( app))
				continue;

			m1.put( app, ps.getPm_v());
		}

		tool.setName( as.getName());
		tool.getParams().clear();
		tool.getParams().putAll( m1);

		return true;
	}
	
	private void init()
	{
		//this.setPreferredSize( new Dimension( 350, 340));
		setLayout( null);

		JLabel toolTypeLabel = new JLabel( "actions");
		toolTypeLabel.setBounds( 5, 5, 60, 20);

		add( toolTypeLabel);
		
		List<String> configClass =Tool.getAllActions();
		
		classComboBox = new JComboBox( configClass.toArray());
		classComboBox.setBounds( 70, 5, 280, 20);

		add( classComboBox);
		classComboBox.addItemListener( new ItemListener()
		{
			public void itemStateChanged( ItemEvent arg0)
			{
				ClassChange();
			}
		});

		JLabel lb = new JLabel( "action param");
		lb.setBounds( 5, 30, 120, 20);
		add( lb);
		callFunModel = new EditableTableModel( new String[]{"name", "value"}, data);
		callFunListModel = new DefaultListSelectionModel();
		callFunListModel.setSelectionMode( DefaultListSelectionModel.SINGLE_SELECTION);
		callFunTable = new JTable( callFunModel);
		callFunTable.setSelectionModel( callFunListModel);
		callFunTable.setAutoscrolls( true);
		
		JScrollPane jScrollPane2 = new JScrollPane();
		jScrollPane2.setBounds( new Rectangle( 5, 55, 460, 330));

		jScrollPane2.getViewport().add( callFunTable, null);
		add( jScrollPane2, null);
		callFunListModel.addListSelectionListener( new ListSelectionListener()
		{
			public void valueChanged( ListSelectionEvent arg0)
			{
				callListChange( null);
			}
		});

		pm_value =new JTextField();
		TableColumn modeColumn = callFunTable.getColumn( "value");
		modeColumn.setCellEditor( new DefaultCellEditor( pm_value));
	}
	
	private void initValidateClass()
	{
		classComboBox.removeAllItems();
		
		Iterator ff =Tool.getAllActions().iterator();
		while( ff.hasNext())
		{
			AppSelector as =new AppSelector( (String)ff.next());
			classComboBox.addItem( as);
		}
		
		//设置初试信息
		String id =tool.getName();
		if( id == null || id.equals( ""))
			return;
		
		classComboBox.setSelectedItem( findAppClass( id));
		
		Map mm =tool.getParams();
		data.clear();
		
		ff =mm.keySet().iterator();
		while( ff.hasNext())
		{
			String n =(String)ff.next();
			String v =(String)mm.get( n);
			ParameterSelector ps =new ParameterSelector( n, v);
			data.add( ps);
		}
		
		callFunTable.updateUI();
	}
	
	private void callListChange( String type)
	{
		int sel = callFunTable.getSelectedRow();
		if( sel == -1)
			return;
	
		ParameterSelector ps =(ParameterSelector) data.get( sel);
		if( ps == null)
			return;
		
		pm_value.setText( ps.getPm_v());
	}

	private void ClassChange()
	{
		try
		{
			AppSelector cls =( AppSelector) classComboBox.getSelectedItem();
			if( cls == null)
				return;
			
			data.clear();

			Map ms =Tool.getParamsOfAction( cls.getName());
			
			Iterator ff =ms.keySet().iterator();
			while( ff.hasNext())
			{
				String id =( String)ff.next();
				String t =( String)ms.get( id);

				ParameterSelector param = new ParameterSelector( id, t);
				data.add( param);
			}
			
			callFunTable.updateUI();
		}
		catch( Exception ex)
		{
			ex.printStackTrace();
		}
	}
	
	class EditableTableModel extends AbstractTableModel
	{
		private String[] names;
		private List datas;

		public EditableTableModel( String[] names, List f)
		{
			this.names = names;
			this.datas = f;
		}
		public int getColumnCount()
		{
			return names.length;
		}
		public int getRowCount()
		{
			return datas.size();
		}
		public Object getValueAt( int row, int col)
		{
			ParameterSelector property = (ParameterSelector) datas.get( row);
			String s = "";
			if( col == 0)
			{
				s = property.getPm_n();
			}
			else if( col == 1)
			{
				s = property.getPm_v();
			}
			
			return s;
		}
		public String getColumnName( int column)
		{
			return names[column];
		}
		public boolean isCellEditable( int row, int col)
		{
			return col != 0;
		}
		public void setValueAt( Object aValue, int row, int column)
		{
			ParameterSelector property = (ParameterSelector) datas.get( row);
			if( column == 1)
			{
				property.setPm_v( pm_value.getText().trim());
			}
		}
	}

	public class ParameterSelector
	{
		private String pm_n ="";
		private String pm_v ="";

		public ParameterSelector( String n, String v)
		{
			pm_n =n;
			pm_v =v;
		}

		public String getPm_n() {
			return pm_n;
		}

		public void setPm_n(String pm_n) {
			this.pm_n = pm_n;
		}

		public String getPm_v() {
			return pm_v;
		}

		public void setPm_v(String pm_v) {
			this.pm_v = pm_v;
		}
		
	}

	class AppSelector
	{
		private String name;

		public AppSelector( String name)
		{
			this.name = name;
		}

		public String toString()
		{
			return name;
		}

		/**
		 * @return Returns the name.
		 */
		public String getName()
		{
			return name;
		}
	}
}