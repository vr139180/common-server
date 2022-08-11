package com.cms.designer.util;

public class Scale
{
	public static double RATE = 0.75;
	public static final Scale[] DEFAULT_SET =
		{
			new Scale(2.0),
			new Scale(1.75),
			new Scale(1.5),
			new Scale(1.25),
			new Scale(1.0),
			new Scale(0.75),
			new Scale(0.5),
			new Scale(0.25)};

	private double value;

	public Scale(double value)
	{
		this.value = value;
	}

	public double getValue()
	{
		return value;
	}

	public String toString()
	{
		return (int) (value * 100) + "%";
	}

	public boolean equals(Object object)
	{
		if (object instanceof Scale)
		{
			return ((Scale)object).getValue() == value;
		}
		return false;
	}

	/**	Parse the given String to create a Scale object.  The given String can
		either be a percentage value such as 110%, or it can be a double such
		as 1.10.
		
		@param scale The scale String
		@return The Scale object
		@throws NumberFormatException
	*/

	public static Scale parseScale(String scale) throws NumberFormatException
	{
		scale = scale.trim();
		if (scale.endsWith("%"))
		{
			return new Scale(
				Double.parseDouble(scale.substring(0, scale.length() - 1))
					/ 100);
		}
		else
		{
			return new Scale(Double.parseDouble(scale));
		}
	}
}
