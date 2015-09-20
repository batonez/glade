package ru.hatchery.glade.ui.font;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;

import au.com.bytecode.opencsv.CSVReader;
import ru.hatchery.glade.model.UniqueArrayList;
import ru.hatchery.glade.model.Vector2i;
import ru.hatchery.glade.model.Vector3f;
import ru.hatchery.glade.model.Vector4f;
import ru.hatchery.glade.render.Drawable;
import ru.hatchery.glade.render.Texture;
import ru.hatchery.glade.render.meshes.Rectangle;
import ru.hatchery.glade.ui.Label;
import ru.hatchery.glade.util.TextureLoader;

public class BitmapFont
{
	private Texture atlas;
	private Integer cellWidth, cellHeight;
	private int firstGlyphAsciiCode;
	private int glyphHeight;
	private int[] glyphWidths;
	private int numberOfGlyphsInARow;
	
	// Desired result font height in screen coordinates
	private float fontSize;
	
	private BitmapFont(Texture atlas, int cellWidth, int cellHeight)
	{
		if (cellWidth == 0 || cellHeight == 0 || atlas == null || cellWidth > atlas.textureWidth || cellHeight > atlas.textureHeight) {
			throw new RuntimeException("Invalid values for BitmapFont constructor");
		}
		
		this.atlas = atlas;
		this.cellWidth = cellWidth;
		this.cellHeight = cellHeight;
		
		fontSize = 1;
		numberOfGlyphsInARow = atlas.textureWidth / cellWidth;
		glyphWidths = new int[numberOfGlyphsInARow * (atlas.textureHeight / cellHeight)];
		
		Arrays.fill(glyphWidths, cellWidth);
	}
	
	public static BitmapFont createFromCSV(InputStream atlasData, InputStream csvData)
	{
		if (atlasData == null || csvData == null) {
			throw new RuntimeException("Provided atlas data or CSV data is null");
		}
		
		BitmapFont font = null;
		
		CSVReader reader = new CSVReader(new InputStreamReader(csvData));
		String[] line;
		String paramName;
		
		try {
			line = reader.readNext();
			int declaredAtlasWidth = Integer.parseInt(line[1]);
			
			line = reader.readNext();
			int declaredAtlasHeight = Integer.parseInt(line[1]);
			
			line = reader.readNext();
			int cellWidth = Integer.parseInt(line[1]);
			line = reader.readNext();
			int cellHeight = Integer.parseInt(line[1]);
			
			Texture atlas = TextureLoader.loadTexture(atlasData, cellWidth, cellHeight);
			
			if (declaredAtlasWidth != atlas.textureWidth) {
				reader.close();
				throw new RuntimeException("CSV data doesn't match atlas data");
			}
			
			if (declaredAtlasHeight != atlas.textureHeight) {
				reader.close();
				throw new RuntimeException("CSV data doesn't match atlas data");
			}
			
			font = new BitmapFont(atlas, cellWidth, cellHeight);
			
			line = reader.readNext();
			font.setFirstGlyphAsciiCode((char)Integer.parseInt(line[1]));
			
			line = reader.readNext();
			line = reader.readNext();
			font.setGlyphHeight(Integer.parseInt(line[1]));
			line = reader.readNext();
			
			while ((line = reader.readNext()) != null) {
				paramName = line[0].toLowerCase();
				
				if (paramName.startsWith("char")) {
					if (paramName.endsWith("base width")) {
						font.setGlyphWidth(
							font.getGlyphIndexForAsciiCode((char)Integer.parseInt(paramName.split(" ")[1])),
							Integer.parseInt(line[1])
						);
					}
				}
			}
			
			reader.close();
		} catch (IOException e) {
			System.out.println("Error reading CSV data");
			e.printStackTrace();
		}
		
		return font;
	}
	
	public Integer getGlyphIndexForAsciiCode(char asciiCode)
	{
		int glyphIndex = asciiCode - firstGlyphAsciiCode;
		
		if (glyphIndex < 0 || glyphIndex >= glyphWidths.length) {
			glyphIndex = glyphWidths.length - 1;
		}
		
		return glyphIndex;
	}
	
