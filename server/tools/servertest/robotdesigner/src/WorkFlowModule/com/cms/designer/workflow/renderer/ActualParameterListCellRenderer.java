package com.cms.designer.workflow.renderer;

import java.awt.Component;

import javax.swing.DefaultListCellRenderer;
import javax.swing.JList;

/**
 * @author Administrator
 *
 * To change this generated comment edit the template variable "typecomment":
 * Window>Preferences>Java>Templates.
 * To enable and disable the creation of type comments go to
 * Window>Preferences>Java>Code Generation.
 */
public class ActualParameterListCellRenderer 
		extends DefaultListCellRenderer{

    public Component getListCellRendererComponent(
         JList list,
         Object value,
         int index,
         boolean isSelected,
         boolean cellHasFocus)
     {
         super.getListCellRendererComponent(list, value, index, 
            isSelected, cellHasFocus);
            
         return this;
     }
}
