
#pragma once

#pragma warning(push, 0)
#include <d3dx9math.h>
#pragma warning(pop)
#include <vector>
#include <list>


// Fluid magic numbers
const double FluidTimestep = 0.005f;
const double FluidSmoothLen = 0.012f;
const double FluidStaticStiff = 3000.0f;
const double FluidRestDensity = 1000.0f;
const double FluidWaterMass = 0.0002f;
const double FluidViscosity = 0.1f;
const double FluidStiff = 200.0f;
const double FluidInitialSpacing = 0.0045f;

// "Poly 6" Kernel - Used for Density
const double poly6_coef = 315.0f / (64.0f * D3DX_PI * pow(FluidSmoothLen, 9));
// Gradient of the "Spikey" Kernel - Used for Pressure
const double grad_spiky_coef = -45.0f / (D3DX_PI * pow(FluidSmoothLen, 6));
// Laplacian of the "Viscosity" Kernel - Used for Viscosity
const double lap_vis_coef = 45.0f / (D3DX_PI * pow(FluidSmoothLen, 6));

/*****************************************************************************/

struct FluidNeighborRecord 
{
	unsigned int p; // Particle Index
	unsigned int n; // Neighbor Index		
	double distsq; // Distance Squared and Non-Squared (After Density Calc)
};

struct FluidGridOffset 
{
	unsigned int offset; // offset into gridindices
	unsigned int count; // number of particles in cell
};

/*****************************************************************************/

struct Particle
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 vel;
	D3DXVECTOR2 acc;
	double density = 0;
	double pressure = 0;
};

class Fluid 
{
	public:
		/* Common Interface */
		Fluid();
		~Fluid();

		void Create(double w, double h);
		void Fill(double size);
		void Clear();
		void Update(double dt);

		/* Common Data */
		unsigned int * gridindices;
		std::vector<Particle> particles;
		std::vector<int> particleToCell;
		FluidGridOffset * gridoffsets;
		unsigned int neighbors_capacity;
		unsigned int num_neighbors;
		static FluidNeighborRecord neighbors[];
		
		unsigned int Size()					{ return particles.size(); }
		unsigned int Step()					{ return step; }
		void Pause( bool p )				{ paused = p; }
		void PauseOnStep( unsigned int p )	{ pause_step = p; }
		double Width()						{ return width; }
		double Height()						{ return height; }
		Particle& particle_at(std::size_t index);


		
		/* Simulation */
		void UpdateGrid();
		__forceinline void ExpandNeighbors();
		void GetNeighbors();
		void ComputeDensity();
		void SqrtDist();
		void ComputeForce();
		void Integrate(double dt);

	private:
		/* Run State */
		unsigned int step;
		bool paused;
		unsigned int pause_step;

		/* World Size */
		double width;
		double height;
		int grid_w;
		int grid_h;
};
