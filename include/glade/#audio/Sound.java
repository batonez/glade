package ru.hatchery.glade.audio;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.ShortBuffer;

import ru.hatchery.glade.exception.GladeInputStreamIsNullException;
import ru.hatchery.glade.exception.GladeRuntimeException;
import ru.hatchery.glade.model.Vector3f;

public class Sound
{
  public final Format    format;
  public final Integer   sampleRate;
  public ByteBuffer     data;
  
  public Integer   dataBufferId;
  
  public Integer     sourceId;
  public Vector3f   sourcePosition = new Vector3f();
  public Vector3f   sourceVelocity = new Vector3f();
  
  private Request request = Request.NONE; 
  
  public enum Format
  {
    MONO8, MONO16, STEREO8, STEREO16;
  }
  
  /**
   * Creates a WaveData container from the specified stream
   * 
   * @param ais AudioInputStream to read from
   * @return WaveData containing data, or null if a failure occured
   * @author LWJGL Community
   * @throws IOException 
   * @throws UnsupportedAudioFileException 
   * 
   * TODO IMPLEMENT Crossplatform
   */
  public static Sound create(InputStream is) throws IOException
  {
    if (is == null) {
      throw new GladeInputStreamIsNullException(); 
    }
    
    Sound sound = new Sound(Format.STEREO16, ByteBuffer.allocateDirect(0), 1);
    
    return sound;
    
    /*
    AudioInputStream ais = AudioSystem.getAudioInputStream(new BufferedInputStream(is));
    AudioFormat audioformat = ais.getFormat();

    // get channels
    Format format = null;
    if (audioformat.getChannels() == 1) {
      if (audioformat.getSampleSizeInBits() == 8) {
        format = Format.MONO8;
      } else if (audioformat.getSampleSizeInBits() == 16) {
        format = Format.MONO16;
      } else {
        assert false : "Illegal sample size";
      }
    } else if (audioformat.getChannels() == 2) {
      if (audioformat.getSampleSizeInBits() == 8) {
        format = Format.STEREO8;
      } else if (audioformat.getSampleSizeInBits() == 16) {
        format = Format.STEREO16;
      } else {
        assert false : "Illegal sample size";
      }
    } else {
      assert false : "Only mono or stereo is supported";
    }

    //read data into buffer
    ByteBuffer buffer = null;
    try {
      int available = ais.available();
      if(available <= 0) {
        available = ais.getFormat().getChannels() * (int) ais.getFrameLength() * ais.getFormat().getSampleSizeInBits() / 8;
      }
      byte[] buf = new byte[ais.available()];
      int read = 0, total = 0;
      while ((read = ais.read(buf, total, buf.length - total)) != -1
        && total < buf.length) {
        total += read;
      }
      buffer = convertAudioBytes(buf, audioformat.getSampleSizeInBits() == 16, audioformat.isBigEndian() ? ByteOrder.BIG_ENDIAN : ByteOrder.LITTLE_ENDIAN);
    } catch (IOException ioe) {
      return null;
    }

    //create our result
    Sound sound = new Sound(format, buffer, (int) audioformat.getSampleRate());

    //close stream
    try {
      ais.close();
    } catch (IOException ioe) {
      ioe.printStackTrace();
    }

    return sound;*/
  }
  
  public void erase()
  {
    data = null;
  }
  
  /**
   * @author LWJGL Community
   */
  private static ByteBuffer convertAudioBytes(byte[] audio_bytes, boolean two_bytes_data, ByteOrder order)
  {
    ByteBuffer dest = ByteBuffer.allocateDirect(audio_bytes.length);
    dest.order(ByteOrder.nativeOrder());
    ByteBuffer src = ByteBuffer.wrap(audio_bytes);
    src.order(order);
    if (two_bytes_data) {
      ShortBuffer dest_short = dest.asShortBuffer();
      ShortBuffer src_short = src.asShortBuffer();
      while (src_short.hasRemaining())
        dest_short.put(src_short.get());
    } else {
      while (src.hasRemaining())
        dest.put(src.get());
    }
    dest.rewind();
    return dest;
  }  
  
  private Sound(Format format, ByteBuffer data, int sampleRate)
  {
    if (sampleRate <= 0) {
      throw new GladeRuntimeException("Unexpected wave sample rate");
    }
    
    this.format = format;
    this.sampleRate = sampleRate;
    this.data = data;
  }
  
  public void play()
  {
    request = Request.PLAY;
  }
  
  public void stop()
  {
    request = Request.STOP;
  }
  
  public void pause()
  {
    request = Request.PAUSE;
  }
  
  public void resetRequest()
  {
    request = Request.NONE;
  }
  
  public boolean isPlayRequested()
  {
    return request == Request.PLAY;
  }
  
  public boolean isStopRequested()
  {
    return request == Request.STOP;
  }
  
  public boolean isPauseRequested()
  {
    return request == Request.PAUSE;
  }
  
  private enum Request
  {
    STOP, PLAY, PAUSE, NONE;
  }
}
