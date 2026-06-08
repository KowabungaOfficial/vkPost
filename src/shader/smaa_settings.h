

layout(constant_id = 0) const float screenWidth = 1920;
layout(constant_id = 1) const float screenHeight = 1080;
layout(constant_id = 2) const float reverseScreenWidth = 1.0/1920.0;
layout(constant_id = 3) const float reverseScreenHeight = 1.0/1080.0;
layout(constant_id = 4) const float threshold = 0.05;
layout(constant_id = 5) const float depthThreshold = 0.01;
layout(constant_id = 6) const int   maxSearchSteps = 32;
layout(constant_id = 7) const int   maxSearchStepsDiag = 16;
layout(constant_id = 8) const int   cornerRounding = 25;

// Depth settings
layout(constant_id = 9) const int      depthInputIsUpsideDown = 0;
layout(constant_id = 10) const int      depthInputIsReverse = 0;
layout(constant_id = 11) const int      depthInputIsLogarithmic = 0;
layout(constant_id = 12) const float    depthMultiplier = 1.0;
layout(constant_id = 13) const float    depthLinearizationFarPlane = 1000.0;

#define SMAA_RT_METRICS vec4(reverseScreenWidth, reverseScreenHeight, screenWidth, screenHeight)
#define SMAA_GLSL_4 1
#define SMAA_THRESHOLD threshold
#define SMAA_DEPTH_THRESHOLD depthThreshold
#define SMAA_MAX_SEARCH_STEPS maxSearchSteps
#define SMAA_MAX_SEARCH_STEPS_DIAG maxSearchStepsDiag
#define SMAA_CORNER_ROUNDING cornerRounding

// Depth linearization function
float linearizeDepth(sampler2D depthSampler, vec2 texcoord)
{
    vec2 tc = texcoord;
    if (depthInputIsUpsideDown != 0) {
        tc.y = 1.0 - tc.y;
    }

    float depth = textureLod(depthSampler, tc, 0.0).x * depthMultiplier;

    if (depthInputIsLogarithmic != 0) {
        const float C = 0.01;
        depth = (exp(depth * log(C + 1.0)) - 1.0) / C;
    }

    if (depthInputIsReverse != 0) {
        depth = 1.0 - depth;
    }

    const float N = 1.0;
    depth /= depthLinearizationFarPlane - depth * (depthLinearizationFarPlane - N);

    return depth;
}
