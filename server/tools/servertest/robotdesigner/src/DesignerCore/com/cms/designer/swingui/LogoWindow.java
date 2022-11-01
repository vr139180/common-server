package com.cms.designer.swingui;

import java.awt.AlphaComposite;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.util.Calendar;

import javax.swing.BorderFactory;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;

import com.cms.designer.util.ResourceUtil;
import com.cms.designer.util.Utilities;

/**
 * @author Administrator
 *
 * To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Generation - Code and Comments
 */
public class LogoWindow extends JFrame
{
	//一个实例
	private static LogoWindow instance =null;

	//logo窗体的特征
	private final static int logoWidth = 432;
	private final static int logoHeight = 225;

	//显示的图片
	final Icon logoIcon = ResourceUtil.getIconManager().getIcon("splash.gif");
	ImageIcon imageIcon = (ImageIcon)logoIcon;
	final Image image = imageIcon.getImage();

	//显示logo图片的窗口
	JLabel lab = null; 

	/**
	 * 显示logo窗口
	 */
	public static synchronized void showLogoWindow()
	{
		if( instance == null)
			instance =new LogoWindow();

		instance.pack();
		Utilities.center( instance);
		instance.show();
	}
	
	/**
	 * 撤销logo窗口
	 */
	public static synchronized void disposeLogoWindow()
	{
		if( instance != null)
		{
			instance.dispose();
			instance =null;
		}
	}
	
	/* (non-Javadoc)
	 * @see javax.swing.JFrame#frameInit()
	 */
	protected void frameInit()
	{
		super.frameInit();
		this.setDefaultCloseOperation( DO_NOTHING_ON_CLOSE);
		this.setMenuBar( null);
		this.setResizable( false);

		JLabel info = new JLabel("Initializing....", JLabel.CENTER);
		info.setForeground(Color.black);
		
		lab =new JLabel()
			{
				public void paint(Graphics g)
				{
					super.paint(g);

					Graphics2D g2 = (Graphics2D)g;
					g2.setRenderingHint(
						RenderingHints.KEY_ANTIALIASING,
						RenderingHints.VALUE_ANTIALIAS_ON);

					g2.setFont(new Font("Arial", Font.BOLD, 27));
					g2.setColor(Color.WHITE);
					Composite originalComposite = g2.getComposite();
					g2.setComposite(
						AlphaComposite.getInstance(
							AlphaComposite.SRC_OVER,
							0.5f));
					g2.drawString("WorkFlow Designer", 12, 32);

					g2.setFont(new Font("Arial", Font.BOLD, 27));
					g2.setColor(Color.BLACK);
					g2.setComposite(originalComposite);
					g2.drawString("WorkFlow Designer", 10, 30);

					g2.setFont(new Font("Arial", Font.BOLD, 12));
					g2.drawString("v 1.0", 25, 50);
					g2.setFont(new Font("Arial", Font.BOLD, 10));
					g2.drawString(
						"(c) "+ Calendar.getInstance().get(Calendar.YEAR)
							+ "THE EXCENON PROJECT", 10, 70);

					int imageWidth = image.getWidth(this);
					int imageHeight = image.getHeight(this);
					g2.drawImage(image, 0, 0, logoWidth, logoHeight, this);
					g2.drawImage(image, 0, 0, imageWidth, imageHeight, this);
				}
			};

		getContentPane().add(lab, BorderLayout.CENTER);
		
		lab.setLayout(new BorderLayout());
		lab.setPreferredSize(new Dimension(logoWidth, logoHeight));
		lab.add(info, BorderLayout.SOUTH);
		lab.setBorder(BorderFactory.createRaisedBevelBorder());
		
		info.setPreferredSize(new Dimension(lab.getWidth(), 24));

		//GPGraphpad.init();
		info.setText("Starting...");
	}

}
