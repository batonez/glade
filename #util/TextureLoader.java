package ru.hatchery.glade.util;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import de.matthiasmann.twl.utils.PNGDecoder;

import ru.hatchery.glade.exception.GladeInputStreamIsNullException;
import ru.hatchery.glade.render.Texture;

public class TextureLoader
{
	public static Texture loadTexture(InputStream in, int frameWidth, int frameHeight) throws IOException
	{
		if (in == null) {
			throw new GladeInputStreamIsNullException("Provided bitmap data stream is null, texture could not be loaded");
		}
		
		if (frameWidth < 0 || frameHeight < 0) {
			throw new RuntimeException("Provided frameWidth and frameHeight must not be less than zero");
		}
		
		PNGDecoder png = new PNGDecoder(in);
		Integer width = png.getWidth();
		Integer height = png.getHeight();
		ByteBuffer pixels = ByteBuffer.allocateDirect(width * height * 4);
		pixels.order(ByteOrder.nativeOrder());
		pixels.position(0);
		
		png.decode(pixels, width * 4, PNGDecoder.Format.RGBA);
		
		if (frameWidth == 0) {  // if the user doesn't know frame dimensions
			frameWidth = width; // we assume the whole texture is one frame
		} 
		
		if (frameHeight == 0) {
			frameHeight = height;
		}
		
		int numberOfFrames = width / frameWidth;
		numberOfFrames = numberOfFrames < 1 ? 1 : numberOfFrames;
		
		int numberOfAnimations = height / frameHeight;
		numberOfAnimations = numberOfAnimations < 1 ? 1 : numberOfAnimations;
		
		return new Texture(
			width,
			height,
			numberOfAnimations,
			numberOfFrames,
			pixels
		);
	}
	
	public static Texture loadTexture(InputStream bitmapData) throws IOException
	{
		return loadTexture(bitmapData, 0, 0);
	}
}
