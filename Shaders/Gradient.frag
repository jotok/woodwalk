#version 120
uniform vec2 windowSize;
uniform vec4 horizonColour; // nB: colours must be vec4's for sfml's api
uniform vec4 zenithColour;
uniform sampler2D randomTexture;
uniform vec2 scale;

void main(void)
{	
	const float MAX_PIXEL_MIX = 4; // how many pixels away can we mix
	vec2 normalisedFragCoord = gl_FragCoord.xy/windowSize * scale;
	float inverseHeight = 1.0/windowSize.y;
	// perturb the coordinate in y direction by a random amount
	float randomValue = texture2D(randomTexture, normalisedFragCoord).r;
	float perturbedFragCoordY = normalisedFragCoord.y + randomValue*inverseHeight*MAX_PIXEL_MIX;
	gl_FragColor = mix(horizonColour,zenithColour,perturbedFragCoordY);
}
