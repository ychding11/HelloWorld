// This file contains the definition the ViewPlane class

#include "ViewPlane.h"
#include "MultiJittered.h"
#include "Regular.h"
#include <cstdio> 

// ---------------------------------------------------------------- default constructor	
ViewPlane::ViewPlane(void)
	 	:hres(800), vres(600)
		,s(1.0), num_samples(1)
		,sampler_ptr(NULL)
		,gamma(1.0), inv_gamma(1.0)
		,show_out_of_gamut(false)
        ,image_buffer(vres, vector<RGBColor>(hres))
{}


// ---------------------------------------------------------------- constructor	
ViewPlane::ViewPlane(int h, int v)
	    :hres(h), vres(v)
		,s(1.0), num_samples(1)
		,sampler_ptr(NULL)
		,gamma(1.0), inv_gamma(1.0)
		,show_out_of_gamut(false)
        ,image_buffer(vres, vector<RGBColor>(hres))
{}

// ---------------------------------------------------------------- copy constructor
ViewPlane::ViewPlane(const ViewPlane& vp)
	    :hres(vp.hres), vres(vp.vres)
		,s(vp.s), num_samples(vp.num_samples)
		,gamma(vp.gamma), inv_gamma(vp.inv_gamma)
		,show_out_of_gamut(vp.show_out_of_gamut)
        ,image_buffer(vres, vector<RGBColor>(hres))
{
	if(vp.sampler_ptr != NULL)
    {
		sampler_ptr = vp.sampler_ptr->clone();
	}
}

// ---------------------------------------------------------------- assignment operator
ViewPlane&
ViewPlane::operator=(const ViewPlane& rhs)
{
	if (this == &rhs) return (*this);
    this->~ViewPlane(); 
	hres				= rhs.hres;
	vres				= rhs.vres;
	s					= rhs.s;
	num_samples			= rhs.num_samples;
	gamma				= rhs.gamma;
	inv_gamma			= rhs.inv_gamma;
	show_out_of_gamut	= rhs.show_out_of_gamut;
    image_buffer        = rhs.image_buffer;

	// need to do a deep copy of the sampler
	if(rhs.sampler_ptr != NULL)
    {
		sampler_ptr = rhs.sampler_ptr->clone();
	}

	return (*this);
}


// -------------------------------------------------------------- destructor
ViewPlane::~ViewPlane(void)
{
	delete sampler_ptr;
}


// -------------------------------------------------------------- set_samples
void
ViewPlane::set_samples(const int n)
{
	num_samples = n;

    delete sampler_ptr;
	sampler_ptr = NULL;

	if (num_samples > 1)
		sampler_ptr = new MultiJittered(num_samples);
	else
		sampler_ptr = new Regular(1);
}


// -------------------------------------------------------------- set_sampler
void
ViewPlane::set_sampler(Sampler* sp)
{
    if (!sp)
    {
        fprintf(stderr, "- ERROR Samper pointer is NULL.\n" );
        return;
    }
	delete sampler_ptr;
	sampler_ptr = NULL;

	num_samples = sp->get_num_samples();
	sampler_ptr = sp;
}

void
ViewPlane:: write_to_buffer(int row, int col, RGBColor c)
{
    if (row > vres || row < 0 || col > hres || col < 0)
    {
        fprintf(stderr, "- ERROR Write color to buffer. [%d, %d]\n", row, col);
        return;
    }
    RGBColor color = convert_color(c);
    image_buffer[row][col] = color;
}

void
ViewPlane::save_to_ppm(string filename)
{
 int w = hres, h = vres;
 FILE *f = NULL;
 if (filename.empty()) f = fopen("image.ppm", "w");
 else                  f = fopen(filename.c_str(), "w");
 fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
 for (int i=0; i < h; i++)
    for (int j=0; j < w; j++)
        fprintf(f,"%d %d %d ", int(image_buffer[i][j].r), int(image_buffer[i][j].g), int(image_buffer[i][j].b));
 if (f)
 {        
    fclose(f);
    fprintf(stdout, "- Save buffer to image file. \n");
 }
}


RGBColor 
ViewPlane::convert_color(const RGBColor& raw_color) const
{
	RGBColor mapped_color;

	if (show_out_of_gamut)
		mapped_color = clamp_to_color(raw_color);
	else
		mapped_color = max_to_one(raw_color);
	
	if (gamma != 1.0)
		mapped_color = mapped_color.powc(inv_gamma);

   mapped_color.r = (int)(mapped_color.r * 255);
   mapped_color.g = (int)(mapped_color.g * 255);
   mapped_color.b = (int)(mapped_color.b * 255);
   
   return mapped_color;
}

RGBColor
ViewPlane::max_to_one(const RGBColor& c) const
{
	float max_value = max(c.r, max(c.g, c.b));
	
	if (max_value > 1.0)
		return (c / max_value);
	else
		return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one
RGBColor
ViewPlane ::clamp_to_color(const RGBColor& raw_color) const
{
	RGBColor c(raw_color);
	
	if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0)
	{
		c.r = 1.0; c.g = 0.0; c.b = 0.0;
	}
		
	return (c);
}
