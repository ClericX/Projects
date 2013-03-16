/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

import static org.lwjgl.opengl.GL11.*;

import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.awt.image.DataBufferInt;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.Vector;
import javax.imageio.ImageIO;
import org.lwjgl.opengl.GL11;

public class Texture
{
	private static Vector<Texture> AllLoadedTextures = new Vector();
	private static int LastBoundTextureID = 0;
	
	public int ID;
	public int TextureWidth;
	public int TextureHeight;
	public int ImageWidth;
	public int ImageHeight;
	public BufferedImage ImageData;
	
	public void FreeGLTexture()
	{
		GL11.glDeleteTextures(ID);
		ID = -1;
	}
	
	public void Bind()
	{
		if (LastBoundTextureID != ID)
		{
			glBindTexture(GL_TEXTURE_2D, ID);
			LastBoundTextureID = ID;
		}
	}
	
	public void Unbind()
	{
		if (LastBoundTextureID != 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			LastBoundTextureID = 0;
		}
	}
	
	public static Texture Load(String path)
	{
		Texture ret = new Texture();
		
		ret.ID = glGenTextures();
		
		ret.Bind();
		
		if (glIsTexture(ret.ID) == false)
		{
			System.out.println("Failed to initialize an OpenGL texture! OpenGL did not return a valid Texture ID.");
		}
		
		try
		{
			ret.ImageData = ImageIO.read(new File(path));
		}
		catch (IOException e)
		{
			System.out.println("Failed to load texture: \"" + path + "\"");
			e.printStackTrace();
		}
		
		ret.ImageWidth = ret.ImageData.getWidth();
		ret.ImageHeight = ret.ImageData.getHeight();
		ret.TextureWidth = glGetTexLevelParameteri(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH);
		ret.TextureWidth = glGetTexLevelParameteri(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// If you do not want lighting and coloring to effect your texture and you would
		// like to display the texture unchanged when coloring is applied replace GL_MODULATE with GL_DECAL.
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		
		ByteBuffer databuf = null;
		
		// If there's no alpha raster, that means the image has no alpha channel so we just need a buffer with BGR info.
		if (ret.ImageData.getAlphaRaster() == null)
		{
			byte[] oldData = ((DataBufferByte)(ret.ImageData).getRaster().getDataBuffer()).getData();
			byte[] data = new byte[oldData.length];

			for (int i = 0; i < oldData.length / 3; i++)
			{
				int baseIndex = (i * 3);
				byte oldR = oldData[baseIndex];
				byte oldG = oldData[baseIndex+1];
				byte oldB = oldData[baseIndex+2];
				data[baseIndex] = oldB;
				data[baseIndex+1] = oldG;
				data[baseIndex+2] = oldR;
			}

			databuf = ByteBuffer.allocateDirect(data.length);
			databuf.put(data);
			databuf.flip();

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, ret.ImageWidth, ret.ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, databuf );
		}
		// If there is an alpha raster, that means we need to treat the image differently when we parse pixel data.
		else
		{
			byte[] oldData = ((DataBufferByte)(ret.ImageData).getAlphaRaster().getDataBuffer()).getData();
			byte[] data = new byte[oldData.length];

			for (int i = 0; i < oldData.length / 4; i++)
			{
				int baseIndex = (i * 4);
				byte oldA = oldData[baseIndex];
				byte oldR = oldData[baseIndex+1];
				byte oldG = oldData[baseIndex+2];
				byte oldB = oldData[baseIndex+3];
				data[baseIndex] = oldB;
				data[baseIndex+1] = oldG;
				data[baseIndex+2] = oldR;
				data[baseIndex+3] = oldA;
			}

			databuf = ByteBuffer.allocateDirect(data.length);
			databuf.put(data);
			databuf.flip();

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, ret.ImageWidth, ret.ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, databuf );
		}

		ret.Unbind();
		
		AllLoadedTextures.add(ret);
		
		return ret;
	}
}
