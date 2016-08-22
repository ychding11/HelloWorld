#ifndef __VIEW_PLANE__
#define __VIEW_PLANE__

#include "Sampler.h"
#include "RGBColor.h"
#include <vector> 

using namespace std;

//-------------------------------------------------------------------------------------- class ViewPlane
class ViewPlane
{

	public:
		int 			hres;						// horizontal image resolution
		int 			vres;						// vertical image resolution
		float			s;							// pixel size
		int				num_samples;				// number of samples per pixel
		int				max_depth;					// max number of reflected bounce rays
		
		float			gamma;						// gamma correction factor
		float			inv_gamma;					// the inverse of the gamma correction factor
		bool			show_out_of_gamut;			// display red if RGBColor out of gamut
		Sampler*		sampler_ptr;
        vector<vector<RGBColor> > image_buffer;

	public:

		ViewPlane();								// default Constructor

		ViewPlane(int h, int v);

		ViewPlane(const ViewPlane& vp);				// copy constructor

		ViewPlane& operator= (const ViewPlane& rhs);		// assignment operator

		~ViewPlane();							// destructor

		int
		get_hres();

		int
		get_vres();

		void
		set_pixel_size(const float size);

		void
		set_gamma(const float g);

		void
		set_gamut_display(const bool show);

		void
		set_samples(const int n);

		void
		set_sampler(Sampler* sp);

		void
		set_max_depth(int depth);

        void
        write_to_buffer(int row, int col, RGBColor c);

        void
        save_to_ppm(void);

private:

        RGBColor 
        convert_color(const RGBColor& raw_color) const;

        RGBColor
        max_to_one(const RGBColor& c) const;

        RGBColor
        clamp_to_color(const RGBColor& raw_color) const;
};


// ------------------------------------------------------------------------------ set_hres
inline int
ViewPlane::get_hres()
{
	return hres;
}


// ------------------------------------------------------------------------------ set_vres
inline int
ViewPlane::get_vres()
{
	return vres;
}


// ------------------------------------------------------------------------------ set_pixel_size
inline void
ViewPlane::set_pixel_size(const float size)
{
	s = size;
}


// ------------------------------------------------------------------------------ set_gamma
inline void
ViewPlane::set_gamma(const float g) {
	gamma = g;
	inv_gamma = 1.0 / gamma;
}


// ------------------------------------------------------------------------------ set_gamut_display
inline void
ViewPlane::set_gamut_display(const bool show) {
	show_out_of_gamut = show;
}


// ------------------------------------------------------------------------------ set_max_depth
inline void
ViewPlane::set_max_depth(int depth) {
	max_depth = depth;
}

#endif
