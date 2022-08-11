package com.cms.core.util;

public class FontDescribe
{
	private String fontName = null;

	private String fontStyle = null;

	private String fontSize = null;

	private String foreColor = null;

	public FontDescribe( String fontName, String fontStyle, String fontSize, String foreColor)
	{
		this.fontName = fontName;
		this.fontStyle = fontStyle;
		this.fontSize = fontSize;
		this.foreColor = foreColor;
	}

	public String getFontName()
	{
		return fontName;
	}

	public void setFontName( String fontName)
	{
		this.fontName = fontName;
	}

	public String getFontStyle()
	{
		return fontStyle;
	}

	public void setFontStyle( String fontStyle)
	{
		this.fontStyle = fontStyle;
	}

	public String getFontSize()
	{
		return fontSize;
	}

	public void setFontSize( String fontSize)
	{
		this.fontSize = fontSize;
	}

	public String getForeColor()
	{
		return foreColor;
	}

	public void setForeColor( String foreColor)
	{
		this.foreColor = foreColor;
	}

	public String toString()
	{
		return "[" + fontName + "," + fontStyle + "," + fontSize + "," + foreColor + "," + "]";
	}
}
