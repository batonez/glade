#pragma once

class Texture {
public:
    const unsigned int textureWidth;
    const unsigned int textureHeight;
    const unsigned char numberOfFrames;
    const unsigned char numberOfAnimations;
    const float frameWidth;
    const float frameHeight;
    const float texCoordFrameWidth;  // Ўирина кадра в координатах S
    const float texCoordFrameHeight; // ¬ысота кадра в координатах T
	
private:
	unsigned char* data;
	int bufferId;
	
public:
    Texture(int texWidth, int texHeight, int numOfAnimations, int numOfFrames, float* textureData):
		textureWidth(texWidth),
		textureHeight(texHeight),
    	numberOfFrames(numOfFrames),
    	numberOfAnimations(numOfAnimations),
    	frameWidth((float) textureWidth / (float) numberOfFrames),
    	frameHeight((float) textureHeight / (float) numberOfAnimations),
    	texCoordFrameWidth(1 / (float) numberOfFrames),
    	texCoordFrameHeight(1 / (float) numberOfAnimations)
	{
		if (texWidth <= 0 || texHeight <= 0 || numberOfAnimations <= 0 || numberOfFrames <= 0 || textureData == NULL) {
			// throw new RuntimeException("Incorrect parameters for texture constructorz method.");
		}
			
		unsigned int texSize = texWidth * texHeight;
		data = new unsigned char[texSize];
		memcpy(data, textureData, texSize * sizeof(float));
    }
    
	~Texture(void) {
		erase();
	}
	
    bool isTextureSizePowerOfTwo() {
    	return isPowerOfTwo(textureWidth) && isPowerOfTwo(textureHeight);
    }
    
    bool hasVideoBufferHandle() {
    	return bufferId != 0;
    }
    
    bool isErased() {
    	return data == NULL;
    }
    
    void erase() {
		if (data != NULL) {
			delete [] data;
			data = NULL;
		}
    }
    
    unsigned char* getData() {
    	return data;
    }
    
    void setVideoBufferHandle(int bufferId) {
		if (bufferId == 0) {
			// 0 используетс€ дл€ индикации отсутсви€ bufferId, нужно отловить эту ситуацию
		}
		
    	bufferId = bufferId;
    }
    
    int getVideoBufferHandle() {
    	return bufferId;
    }
    
private:
	bool isPowerOfTwo(int x) {
	    return (x <= 0) ? false : (x & (x - 1)) == 0;    
	}
};