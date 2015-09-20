package ru.hatchery.glade.util;

public class Envutil
{
	private static final String keys [] = {
		"sun.arch.data.model",
		"com.ibm.vm.bitmode",
		"os.arch",
	};
	
	public static Boolean is64bit()
	{
		Boolean result = null;
		
		for (String key : keys ) {
			String property = System.getProperty(key);
			
			if (property != null) {
				result = (property.indexOf("64") >= 0);
			}
		}
		
		return result;
	}
}
