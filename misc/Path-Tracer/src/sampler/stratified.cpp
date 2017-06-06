
// samplers/stratified.cpp*
#include "stratified.h"
#include "sampling.h"

// StratifiedSampler Method Definitions
// Override base class
void StratifiedSampler::StartPixel(const Point2i &p)
{
    // Generate single stratified samples for the pixel
    for (size_t i = 0; i < samples1D.size(); ++i)
    {
        StratifiedSample1D(&samples1D[i][0], xPixelSamples * yPixelSamples, rng, jitterSamples);
        Shuffle(&samples1D[i][0], xPixelSamples * yPixelSamples, 1, rng);
    }
    for (size_t i = 0; i < samples2D.size(); ++i)
    {
        StratifiedSample2D(&samples2D[i][0], xPixelSamples, yPixelSamples, rng, jitterSamples);
        Shuffle(&samples2D[i][0], xPixelSamples * yPixelSamples, 1, rng);
    }

    // Generate arrays of stratified samples for the pixel
    for (size_t i = 0; i < samples1DArraySizes.size(); ++i)
        for (int64_t j = 0; j < samplesPerPixel; ++j)
		{
            int count = samples1DArraySizes[i];
            StratifiedSample1D(&sampleArray1D[i][j * count], count, rng, jitterSamples);
            Shuffle(&sampleArray1D[i][j * count], count, 1, rng);
        }
    for (size_t i = 0; i < samples2DArraySizes.size(); ++i)
        for (int64_t j = 0; j < samplesPerPixel; ++j)
		{
            int count = samples2DArraySizes[i];
            LatinHypercube(&sampleArray2D[i][j * count].x, count, 2, rng);
        }
    PixelSampler::StartPixel(p);
}

std::unique_ptr<Sampler> StratifiedSampler::Clone(int seed)
{
    StratifiedSampler *ss = new StratifiedSampler(*this);
    ss->rng.SetSequence(seed);
    return std::unique_ptr<Sampler>(ss);
}
