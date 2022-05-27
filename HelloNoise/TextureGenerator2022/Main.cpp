#include <iostream>
#include <noise/noise.h>
#include "noiseutils.h"
#include <sstream>

using namespace noise;

utils::NoiseMap BuildAHeightMap (int w, int h, float *bounds) {
	module::Perlin myModule;

	utils::NoiseMap heightMap;

	// 2 - Explorar diferentes oitavas, frequências e persistências: modificar parametros
	//myModule.SetOctaveCount(3);
	//myModule.SetFrequency(1.1);
	//myModule.SetPersistence(0.1);

   utils::NoiseMapBuilderPlane heightMapBuilder;
   heightMapBuilder.SetSourceModule (myModule);
   heightMapBuilder.SetDestNoiseMap (heightMap);
   heightMapBuilder.SetDestSize (w, h);
   heightMapBuilder.SetBounds (bounds[0], bounds[1], bounds[2], bounds[3]);
   heightMapBuilder.Build ();

   return heightMap;
}

utils::NoiseMap BuildASphericalHeightMap(int w, int h, float* bounds) {
	module::Perlin myModule;

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderSphere heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(512, 256);
	heightMapBuilder.SetBounds(-90.0, 90.0, -180.0, 180.0);
	heightMapBuilder.Build();

	return heightMap;
}


void RenderHeightMap(utils::NoiseMap heightMap, std::string filename){
   utils::RendererImage renderer;
   utils::Image image;
   renderer.SetSourceNoiseMap (heightMap);
   renderer.SetDestImage (image);
 
   // 3 - Criar diferentes mapeamentos de biomas: alterar gradientes
  renderer.ClearGradient ();
  renderer.AddGradientPoint(-1.00, utils::Color(32, 160, 0, 255)); // grass
  renderer.AddGradientPoint(-0.25, utils::Color(224, 224, 0, 255)); // dirt
  renderer.AddGradientPoint(0.25, utils::Color(128, 128, 128, 255)); // rock
  renderer.AddGradientPoint(1.00, utils::Color(255, 255, 255, 255)); // snow

   renderer.EnableLight ();
   renderer.SetLightContrast (3); // Triple the contrast
   renderer.SetLightBrightness (2); // Double the brightness

   renderer.Render ();

   utils::WriterBMP writer;
   writer.SetSourceImage (image);
   writer.SetDestFilename (filename.c_str());
   writer.WriteDestFile ();
}

int main (int argc, char** argv)
{

	// 1 - Gerar mapas vizinhos: Modificar os valores dos bounds
	utils::NoiseMap heightMap;
	float bounds[4] = { 6.0, 10.0, 5.0, 9.0 };
	
	//Modo padrão
	heightMap = BuildAHeightMap(300, 300, bounds);
	RenderHeightMap(heightMap,"../example.bmp");

   std::cout << "Success!\n";
  
   getchar();

  return 0;
}