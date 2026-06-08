#version 450
#extension GL_GOOGLE_include_directive : require

layout(set = 0, binding = 5) uniform sampler2D depthImg;
layout(location = 0) out vec4 fragColor;
layout(location = 0) in vec2 textureCoord;
layout(location = 1) in vec4[3] offsets;

#include "smaa_settings.h"

#define SMAA_INCLUDE_VS 0
#define SMAA_INCLUDE_PS 1

// Override SMAA's sampling to use linearized depth
// Should just multi-pass this really...
#define SMAASampleLevelZero(tex, coord) vec4(linearizeDepth(tex, coord))
#define SMAASampleLevelZeroPoint(tex, coord) vec4(linearizeDepth(tex, coord))
#define SMAASampleLevelZeroOffset(tex, coord, offset) vec4(linearizeDepth(tex, coord + offset * SMAA_RT_METRICS.xy))
#define SMAAGather(tex, coord) vec4( \
    linearizeDepth(tex, coord + vec2(0.0, 1.0) * SMAA_RT_METRICS.xy), \
    linearizeDepth(tex, coord + vec2(1.0, 1.0) * SMAA_RT_METRICS.xy), \
    linearizeDepth(tex, coord + vec2(1.0, 0.0) * SMAA_RT_METRICS.xy), \
    linearizeDepth(tex, coord + vec2(0.0, 0.0) * SMAA_RT_METRICS.xy)  \
)

#include "smaa.h"

void main()
{
    fragColor = vec4(SMAADepthEdgeDetectionPS(textureCoord, offsets, depthImg), 0.0, 0.0);
}
