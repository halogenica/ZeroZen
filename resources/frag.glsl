#version 120

uniform sampler2D   tex0;
uniform vec2        resolution;
uniform float       resolutionScale;
uniform float       timer;
uniform vec2        shockPos1;
uniform vec2        shockPos2;
uniform vec2        shockPos3;
uniform vec2        shockPos4;
uniform float       shockTime1;
uniform float       shockTime2;
uniform float       shockTime3;
uniform float       shockTime4;

#define _PI 3.1415926535897932384626433832795

float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*57.0 + 113.0*p.z;
    
    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}

vec2 getPixelShift(vec2 center,vec2 pixelpos,float startradius,float size,float shockfactor)
{
    float m_distance = distance(center, pixelpos);
    if (m_distance > startradius && m_distance < startradius+size)
    {
        float sin_dist = sin((m_distance - startradius) / size * _PI) * shockfactor;
        return (pixelpos - normalize(pixelpos - center) * sin_dist) / resolution.xy;
    }
    else
        return gl_FragCoord.xy / resolution.xy;
}

void main(void)
{
    float startradius1 = shockTime1 * 600.0;
    float size1 = shockTime1 * 200.0;
    
    float startradius2 = shockTime2 * 600.0;
    float size2 = shockTime2 * 200.0;
    
    float startradius3 = shockTime3 * 600.0;
    float size3 = shockTime3 * 200.0;
    
    float startradius4 = shockTime4 * 600.0;
    float size4 = shockTime4 * 200.0;
    
    
    vec2 uv = gl_FragCoord.xy / resolution.xy;
    
    float minTime = 10;
    float timeMod = 0;
    
    if (shockTime1 == 0 && shockTime2 == 0 && shockTime3 == 0 && shockTime4 == 0)
    {
        minTime = 0;
    }
    else
    {
        if (shockTime1 != 0)
        {
            minTime = min(shockTime1, minTime);
        }
        if (shockTime2 != 0)
        {
            minTime = min(shockTime2, minTime);
        }
        if (shockTime3 != 0)
        {
            minTime = min(shockTime3, minTime);
        }
        if (shockTime4 != 0)
        {
            minTime = min(shockTime4, minTime);
        }
        
        if (minTime > 0)
        {
            timeMod = exp(0-minTime);
        }
    }
    
    // Calculate color aberration
    float blurx = noise(vec3(timer * 10.0, 0.0, 0.0)) * 2.0 - 1.0;
    float offsetx = blurx * 0.0125 * timeMod;
    
    float blury = noise(vec3(timer * 10.0, 1.0, 0.0)) * 2.0 - 1.0;
    float offsety = blury * 0.005 * timeMod;
    
    vec2 ruv = vec2(offsetx, offsety);
    vec2 guv = vec2(-offsetx, -offsety);
    vec2 buv = vec2(0.00, 0.0);
    
    // Calculate shockwave
    vec2 total_shift =  getPixelShift(shockPos1 * resolutionScale, gl_FragCoord.xy, startradius1, size1, 40.0) +
    getPixelShift(shockPos2 * resolutionScale, gl_FragCoord.xy, startradius2, size2, 40.0) +
    getPixelShift(shockPos3 * resolutionScale, gl_FragCoord.xy, startradius3, size3, 40.0) +
    getPixelShift(shockPos4 * resolutionScale, gl_FragCoord.xy, startradius4, size4, 40.0);
    
    total_shift = total_shift/4;
    
    // Final color
    float r = texture2D(tex0, ruv + total_shift).r;
    float g = texture2D(tex0, guv + total_shift).g;
    float b = texture2D(tex0, buv + total_shift).b;
    
    gl_FragColor = vec4(r, g, b, 1.0);
}
