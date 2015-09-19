#pragma once

class Material {
public:
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;
	
	Material(void) {
		ambient[0] = 0.9f; ambient[1] = 0.3f; ambient[2] =  0.1f; ambient[3] =  1.0f;
		diffuse[0] = 0.9f; diffuse[1] = 0.3f; diffuse[2] = 0.1f; diffuse[3] = 1.0f;
		specular[0] = 1.0f; specular[1] = 1.0f; specular[2] = 1.0f; specular[3] =  1.0f;
		ambient[0] = 0.9f; ambient[1] = 0.3f; ambient[2] = 0.1f; ambient[3] = 1.0f;
		shininess = 128.0f;
	}
};