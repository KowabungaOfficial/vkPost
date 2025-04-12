// LICENSE
// =======
// Copyright (c) 2017-2019 Advanced Micro Devices, Inc. All rights reserved.
// -------
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// -------
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
// -------
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
#version 450

layout(location = 0) in vec2 v_texcoord;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D inputImage;
layout(set = 0, binding = 1) uniform SMAAParams {
    float smaaThreshold;    // Edge detection sensitivity (e.g., 0.1)
    int smaaMaxSearchSteps; // Max steps for edge search (e.g., 16)
};

float Luma(vec3 color) {
    return dot(color, vec3(0.299, 0.587, 0.114));
}

void main() {
    vec2 resolution = vec2(textureSize(inputImage, 0));
    vec2 pixelSize = 1.0 / resolution;
    vec3 center = texture(inputImage, v_texcoord).rgb;

    // Sample neighbors for edge detection
    vec3 left = texture(inputImage, v_texcoord + vec2(-pixelSize.x, 0.0)).rgb;
    vec3 right = texture(inputImage, v_texcoord + vec2(pixelSize.x, 0.0)).rgb;
    vec3 up = texture(inputImage, v_texcoord + vec2(0.0, -pixelSize.y)).rgb;
    vec3 down = texture(inputImage, v_texcoord + vec2(0.0, pixelSize.y)).rgb;

    // Compute luminance for edge detection
    float lumaCenter = Luma(center);
    float lumaLeft = Luma(left);
    float lumaRight = Luma(right);
    float lumaUp = Luma(up);
    float lumaDown = Luma(down);

    // Detect edges
    vec2 edges = vec2(0.0);
    edges.x = max(abs(lumaLeft - lumaCenter), abs(lumaRight - lumaCenter)) > smaaThreshold ? 1.0 : 0.0;
    edges.y = max(abs(lumaUp - lumaCenter), abs(lumaDown - lumaCenter)) > smaaThreshold ? 1.0 : 0.0;

    if (dot(edges, vec2(1.0)) == 0.0) {
        outColor = vec4(center, 1.0); // No edge, keep original
    } else {
        // Simplified blending
        vec3 blend = (left + right + up + down) * 0.25;
        outColor = vec4(mix(center, blend, 0.5), 1.0);
    }
}