	public Vector2i getGlyphPositionForIndex(int glyphIndex)
	{
		Vector2i position = new Vector2i();
		
		if (glyphIndex < 0 || glyphIndex >= glyphWidths.length) {
			glyphIndex = glyphWidths.length - 1;
		}
		
		position.x = glyphIndex % numberOfGlyphsInARow;
		position.y = glyphIndex / numberOfGlyphsInARow;
		
		return position;
	}
	
	public List<Drawable> getDrawablesForString(String string)
	{
		List<Drawable> text = new UniqueArrayList<Drawable>(); 
		char[] characters = string.toCharArray();
		int glyphIndex;
		Drawable rectangle;
		Vector2i glyphPosition;
		float nextOffsetX = 0;
		float halfStringWidth = getStringWidth(string);
		
		for (char asciiCode: characters) {
			glyphIndex = getGlyphIndexForAsciiCode(asciiCode);
			glyphPosition = getGlyphPositionForIndex(glyphIndex);
			
			rectangle = new Drawable(Rectangle.INSTANCE);
			rectangle.getTransform().getScale().x = ((float)getGlyphWidth(glyphIndex) / (float)glyphHeight) * fontSize;
			rectangle.getTransform().getScale().y = fontSize;
			
			nextOffsetX += rectangle.getTransform().getScale().x;
			rectangle.getTransform().getPosition().x = nextOffsetX - halfStringWidth;
			nextOffsetX += rectangle.getTransform().getScale().x;

			rectangle.setTexture(atlas);
			rectangle.getTextureTransform().setCurrentFrameNumber(glyphPosition.x);
			rectangle.getTextureTransform().setCurrentAnimationNumber(glyphPosition.y);
			rectangle.getTextureTransform().setCurrentFrameWidth(getGlyphWidth(glyphIndex));
			rectangle.getTextureTransform().setCurrentFrameHeight(glyphHeight);
			
			text.add(rectangle);
		}
		
		return text;
	}
	
	public float getStringWidth(String string)
	{
		float stringWidth = 0;
		
		if (string != null) {
			for (char asciiCode: string.toCharArray()) {
				stringWidth += ((float)getGlyphWidth(getGlyphIndexForAsciiCode(asciiCode))/(float)glyphHeight) * fontSize;
			}
		}
		
		return stringWidth;
	}
	
	public Label getWidgetForString(String string)
	{
		return getWidgetForString(string, new Vector3f());
	}
	
	public Label getWidgetForString(String string, Vector3f textColor)
	{
		Label label = new Label();
		
		if (string != null) {
			label.addDrawables(getDrawablesForString(string));
		}
		
		label.getTransform().setScaleTo(new Vector3f(getStringWidth(string), fontSize, 1));
		label.setTextColor(textColor);
				
		return label;
	}
		
	public Texture getAtlas()
	{
		return atlas;
	}
	
	public Integer getCellWidth()
	{
		return cellWidth;
	}
	
	public Integer getCellHeight()
	{
		return cellHeight;
	}
	
	public int getGlyphWidth(int glyphIndex)
	{
		return glyphWidths[glyphIndex];
	}
	
	public int getGlyphHeight()
	{
		return glyphHeight;
	}
	
	public Integer getNumberOfChars()
	{
		return glyphWidths.length;
	}
	
	public void setFontSize(float fontSize)
	{
		this.fontSize = fontSize;
	}
	
	public float getFontSize()
	{
		return fontSize;
	}
	
	private void setFirstGlyphAsciiCode(char firstGlyphAsciiCode)
	{
		this.firstGlyphAsciiCode = firstGlyphAsciiCode;
	}
	
	private void setGlyphHeight(int glyphHeight)
	{
		this.glyphHeight = glyphHeight;
	}
	
	private void setGlyphWidth(int glyphIndex, int width)
	{
		if (glyphIndex < 0 || glyphIndex >= glyphWidths.length) {
			return;
		}
		
		if (width > cellWidth) {
			width = cellWidth;
		}
		
		glyphWidths[glyphIndex] = width;
	}
}
