#include "sphere.hpp"

// Creates a VAO containing a sphere with a resolution specified by slices and layers, with a radius of 1.

unsigned int createCircleVAO(unsigned int slices, unsigned int layers,float R, float G, float B) {

	// Calculating how large our buffers have to be
	// The sphere is defined as layers containing rectangles. Each rectangle requires us to draw two triangles
	const unsigned int PRIMITIVES_PER_RECTANGLE = 2;
	const unsigned int triangleCount = slices * layers * PRIMITIVES_PER_RECTANGLE;

	// Allocating buffers
	const unsigned int VERTICES_PER_TRIANGLE = 3;
	const unsigned int COMPONENTS_PER_VERTEX = 3;
	float* vertices = new float[triangleCount * VERTICES_PER_TRIANGLE * COMPONENTS_PER_VERTEX];
	unsigned int* indices = new unsigned int[triangleCount * VERTICES_PER_TRIANGLE];

	// Slices require us to define a full revolution worth of triangles.
	// Layers only requires angle varying between the bottom and the top (a layer only covers half a circle worth of angles)
	const float degreesPerLayer = 180.0 / (float) layers;
	const float degreesPerSlice = 360.0 / (float) slices;

	// Keeping track of the triangle index in the buffer. 
	// This implementation is fairly naive in the sense that it does not reuse vertices with the index buffer.
	int i = 0;

	// Constructing the sphere one layer at a time
	for (int layer = 0; layer < layers; layer++) {
		int nextLayer = layer + 1;

		// Angles between the vector pointing to any point on a particular layer and the negative z-axis
		float currentAngleZDegrees = degreesPerLayer * layer;
		float nextAngleZDegrees = degreesPerLayer * nextLayer;

		// All coordinates within a single layer share z-coordinates. 
		// So we can calculate those of the current and subsequent layer here.
		float currentZ = -cos(toRadians(currentAngleZDegrees));
		float nextZ = -cos(toRadians(nextAngleZDegrees));

		// The row of vertices forms a circle around the vertical diagonal (z-axis) of the sphere.
		// These radii are also constant for an entire layer, so we can precalculate them.
		float radius = sin(toRadians(currentAngleZDegrees));
		float nextRadius = sin(toRadians(nextAngleZDegrees));

		// Now we can move on to constructing individual slices within a layer
		for (int slice = 0; slice < slices; slice++) {
			
			// The direction of the start and the end of the slice in the xy-plane
			float currentSliceAngleDegrees = slice * degreesPerSlice;
			float nextSliceAngleDegrees = (slice + 1) * degreesPerSlice;

			// Determining the direction vector for both the start and end of the slice
			float currentDirectionX = cos(toRadians(currentSliceAngleDegrees));
			float currentDirectionY = sin(toRadians(currentSliceAngleDegrees));

			float nextDirectionX = cos(toRadians(nextSliceAngleDegrees));
			float nextDirectionY = sin(toRadians(nextSliceAngleDegrees));

			// Now we have all information needed to create triangles

			// Triangle 1
			
			vertices[3 * i + 0] = radius * currentDirectionX;
			vertices[3 * i + 1] = radius * currentDirectionY;
			vertices[3 * i + 2] = currentZ;

			indices[i] = i;
			i++;

			vertices[3 * i + 0] = radius * nextDirectionX;
			vertices[3 * i + 1] = radius * nextDirectionY;
			vertices[3 * i + 2] = currentZ;

			indices[i] = i;
			i++;

			vertices[3 * i + 0] = nextRadius * nextDirectionX;
			vertices[3 * i + 1] = nextRadius * nextDirectionY;
			vertices[3 * i + 2] = nextZ;

			indices[i] = i;
			i++;

			// Triangle 2

			vertices[3 * i + 0] = radius * currentDirectionX;
			vertices[3 * i + 1] = radius * currentDirectionY;
			vertices[3 * i + 2] = currentZ;

			indices[i] = i;
			i++;

			vertices[3 * i + 0] = nextRadius * nextDirectionX;
			vertices[3 * i + 1] = nextRadius * nextDirectionY;
			vertices[3 * i + 2] = nextZ;

			indices[i] = i;
			i++;

			vertices[3 * i + 0] = nextRadius * currentDirectionX;
			vertices[3 * i + 1] = nextRadius * currentDirectionY;
			vertices[3 * i + 2] = nextZ;

			indices[i] = i;
			i++;
		}
	}
  int colorCount = triangleCount*3*4;
  float* colors = new float[colorCount];
  for(i=0;i<colorCount;i+=4){
    float r = 0.5+rand()*0.5/(RAND_MAX);
    colors[i]=R*r;
    colors[i+1]=G*r;
    colors[i+2]=B*r;
    colors[i+3]=1;
  }
  //udes to manipulate shade, didnt work as intended
  float* normals = new float[9*triangleCount];
  for(int i=0;i<triangleCount*9;i+=9){
    glm::vec3 node1 = glm::vec3(vertices[i],vertices[i+1],vertices[i+2]);
    glm::vec3 node2 = glm::vec3(vertices[i+3],vertices[i+4],vertices[i+5]);
    glm::vec3 node3 = glm::vec3(vertices[6],vertices[i+7],vertices[i+8]);

    glm::vec3 vector1 = node1-node2;
    glm::vec3 vector2 = node3-node2;
    glm::vec3 normal = (glm::cross(vector1,vector2));
    //printf("%.2f,%.2f,%.2f \n",normal.x,normal.y,normal.z);
    for(int k=0;k<9;k+=3){
      normals[i+k+0]=normal.x;
      normals[i+k+1]=normal.y;
      normals[i+k+2]=normal.z;
    }
  }
	unsigned int vao_id = setupVAO(vertices,indices,colors,normals,triangleCount);

	// Cleaning up after ourselves
	delete[] vertices;
	delete[] indices;
  delete[] colors;

	return vao_id;
}
